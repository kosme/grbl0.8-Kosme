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
    
private:
    Ui::depthProbe *ui;
    //QQueue<QString> commands;
    QLinkedList<QString> commands;
    QLinkedList<QString> edited;
    int port_nr;
    double depth;
    boolean portOpen;
    boolean zeroed;
    RS232 port;
    QString styleSheet;
    void getZero();
    double getRealZero(double X, double Y);
    void writeFile(QString line);
};

#endif // DEPTHPROBE_H
