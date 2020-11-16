#ifndef VERIFIED_H
#define VERIFIED_H

#include <tchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <Softpub.h>
#include <wincrypt.h>
#include <wintrust.h>
#include <softpub.h>
#include <Qt>
#include <QCoreApplication>
#include <QDebug>

#define _UNICODE 1
#define UNICODE 1

void get_signed(QString *sign, QString path);
BOOL VerifyEmbeddedSignature(LPCWSTR pwszSourceFile, QString *sign);
#endif // VERIFIED_H
