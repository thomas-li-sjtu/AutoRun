#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "publisher.h"
#include "description.h"
#include "verified.h"

void init_ui(QTableWidget *table[], int size);
void show_logon_table(QTableWidget *table, QString regedit_path);
void show_services_table(QTableWidget *table_services, QString regedit_path);
void show_drivers_table(QTableWidget *table_drivers, QString regedit_path);
void show_schetasks_table(QTableWidget *table_schetasks);
void show_knowndlls_table(QTableWidget *table_knowndlls, QString regedit_path);

QString preprocess(QString value);
void publisher_preprocess(QTableWidget *table, QString value, int row_counter);

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QTableWidget* table[5];
    table[0] = ui->table_logon;
    table[1] = ui->table_drivers;
    table[2] = ui->table_service;
    table[3] = ui->table_schetasks;
    table[4] = ui->table_knowndlls;
    init_ui(table, 5);

    QStringList logon_list;  //保存logon的查找位置
    logon_list << "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"
               << "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer\\Run"
               << "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\RunOnce"
               << "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\RunOnceEx"
               << "HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"
               << "HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer\\Run"
               << "HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\RunOnce"
               << "HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\RunOnceEx";
    foreach(QString logon_loc, logon_list)
    {
        show_logon_table(ui->table_logon, logon_loc);  //依次显示
    }
    qDebug() << "logon done";
    QString drivers("HKEY_LOCAL_MACHINE\\System\\CurrentControlSet\\Services"); //drivers查找位置
    show_drivers_table(ui->table_drivers, drivers);
    qDebug() << "drivers done";
    QString services("HKEY_LOCAL_MACHINE\\System\\CurrentControlSet\\Services"); //services查找位置
    show_services_table(ui->table_service, services);
    qDebug() << "services done";

    show_schetasks_table(ui->table_schetasks);
    qDebug() << "st done";
    QString knowndlls = "HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Session Manager\\KnownDLLs";
    show_knowndlls_table(ui->table_knowndlls, knowndlls);
}


MainWindow::~MainWindow()  //析构
{
    delete ui;
}


void init_ui(QTableWidget *table[], int size)  //初始化ui
{
    for(int tmp = 0; tmp < size; tmp++)
    {
        table[tmp]->hide();
        table[tmp]->setColumnWidth(0, 50);
        table[tmp]->setColumnWidth(1, 150);
        table[tmp]->setColumnWidth(2, 200);
        table[tmp]->setColumnWidth(3, 300);
        table[tmp]->setColumnWidth(4, 500);
        table[tmp]->setColumnWidth(5, 200);
    }
}


QString preprocess(QString value)
{
    //替换部分系统变量
    if(!value.contains(":"))
    {
        if(value.contains("system32", Qt::CaseInsensitive))
            value = value.replace(value.indexOf("system32", 0, Qt::CaseInsensitive), 8, "c:\\Windows\\System32");
        if(value.contains("syswow64", Qt::CaseInsensitive))
            value = value.replace(value.indexOf("syswow64", 0, Qt::CaseInsensitive), 8, "c:\\Windows\\SysWOW64");
    }
    if(value.contains("%ProgramFiles%", Qt::CaseInsensitive))
        value = value.replace(value.indexOf("%ProgramFiles%", 0, Qt::CaseInsensitive), 14, "C:\\Program Files");
    if(value.contains("%localappdata%", Qt::CaseInsensitive))
        value = value.replace(value.indexOf("%localappdata%", 0, Qt::CaseInsensitive), 14, "C:\\Users\\ASUS\\AppData\\Local");
    if(value.contains("%systemroot%", Qt::CaseInsensitive))
        value = value.replace(value.indexOf("%systemroot%", 0, Qt::CaseInsensitive), 12, "C:\\Windows");
    if(value.contains("%windir%", Qt::CaseInsensitive))
        value = value.replace(value.indexOf("%windir%", 0, Qt::CaseInsensitive), 8, "C:\\Windows");
    if(value.contains("\\??\\", Qt::CaseInsensitive))
        value = value.replace(value.indexOf("\\??\\"), 4, "");
    if(value.contains("\\systemroot", Qt::CaseInsensitive))
        value = value.replace(value.indexOf("\\systemroot", 0, Qt::CaseInsensitive), 11, "C:\\Windows");
    if(value.contains("C:\\Windows\\c:\\Windows", Qt::CaseInsensitive))
        value = value.replace(value.indexOf("C:\\Windows\\c:\\Windows", 0, Qt::CaseInsensitive), 21, "c:\\Windows");
    if(value.contains("c:\\Windows\\C:\\Windows", Qt::CaseInsensitive))
        value = value.replace(value.indexOf("c:\\Windows\\C:\\Windows", 0, Qt::CaseInsensitive), 21, "c:\\Windows");

    //取出value
    if(value.indexOf("\"") >= 0)
        value = value.split("\"")[1];
    if(value.indexOf(" /") >= 0)
        value = value.split(" /")[0];
    if(value.indexOf(" -") >= 0)
        value = value.split(" -")[0];

    return value;
}


