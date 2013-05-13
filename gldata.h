#ifndef GLDATA_H
#define GLDATA_H

#include <string.h>
#include <qstring.h>
#include "F:\MotionCapture\project\lib\CordAnm.h"
#include "F:\MotionCapture\project\lib\CASEParser.h"
#include "F:\MotionCapture\project\lib\CBVHParser.h"
#include "F:\MotionCapture\project\lib\GLPOS.h"
#include "F:\MotionCapture\project\lib\CVector3f.h"

class GLData
{
public:
    GLData();
    ~GLData();
    void process(const char *dir,QString suffix);
    void destroy();
    void init();
public:
    bool valid;//it is valid when data are set
    bool DRAW_CURVE;

    int frameNum;
    int boneNum;
    int *parent_of;
    float *data;

    //play
    QString name;
    int curr;//curr_frame
    float bone_width;//bone width
    bool pause;//pause = true
    float root_offset[3];//root pos offset x y z
};

#endif // GLDATA_H
