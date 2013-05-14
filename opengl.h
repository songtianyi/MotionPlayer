#ifndef OPENGL_H
#define OPENGL_H

#include <QGLWidget>
#include <QObject>
#include <stdio.h>
#include <math.h>
#include "qevent.h"
#include "gldata.h"
#include "qtimer.h"
#include "omp.h"

#ifndef GL_SIZE_WIDTH
#define GL_SIZE_WIDTH 1000
#endif

#ifndef GL_SIZE_HEIGHT
#define GL_SIZE_HEIGHT 575
#endif


class OpenGL : public QGLWidget
{
    Q_OBJECT
public:
    explicit OpenGL(QWidget *parent = 0);
    ~OpenGL();
    void getObjectData(const char *dir,const QString suffix,const int index);
    void destroyObject(const int index);

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width,int height);

    void paintObject(const int);
    void paintCurve(const int);
    void paintCoord();

    void drawGround();






signals:


    void initDataChanged(const int index,const int sliderValue,const int sliderMax,const int dialValue);
    void sliderValueChanged(const int index,const int v);

     void statusStr(QString s,int sec = (1<<13));


public slots:

    void setCurrFrameA(int);
    void setCurrFrameB(int);

    void setTimeInterval(int);


    void setGroundSize(int);

    void commonUpdate();




public:

    float x_angle,y_angle,radio;
    int ground_size;


    GLData array[2];//one for each

    QTimer timer;
    QFont textFont;//common



};
#endif // OPENGL_H