void publisher_preprocess(QTableWidget *table,
                          QString value,
                          int row_counter)
{

    QString path = QCoreApplication::applicationDirPath();
    QString msg_result("");
    QString timestamp("");
    get_publisher(value, &msg_result);  //获取publisher
    QString sign("");
    get_signed(&sign, value);  //获取sign
    table->setItem(row_counter, 3, new QTableWidgetItem(sign + msg_result));

    QString description("");  //获取description
    get_description(&description, value);
    table->setItem(row_counter, 2, new QTableWidgetItem(description));

    if(msg_result == "")
    {
        QProcess process;
        QString sigcheck = path + QString("/sigcheck64.exe \"") + value + QString("\"");  //用其他工具读
        process.start(sigcheck);
        process.waitForFinished();
        msg_result = process.readAllStandardOutput();
        if(msg_result.contains("No matching files were found.", Qt::CaseInsensitive))
        {
            table->setItem(row_counter, 2, new QTableWidgetItem(("No matching " + value))); //未匹配到
            table->setSpan(row_counter, 2, 1, 2);
            table->item(row_counter, 0)->setBackgroundColor(QColor(255,99,71));
            table->item(row_counter, 1)->setBackgroundColor(QColor(255,99,71));
            table->item(row_counter, 2)->setBackgroundColor(QColor(255,99,71));
            table->item(row_counter, 4)->setBackgroundColor(QColor(255,99,71));
            table->item(row_counter, 5)->setBackgroundColor(QColor(255,99,71));
        }
        else
        {
            QStringList sig_list = msg_result.split("\n\t");

            table->setItem(row_counter, 2, new QTableWidgetItem(sig_list[5].split(":\t")[1]));
            table->setItem(row_counter, 3,
                           new QTableWidgetItem("(" + sig_list[1].split(":\t")[1] + ") " + sig_list[3].split(":\t")[1]));
            if(table->item(row_counter, 3)->text().split(')')[1][1] == "n"
                    || table->item(row_counter, 3)->text().split(')')[1][1] == "?")  // n/a
            {
                table->setItem(row_counter, 3,
                               new QTableWidgetItem("(" + sig_list[1].split(":\t")[1] + ") " + sig_list[4].split(":\t")[1]));
                for(int i = 0; i < 6; i++)
                    table->item(row_counter, i)->setBackgroundColor(QColor(255,227,132));
            }

            QStringList time = sig_list[2].split(":\t")[1].split(" ");
            table->setItem(row_counter, 5,
                           new QTableWidgetItem(time[1] + " " + time[0]));
        }
    }
}


