/**
 * opengl.h
 *
 * Copyright (c) 2013, Dalian Nationalities University. All Rights  Reserved.
 * Tianyi Song <songtianyi630@163.com>
 *
 * You can use this file in your project, but do not redistribute it and/or modify
 * it.
 *
 */


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

    void _drawLine(const CVector3f s,const CVector3f e);
    void _drawPoint(const CVector3f p,const float pointSize);
    void _drawQuad(const CVector3f p1,const CVector3f p2,const CVector3f p3,const CVector3f p4);

    void drawObject(const int);
    void drawCurve(const int);
    void drawCoord();

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

    void setScaleFactor(int);




public:

    float x_angle,y_angle,radio;
    int ground_size;


    GLData array[2];//one for each

    QTimer timer;
    QFont textFont;//common

    float scale_factor;

    bool showBoneNum;
    bool showCoord;

};
#endif // OPENGL_H
