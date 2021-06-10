#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <fcntl.h>   /* File Control Definitions           */
#include <termios.h> /* POSIX Terminal Control Definitions */
#include <unistd.h>  /* UNIX Standard Definitions 	   */
#include <errno.h>   /* ERROR Number Definitions           */
#include <QtSerialPort/QSerialPort>
#include <QString>
#include <QMessageBox>
#include "e32.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void readComData();
    void on_openPortButton_clicked();
    void on_getParButton_clicked();
    void on_setParButton_clicked();

private:
    Ui::MainWindow *ui;
    QSerialPort *comport;
    e32 *modem;

    QByteArray rxData;
    bool modelGot = false;

    void requestModel();
    void requestParam();
    void editEnable();
    void editDisable();
};
#endif // MAINWINDOW_H