void show_logon_table(QTableWidget *table, QString regedit_path)
{
    QSettings *get_reg = new QSettings(regedit_path, QSettings::NativeFormat);  //获得注册表
    QStringList childkeys = get_reg->childKeys();  //获得子键
    QTableWidgetItem *item = new QTableWidgetItem(regedit_path);

    int row_counter = table->rowCount(); //当前表格的行数

    item->setBackgroundColor(QColor(64,224,205));  //在之前表格的下一行显示regeditpath
    table->setRowCount(row_counter + 1);
    table->setItem(row_counter, 0, item);
    table->setSpan(row_counter++, 0, 1, 6);  //合并显示

    //处理值与数据
    foreach(QString childkey, childkeys)
    {
        table->setRowCount(row_counter + 1);
        QString value = get_reg->value(childkey).toString();  //获得数据

        value = preprocess(value);

        table->setItem(row_counter, 1, new QTableWidgetItem(childkey.toLocal8Bit().data()));  //显示值
        table->setItem(row_counter, 4, new QTableWidgetItem(value));  //显示数据

        QFileInfo timestamp(value.toLocal8Bit().data());  //时间戳
        table->setItem(row_counter, 5, new QTableWidgetItem(timestamp.lastModified().toString("yyyy/MM/dd hh:mm")));

        QFileIconProvider icon_provider;
        QIcon icon = icon_provider.icon(timestamp); //获取图标
        table->setItem(row_counter, 0, new QTableWidgetItem(icon, " "));
        publisher_preprocess(table, value, row_counter);

        row_counter += 1;
    }
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->verticalHeader()->setVisible(false);

    delete get_reg;
}


void show_services_table(QTableWidget *table_services, QString regedit_path)
{
    QSettings *get_reg = new QSettings(regedit_path, QSettings::NativeFormat);
    QTableWidgetItem *item_service = new QTableWidgetItem(regedit_path);
    QStringList childGroups = get_reg->childGroups();  //获得子目录

    int row_counter_service = table_services->rowCount();
    item_service->setBackgroundColor(QColor(64,224,205));
    table_services->setRowCount(row_counter_service + 1);
    table_services->setItem(row_counter_service, 0, item_service);
    table_services->setSpan(row_counter_service++, 0, 1, 6);

    foreach (QString group, childGroups)
    {
        get_reg->beginGroup(group);

        int Start = get_reg->value("Start", -1).toInt();
        if(Start == 0 || Start == 1 || Start == 2)
        {
            QString value = get_reg->value("ImagePath", "ERROR!").toString();

            //填写表项
            if(!value.contains("ERROR!") && !(value.contains("system32\\drivers\\", Qt::CaseInsensitive)))
            {
                table_services->setRowCount(row_counter_service + 1);
                value = preprocess(value);

                table_services->setItem(row_counter_service, 1, new QTableWidgetItem(group));

                table_services->setItem(row_counter_service, 4, new QTableWidgetItem(value));

                QFileInfo info(value.toLocal8Bit().data());  //获取图标与时间戳
                QFileIconProvider icon_provider;
                QIcon icon = icon_provider.icon(info);
                table_services->setItem(row_counter_service, 0, new QTableWidgetItem(icon, " "));
                table_services->setItem(row_counter_service, 5,
                                        new QTableWidgetItem(info.lastModified().toString("yyyy/MM/dd hh:mm")));

                publisher_preprocess(table_services, value, row_counter_service);

                row_counter_service += 1;
            }
        }
        get_reg->endGroup();
    }
    table_services->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table_services->verticalHeader()->setVisible(false);

    delete get_reg;
}


