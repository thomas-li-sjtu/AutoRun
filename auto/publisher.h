#ifndef PUBLISHER_H
#define PUBLISHER_H

#define _CRT_SECURE_NO_DEPRECATE
#include <windows.h>
#include <wincrypt.h>
#include <wintrust.h>
#include <stdio.h>
#include <tchar.h>
#include <string.h>
#include <Qt>
#include <QCoreApplication>
#include <QDebug>

#define ENCODING (X509_ASN_ENCODING | PKCS_7_ASN_ENCODING)

typedef struct {
    LPWSTR lpszProgramName;
    LPWSTR lpszPublisherLink;
    LPWSTR lpszMoreInfoLink;
} SPROG_PUBLISHERINFO, *PSPROG_PUBLISHERINFO;

wchar_t *multiByteToWideChar(QString str);
void get_publisher(QString path, QString *publisher);
BOOL PrintCertificateInfo(PCCERT_CONTEXT pCertContext, QString *publisher);
LPWSTR AllocateAndCopyWideString(LPCWSTR inputString);
BOOL GetProgAndPublisherInfo(PCMSG_SIGNER_INFO pSignerInfo, PSPROG_PUBLISHERINFO Info);
BOOL GetDateOfTimeStamp(PCMSG_SIGNER_INFO pSignerInfo, SYSTEMTIME *st);
BOOL GetTimeStampSignerInfo(PCMSG_SIGNER_INFO pSignerInfo, PCMSG_SIGNER_INFO *pCounterSignerInfo);
#endif // PUBLISHER_H
