/********************************************************************************
** Form generated from reading UI file 'depthprobe.ui'
**
** Created: Tue 28. Aug 16:12:27 2012
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DEPTHPROBE_H
#define UI_DEPTHPROBE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_depthProbe
{
public:
    QWidget *centralWidget;
    QGroupBox *grpFile;
    QPushButton *btnBegin;
    QLineEdit *filePath;
    QPushButton *btnOpenFile;
    QGroupBox *grpPort;
    QPushButton *btnOpenPort;
    QComboBox *cmbPort;
    QGroupBox *grpUnits;
    QRadioButton *rdbMm;
    QRadioButton *rdbInch;
    QProgressBar *progressBar;
    QPushButton *btnZero;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *depthProbe)
    {
        if (depthProbe->objectName().isEmpty())
            depthProbe->setObjectName(QString::fromUtf8("depthProbe"));
        depthProbe->resize(382, 300);
        centralWidget = new QWidget(depthProbe);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        grpFile = new QGroupBox(centralWidget);
        grpFile->setObjectName(QString::fromUtf8("grpFile"));
        grpFile->setEnabled(false);
        grpFile->setGeometry(QRect(150, 20, 211, 81));
        btnBegin = new QPushButton(grpFile);
        btnBegin->setObjectName(QString::fromUtf8("btnBegin"));
        btnBegin->setEnabled(false);
        btnBegin->setGeometry(QRect(110, 50, 75, 23));
        filePath = new QLineEdit(grpFile);
        filePath->setObjectName(QString::fromUtf8("filePath"));
        filePath->setGeometry(QRect(10, 20, 181, 20));
        btnOpenFile = new QPushButton(grpFile);
        btnOpenFile->setObjectName(QString::fromUtf8("btnOpenFile"));
        btnOpenFile->setGeometry(QRect(20, 50, 75, 23));
        grpPort = new QGroupBox(centralWidget);
        grpPort->setObjectName(QString::fromUtf8("grpPort"));
        grpPort->setGeometry(QRect(10, 20, 131, 81));
        btnOpenPort = new QPushButton(grpPort);
        btnOpenPort->setObjectName(QString::fromUtf8("btnOpenPort"));
        btnOpenPort->setGeometry(QRect(20, 50, 81, 21));
        cmbPort = new QComboBox(grpPort);
        cmbPort->setObjectName(QString::fromUtf8("cmbPort"));
        cmbPort->setGeometry(QRect(20, 20, 91, 22));
        grpUnits = new QGroupBox(centralWidget);
        grpUnits->setObjectName(QString::fromUtf8("grpUnits"));
        grpUnits->setEnabled(false);
        grpUnits->setGeometry(QRect(10, 110, 91, 61));
        rdbMm = new QRadioButton(grpUnits);
        rdbMm->setObjectName(QString::fromUtf8("rdbMm"));
        rdbMm->setGeometry(QRect(10, 20, 71, 17));
        rdbMm->setChecked(true);
        rdbInch = new QRadioButton(grpUnits);
        rdbInch->setObjectName(QString::fromUtf8("rdbInch"));
        rdbInch->setGeometry(QRect(10, 40, 71, 17));
        progressBar = new QProgressBar(centralWidget);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setGeometry(QRect(30, 210, 118, 23));
        progressBar->setValue(53);
        btnZero = new QPushButton(centralWidget);
        btnZero->setObjectName(QString::fromUtf8("btnZero"));
        btnZero->setEnabled(false);
        btnZero->setGeometry(QRect(210, 140, 81, 21));
        depthProbe->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(depthProbe);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 382, 20));
        depthProbe->setMenuBar(menuBar);
        mainToolBar = new QToolBar(depthProbe);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        depthProbe->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(depthProbe);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        depthProbe->setStatusBar(statusBar);

        retranslateUi(depthProbe);

        QMetaObject::connectSlotsByName(depthProbe);
    } // setupUi

    void retranslateUi(QMainWindow *depthProbe)
    {
        depthProbe->setWindowTitle(QApplication::translate("depthProbe", "depthProbe", 0, QApplication::UnicodeUTF8));
        grpFile->setTitle(QApplication::translate("depthProbe", "File", 0, QApplication::UnicodeUTF8));
        btnBegin->setText(QApplication::translate("depthProbe", "Begin", 0, QApplication::UnicodeUTF8));
        btnOpenFile->setText(QApplication::translate("depthProbe", "Open File", 0, QApplication::UnicodeUTF8));
        grpPort->setTitle(QApplication::translate("depthProbe", "Port", 0, QApplication::UnicodeUTF8));
        btnOpenPort->setText(QApplication::translate("depthProbe", "Open", 0, QApplication::UnicodeUTF8));
        grpUnits->setTitle(QApplication::translate("depthProbe", "Units", 0, QApplication::UnicodeUTF8));
        rdbMm->setText(QApplication::translate("depthProbe", "Millimeters", 0, QApplication::UnicodeUTF8));
        rdbInch->setText(QApplication::translate("depthProbe", "Inches", 0, QApplication::UnicodeUTF8));
        btnZero->setText(QApplication::translate("depthProbe", "Zero machine", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class depthProbe: public Ui_depthProbe {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DEPTHPROBE_H
