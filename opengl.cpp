#include "opengl.h"

OpenGL::OpenGL(QWidget *parent) :
    QGLWidget(parent)
{   
        x_angle = 0.0f;
        y_angle = 0.0f;
        radio = 10.0f;

        scale_factor = 1.0f;

        ground_size = 5;

        initializeGL();
        resizeGL(GL_SIZE_WIDTH, GL_SIZE_HEIGHT);


        QObject::connect(&timer,SIGNAL(timeout()),this,SLOT(commonUpdate()));
        timer.setInterval(30);


        //font set
        textFont.setBold(true);
        textFont.setPixelSize(21);

        newMain();
        int temp[100]  = {1,6,-1,-1,-1,-1,7,8,9,-1};
        memcpy(childR,temp,sizeof(int)*100);

}


void OpenGL::setGroundSize(int v)
{
    ground_size = v;
    updateGL();
}
void OpenGL::setScaleFactor(int v)
{
    scale_factor = v *1.0f / 1000;
    updateGL();
}
void OpenGL::commonUpdate()
{
    updateGL();//play curr frame


    for(int i = 0;i < 2;i++)
    {

        if(array[i].pause)
            continue;

        array[i].curr++;//ready for next frame

        if(array[i].curr >= array[i].frameNum)
            array[i].curr = 0;//cyclic

        sliderValueChanged(i,array[i].curr);
    }


}

void OpenGL::setCurrFrameA(int v)
{
    array[0].curr = v;
    updateGL();
}
void OpenGL::setCurrFrameB(int v)
{
    array[1].curr = v;
    updateGL();
}
void OpenGL::setTimeInterval(int v)
{
    timer.setInterval(v);
}

void OpenGL::destroyObject(const int index)
{
    array[index].destroy();
    array[index].init();
    statusStr("Object Destroied");
}

void OpenGL::getObjectData(const char *dir,const QString suffix,const int index)
{

    //get new data,and the same time destroy the old data
    array[index].process(dir,suffix);

    //new object init
    if(array[index].name.size() == 0)
    {
        //not set yet,so give the object a default
        array[index].name = index == 0?"A":"B";
    }
    timer.start();
    initDataChanged(index,0,array[index].frameNum,timer.interval());
}

void OpenGL::_drawLine(const CVector3f s, const CVector3f e)
{
    glBegin(GL_LINES);
    glVertex3f(s.x,s.y,s.z);
    glVertex3f(e.x,e.y,e.z);//
    glEnd();
}
void OpenGL::_drawPoint(const CVector3f p, const float pointSize)
{
    glPointSize(pointSize);
    glBegin(GL_POINTS);
    glVertex3f(p.x,p.y,p.z);//
    glEnd();
}
void OpenGL::_drawQuad(const CVector3f p1, const CVector3f p2, const CVector3f p3, const CVector3f p4)
{
    glBegin(GL_QUADS);
    glVertex3f(p1.x, p1.y, p1.z);
    glVertex3f(p2.x, p2.y, p2.z);
    glVertex3f(p3.x, p3.y, p3.z);
    glVertex3f(p4.x, p4.y, p4.z);
    glEnd();
}


