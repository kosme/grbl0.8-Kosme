#ifndef DEPTHPROBE_H
#define DEPTHPROBE_H

#include <QMainWindow>
#include <QFileDialog>
#include <QQueue>
#include <QLinkedList>
#include <QFile>
#include <QTextStream>
#include "rs232.h"

namespace Ui {
class depthProbe;
}

class depthProbe : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit depthProbe(QWidget *parent = 0);
    ~depthProbe();

private slots:
    void openFile();
    void openPort();
    void Run();
    void Zero();
#ifdef DEBUG
    void button();
#endif
    
private:
    Ui::depthProbe *ui;
    //QQueue<QString> commands;
    QLinkedList<QString> commands;
    QLinkedList<QString> edited;
    int port_nr;
    int origSize;
    double depth;
    boolean portOpen;
    boolean zeroed;
    char buf[4096];
    char line[40];
    RS232 port;
    QString styleSheet;
    void getZero();
    double getRealZero(double X, double Y, bool reset);
    void updateBar(double tam, double orig);
    double depthProbeZero();
    void sendSerial(QString message);
    QString readSerial(boolean flush);
    void pause(long milis);
};

#endif // DEPTHPROBE_H
