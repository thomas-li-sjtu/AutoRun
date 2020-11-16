// Summary
// You can use the WinVerifyTrust() API to verify an Authenticode signed executable.

// Although a signature is verified, a program may also have to do the following:
// Determine the details of the certificate that signed the executable.
// Determine the date and time that the file was time stamped.
// Retrieve the URL link associated with the file.
// Retrieve the timestamp certificate.
// This article demonstrates how to use CryptQueryObject() API to retrieve detailed information from an Authenticode signed executable.
// More Information
// Authenticode signatures are PKCS7-based, therefore you can use CryptQueryObject and other Crypto API functions to retrieve the PKCS7 signature and to decode the PKCS7 attributes.

// The following sample c code demonstrates how to use these APIs.
#include "publisher.h"
#include <string>
using namespace std;

wchar_t *multiByteToWideChar(QString str)
{
    const char* pCStrKey = str.toStdString().c_str();

    //第一次调用返回转换后的字符串长度，用于确认为wchar_t*开辟多大的内存空间
    int pSize = MultiByteToWideChar(CP_OEMCP, 0, pCStrKey, strlen(pCStrKey) + 1, NULL, 0);
    wchar_t *pWCStrKey = new wchar_t[pSize];
    //第二次调用将单字节字符串转换成双字节字符串
    MultiByteToWideChar(CP_OEMCP, 0, pCStrKey, strlen(pCStrKey) + 1, pWCStrKey, pSize);
    return pWCStrKey;
}

