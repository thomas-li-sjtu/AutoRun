/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QPushButton *Service;
    QPushButton *Logon;
    QPushButton *Drivers;
    QPushButton *ScheTasks;
    QTableWidget *table_logon;
    QTableWidget *table_service;
    QTableWidget *table_drivers;
    QTableWidget *table_schetasks;
    QTableWidget *table_knowndlls;
    QPushButton *Knowdlls;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1484, 654);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        Service = new QPushButton(centralwidget);
        Service->setObjectName(QString::fromUtf8("Service"));
        Service->setGeometry(QRect(20, 30, 91, 31));
        Logon = new QPushButton(centralwidget);
        Logon->setObjectName(QString::fromUtf8("Logon"));
        Logon->setGeometry(QRect(140, 30, 91, 31));
        Drivers = new QPushButton(centralwidget);
        Drivers->setObjectName(QString::fromUtf8("Drivers"));
        Drivers->setGeometry(QRect(260, 30, 91, 31));
        ScheTasks = new QPushButton(centralwidget);
        ScheTasks->setObjectName(QString::fromUtf8("ScheTasks"));
        ScheTasks->setGeometry(QRect(380, 30, 91, 31));
        table_logon = new QTableWidget(centralwidget);
        if (table_logon->columnCount() < 6)
            table_logon->setColumnCount(6);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        table_logon->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        table_logon->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        table_logon->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        table_logon->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        table_logon->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        table_logon->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        table_logon->setObjectName(QString::fromUtf8("table_logon"));
        table_logon->setGeometry(QRect(10, 70, 1461, 531));
        table_service = new QTableWidget(centralwidget);
        if (table_service->columnCount() < 6)
            table_service->setColumnCount(6);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        table_service->setHorizontalHeaderItem(0, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        table_service->setHorizontalHeaderItem(1, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        table_service->setHorizontalHeaderItem(2, __qtablewidgetitem8);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        table_service->setHorizontalHeaderItem(3, __qtablewidgetitem9);
        QTableWidgetItem *__qtablewidgetitem10 = new QTableWidgetItem();
        table_service->setHorizontalHeaderItem(4, __qtablewidgetitem10);
        QTableWidgetItem *__qtablewidgetitem11 = new QTableWidgetItem();
        table_service->setHorizontalHeaderItem(5, __qtablewidgetitem11);
        table_service->setObjectName(QString::fromUtf8("table_service"));
        table_service->setGeometry(QRect(10, 70, 1461, 531));
        table_drivers = new QTableWidget(centralwidget);
        if (table_drivers->columnCount() < 6)
            table_drivers->setColumnCount(6);
        QTableWidgetItem *__qtablewidgetitem12 = new QTableWidgetItem();
        table_drivers->setHorizontalHeaderItem(0, __qtablewidgetitem12);
        QTableWidgetItem *__qtablewidgetitem13 = new QTableWidgetItem();
        table_drivers->setHorizontalHeaderItem(1, __qtablewidgetitem13);
        QTableWidgetItem *__qtablewidgetitem14 = new QTableWidgetItem();
        table_drivers->setHorizontalHeaderItem(2, __qtablewidgetitem14);
        QTableWidgetItem *__qtablewidgetitem15 = new QTableWidgetItem();
        table_drivers->setHorizontalHeaderItem(3, __qtablewidgetitem15);
        QTableWidgetItem *__qtablewidgetitem16 = new QTableWidgetItem();
        table_drivers->setHorizontalHeaderItem(4, __qtablewidgetitem16);
        QTableWidgetItem *__qtablewidgetitem17 = new QTableWidgetItem();
        table_drivers->setHorizontalHeaderItem(5, __qtablewidgetitem17);
        table_drivers->setObjectName(QString::fromUtf8("table_drivers"));
        table_drivers->setGeometry(QRect(10, 70, 1461, 531));
        table_schetasks = new QTableWidget(centralwidget);
        if (table_schetasks->columnCount() < 6)
            table_schetasks->setColumnCount(6);
        QTableWidgetItem *__qtablewidgetitem18 = new QTableWidgetItem();
        table_schetasks->setHorizontalHeaderItem(0, __qtablewidgetitem18);
        QTableWidgetItem *__qtablewidgetitem19 = new QTableWidgetItem();
        table_schetasks->setHorizontalHeaderItem(1, __qtablewidgetitem19);
        QTableWidgetItem *__qtablewidgetitem20 = new QTableWidgetItem();
        table_schetasks->setHorizontalHeaderItem(2, __qtablewidgetitem20);
        QTableWidgetItem *__qtablewidgetitem21 = new QTableWidgetItem();
        table_schetasks->setHorizontalHeaderItem(3, __qtablewidgetitem21);
        QTableWidgetItem *__qtablewidgetitem22 = new QTableWidgetItem();
        table_schetasks->setHorizontalHeaderItem(4, __qtablewidgetitem22);
        QTableWidgetItem *__qtablewidgetitem23 = new QTableWidgetItem();
        table_schetasks->setHorizontalHeaderItem(5, __qtablewidgetitem23);
        table_schetasks->setObjectName(QString::fromUtf8("table_schetasks"));
        table_schetasks->setGeometry(QRect(10, 70, 1461, 531));
        table_knowndlls = new QTableWidget(centralwidget);
        if (table_knowndlls->columnCount() < 6)
            table_knowndlls->setColumnCount(6);
        QTableWidgetItem *__qtablewidgetitem24 = new QTableWidgetItem();
        table_knowndlls->setHorizontalHeaderItem(0, __qtablewidgetitem24);
        QTableWidgetItem *__qtablewidgetitem25 = new QTableWidgetItem();
        table_knowndlls->setHorizontalHeaderItem(1, __qtablewidgetitem25);
        QTableWidgetItem *__qtablewidgetitem26 = new QTableWidgetItem();
        table_knowndlls->setHorizontalHeaderItem(2, __qtablewidgetitem26);
        QTableWidgetItem *__qtablewidgetitem27 = new QTableWidgetItem();
        table_knowndlls->setHorizontalHeaderItem(3, __qtablewidgetitem27);
        QTableWidgetItem *__qtablewidgetitem28 = new QTableWidgetItem();
        table_knowndlls->setHorizontalHeaderItem(4, __qtablewidgetitem28);
        QTableWidgetItem *__qtablewidgetitem29 = new QTableWidgetItem();
        table_knowndlls->setHorizontalHeaderItem(5, __qtablewidgetitem29);
        table_knowndlls->setObjectName(QString::fromUtf8("table_knowndlls"));
        table_knowndlls->setGeometry(QRect(10, 70, 1461, 531));
        Knowdlls = new QPushButton(centralwidget);
        Knowdlls->setObjectName(QString::fromUtf8("Knowdlls"));
        Knowdlls->setGeometry(QRect(500, 30, 101, 31));
        MainWindow->setCentralWidget(centralwidget);
        Service->raise();
        Logon->raise();
        Drivers->raise();
        ScheTasks->raise();
        Knowdlls->raise();
        table_drivers->raise();
        table_knowndlls->raise();
        table_logon->raise();
        table_schetasks->raise();
        table_service->raise();
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1484, 26));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        Service->setText(QCoreApplication::translate("MainWindow", "Service", nullptr));
        Logon->setText(QCoreApplication::translate("MainWindow", "Logon", nullptr));
        Drivers->setText(QCoreApplication::translate("MainWindow", "Drivers", nullptr));
        ScheTasks->setText(QCoreApplication::translate("MainWindow", "ScheTasks", nullptr));
        QTableWidgetItem *___qtablewidgetitem = table_logon->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("MainWindow", "icon", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = table_logon->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("MainWindow", "entry", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = table_logon->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("MainWindow", "description", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = table_logon->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("MainWindow", "publisher", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = table_logon->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("MainWindow", "image path", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = table_logon->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("MainWindow", "timestamp", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = table_service->horizontalHeaderItem(0);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("MainWindow", "icon", nullptr));
        QTableWidgetItem *___qtablewidgetitem7 = table_service->horizontalHeaderItem(1);
        ___qtablewidgetitem7->setText(QCoreApplication::translate("MainWindow", "entry", nullptr));
        QTableWidgetItem *___qtablewidgetitem8 = table_service->horizontalHeaderItem(2);
        ___qtablewidgetitem8->setText(QCoreApplication::translate("MainWindow", "description", nullptr));
        QTableWidgetItem *___qtablewidgetitem9 = table_service->horizontalHeaderItem(3);
        ___qtablewidgetitem9->setText(QCoreApplication::translate("MainWindow", "publisher", nullptr));
        QTableWidgetItem *___qtablewidgetitem10 = table_service->horizontalHeaderItem(4);
        ___qtablewidgetitem10->setText(QCoreApplication::translate("MainWindow", "image path", nullptr));
        QTableWidgetItem *___qtablewidgetitem11 = table_service->horizontalHeaderItem(5);
        ___qtablewidgetitem11->setText(QCoreApplication::translate("MainWindow", "timestamp", nullptr));
        QTableWidgetItem *___qtablewidgetitem12 = table_drivers->horizontalHeaderItem(0);
        ___qtablewidgetitem12->setText(QCoreApplication::translate("MainWindow", "icon", nullptr));
        QTableWidgetItem *___qtablewidgetitem13 = table_drivers->horizontalHeaderItem(1);
        ___qtablewidgetitem13->setText(QCoreApplication::translate("MainWindow", "entry", nullptr));
        QTableWidgetItem *___qtablewidgetitem14 = table_drivers->horizontalHeaderItem(2);
        ___qtablewidgetitem14->setText(QCoreApplication::translate("MainWindow", "description", nullptr));
        QTableWidgetItem *___qtablewidgetitem15 = table_drivers->horizontalHeaderItem(3);
        ___qtablewidgetitem15->setText(QCoreApplication::translate("MainWindow", "publisher", nullptr));
        QTableWidgetItem *___qtablewidgetitem16 = table_drivers->horizontalHeaderItem(4);
        ___qtablewidgetitem16->setText(QCoreApplication::translate("MainWindow", "image path", nullptr));
        QTableWidgetItem *___qtablewidgetitem17 = table_drivers->horizontalHeaderItem(5);
        ___qtablewidgetitem17->setText(QCoreApplication::translate("MainWindow", "timestamp", nullptr));
        QTableWidgetItem *___qtablewidgetitem18 = table_schetasks->horizontalHeaderItem(0);
        ___qtablewidgetitem18->setText(QCoreApplication::translate("MainWindow", "icon", nullptr));
        QTableWidgetItem *___qtablewidgetitem19 = table_schetasks->horizontalHeaderItem(1);
        ___qtablewidgetitem19->setText(QCoreApplication::translate("MainWindow", "entry", nullptr));
        QTableWidgetItem *___qtablewidgetitem20 = table_schetasks->horizontalHeaderItem(2);
        ___qtablewidgetitem20->setText(QCoreApplication::translate("MainWindow", "description", nullptr));
        QTableWidgetItem *___qtablewidgetitem21 = table_schetasks->horizontalHeaderItem(3);
        ___qtablewidgetitem21->setText(QCoreApplication::translate("MainWindow", "publisher", nullptr));
        QTableWidgetItem *___qtablewidgetitem22 = table_schetasks->horizontalHeaderItem(4);
        ___qtablewidgetitem22->setText(QCoreApplication::translate("MainWindow", "image path", nullptr));
        QTableWidgetItem *___qtablewidgetitem23 = table_schetasks->horizontalHeaderItem(5);
        ___qtablewidgetitem23->setText(QCoreApplication::translate("MainWindow", "timestamp", nullptr));
        QTableWidgetItem *___qtablewidgetitem24 = table_knowndlls->horizontalHeaderItem(0);
        ___qtablewidgetitem24->setText(QCoreApplication::translate("MainWindow", "icon", nullptr));
        QTableWidgetItem *___qtablewidgetitem25 = table_knowndlls->horizontalHeaderItem(1);
        ___qtablewidgetitem25->setText(QCoreApplication::translate("MainWindow", "entry", nullptr));
        QTableWidgetItem *___qtablewidgetitem26 = table_knowndlls->horizontalHeaderItem(2);
        ___qtablewidgetitem26->setText(QCoreApplication::translate("MainWindow", "description", nullptr));
        QTableWidgetItem *___qtablewidgetitem27 = table_knowndlls->horizontalHeaderItem(3);
        ___qtablewidgetitem27->setText(QCoreApplication::translate("MainWindow", "publisher", nullptr));
        QTableWidgetItem *___qtablewidgetitem28 = table_knowndlls->horizontalHeaderItem(4);
        ___qtablewidgetitem28->setText(QCoreApplication::translate("MainWindow", "image path", nullptr));
        QTableWidgetItem *___qtablewidgetitem29 = table_knowndlls->horizontalHeaderItem(5);
        ___qtablewidgetitem29->setText(QCoreApplication::translate("MainWindow", "timestamp", nullptr));
        Knowdlls->setText(QCoreApplication::translate("MainWindow", "KnownDlls", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