void OpenGL::drawGround()
{
    for(int i = 1;i < ground_size;i++)
    {
        for(int j = 1;j < ground_size;j++)
       {
            if((i+j)&1)
                glColor3f(0.7,0.7,0.7);
            else
                glColor3f(0.4,0.4,0.4);
            glBegin(GL_QUADS);
            glVertex3f(i, j, 0.0f);
            glVertex3f(i-1, j, 0.0f);
            glVertex3f( i-1, j-1, 0.0f);
            glVertex3f( i, j-1, 0.0f);
            glEnd();

            glBegin(GL_QUADS);
            glVertex3f(-i, j, 0.0f);
            glVertex3f(-i+1, j, 0.0f);
            glVertex3f( -i+1, j-1, 0.0f);
            glVertex3f( -i, j-1, 0.0f);
            glEnd();

            glBegin(GL_QUADS);
            glVertex3f(i, -j, 0.0f);
            glVertex3f(i-1, -j, 0.0f);
            glVertex3f( i-1, -j+1, 0.0f);
            glVertex3f( i, -j+1, 0.0f);
            glEnd();

            glBegin(GL_QUADS);
            glVertex3f(-i, -j, 0.0f);
            glVertex3f(-i+1, -j, 0.0f);
            glVertex3f( -i+1, -j+1, 0.0f);
            glVertex3f( -i, -j+1, 0.0f);
            glEnd();
        }
    }

}
void OpenGL::drawObject(const int j)
{
    glPushMatrix();
    glTranslatef(array[j].root_offset[0],array[j].root_offset[1],array[j].root_offset[2]);
    int index = array[j].curr*array[j].boneNum;

    glColor3f(1,1,0);//yellow text
    glLineWidth(3);//bone width
    renderText(
         array[j].data[index].x * scale_factor,\
         array[j].data[index].y * scale_factor,\
         array[j].data[index].z * scale_factor+1.7* scale_factor,array[j].name,textFont);


    for(int i  = 0;i < array[j].boneNum;i++)
    {
        //
        if(i == 1)continue;

        if(array[j].parent_of[i] == -1)
        {
            //
            glPointSize(3.1);
            glBegin(GL_POINTS);
            glColor3f(1,1,0);//yellow point
            glVertex3f(
                    array[j].data[index + i].x * scale_factor,\
                    array[j].data[index + i].y * scale_factor,\
                    array[j].data[index + i].z * scale_factor);
            glEnd();
            glColor3f(0.5,0.5,0.5);
            textFont.setPixelSize(9);
            renderText(array[j].data[index + i].x * scale_factor,\
                       array[j].data[index + i].y * scale_factor,\
                       array[j].data[index + i].z * scale_factor,\
                       QString::number(i),textFont);
            textFont.setPixelSize(21);
            continue;
        }

        int parent_index = array[j].parent_of[i];
        int child_index = i;

        glColor3f(0.0f, 0.0f, 1.0f);//blue bone
        glBegin(GL_LINES);
        glVertex3f(
                array[j].data[index + parent_index].x * scale_factor,\
                array[j].data[index + parent_index].y * scale_factor,\
                array[j].data[index + parent_index].z * scale_factor);
        glVertex3f(
                array[j].data[index + child_index].x * scale_factor,\
                array[j].data[index + child_index].y * scale_factor,\
                array[j].data[index + child_index].z * scale_factor);
        glEnd();

        //
        glPointSize(3.1);
        glBegin(GL_POINTS);
        glColor3f(1,1,0);//yellow point
        glVertex3f(
                array[j].data[index + parent_index].x * scale_factor,\
                array[j].data[index + parent_index].y * scale_factor,\
                array[j].data[index + parent_index].z * scale_factor);
        glEnd();

        glColor3f(0.5,0.5,0.5);
        textFont.setPixelSize(9);

        renderText(array[j].data[index + child_index].x * scale_factor,\
                   array[j].data[index + child_index].y * scale_factor,\
                   array[j].data[index + child_index].z * scale_factor,\
                   QString::number(i),textFont);

        textFont.setPixelSize(21);


  }
  glPopMatrix();
}
void OpenGL::drawProcess(CVector3f toe1,CVector3f foot0,CVector3f toe0)
{
    glPointSize(1);
    glBegin(GL_LINES);
    glVertex3f(0,0,0);
    glVertex3f(foot0.x,foot0.y,foot0.z);//
    glEnd();

    glBegin(GL_LINES);
    glVertex3f(foot0.x,foot0.y,foot0.z);
    glVertex3f(toe0.x,toe0.y,toe0.z);//
    glEnd();

    glBegin(GL_POINTS);
    glColor3f(1,1,0);//yellow point
    glVertex3f(0,0,0);
    glEnd();
    glBegin(GL_POINTS);
    glColor3f(foot0.x,foot0.y,foot0.z);//yellow point
    glVertex3f(0,0,0);
    glEnd();
    glBegin(GL_POINTS);
    glColor3f(1,1,0);//yellow point
    glVertex3f(toe0.x,toe0.y,toe0.z);
    glEnd();

    QFont textFont;
    renderText(
         toe1.x,\
         toe1.y,\
         toe1.z,"*",textFont);
}
void OpenGL::drawCoord()
{
    textFont.setBold(false);
    glColor3f(1,1,0);//yellow coord
    glBegin(GL_LINES);
    glVertex3f(0,0,0);
    glVertex3f(1,0,0);//x-axis
    glEnd();
    renderText(1,0,0
 ,"X",textFont);

    glBegin(GL_LINES);
    glVertex3f(0,0,0);
    glVertex3f(0,1,0);//y-axis
    glEnd();
    renderText(0,1,0
 ,"Y",textFont);

    glBegin(GL_LINES);
    glVertex3f(0,0,0);
    glVertex3f(0,0,1);//z-axis
    glEnd();
    renderText(0,0,1
 ,"Z",textFont);

            //假设calf的位置是 0  0 0

            //构造calf的旋转量，正确的旋转量是从文件中找的
            CQuaternion qCalf0(0.78f,0.55f,0.55f,0.8165f);//wrong
            qCalf0.normalize();
            CQuaternion qCalf1(0.78f,0.55f,0.55f,0.0165f);//right
            qCalf1.normalize();


            HMat44 qCalf1m = qCalf1.toHMat44();
            HMat44 qCalf0m = qCalf0.toHMat44();//转换成矩阵

            //构造foot的旋转量，正确的旋转量是从文件中找的
            CQuaternion qfoot0(0.78f,0.55f,0.55f,0.0065f);//wrong
            qfoot0.normalize();
            CQuaternion qfoot1( 0.78f,0.55f,0.55f,0.206f);//right
            qfoot1.normalize();


            HMat44 qfoot1m = qfoot1.toHMat44();
            HMat44 qfoot0m = qfoot0.toHMat44();//转换成矩阵


            //calf to foot
            FLOAT  Tc2f[16] ={ \
                    1,0,0,2.0f,\
                    0,1,0,1.56f,\
                    0,0,1,1.0f,\
                    0,0,0,1

            };//T

            //foot to toe
            FLOAT  Tf2t[16] ={ \
                    1,0,0,2.0f,\
                    0,1,0,1.56f,\
                    0,0,1,1.0f,\
                    0,0,0,1

            };//T

            FLOAT foot0[16],foot1[16];
            FLOAT toe0[16],toe1[16];
            __matrixMultiXYZ(foot0,(const FLOAT *)qCalf0m.mat,(const FLOAT *)Tc2f,4,4,4);//R * T
            __matrixMultiXYZ(foot1,(const FLOAT *)qCalf1m.mat,(const FLOAT *)Tc2f,4,4,4);


            __matrixMultiXYZ(toe0,(const FLOAT *)foot0,(const FLOAT *)qfoot0m.mat,4,4,4);//R * T * R
            __matrixMultiXYZ(toe1,(const FLOAT *)foot1,(const FLOAT *)qfoot1m.mat,4,4,4);
            __matrixMultiXYZ(toe0,(const FLOAT *)toe0,(const FLOAT *)Tf2t,4,4,4);//R * T * R * T
            __matrixMultiXYZ(toe1,(const FLOAT *)toe1,(const FLOAT *)Tf2t,4,4,4);

            //输出初始位置和距离
            //printf("%.9f\n",euclideanDist(CVector3f(toe0[3],toe0[7],toe0[11]),\
            //                        CVector3f(toe1[3],toe1[7],toe1[11])));
            printf("%.9f\n",euclideanDist(CVector3f(foot0[3],foot0[7],foot0[11]),\
                                    CVector3f(foot1[3],foot1[7],foot1[11])));
            drawProcess(CVector3f(toe1[3],toe1[7],toe1[11]),\
                     CVector3f(foot0[3],foot0[7],foot0[11]),\
                     CVector3f(toe0[3],toe0[7],toe0[11]));
            renderText(
                 toe0[3],\
                 toe0[7],\
                 toe0[11],QString::number(0),textFont);


            //循环矫正
            int trys = 0;
            while(trys < 10)
            {
                /*
                    CVector3f root(foot0[3],foot0[7],foot0[11]);
                    CVector3f curEnd(toe0[3],toe0[7],toe0[11]);
                    CVector3f desireEnd(toe1[3],toe1[7],toe1[11]);
                    */
                CVector3f root(0,0,0);
                CVector3f curEnd(foot0[3],foot0[7],foot0[11]);
                CVector3f desireEnd(foot1[3],foot1[7],foot1[11]);

                    CQuaternion res = CCD(root,curEnd,desireEnd);

                    qCalf0 *= res.conjugated();
                    qCalf0m = qCalf0.toHMat44();


                    //重新计算错误脚趾矫正后的位置
                    __matrixMultiXYZ(foot0,(const FLOAT *)qCalf0m.mat,(const FLOAT *)Tc2f,4,4,4);
                    __matrixMultiXYZ(toe0,(const FLOAT *)foot0,(const FLOAT *)qfoot0m.mat,4,4,4);
                    __matrixMultiXYZ(toe0,(const FLOAT *)toe0,(const FLOAT *)Tf2t,4,4,4);

                    //printf("%.9f\n",euclideanDist(CVector3f(toe0[3],toe0[7],toe0[11]),\
                    //                        CVector3f(toe1[3],toe1[7],toe1[11])));
                    printf("%.9f\n",euclideanDist(CVector3f(foot0[3],foot0[7],foot0[11]),\
                                            CVector3f(foot1[3],foot1[7],foot1[11])));
                    if(trys < 5)
                    {
                        drawProcess(CVector3f(toe1[3],toe1[7],toe1[11]),\
                                 CVector3f(foot0[3],foot0[7],foot0[11]),\
                                 CVector3f(toe0[3],toe0[7],toe0[11]));
                    renderText(
                         toe0[3],\
                         toe0[7],\
                         toe0[11],QString::number(++trys),textFont);
                    renderText(
                         foot1[3],\
                         foot1[7],\
                         foot1[11],"*",textFont);
                    }
                    else
                        trys++;

/*
                    //将根节点变为Calf继续矫正
                    root.setXYZ(foot0[3],foot0[7],foot0[11]);
                    curEnd.setXYZ(toe0[3],toe0[7],toe0[11]);
                    desireEnd.setXYZ(toe1[3],toe1[7],toe1[11]);
                    res = CCD(root,curEnd,desireEnd);
                    qCalf0 = qCalf0 * res.conjugated();
                    qCalf0m = qCalf0.toHMat44();
                    //重新计算错误脚趾矫正后的位置
                    __matrixMultiXYZ(foot0,(const FLOAT *)qCalf0m.mat,(const FLOAT *)Tc2f,4,4,4);
                    __matrixMultiXYZ(toe0,(const FLOAT *)foot0,(const FLOAT *)qfoot0m.mat,4,4,4);
                    __matrixMultiXYZ(toe0,(const FLOAT *)toe0,(const FLOAT *)Tf2t,4,4,4);
                    printf("%.9f\n",euclideanDist(CVector3f(toe0[3],toe0[7],toe0[11]),\
                                            CVector3f(toe1[3],toe1[7],toe1[11])));
                    if(trys < 5)
                    {
                        drawProcess(CVector3f(toe1[3],toe1[7],toe1[11]),\
                                 CVector3f(foot0[3],foot0[7],foot0[11]),\
                                 CVector3f(toe0[3],toe0[7],toe0[11]));
                    renderText(
                         toe0[3],\
                         toe0[7],\
                         toe0[11],QString::number(++trys),textFont);
                    }
                    else
                        trys++;
                        */
            }


}
void OpenGL::drawCurve(const int j)
{
    glPushMatrix();
    glTranslatef(array[j].root_offset[0],array[j].root_offset[1],array[j].root_offset[2]);

    for(int bone_id = 6; bone_id <= 8;bone_id++)
    {
        for(int i = array[j].curr < 21?1:array[j].curr-20;i <= array[j].curr;i++)
    {
        glColor3f(1,1,0);//yellow curve
        glLineWidth(0.8);

        //int bone_id = 8;
        int r_index = (i-1)*array[j].boneNum + bone_id;
        int b_index = i*array[j].boneNum + bone_id;



        glBegin(GL_LINES);
        glVertex3f(
                array[j].data[r_index].x,\
                array[j].data[r_index].y,\
                array[j].data[r_index].z);
        glVertex3f(
                array[j].data[b_index ].x,\
                array[j].data[b_index ].y,\
                array[j].data[b_index ].z);
        glEnd();
    }
}
      glPopMatrix();
}
void OpenGL::IK(const int frameIndex,const int boneIndex,\
                const int *parent_of,\
                const AnmHeader *pAnmHead,const ActionHeader *pActHead,\
                CQuaternion *rotkey,const CVector3f *poskey,\
                CVector3f *right,CVector3f *ratio4)
{
        int nClipNum = pActHead->m_lastFrame-pActHead->m_firstFrame+1;
        int currBone = boneIndex;
        //right[INDEX(boneIndex,frameIndex,nClipNum)].setXYZ(0.1,0.1,0.1);
        renderText(right[INDEX(boneIndex,frameIndex,nClipNum)].x,right[INDEX(boneIndex,frameIndex,nClipNum)].y,right[INDEX(boneIndex,frameIndex,nClipNum)].z,
     "*",textFont);
        int trys = 0;
        do
        {

                printf("%lf\n",euclideanDist(ratio4[INDEX(boneIndex,frameIndex,nClipNum)],\
                        right[INDEX(boneIndex,frameIndex,nClipNum)]));
                for(int i = 1;i < pActHead->m_boneAnmCount;i++)
                {
                    _drawLine(ratio4[parent_of[i] * nClipNum + frameIndex],ratio4[i*nClipNum + frameIndex]);
                }
                CQuaternion res = CCD(ratio4[INDEX(parent_of[currBone],frameIndex,nClipNum)],\
                        ratio4[INDEX(boneIndex,frameIndex,nClipNum)],\
                        right[INDEX(boneIndex,frameIndex,nClipNum)]);

                rotkey[INDEX(pActHead->m_boneRotIndex[parent_of[currBone]],frameIndex,nClipNum)] = rotkey[INDEX(pActHead->m_boneRotIndex[parent_of[currBone]],frameIndex,nClipNum)]*res.conjugated();

                GLPOS *gl = new GLPOS;
                gl->getGLPos(ratio4,pActHead,pAnmHead,rotkey,poskey);
                currBone = parent_of[currBone];
                if(currBone == 7)
                        currBone = boneIndex;

        }
        while( ++trys < 3 && \
                (euclideanDist(ratio4[INDEX(boneIndex,frameIndex,nClipNum)],\
                right[INDEX(boneIndex,frameIndex,nClipNum)]) > 0.000171473));
}
void OpenGL::IK(const int frameIndex,int boneIndex,\
                const int *parent_of,\
                const AnmHeader *pAnmHead,const ActionHeader *pActHead,\
                CQuaternion *rotkey,const CVector3f *poskey,\
                CVector3f *right,CVector3f *ratio4,const int *child)
{
        int nClipNum = pActHead->m_lastFrame-pActHead->m_firstFrame+1;
        int trys = 0;
        do
        {
            for(int i = 1;i < pActHead->m_boneAnmCount;i++)
            {
                _drawLine(ratio4[parent_of[i] * nClipNum + frameIndex],ratio4[i*nClipNum + frameIndex]);
            }
        for(int currBone = boneIndex;currBone != -1;currBone = child[currBone])
        {
            FLOAT dist = euclideanDist(ratio4[INDEX(boneIndex,frameIndex,nClipNum)],
                        right[INDEX(boneIndex,frameIndex,nClipNum)]);



            //printf("%d %d %0.16lf\n",frameIndex,currBone,dist);
                    //printf("%0.9lf %0.9lf\n",minn,dist);

            CQuaternion res = CCD(ratio4[INDEX(parent_of[currBone],frameIndex,nClipNum)],\
                ratio4[INDEX(boneIndex,frameIndex,nClipNum)],\
                right[INDEX(boneIndex,frameIndex,nClipNum)]);
            rotkey[INDEX(pActHead->m_boneRotIndex[parent_of[currBone]],frameIndex,nClipNum)] = rotkey[INDEX(pActHead->m_boneRotIndex[parent_of[currBone]],frameIndex,nClipNum)]*res.conjugated();

            GLPOS *gl = new GLPOS;
            gl->getGLPosOneFrameAllBone(ratio4,pActHead,pAnmHead,rotkey,poskey,frameIndex);
        }
                ++trys;
        }
        while(trys  < 1010);

    printf("%d\n",trys);
}
void OpenGL::newMain()