void get_publisher(QString path, QString *publisher)
{
    wchar_t* tmp = multiByteToWideChar(path);

    WCHAR szFileName[MAX_PATH];
    HCERTSTORE hStore = NULL;
    HCRYPTMSG hMsg = NULL;
    PCCERT_CONTEXT pCertContext = NULL;
    BOOL fResult;
    DWORD dwEncoding, dwContentType, dwFormatType;
    PCMSG_SIGNER_INFO pSignerInfo = NULL;
    PCMSG_SIGNER_INFO pCounterSignerInfo = NULL;
    DWORD dwSignerInfo;
    CERT_INFO CertInfo;
    SPROG_PUBLISHERINFO ProgPubInfo;
    SYSTEMTIME st;

    ZeroMemory(&ProgPubInfo, sizeof(ProgPubInfo));

#ifdef UNICODE
    lstrcpynW(szFileName, tmp, MAX_PATH);
#else
    if (mbstowcs(szFileName, argv[1], MAX_PATH) == -1)
    {
        printf("Unable to convert to unicode.\n");
        ;
    }
#endif
//    _tprintf(_T("Subject Name: %s\n"), szFileName);
    // Get message handle and store handle from the signed file.
    fResult = CryptQueryObject(CERT_QUERY_OBJECT_FILE,
                                szFileName,
                                CERT_QUERY_CONTENT_FLAG_PKCS7_SIGNED_EMBED,
                                CERT_QUERY_FORMAT_FLAG_BINARY,
                                0,
                                &dwEncoding,
                                &dwContentType,
                                &dwFormatType,
                                &hStore,
                                &hMsg,
                                NULL);
    if (!fResult)
    {
        *publisher = "";
        return;
    }
    // Get signer information size.
    fResult = CryptMsgGetParam(hMsg,
                                CMSG_SIGNER_INFO_PARAM,
                                0,
                                NULL,
                                &dwSignerInfo);
    if (!fResult)
        return ;

    // Allocate memory for signer information.
    pSignerInfo = (PCMSG_SIGNER_INFO)LocalAlloc(LPTR, dwSignerInfo);
    if (!pSignerInfo)
        return ;


    // Get Signer Information.
    fResult = CryptMsgGetParam(hMsg,
                                CMSG_SIGNER_INFO_PARAM,
                                0,
                                (PVOID)pSignerInfo,
                                &dwSignerInfo);
    if (!fResult)
        return ;

    // Get program name and publisher information from
    // signer info structure.
    if (GetProgAndPublisherInfo(pSignerInfo, &ProgPubInfo))
    {
        if (ProgPubInfo.lpszProgramName != NULL)
        {
//            wprintf(L"Program Name : %s\n",
//                ProgPubInfo.lpszProgramName);
        }

        if (ProgPubInfo.lpszPublisherLink != NULL)
        {
            wprintf(L"Publisher Link : %s\n",
                ProgPubInfo.lpszPublisherLink);
        }

        if (ProgPubInfo.lpszMoreInfoLink != NULL)
        {
//            wprintf(L"MoreInfo Link : %s\n",
//                ProgPubInfo.lpszMoreInfoLink);
        }
    }

    // Search for the signer certificate in the temporary
    // certificate store.
    CertInfo.Issuer = pSignerInfo->Issuer;
    CertInfo.SerialNumber = pSignerInfo->SerialNumber;

    pCertContext = CertFindCertificateInStore(hStore,
                                                ENCODING,
                                                0,
                                                CERT_FIND_SUBJECT_CERT,
                                                (PVOID)&CertInfo,
                                                NULL);
    if (!pCertContext)
        return ;

    // Print Signer certificate information.
    PrintCertificateInfo(pCertContext, publisher);

    /*
    // Get the timestamp certificate signerinfo structure.
    if (GetTimeStampSignerInfo(pSignerInfo, &pCounterSignerInfo))
    {
        // Search for Timestamp certificate in the temporary
        // certificate store.
        CertInfo.Issuer = pCounterSignerInfo->Issuer;
        CertInfo.SerialNumber = pCounterSignerInfo->SerialNumber;

        pCertContext = CertFindCertificateInStore(hStore,
                                            ENCODING,
                                            0,
                                            CERT_FIND_SUBJECT_CERT,
                                            (PVOID)&CertInfo,
                                            NULL);
        if (!pCertContext)
        {
            _tprintf(_T("CertFindCertificateInStore failed with %x\n"),
                GetLastError());
            ;
        }

        // Print timestamp certificate information.
        _tprintf(_T("TimeStamp Certificate:\n\n"));
        PrintCertificateInfo(pCertContext, publisher);
        _tprintf(_T("\n"));

        // Find Date of timestamp.
        if (GetDateOfTimeStamp(pCounterSignerInfo, &st))
        {
            _tprintf(_T("Date of TimeStamp : %02d/%02d/%04d %02d:%02d\n"),
                                        st.wMonth,
                                        st.wDay,
                                        st.wYear,
                                        st.wHour,
                                        st.wMinute);
        }
        _tprintf(_T("\n"));
    }
    */

    // Clean up.
    if (ProgPubInfo.lpszProgramName != NULL)
        LocalFree(ProgPubInfo.lpszProgramName);
    if (ProgPubInfo.lpszPublisherLink != NULL)
        LocalFree(ProgPubInfo.lpszPublisherLink);
    if (ProgPubInfo.lpszMoreInfoLink != NULL)
        LocalFree(ProgPubInfo.lpszMoreInfoLink);

    if (pSignerInfo != NULL) LocalFree(pSignerInfo);
    if (pCounterSignerInfo != NULL) LocalFree(pCounterSignerInfo);
    if (pCertContext != NULL) CertFreeCertificateContext(pCertContext);
    if (hStore != NULL) CertCloseStore(hStore, 0);
    if (hMsg != NULL) CryptMsgClose(hMsg);
}

BOOL PrintCertificateInfo(PCCERT_CONTEXT pCertContext, QString *publisher)
{
    BOOL fReturn = FALSE;
    LPTSTR szName = NULL;
    DWORD dwData;


    // Print Serial Number.
//    _tprintf(_T("Serial Number: "));
    dwData = pCertContext->pCertInfo->SerialNumber.cbData;

    // Get Issuer name size.
    if (!(dwData = CertGetNameString(pCertContext,
                                        CERT_NAME_SIMPLE_DISPLAY_TYPE,
                                        CERT_NAME_ISSUER_FLAG,
                                        NULL,
                                        NULL,
                                        0)))
        return 0;

    // Allocate memory for Issuer name.
    szName = (LPTSTR)LocalAlloc(LPTR, dwData * sizeof(TCHAR));
    if (!szName)
        return 0;

    // Get Issuer name.
    if (!(CertGetNameString(pCertContext,
                            CERT_NAME_SIMPLE_DISPLAY_TYPE,
                            CERT_NAME_ISSUER_FLAG,
                            NULL,
                            szName,
                            dwData)))
        return 0;

    // print Issuer name.
//    _tprintf(_T("Issuer Name: %s\n"), szName);
    LocalFree(szName);
    szName = NULL;

    // Get Subject name size.
    if (!(dwData = CertGetNameString(pCertContext,
                                        CERT_NAME_SIMPLE_DISPLAY_TYPE,
                                        0,
                                        NULL,
                                        NULL,
                                        0)))
        return 0;

    // Allocate memory for subject name.
    szName = (LPTSTR)LocalAlloc(LPTR, dwData * sizeof(TCHAR));
    if (!szName)
        return 0;

    // Get subject name.
    if (!(CertGetNameString(pCertContext,
                            CERT_NAME_SIMPLE_DISPLAY_TYPE,
                            0,
                            NULL,
                            szName,
                            dwData)))
        return 0;

    // Print Subject Name.
    *publisher = QString::fromWCharArray(szName);

    fReturn = TRUE;

    if (szName != NULL)
        LocalFree(szName);

    return fReturn;
}