void show_drivers_table(QTableWidget *table_drivers, QString regedit_path)
{
    QSettings *get_reg = new QSettings(regedit_path, QSettings::NativeFormat);
    QTableWidgetItem *item_drivers = new QTableWidgetItem(regedit_path);
    QStringList childGroups = get_reg->childGroups();  //获得子目录

    int row_counter_drivers = table_drivers->rowCount();
    item_drivers ->setBackgroundColor(QColor(64,224,205));
    table_drivers->setRowCount(row_counter_drivers + 1);
    table_drivers->setItem(row_counter_drivers, 0, item_drivers );
    table_drivers->setSpan(row_counter_drivers++, 0, 1, 6);

    foreach (QString group, childGroups)
    {
        get_reg->beginGroup(group);

        int Start = get_reg->value("Start", -1).toInt();
        if(Start == 0 || Start == 1 || Start == 2)
        {
            QString value = get_reg->value("ImagePath", "ERROR!").toString();

            //填写表项
            if(!value.contains("ERROR!") && (value.contains("system32\\drivers\\", Qt::CaseInsensitive)))
            {
                table_drivers->setRowCount(row_counter_drivers + 1);
                value = preprocess(value);

                table_drivers->setItem(row_counter_drivers, 1, new QTableWidgetItem(group));

                table_drivers->setItem(row_counter_drivers, 4, new QTableWidgetItem(value));

                QFileInfo info(value.toLocal8Bit().data());  //获取图标与时间戳
                QFileIconProvider icon_provider;
                QIcon icon = icon_provider.icon(info);
                table_drivers->setItem(row_counter_drivers, 0, new QTableWidgetItem(icon, " "));
                table_drivers->setItem(row_counter_drivers, 5, new QTableWidgetItem(info.created().toString("yyyy/MM/dd hh:mm")));

                QProcess process;  //获取签名
                QString path = QCoreApplication::applicationDirPath();
                QString sigcheck = path + QString("/sigcheck64.exe \"") + value + QString("\"");  //命令+签名路径
                process.start(sigcheck);
                process.waitForFinished();
                QString msg_result = process.readAllStandardOutput();
                if(msg_result.contains("No matching files were found.", Qt::CaseInsensitive))
                {
                    table_drivers->setItem(row_counter_drivers, 2, new QTableWidgetItem(("No matching " + value))); //未匹配到
                    table_drivers->setSpan(row_counter_drivers, 2, 1, 2);
                    table_drivers->item(row_counter_drivers, 0)->setBackgroundColor(QColor(255,99,71));
                    table_drivers->item(row_counter_drivers, 1)->setBackgroundColor(QColor(255,99,71));
                    table_drivers->item(row_counter_drivers, 2)->setBackgroundColor(QColor(255,99,71));
                    table_drivers->item(row_counter_drivers, 4)->setBackgroundColor(QColor(255,99,71));
                    table_drivers->item(row_counter_drivers, 5)->setBackgroundColor(QColor(255,99,71));
                }
                else
                {
                    QStringList sig_list = msg_result.split("\n\t");

                    table_drivers->setItem(row_counter_drivers, 2, new QTableWidgetItem(sig_list[5].split(":\t")[1]));

                    table_drivers->setItem(row_counter_drivers, 3,
                                            new QTableWidgetItem("(" + sig_list[1].split(":\t")[1] + ") " + sig_list[3].split(":\t")[1]));
                    if(table_drivers->item(row_counter_drivers, 3)->text().split(')')[1][1] == "n"
                            || table_drivers->item(row_counter_drivers, 3)->text().split(')')[1][1] == "?")  // n/a
                    {
                        table_drivers->setItem(row_counter_drivers, 3,
                                                new QTableWidgetItem("(" + sig_list[1].split(":\t")[1] + ") " + sig_list[4].split(":\t")[1]));
                        for(int i = 0; i < 6; i++)
                            table_drivers->item(row_counter_drivers, i)->setBackgroundColor(QColor(255,227,132));
                    }
                    QStringList time = sig_list[2].split(":\t")[1].split(" ");
                    table_drivers->setItem(row_counter_drivers, 5,
                                   new QTableWidgetItem(time[1] + " " + time[0]));
                }
                row_counter_drivers += 1;
            }
        }
        get_reg->endGroup();
    }
    table_drivers->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table_drivers->verticalHeader()->setVisible(false);

    delete get_reg;
}


