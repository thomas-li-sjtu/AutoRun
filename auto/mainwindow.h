#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QFileInfo>
#include <QFileIconProvider>
#include <QDateTime>
#include <QProcess>
#include <Qt>
#include <QTableWidgetItem>
#include <QCoreApplication>
#include <QDebug>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private:
    Ui::MainWindow *ui;

private slots:
    void on_Drivers_clicked();
    void on_Logon_clicked();
    void on_ScheTasks_clicked();
    void on_Service_clicked();
    void on_Knowdlls_clicked();

    void on_table_logon_itemClicked(QTableWidgetItem *item);
    void on_table_service_itemClicked(QTableWidgetItem *item);
    void on_table_drivers_itemClicked(QTableWidgetItem *item);
    void on_table_schetasks_itemClicked(QTableWidgetItem *item);
    void on_table_knowndlls_itemClicked(QTableWidgetItem *item);
};

#endif // MAINWINDOW_H