{
    char gBaseDir[100] = "I:\\MotionCapture\\ASE\\";
       int nBaseDirLen = strlen(gBaseDir);
       FILE *pFileDB = fopen("I:\\MotionCapture\\asedb.txt","r");
       char gFileName[256];
       int nFileCount = 0;

       while(fscanf(pFileDB,"%s",gFileName) != EOF)
       {
                   nFileCount++;
                   if(nFileCount != 4) continue;

           //ase
                   strcpy(gBaseDir+nBaseDirLen-4,"ASE\\");
           strcpy(gBaseDir+nBaseDirLen,gFileName);
           CASEParser *ap = new CASEParser;
           actHead = new ActionHeader;
           ap->getActionHeader(gBaseDir,actHead);
                   assert(actHead->m_boneAnmCount+actHead->m_helpAnmCount < 100);
           anmHead = new AnmHeader[actHead->m_boneAnmCount];
                   hanmHead = 0;
                   hanmHead = new HAnmHeader[actHead->m_helpAnmCount];
           int nClipNum = actHead->m_lastFrame-actHead->m_firstFrame+1;
           rotkey = new CQuaternion[actHead->m_boneRotCount*nClipNum];
                   aapkey = new CAAP[actHead->m_boneRotCount*nClipNum];
                   hrotkey = 0;
                   haapkey = 0;
                   hrotkey = new CQuaternion[actHead->m_helpRotCount*nClipNum];
                   haapkey = new CAAP[actHead->m_helpRotCount*nClipNum];
           poskey = new CVector3f[actHead->m_bonePosCount*nClipNum];
                   hposkey = 0;
                   hposkey = new CVector3f[actHead->m_helpPosCount*nClipNum];
           ap->parse(gBaseDir,poskey,hposkey,rotkey,hrotkey,aapkey,haapkey,anmHead,hanmHead,actHead);


                   strcpy(gBaseDir+strlen(gBaseDir)-3,"bz");

                   //
                   for(int i = 0;i < actHead->m_boneRotCount;i++)
                   {
                           int index = i*nClipNum;
                           for(int j = 0;j < nClipNum;j++)
                           {
                                   if(rotkey[index+j].w < 0)
                                   {
                                           rotkey[index+j] = -rotkey[index+j];
                                   }
                           }
                   }

                   //
                   for(int i = 1;i < nClipNum;i++)
                   {
                           for(int j = 0;j < actHead->m_boneRotCount;j++)
                           {
                                   rotkey[j*nClipNum+i] = rotkey[j*nClipNum+i-1]* rotkey[j*nClipNum+i];
                                   rotkey[j*nClipNum+i].normalize();
                           }
                   }


                   GLPOS *gl = new GLPOS;
                   right = new CVector3f[actHead->m_boneAnmCount*nClipNum];
                   gl->getGLPos(right,actHead,anmHead,rotkey,poskey);//correct position


                   //
                   CQuatInterp *tp = new CQuatInterp;
                   for(int i = 0;i < actHead->m_boneRotCount;i++)
                   {
                           if(i == 3 || i == 6) continue;
                           CQuaternion *q = &rotkey[i*nClipNum];
                           CQuaternion rs;

                           for(int j = 3;(j+3)*2 < nClipNum;j += 2)
                           {
                                   tp->squad(rs,q[(j-3)*2],q[(j-1)*2],q[(j+1)*2],q[(j+3)*2],0.5f);
                                   rs.normalize();
                                   q[j*2] = rs;

                           }

                           for(int j = 3;j+3 < nClipNum;j+=2)
                           {
                                   tp->squad(rs,q[j-3],q[j-1],q[j+1],q[j+3],0.5f);
                                   rs.normalize();
                                   q[j] = rs;
                           }
                   }

                   delete tp;tp = 0;

                   //
                   printf("%d\n",actHead->m_boneAnmCount);
                   //
                   father = new int[actHead->m_boneAnmCount];
                   father[0] = -1;
                   for(int j = 0;j < actHead->m_boneAnmCount;j++)
                   {
                           //
                           int flag = 0;
                           for(int k = j-1;k >= 0;k--)
                           {
                                   if( strcmp(anmHead[j].m_nodeParentName,anmHead[k].m_nodeName) == 0 )
                                   {
                                           father[j] = k;
                                           flag = 1;

                                           break;
                                   }
                           }
                           if(j == 0) flag = 1;
                           assert(flag == 1);
                           printf("%d ",father[j]);
                   }printf("\n");


                   printf("%d\n",nClipNum);


                   ratio4= new CVector3f[actHead->m_boneAnmCount*nClipNum];
                   gl->getGLPos(ratio4,actHead,anmHead,rotkey,poskey);//wrong position
                   CVector3f *ori= new CVector3f[2*nClipNum];

           delete gl; gl = 0;
           delete ap; ap = 0;
       }

           fclose(pFileDB); pFileDB = 0;
}
void OpenGL::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer

    glLoadIdentity();			// Reset The Current Modelview Matrix
    glTranslatef(0,-2,0);//
    gluLookAt(radio*cos(x_angle)*cos(y_angle), radio*sin(x_angle)*cos(y_angle), radio*sin(y_angle),
        0,0,0,
        0.0f,0.0f,1.0f);


    for(int j = 0;j < 2;j++)//
    {
        if(array[j].valid == false)
            continue;

        drawObject(j);

        if(array[j].DRAW_CURVE)
        {
            //drawCurve(j);
        }
    }
    drawGround();
    //drawCoord();
    //IK(3,7,father,anmHead,actHead,rotkey,poskey,right,ratio4,childR);


}