void show_schetasks_table(QTableWidget *table_schetasks)
{
    QTableWidgetItem *item = new QTableWidgetItem("Task Scheduler");
    int row_counter = table_schetasks->rowCount(); //当前表格的行数
    item->setBackgroundColor(QColor(64,224,205));  //在之前表格的下一行显示regeditpath
    table_schetasks->setRowCount(row_counter + 1);
    table_schetasks->setItem(row_counter, 0, item);
    table_schetasks->setSpan(row_counter++, 0, 1, 6);  //合并显示

    QProcess process;
    QString path = QCoreApplication::applicationDirPath();
    QString get_tasks_info = path + "/" + QString("schetasks.exe");
    process.start(get_tasks_info);
    process.waitForFinished();
    QString output = process.readAllStandardOutput();
    QStringList schelist = output.split("\r\n"); //获得taskname与imagepath

    foreach(QString str, schelist)
    {
        if(str.length() != 0 && !str.contains("cannot", Qt::CaseInsensitive))  //过滤
        {
            table_schetasks->setRowCount(row_counter + 1);
            QStringList task_info = str.split('\t');
            if(task_info.size() == 2)  //1,2
                continue;
            QStringList namelist = task_info[1].split(":");
            QStringList pathlist = task_info[2].split("Path:");
            QString task_name = namelist[1];
            QString task_path = pathlist[1];

            table_schetasks->setItem(row_counter, 1, new QTableWidgetItem(task_name));  //显示值

            task_path = preprocess(task_path);
            table_schetasks->setItem(row_counter, 4, new QTableWidgetItem(task_path));  //显示数据

            QFileInfo timestamp(task_path);  //时间戳
            table_schetasks->setItem(row_counter, 5, new QTableWidgetItem(timestamp.lastModified().toString("yyyy/MM/dd hh:mm")));
            QFileIconProvider icon_provider;
            QIcon icon = icon_provider.icon(timestamp); //获取图标
            table_schetasks->setItem(row_counter, 0, new QTableWidgetItem(icon, " "));

            publisher_preprocess(table_schetasks, task_path.remove(QRegExp("^ +\\s*")), row_counter);

            row_counter += 1;
        }
    }
}


void show_knowndlls_table(QTableWidget *table_knowndlls, QString regedit_path)
{
    QSettings *get_reg = new QSettings(regedit_path, QSettings::NativeFormat);  //获得注册表
    QStringList childkeys = get_reg->childKeys();  //获得子键
    QTableWidgetItem *item = new QTableWidgetItem(regedit_path);

    int row_counter = table_knowndlls->rowCount(); //当前表格的行数

    item->setBackgroundColor(QColor(64,224,205));  //在之前表格的下一行显示regeditpath
    table_knowndlls->setRowCount(row_counter + 1);
    table_knowndlls->setItem(row_counter, 0, item);
    table_knowndlls->setSpan(row_counter++, 0, 1, 6);  //合并显示

    foreach(QString childkey, childkeys)
    {
        table_knowndlls->setRowCount(row_counter + 1);
        QString value = get_reg->value(childkey).toString();  //获得数据

        value = preprocess(value);
        qDebug() << value;

        table_knowndlls->setItem(row_counter, 1, new QTableWidgetItem(childkey.toLocal8Bit().data()));  //显示值
        table_knowndlls->setItem(row_counter, 4, new QTableWidgetItem("C:\\Windows\\System32\\" + value));  //显示数据
        QFileInfo timestamp(value.toLocal8Bit().data());  //时间戳
        table_knowndlls->setItem(row_counter, 5, new QTableWidgetItem(timestamp.lastModified().toString("yyyy-MM-dd hh:mm:ss")));
        QFileIconProvider icon_provider;
        QIcon icon = icon_provider.icon(timestamp); //获取图标
        table_knowndlls->setItem(row_counter, 0, new QTableWidgetItem(icon, " "));
        publisher_preprocess(table_knowndlls, "C:\\Windows\\System32\\" + value, row_counter);
        row_counter += 1;

        table_knowndlls->setRowCount(row_counter + 1);
        table_knowndlls->setItem(row_counter, 1, new QTableWidgetItem(childkey.toLocal8Bit().data()));  //显示值
        table_knowndlls->setItem(row_counter, 4, new QTableWidgetItem("C:\\Windows\\SysWOW64\\" + value));  //显示数据
        table_knowndlls->setItem(row_counter, 0, new QTableWidgetItem(icon, " "));
        table_knowndlls->setItem(row_counter, 5, new QTableWidgetItem(timestamp.lastModified().toString("yyyy/MM/dd hh:mm")));
        publisher_preprocess(table_knowndlls, "C:\\Windows\\SysWOW64\\" + value, row_counter);
        row_counter += 1;
    }
    table_knowndlls->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table_knowndlls->verticalHeader()->setVisible(false);

    delete get_reg;
}


