#ifndef GLDATA_H
#define GLDATA_H

#include <string.h>
#include <qstring.h>
#include <qinputdialog.h>
#include <qlineedit.h>
#include "I:\MotionCapture\project\lib\CordAnm.h"
#include "I:\MotionCapture\project\lib\CASEParser.h"
#include "I:\MotionCapture\project\lib\CBVHParser.h"
#include "I:\MotionCapture\project\lib\GLPOS.h"
#include "I:\MotionCapture\project\lib\CVector3f.h"
#include "I:\MotionCapture\project\lib\TRCParser.h"
#include "I:\MotionCapture\project\lib\def.h"

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

    int frameNum;
    int boneNum;
    int *parent_of;


    CVector3f *data;


    //play
    QString objectName;
    int curr;//curr_frame
    bool pause;//pause = true
    float root_offset[3];//root pos offset x y z

};

#endif // GLDATA_H