void OpenGL::resizeGL(int width, int height)
{
    if (height==0)										// Prevent A Divide By Zero By
    {
        height=1;										// Making Height Equal One
    }

    glViewport(0,0,width,height);						// Reset The Current Viewport

    glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
    glLoadIdentity();									// Reset The Projection Matrix

    // Calculate The Aspect Ratio Of The Window
    gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);

    glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
    glLoadIdentity();									// Reset The Modelview Matrix

}
void OpenGL::initializeGL()
{
    glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
    glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
    glClearDepth(1.0f);									// Depth Buffer Setup
    glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
    glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
    glMatrixMode(GL_MODELVIEW);							// Select The Projection Matrix
    glLoadIdentity();									// Reset The Projection Matrix

}

OpenGL::~OpenGL()
{
    delete [] ratio4; ratio4 = 0;
    delete [] right; right = 0;
    delete [] father; father = 0;


    //free memory
delete actHead; actHead = 0;
delete [] anmHead; anmHead = 0;
delete [] rotkey; rotkey = 0;
    delete [] aapkey; aapkey = 0;
delete [] poskey; poskey = 0;
    delete [] hanmHead; hanmHead = 0;
delete [] hrotkey; hrotkey = 0;
    delete [] haapkey; haapkey = 0;
delete [] hposkey; hposkey = 0;
}