void MainWindow::on_Drivers_clicked()
{
    ui->Drivers->setStyleSheet("background-color:white;");
    ui->Drivers->setDisabled(true);
    ui->table_drivers->show();

    ui->Logon->setStyleSheet("");
    ui->Logon->setEnabled(true);
    ui->table_logon->hide();

    ui->Service->setStyleSheet("");
    ui->Service->setEnabled(true);
    ui->table_service->hide();

    ui->ScheTasks->setStyleSheet("");
    ui->ScheTasks->setEnabled(true);
    ui->table_schetasks->hide();

    ui->Knowdlls->setStyleSheet("");
    ui->Knowdlls->setEnabled(true);
    ui->table_knowndlls->hide();
}


void MainWindow::on_Logon_clicked()
{
    ui->Logon->setStyleSheet("background-color:white;");
    ui->Logon->setDisabled(true);
    ui->table_logon->show();

    ui->Service->setStyleSheet("");
    ui->Service->setEnabled(true);
    ui->table_service->hide();

    ui->Drivers->setStyleSheet("");
    ui->Drivers->setEnabled(true);
    ui->table_drivers->hide();

    ui->ScheTasks->setStyleSheet("");
    ui->ScheTasks->setEnabled(true);
    ui->table_schetasks->hide();

    ui->Knowdlls->setStyleSheet("");
    ui->Knowdlls->setEnabled(true);
    ui->table_knowndlls->hide();
}


void MainWindow::on_ScheTasks_clicked()
{
    ui->ScheTasks->setStyleSheet("background-color:white;");
    ui->ScheTasks->setDisabled(true);
    ui->table_schetasks->show();

    ui->Logon->setStyleSheet("");
    ui->Logon->setEnabled(true);
    ui->table_logon->hide();

    ui->Service->setStyleSheet("");
    ui->Service->setEnabled(true);
    ui->table_service->hide();

    ui->Drivers->setStyleSheet("");
    ui->Drivers->setEnabled(true);
    ui->table_drivers->hide();

    ui->Knowdlls->setStyleSheet("");
    ui->Knowdlls->setEnabled(true);
    ui->table_knowndlls->hide();
}


void MainWindow::on_Service_clicked()
{
    ui->Service->setStyleSheet("background-color:white;");
    ui->Service->setDisabled(true);
    ui->table_service->show();

    ui->Logon->setStyleSheet("");
    ui->Logon->setEnabled(true);
    ui->table_logon->hide();

    ui->Drivers->setStyleSheet("");
    ui->Drivers->setEnabled(true);
    ui->table_drivers->hide();

    ui->ScheTasks->setStyleSheet("");
    ui->ScheTasks->setEnabled(true);
    ui->table_schetasks->hide();

    ui->Knowdlls->setStyleSheet("");
    ui->Knowdlls->setEnabled(true);
    ui->table_knowndlls->hide();
}


void MainWindow::on_Knowdlls_clicked()
{
    ui->Knowdlls->setStyleSheet("background-color:white;");
    ui->Knowdlls->setDisabled(true);
    ui->table_knowndlls->show();

    ui->ScheTasks->setStyleSheet("");
    ui->ScheTasks->setEnabled(true);
    ui->table_schetasks->hide();

    ui->Logon->setStyleSheet("");
    ui->Logon->setEnabled(true);
    ui->table_logon->hide();

    ui->Service->setStyleSheet("");
    ui->Service->setEnabled(true);
    ui->table_service->hide();

    ui->Drivers->setStyleSheet("");
    ui->Drivers->setEnabled(true);
    ui->table_drivers->hide();
}


void MainWindow::on_table_logon_itemClicked(QTableWidgetItem *item)
{

}
void MainWindow::on_table_service_itemClicked(QTableWidgetItem *item)
{

}
void MainWindow::on_table_drivers_itemClicked(QTableWidgetItem *item)
{

}
void MainWindow::on_table_schetasks_itemClicked(QTableWidgetItem *item)
{

}
void MainWindow::on_table_knowndlls_itemClicked(QTableWidgetItem *item)
{

}


