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
#ifdef DEBUG
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(button()));
    ui->pushButton->setVisible(true);
#else
    ui->pushButton->setVisible(false);
#endif
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
        pause(100);
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
    //getZero();
    getRealZero(0,0,true);
    zeroed=true;
}

void depthProbe::Run()
{
    if(!zeroed)
        Zero();
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
                realZero = getRealZero(X,Y,false);
                line.replace(QString::number(Z),QString::number(Z+realZero));
            }
        }
        write.operator <<(line);
        write.operator <<("\r\n");
        commands.removeFirst();
        updateBar(double(commands.size()),double(orig));
    }
    file.close();
    sendSerial("G00 X0 Y0 Z0\r");
    pause(50);
}

void depthProbe::getZero()
{
    port.flush(port_nr);
    double Zpos=0;
    QString comando,num;
    QString received = "";
    while(!received.contains("depth"))
    {
        comando="G00 Z";
        Zpos-=0.02;
        comando.append(num.setNum(Zpos)).append("\r");
        sendSerial(comando);
        pause(50);
        received = readSerial(false);
    }
    port.Reset(port_nr);
    sendSerial("G00 Z01\r");
    pause(100);
}

double depthProbe::getRealZero(double X, double Y,bool reset)
{
    port.flush(port_nr);
    double Zpos=.5;
    QString comando;
    QString received = "";
    comando = "G00 X";
    comando.append(QString::number(X)).append(" Y").append(QString::number(Y)).append("\r");
    sendSerial(comando);
    port.flush(port_nr);
    pause(50);
    while(!received.contains("depth"))
    {
        Zpos-=0.02;
        comando = "G00 Z";
        comando.append(QString::number(Zpos)).append("\r");
        sendSerial(comando);
        pause(50);
        received = readSerial(false);
    }
    if(reset)
        port.Reset(port_nr);
    sendSerial("G00 Z01\r");
    pause(2000);
    return(Zpos);
}

void depthProbe::updateBar(double tam, double orig)
{
    //double b = tam/orig;
    int a = (1-(tam/orig))*100;
    ui->progressBar->setValue(a);
}

double depthProbe::depthProbeZero()
{
    double half=2.56,level = 0;
    int i;
    QString received;
    sendSerial(QString("G00 Z").append(QString::number(level)).append("\r"));
    for(i=0;i<8;i++)
    {
        received=readSerial(true);
        half=half/2;
        if(received.contains("depth"))
        {
            level+=half;
        }
        else
        {
            level-=half;
        }
        sendSerial(QString("G00 Z").append(QString::number(level)).append("\r"));
    }
    return level;
}

void depthProbe::sendSerial(QString message)
{
    strncpy_s(line,message.toAscii().constData(),message.length());
#ifndef DISCONNECTED
    port.SendBuf(port_nr,line,message.length());
#endif
}

QString depthProbe::readSerial(boolean flush)
{
#ifndef DISCONNECTED
    int n;
    if(flush)
    {
        port.flush(port_nr);
        pause(50);
    }
    n=port.PollComport(port_nr,buf,4096);
    pause(250);
    if(n!=0)
        return QString(buf).left(n);
    else
        return "";
#else
    return "";
#endif
}

void depthProbe::pause(long milis)
{
#ifdef Q_WS_X11
        usleep(milis*1000);
#else
        Sleep(milis);
#endif
}

#ifdef DEBUG
void depthProbe::button()
{
    readSerial(true);
    sendSerial("$\r");
    QString popochas = readSerial(false);
    QMessageBox(QMessageBox::Critical,"Interaction needed",popochas,QMessageBox::Ok).exec();
}
#endif
