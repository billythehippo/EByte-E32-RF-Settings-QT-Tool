#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    comport = new QSerialPort(this);
    connect(comport, SIGNAL(readyRead()), this, SLOT(readComData()));
    modem = new e32;
}

MainWindow::~MainWindow()
{
    disconnect(comport, SIGNAL(readyRead()), this, SLOT(readComData()));
    comport->close();
    delete comport;
    delete modem;
    delete ui;
}

void MainWindow::requestModel()
{
    char request[3];
    rxData.clear();
    comport->clear();
    memset(request, 0xC3, 3);
    comport->write(request, 3);
//    comport->waitForReadyRead(20);
}

void MainWindow::requestParam()
{
    char request[3];
    rxData.clear();
    comport->clear();
    memset(request, 0xC1, 3);
    comport->write(request, 3);
//    comport->waitForReadyRead(20);
}

void MainWindow::parseParam(char* params)
{
    uint16_t tmp16;
    uint8_t tmp[6];
    QString str;

    memcpy(tmp, params, 6);
    ui->comboBox_1->setCurrentIndex(modem->getUartSpeed(tmp));
    ui->comboBox_2->setCurrentIndex(modem->getParity(tmp));
    ui->comboBox_3->setCurrentIndex(modem->getAirSpeed(tmp));
    ui->comboBox_4->setCurrentIndex(modem->getPower(tmp));
    ui->comboBox_5->setCurrentIndex(modem->getIODriveMode(tmp));
    ui->comboBox_6->setCurrentIndex(modem->getFEC(tmp));
    ui->comboBox_7->setCurrentIndex(modem->getTramsmissionMode(tmp));
    ui->comboBox_8->setCurrentIndex(modem->getWakeUpTime(tmp));
    tmp16 = (tmp[1]<<8)+tmp[2];
    str.setNum(tmp16);
    ui->lineEdit_2->setText(str);
    tmp16 = modem->getRadioChannel(tmp);
    str.setNum(tmp16);
    ui->lineEdit_3->setText(str);
}

void MainWindow::collectParam(char* params)
{
    uint16_t tmp16;

    params[0] = 0xC0;
    tmp16 = ui->lineEdit_2->text().toInt();
    params[1] = tmp16>>8;
    params[2] = tmp16&0x00FF;
    tmp16 = ui->lineEdit_3->text().toInt();

    modem->setUartSpeed((uint8_t*)params, ui->comboBox_1->currentIndex());
    modem->setParity((uint8_t*)params, ui->comboBox_2->currentIndex());
    modem->setAirSpeed((uint8_t*)params, ui->comboBox_3->currentIndex());
    modem->setPower((uint8_t*)params, ui->comboBox_4->currentIndex());
    modem->setIODriveMode((uint8_t*)params, ui->comboBox_5->currentIndex());
    modem->setFEC((uint8_t*)params, ui->comboBox_6->currentIndex());
    modem->setTramsmissionMode((uint8_t*)params, ui->comboBox_7->currentIndex());
    modem->setWakeUpTime((uint8_t*)params, ui->comboBox_8->currentIndex());
    modem->setRadioChannel((uint8_t*)params, tmp16);
}

void MainWindow::readComData()
{
    uint8_t tmp[8];
    char params[6];
    QString str, sn;
    QByteArray D;

    rxData.append(comport->readAll());
    if (rxData.length()==4)
    {
        memcpy(tmp, rxData, 4);
        if (tmp[0]==0xC3)
        {
            str = "Model: ";
            switch (tmp[1])
            {
            case 0x32:
                freqMHzBase = 410;
                str.append("E32");
                break;
            default:
                break;
            }
            ui->textBrowser->clear();
            ui->textBrowser->append(str);
            str = "Version: ";
            sn.setNum(tmp[2]>>4);
            str.append(sn);
            str.append(".");
            sn.setNum(tmp[2]&0x0F);
            str.append(sn);
            ui->textBrowser->append(str);
            str = "Current Config: ";
            D = rxData;
            str.append(D.toHex());
            ui->textBrowser->append(str);
            modelGot = true;
            requestParam();
        }
    }
    else if (rxData.length()==6)
    {
        memcpy(tmp, rxData, 6);
        if (tmp[0]==0xC0)
        {
            memcpy(params, tmp, 6);
            parseParam(params);
            rxData.clear();
            editEnable();
            str = "Current Frequency is ";
            freqMHz = freqMHzBase + params[4];
            QString fs;
            fs.setNum(freqMHz);
            str.append(fs);
            str.append(" MHz");
            ui->textBrowser->append(str);
            if (paramSet) paramSet = false;
            else QMessageBox::information(0, "INFO", "Parameters are GOT!");
            ui->savePresetButton->setEnabled(true);
        }
    }
}