/*
void publisher_preprocess(QTableWidget *table,
                          QString value,
                          int row_counter)
{
    QProcess process1;  //获取publisher
    QString path = QCoreApplication::applicationDirPath();
    QString sigcheck = path + QString("/authenticode_info.exe \"") + value + QString("\""); //运行exe
    process1.start(sigcheck);
    process1.waitForFinished(3000);
    QString msg_result = QString::fromLocal8Bit(process1.readAllStandardOutput()).trimmed();  //读取结果
    if(msg_result == "")
    {
        QProcess process2;
        sigcheck = path + QString("/sigcheck64.exe \"") + value + QString("\"");  //用其他工具读
        process2.start(sigcheck);
        process2.waitForFinished();
        msg_result = process2.readAllStandardOutput();
        if(msg_result.contains("No matching files were found.", Qt::CaseInsensitive))
        {
            table->setItem(row_counter, 2, new QTableWidgetItem(("No matching " + value))); //未匹配到
            table->setSpan(row_counter, 2, 1, 2);
            table->item(row_counter, 0)->setBackgroundColor(QColor(255,99,71));
            table->item(row_counter, 1)->setBackgroundColor(QColor(255,99,71));
            table->item(row_counter, 2)->setBackgroundColor(QColor(255,99,71));
            table->item(row_counter, 4)->setBackgroundColor(QColor(255,99,71));
            table->item(row_counter, 5)->setBackgroundColor(QColor(255,99,71));
        }
        else
        {
            QStringList sig_list = msg_result.split("\n\t");

            table->setItem(row_counter, 2, new QTableWidgetItem(sig_list[5].split(":\t")[1]));
            table->setItem(row_counter, 3,
                           new QTableWidgetItem("(" + sig_list[1].split(":\t")[1] + ") " + sig_list[3].split(":\t")[1]));
            if(table->item(row_counter, 3)->text().split(')')[1][1] == "n"
                    || table->item(row_counter, 3)->text().split(')')[1][1] == "?")  // n/a
            {
                table->setItem(row_counter, 3,
                               new QTableWidgetItem("(" + sig_list[1].split(":\t")[1] + ") " + sig_list[4].split(":\t")[1]));
                for(int i = 0; i < 6; i++)
                    table->item(row_counter, i)->setBackgroundColor(QColor(255,227,132));
            }
        }
    }
    else
    {
        QProcess process3;
        QString check_signed = path + QString("/verified.exe \"") + value + QString("\"");  //确认签名
        process3.start(check_signed);
        process3.waitForFinished();
        check_signed = QString::fromLocal8Bit(process3.readAllStandardOutput()).trimmed();
        if(check_signed.contains("verified", Qt::CaseInsensitive))
            check_signed = "(Signed)";
        else
            check_signed = "(Unsigned)";

        QString description = path + QString("/description.exe \"") + value + QString("\"");
        QProcess process4;
        process4.start(description);
        process4.waitForFinished();
        description = QString::fromLocal8Bit(process4.readAllStandardOutput()).trimmed();
        table->setItem(row_counter, 2, new QTableWidgetItem(description));

        QStringList result_list = msg_result.split("TimeStamp Certificate:");
        if(result_list.size() == 1)  //只有signer certificate
        {
            result_list = result_list[0].split("\r\n");

            foreach(QString str, result_list)
            {
                if(str.contains("Subject Name", Qt::CaseInsensitive))
                    table->setItem(row_counter, 3, new QTableWidgetItem(check_signed + str.split("Name:")[1]));
            }
        }
        else if(result_list.size() == 2) //有signer certificate和timestamp certificate
        {
            QStringList signer = result_list[0].split("\r\n");
            foreach(QString str, signer)
            {
                if(str.contains("Subject Name", Qt::CaseInsensitive))
                    table->setItem(row_counter, 3, new QTableWidgetItem(check_signed + str.split("Name:")[1]));
            }

            QString timestamp = result_list[1].split("Date of TimeStamp : ")[1].split("\r\n")[0]; //timestamp
            QStringList time = timestamp.split(" ");
            QString date = time[0].split("/")[2] + "-" + time[0].split("/")[0] + "-" + time[0].split("/")[1];
            table->setItem(row_counter, 5, new QTableWidgetItem(date + time[1]));
        }
    }
}
*/