LPWSTR AllocateAndCopyWideString(LPCWSTR inputString)
{
    LPWSTR outputString = NULL;

    outputString = (LPWSTR)LocalAlloc(LPTR,
        (wcslen(inputString) + 1) * sizeof(WCHAR));
    if (outputString != NULL)
        lstrcpyW(outputString, inputString);

    return outputString;
}

BOOL GetProgAndPublisherInfo(PCMSG_SIGNER_INFO pSignerInfo, PSPROG_PUBLISHERINFO Info)
{
    BOOL fReturn = FALSE;
    PSPC_SP_OPUS_INFO OpusInfo = NULL;
    DWORD dwData;
    BOOL fResult;


    // Loop through authenticated attributes and find
    // SPC_SP_OPUS_INFO_OBJID OID.
    for (DWORD n = 0; n < pSignerInfo->AuthAttrs.cAttr; n++)
    {
        if (lstrcmpA(SPC_SP_OPUS_INFO_OBJID,
                    pSignerInfo->AuthAttrs.rgAttr[n].pszObjId) == 0)
        {
            // Get Size of SPC_SP_OPUS_INFO structure.
            fResult = CryptDecodeObject(ENCODING,
                        SPC_SP_OPUS_INFO_OBJID,
                        pSignerInfo->AuthAttrs.rgAttr[n].rgValue[0].pbData,
                        pSignerInfo->AuthAttrs.rgAttr[n].rgValue[0].cbData,
                        0,
                        NULL,
                        &dwData);
            if (!fResult)
                return 0;

            // Allocate memory for SPC_SP_OPUS_INFO structure.
            OpusInfo = (PSPC_SP_OPUS_INFO)LocalAlloc(LPTR, dwData);
            if (!OpusInfo)
                return 0;

            // Decode and get SPC_SP_OPUS_INFO structure.
            fResult = CryptDecodeObject(ENCODING,
                        SPC_SP_OPUS_INFO_OBJID,
                        pSignerInfo->AuthAttrs.rgAttr[n].rgValue[0].pbData,
                        pSignerInfo->AuthAttrs.rgAttr[n].rgValue[0].cbData,
                        0,
                        OpusInfo,
                        &dwData);
            if (!fResult)
                return 0;

            // Fill in Program Name if present.
            if (OpusInfo->pwszProgramName)
            {
                Info->lpszProgramName =
                    AllocateAndCopyWideString(OpusInfo->pwszProgramName);
            }
            else
                Info->lpszProgramName = NULL;

            // Fill in Publisher Information if present.
            if (OpusInfo->pPublisherInfo)
            {

                switch (OpusInfo->pPublisherInfo->dwLinkChoice)
                {
                    case SPC_URL_LINK_CHOICE:
                        Info->lpszPublisherLink =
                            AllocateAndCopyWideString(OpusInfo->pPublisherInfo->pwszUrl);
                        break;

                    case SPC_FILE_LINK_CHOICE:
                        Info->lpszPublisherLink =
                            AllocateAndCopyWideString(OpusInfo->pPublisherInfo->pwszFile);
                        break;

                    default:
                        Info->lpszPublisherLink = NULL;
                        break;
                }
            }
            else
            {
                Info->lpszPublisherLink = NULL;
            }

            // Fill in More Info if present.
            if (OpusInfo->pMoreInfo)
            {
                switch (OpusInfo->pMoreInfo->dwLinkChoice)
                {
                    case SPC_URL_LINK_CHOICE:
                        Info->lpszMoreInfoLink =
                            AllocateAndCopyWideString(OpusInfo->pMoreInfo->pwszUrl);
                        break;

                    case SPC_FILE_LINK_CHOICE:
                        Info->lpszMoreInfoLink =
                            AllocateAndCopyWideString(OpusInfo->pMoreInfo->pwszFile);
                        break;

                    default:
                        Info->lpszMoreInfoLink = NULL;
                        break;
                }
            }
            else
            {
                Info->lpszMoreInfoLink = NULL;
            }

            fReturn = TRUE;

            break; // Break from for loop.
        } // lstrcmp SPC_SP_OPUS_INFO_OBJID
    }


    if (OpusInfo != NULL)
        LocalFree(OpusInfo);

    return fReturn;
}