void MainWindow::editEnable()
{
    ui->comboBox_1->setEnabled(true);
    if (ui->comboBox_1->currentIndex()<0) ui->comboBox_1->setCurrentIndex(3);
    ui->comboBox_2->setEnabled(true);
    if (ui->comboBox_2->currentIndex()<0) ui->comboBox_2->setCurrentIndex(0);
    ui->comboBox_3->setEnabled(true);
    if (ui->comboBox_3->currentIndex()<0) ui->comboBox_3->setCurrentIndex(2);
    ui->comboBox_4->setEnabled(true);
    if (ui->comboBox_4->currentIndex()<0) ui->comboBox_4->setCurrentIndex(0);
    ui->comboBox_5->setEnabled(true);
    if (ui->comboBox_5->currentIndex()<0) ui->comboBox_5->setCurrentIndex(1);
    ui->comboBox_6->setEnabled(true);
    if (ui->comboBox_6->currentIndex()<1) ui->comboBox_6->setCurrentIndex(1);
    ui->comboBox_7->setEnabled(true);
    if (ui->comboBox_7->currentIndex()<1) ui->comboBox_7->setCurrentIndex(0);
    ui->comboBox_8->setEnabled(true);
    if (ui->comboBox_8->currentIndex()<1) ui->comboBox_8->setCurrentIndex(0);

    ui->lineEdit_2->setEnabled(true);
    if (!(ui->lineEdit_2->text().length())) ui->lineEdit_2->setText("0");
    ui->lineEdit_3->setEnabled(true);
    if (!(ui->lineEdit_3->text().length())) ui->lineEdit_3->setText("23");

//    ui->getParButton->setEnabled(true);
    ui->loadPresetButton->setEnabled(true);
    ui->setParButton->setEnabled(true);

    ui->label_1->setEnabled(true);
    ui->label_2->setEnabled(true);
    ui->label_3->setEnabled(true);
    ui->label_4->setEnabled(true);
    ui->label_5->setEnabled(true);
    ui->label_6->setEnabled(true);
    ui->label_7->setEnabled(true);
    ui->label_8->setEnabled(true);
    ui->label_9->setEnabled(true);
    ui->label_10->setEnabled(true);
}

void MainWindow::editDisable()
{
    ui->comboBox_1->setEnabled(false);
    ui->comboBox_2->setEnabled(false);
    ui->comboBox_3->setEnabled(false);
    ui->comboBox_4->setEnabled(false);
    ui->comboBox_5->setEnabled(false);
    ui->comboBox_6->setEnabled(false);
    ui->comboBox_7->setEnabled(false);
    ui->comboBox_8->setEnabled(false);

    ui->lineEdit_2->setEnabled(false);
    ui->lineEdit_3->setEnabled(false);

    ui->getParButton->setEnabled(false);
    ui->setParButton->setEnabled(false);
    ui->savePresetButton->setEnabled(false);
    ui->loadPresetButton->setEnabled(false);

    ui->label_1->setEnabled(false);
    ui->label_2->setEnabled(false);
    ui->label_3->setEnabled(false);
    ui->label_4->setEnabled(false);
    ui->label_5->setEnabled(false);
    ui->label_6->setEnabled(false);
    ui->label_7->setEnabled(false);
    ui->label_8->setEnabled(false);
    ui->label_9->setEnabled(false);
    ui->label_10->setEnabled(false);
}


void MainWindow::on_openPortButton_clicked()
{
    if (ui->openPortButton->text() == "Open Port")
    {
        QString portname = ui->lineEdit_1->text();
        if (portname.length())
        {
            comport->setPortName(portname);
            comport->setBaudRate(QSerialPort::Baud9600);
            comport->setDataBits(QSerialPort::Data8);
            comport->setParity(QSerialPort::NoParity);
            comport->setStopBits(QSerialPort::OneStop);
            comport->setFlowControl(QSerialPort::NoFlowControl);
            if (!comport->isOpen())
            {
                comport->open(QIODevice::ReadWrite);
                ui->openPortButton->setText("Close Port");
                ui->lineEdit_1->setEnabled(false);
                ui->getParButton->setEnabled(true);
                ui->textBrowser->append("Port is open!");
                //editEnable();
            }
            else QMessageBox::information(0, "INFO", "Port is already open!");
        }
        else QMessageBox::critical(0, "ERROR", "Incorrect Port!");
    }
    else
    {
        comport->clear();
        comport->close();
        ui->openPortButton->setText("Open Port");
        ui->lineEdit_1->setEnabled(true);
        editDisable();
        ui->textBrowser->clear();
        modelGot = false;
        paramSet = false;
        rxData.clear();
    }
}


void MainWindow::on_getParButton_clicked()
{
    if (modelGot) requestParam();
    else requestModel();
//    editEnable();
}


void MainWindow::on_setParButton_clicked()
{
    char params[6];
    QString str;
    QByteArray par;
    uint8_t ind;

    collectParam(params);
    str = "Parameters are SET!\nNew parameters: ";
    par.clear();
    for (ind=0; ind<6; ind++) par.append(params[ind]);
    str.append(par.toHex());
    paramSet = true;
    rxData.clear();
    QMessageBox::information(0, "INFO", str);
    comport->clear();
    comport->write(params, 6);
//    comport->waitForReadyRead(20);
}


void MainWindow::on_loadPresetButton_clicked()
{
    char params[6];
    QByteArray par;
    QString filename;

    filename = QFileDialog::getOpenFileName(this, QString::fromUtf8("Open"), QDir::currentPath(), "Config files (*.ini *.conf);;All files (*.* *)");
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly))
    {
        qWarning("Cannot open file");
    }
    else
    {
        QTextStream in(&file);
 //       preset.clear();
 //       preset.append(in.readLine());
        par.append(in.readLine());
        par = par.fromHex(par);
        memcpy(params, par, 6);
        parseParam(params);
    }
    file.close();
}


void MainWindow::on_savePresetButton_clicked()
{
    QString filename;
    char params[6];
    QByteArray par;
    uint8_t i;

    filename = (QFileDialog::getSaveFileName(this, QString::fromUtf8("Save"), QDir::currentPath(), "Config files (*.ini *.conf);;All files (*.* *)"));
    if (!(filename.endsWith(".ini"))) filename.append(".ini");
    QFile file(filename);
    if (file.open(QIODevice::WriteOnly))
    {
        QTextStream out(&file);
        collectParam(params);
        par.clear();
        for (i=0; i<6; i++) par.append(params[i]);
        out << par.toHex() << Qt::endl;
    }
    else
    {
        qWarning("Could not open file");
    }
    file.close();
}

