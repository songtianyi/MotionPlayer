#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtDebug>
#include <stdio.h>
#include <QGLWidget>
#include "qlayout.h"
#include "qtimer.h"
#include "qlist.h"
#include "QFileInfo"
#include "qurl.h"
#include "qmessagebox.h"
#include <qmimedata.h>
#include "iostream"
#include "qstring.h"
#include "opengl.h"
#include "qstatusbar.h"
#include "QScrollBar"
#include "qslider.h"
#include "qradiobutton.h"
#include "qdial.h"
#include "qcheckbox.h"
#include "qpushbutton.h"
#include "qfiledialog.h"
#include "qlabel.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void create();


signals:
    void statusStr(QString s,int sec = (1<<13));



protected slots:
    void openFileA();
    void openFileB();

    void setInitData(const int index,const int sliderValue,const int sliderMax,const int dialValue);
    void setSliderValue(const int index, const int v);

    void setStatusFrameA(int);
    void setStatusFrameB(int);



private:
    void openFile(bool isA);

    void keyPressEvent(QKeyEvent *e);

    void setStatusOffsetValue(const float *offset,const int index);

private:

    OpenGL      *glWidget;
    QStatusBar  *statusBar;

    QVBoxLayout *vbl;
    QHBoxLayout *hbl;
    QVBoxLayout *vblCpL;
    QGridLayout *gblCpR;


    QWidget     *centralWidget;

    QSlider sb[2];//two slider

    QDial dl[3];//speed,color,lineWidth

    QCheckBox cb[2];//checkbox
    QPushButton bn[2];

    QLabel sl[1+8+4];//status label


    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
