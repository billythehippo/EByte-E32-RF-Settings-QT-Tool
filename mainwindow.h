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
#include <QFileDialog>
#include <QTextStream>
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
    void on_loadPresetButton_clicked();
    void on_savePresetButton_clicked();

private:
    Ui::MainWindow *ui;
    QSerialPort *comport;
    e32 *modem;

    QByteArray rxData;
    uint16_t freqMHzBase, freqMHz;
    bool modelGot = false;
    bool paramSet = false;

    void requestModel();
    void requestParam();
    void parseParam(char* params);
    void collectParam(char* params);
    void editEnable();
    void editDisable();
};
#endif // MAINWINDOW_H
