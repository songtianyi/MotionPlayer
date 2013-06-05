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

#include "I:\MotionCapture\project\lib\CQuaternion.h"
#include "I:\MotionCapture\project\lib\HMat44.h"
#include "I:\\MotionCapture\\project\\lib\\CBVHParser.h"
#include "I:\\MotionCapture\\project\\lib\\CVector3f.h"
#include "I:\\MotionCapture\\project\\lib\\CQuaternion.h"
#include "I:\\MotionCapture\\project\\lib\\CQuatInterp.h"
#include "I:\\MotionCapture\\project\\lib\\CASEParser.h"
#include "I:\\MotionCapture\\project\\lib\\AAP.h"
#include "I:\\MotionCapture\\project\\lib\\Bezier.h"
#include "I:\\MotionCapture\\project\\lib\\Huffman.h"
#include "I:\\MotionCapture\\project\\lib\\FPC.h"
#include "I:\\MotionCapture\\project\\lib\\GLPOS.h"
#include "I:\\MotionCapture\\project\\lib\\SplineInterp.h"
#include "I:\\MotionCapture\\project\\lib\\CordAnm.h"
#include "I:\\MotionCapture\\project\\lib\\func.h"
#include "I:\\MotionCapture\\project\\lib\\def.h"
#include "I:\\MotionCapture\\project\\lib\\TRCParser.h"

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
    void newMain();

    void drawGround();

    void IK(const int frameIndex,const int boneIndex,\
                    const int *parent_of,\
                    const AnmHeader *pAnmHead,const ActionHeader *pActHead,\
                    CQuaternion *rotkey,const CVector3f *poskey,\
                    CVector3f *right,CVector3f *ratio4);

    void IK(const int frameIndex,int boneIndex,\
                    const int *parent_of,\
                    const AnmHeader *pAnmHead,const ActionHeader *pActHead,\
                    CQuaternion *rotkey,const CVector3f *poskey,\
                    CVector3f *right,CVector3f *ratio4,const int *child);
void drawProcess(CVector3f toe1,CVector3f foot0,CVector3f toe0);

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

    //temp
    ActionHeader *actHead;
    AnmHeader *anmHead;
    HAnmHeader *hanmHead;
    int nClipNum;
    CQuaternion *rotkey;
    CAAP *aapkey;
    CQuaternion *hrotkey;
    CAAP *haapkey;
    CVector3f *poskey;
    CVector3f *hposkey;
    int *father;
    CVector3f *ratio4;
    CVector3f *right;

    int childR[1000];
    int childL[1000];


};
#endif // OPENGL_H
