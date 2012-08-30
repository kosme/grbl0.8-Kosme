#include "depthprobe.h"
#include "ui_depthprobe.h"

depthProbe::depthProbe(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::depthProbe)
{
    ui->setupUi(this);
#ifdef DISCONNECTED
    zeroed = true;
#else
    zeroed=false;
#endif
    connect(ui->btnOpenFile,SIGNAL(clicked()),this,SLOT(openFile()));
    connect(ui->btnOpenPort,SIGNAL(clicked()),this,SLOT(openPort()));
    connect(ui->btnBegin,SIGNAL(clicked()),this,SLOT(Run()));
    connect(ui->btnZero,SIGNAL(clicked()),this,SLOT(Zero()));
    portOpen = false;
#ifdef Q_WS_X11
    ui->cmbPort->addItem("USB0");
    ui->cmbPort->addItem("USB1");
    ui->cmbPort->addItem("USB2");
    ui->cmbPort->addItem("USB3");
    ui->cmbPort->addItem("USB4");
    ui->cmbPort->addItem("USB5");
    ui->cmbPort->addItem("ACM0");
    ui->cmbPort->addItem("ACM1");
    ui->cmbPort->addItem("ACM2");
    ui->cmbPort->addItem("ACM3");
    ui->cmbPort->addItem("ACM4");
    ui->cmbPort->addItem("ACM5");
#ifdef DEBUG
    ui->cmbPort->setCurrentIndex(6);
#endif
#else
    ui->cmbPort->addItem("COM1");
    ui->cmbPort->addItem("COM2");
    ui->cmbPort->addItem("COM3");
    ui->cmbPort->addItem("COM4");
    ui->cmbPort->addItem("COM5");
    ui->cmbPort->addItem("COM6");
    ui->cmbPort->addItem("COM7");
    ui->cmbPort->addItem("COM8");
    ui->cmbPort->addItem("COM9");
    ui->cmbPort->addItem("COM10");
    ui->cmbPort->addItem("COM11");
    ui->cmbPort->addItem("COM12");
    ui->cmbPort->addItem("COM13");
    ui->cmbPort->addItem("COM14");
    ui->cmbPort->addItem("COM15");
    ui->cmbPort->addItem("COM16");
#ifdef DEBUG
    ui->cmbPort->setCurrentIndex(11);
#endif
#endif
    styleSheet = ui->btnOpenPort->styleSheet();
}

depthProbe::~depthProbe()
{
    if(portOpen)
        port.CloseComport(port_nr);
    delete ui;
}

void depthProbe::openFile()
{
    ui->filePath->setText(QFileDialog::getOpenFileName(this,"Open file","",".Nc file (*.nc)"));
    if(ui->filePath->text()!="")
    {
        ui->btnBegin->setEnabled(true);
        ui->filePath->setReadOnly(true);
    }
    else
    {
        ui->btnBegin->setEnabled(false);
        ui->filePath->setReadOnly(false);
    }
}

void depthProbe::openPort()
{
    port_nr=ui->cmbPort->currentIndex();
    if(!portOpen)
    {
#ifndef DISCONNECTED
        port.Reset(port_nr);
#ifdef Q_WS_X11
            usleep(100000);  // sleep for 100 milliSeconds
#else
            Sleep(100);
#endif
        if(!port.OpenComport(port_nr))
        {
            ui->grpFile->setEnabled(true);
            ui->btnZero->setEnabled(true);
            portOpen=true;
            ui->btnOpenPort->setText("Close");
            ui->btnOpenPort->setStyleSheet("* { background-color: rgb(255,125,100) }");
            ui->cmbPort->setEnabled(false);
        }
        else
            QMessageBox(QMessageBox::Critical,"Error","Could not open port.",QMessageBox::Ok).exec();
#else
        portOpen=true;
        ui->grpFile->setEnabled(true);
        ui->btnZero->setEnabled(true);
        ui->btnOpenPort->setText("Close");
        ui->btnOpenPort->setStyleSheet("* { background-color: rgb(255,125,100) }");
        ui->cmbPort->setEnabled(false);
#endif
    }
    else
    {
#ifndef DISCONNECTED
        port.CloseComport(port_nr);
#endif
        ui->cmbPort->setEnabled(true);
        ui->btnOpenPort->setText("Open");
        ui->btnOpenPort->setStyleSheet(styleSheet);
        ui->grpFile->setEnabled(false);
        ui->btnZero->setEnabled(false);
        portOpen=false;
    }
}

void depthProbe::Zero()
{
    getZero();
    zeroed=true;
}