BOOL GetDateOfTimeStamp(PCMSG_SIGNER_INFO pSignerInfo, SYSTEMTIME *st)
{
    BOOL fResult;
    FILETIME lft, ft;
    DWORD dwData;
    BOOL fReturn = FALSE;

    // Loop through authenticated attributes and find
    // szOID_RSA_signingTime OID.
    for (DWORD n = 0; n < pSignerInfo->AuthAttrs.cAttr; n++)
    {
        if (lstrcmpA(szOID_RSA_signingTime,
                    pSignerInfo->AuthAttrs.rgAttr[n].pszObjId) == 0)
        {
            // Decode and get FILETIME structure.
            dwData = sizeof(ft);
            fResult = CryptDecodeObject(ENCODING,
                        szOID_RSA_signingTime,
                        pSignerInfo->AuthAttrs.rgAttr[n].rgValue[0].pbData,
                        pSignerInfo->AuthAttrs.rgAttr[n].rgValue[0].cbData,
                        0,
                        (PVOID)&ft,
                        &dwData);
            if (!fResult)
            {
                _tprintf(_T("CryptDecodeObject failed with %x\n"),
                    GetLastError());
                break;
            }

            // Convert to local time.
            FileTimeToLocalFileTime(&ft, &lft);
            FileTimeToSystemTime(&lft, st);

            fReturn = TRUE;

            break; // Break from for loop.

        } //lstrcmp szOID_RSA_signingTime
    }

    return fReturn;
}

BOOL GetTimeStampSignerInfo(PCMSG_SIGNER_INFO pSignerInfo, PCMSG_SIGNER_INFO *pCounterSignerInfo)
{
    PCCERT_CONTEXT pCertContext = NULL;
    BOOL fReturn = FALSE;
    BOOL fResult;
    DWORD dwSize;


    *pCounterSignerInfo = NULL;

    // Loop through unathenticated attributes for
    // szOID_RSA_counterSign OID.
    for (DWORD n = 0; n < pSignerInfo->UnauthAttrs.cAttr; n++)
    {
        if (lstrcmpA(pSignerInfo->UnauthAttrs.rgAttr[n].pszObjId,
                        szOID_RSA_counterSign) == 0)
        {
            // Get size of CMSG_SIGNER_INFO structure.
            fResult = CryptDecodeObject(ENCODING,
                        PKCS7_SIGNER_INFO,
                        pSignerInfo->UnauthAttrs.rgAttr[n].rgValue[0].pbData,
                        pSignerInfo->UnauthAttrs.rgAttr[n].rgValue[0].cbData,
                        0,
                        NULL,
                        &dwSize);
            if (!fResult)
            {
                _tprintf(_T("CryptDecodeObject failed with %x\n"),
                    GetLastError());
                return 0;
            }

            // Allocate memory for CMSG_SIGNER_INFO.
            *pCounterSignerInfo = (PCMSG_SIGNER_INFO)LocalAlloc(LPTR, dwSize);
            if (!*pCounterSignerInfo)
            {
                _tprintf(_T("Unable to allocate memory for timestamp info.\n"));
                return 0;
            }

            // Decode and get CMSG_SIGNER_INFO structure
            // for timestamp certificate.
            fResult = CryptDecodeObject(ENCODING,
                        PKCS7_SIGNER_INFO,
                        pSignerInfo->UnauthAttrs.rgAttr[n].rgValue[0].pbData,
                        pSignerInfo->UnauthAttrs.rgAttr[n].rgValue[0].cbData,
                        0,
                        (PVOID)*pCounterSignerInfo,
                        &dwSize);
            if (!fResult)
                _tprintf(_T("CryptDecodeObject failed with %x\n"), GetLastError());

            fReturn = TRUE;

            break; // Break from for loop.
        }
    }

    // Clean up.
    if (pCertContext != NULL)
        CertFreeCertificateContext(pCertContext);


    return fReturn;
}
