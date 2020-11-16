#ifndef DESCRIPTION_H
#define DESCRIPTION_H

#include <tchar.h>
#include <windows.h>
#include <string.h>
#include <string>
#include <iostream>
#include <clocale>
#include <Qt>
#include <QCoreApplication>
#include <QDebug>

void get_description(QString *description, QString path);
BOOL get_file_version_string(LPCTSTR pFileName, LPCTSTR pName, LPTSTR ptBuf, UINT lenBuf);
#endif // DESCRIPTION_H