void depthProbe::Run()
{
    if(!zeroed)
        getZero();
    double realZero;
    double X,Y,Z;
    int orig;
    QString line = "";
    QFile file(ui->filePath->text());
    file.open(QFile::ReadOnly);
    QTextStream code(&file);
    while(!code.atEnd())
    {
        commands.append(code.readLine());
    }
    file.close();
    commands.removeAll("");
    origSize = commands.size();
    orig = commands.size();
    file.open(QFile::WriteOnly);
    QTextStream write(&file);
    while(!commands.empty())
    {
        line = commands.first();
        if((line.toUpper().contains("X"))&&(line.toUpper().contains("Y")))
        {
            QString temp = line.mid(line.toUpper().indexOf("X"));
            X = temp.mid(1,temp.indexOf(" ")).toDouble();
            temp = temp.mid(temp.toUpper().indexOf("Y"));
            Y = temp.mid(1,temp.indexOf(" ")).toDouble();
        }
        if(line.toUpper().contains("Z"))
        {
            if(line.lastIndexOf(" ")>line.toUpper().indexOf("Z"))
            {
                QString temp = line.mid(line.toUpper().indexOf("Z")+1);
                Z=temp.mid(0,temp.indexOf(" ")).toDouble();
            }
            else
                Z=line.mid(line.toUpper().indexOf("Z")+1).toDouble();
            if(Z<0)//edit depth
            {
                realZero = getRealZero(X,Y);
                line.replace(QString::number(Z),QString::number(Z+realZero));
            }
        }
        write.operator <<(line);
        write.operator <<("\r\n");
        commands.removeFirst();
        updateBar(double(commands.size()),double(orig));
    }
    file.close();
    char buf[] = "G00 X0 Y0 Z0\r";
    port.SendBuf(port_nr,buf,13);
#ifdef Q_WS_X11
        usleep(50000);  // sleep for 100 milliSeconds
#else
        Sleep(50);
#endif
}

void depthProbe::getZero()
{
    port.flush(port_nr);
    double Zpos=0;
    int n,i;
    QString comando,num;
    QString received = "";
    char buf[40]="";
    char line[40]="";
    while(!received.contains("depth"))
    {
        comando="G00 Z";
        Zpos-=0.02;
        comando.append(num.setNum(Zpos)).append("\r");
        for(i=0;i<comando.length();i++)
            line[i]=comando.at(i).toAscii();
        port.SendBuf(port_nr,line,i);
#ifdef Q_WS_X11
        usleep(50000);  // sleep for 100 milliSeconds
#else
        Sleep(50);
#endif
        n=port.PollComport(port_nr,buf,40);
        if(n!=0)
            received=QString(buf);
    }
    port.Reset(port_nr);
#ifdef Q_WS_X11
        usleep(5000000);  // sleep for 100 milliSeconds
#else
        Sleep(5000);
#endif
    n=port.PollComport(port_nr,buf,40);
    received=QString(buf);
    comando="G00 Z01\r";
    for(i=0;i<comando.length();i++)
        line[i]=comando.at(i).toAscii();
    port.SendBuf(port_nr,line,i);
#ifdef Q_WS_X11
        usleep(100000);  // sleep for 100 milliSeconds
#else
        Sleep(100);
#endif
}

double depthProbe::getRealZero(double X, double Y)
{
    port.flush(port_nr);
    double Zpos=.5;
    int n,i;
    QString comando;
    QString received = "";
    char buf[40];
    char line[40];
    //Bcomando = "G00 X"+X+" Y"+Y+"\r";
    comando = "G00 X";
    comando.append(QString::number(X)).append(" Y").append(QString::number(Y)).append("\r");
    for(i=0;i<comando.length();i++)
        line[i]=comando.at(i).toAscii();
    port.SendBuf(port_nr,line,i);
    port.flush(port_nr);
#ifdef Q_WS_X11
    usleep(50000);  // sleep for 100 milliSeconds
#else
    Sleep(50);
#endif
    while(!received.contains("depth"))
    {
        Zpos-=0.02;
        //comando="G00 Z"+Zpos+"\r";
        comando = "G00 Z";
        comando.append(QString::number(Zpos)).append("\r");
        //comando.append(num.setNum(Zpos)).append("\r");
        for(i=0;i<comando.length();i++)
            line[i]=comando.at(i).toAscii();
        port.SendBuf(port_nr,line,i);
#ifdef Q_WS_X11
        usleep(50000);  // sleep for 100 milliSeconds
#else
        Sleep(50);
#endif
        n=port.PollComport(port_nr,buf,40);
        if(n!=0)
            received=QString(buf);
    }
    comando="G00 Z01\r";
    for(i=0;i<comando.length();i++)
        line[i]=comando.at(i).toAscii();
    port.SendBuf(port_nr,line,i);
#ifdef Q_WS_X11
        usleep(2000000);  // sleep for 100 milliSeconds
#else
        Sleep(2000);
#endif
        return(Zpos);
}

void depthProbe::updateBar(double tam, double orig)
{
    double b = tam/orig;
    int a = (1-(tam/orig))*100;
    ui->progressBar->setValue(a);
}
