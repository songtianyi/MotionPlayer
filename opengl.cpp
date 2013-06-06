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
        textFont.setPixelSize(21);

        showBoneNum = false;
        showCoord = false;
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
    if(array[index].objectName.size() == 0)
    {
        //not set yet,so give the object a default
        array[index].objectName = (index == 0?"A":"B");
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
#define V3F CVector3f
    for(int i = 1;i < ground_size;i++)
    {
        for(int j = 1;j < ground_size;j++)
       {
            if((i+j)&1)
                glColor3f(0.7,0.7,0.7);
            else
                glColor3f(0.4,0.4,0.4);
            _drawQuad(
            V3F(i, j, 0.0f),\
            V3F(i-1, j, 0.0f),\
            V3F( i-1, j-1, 0.0f),\
            V3F(i, j-1, 0.0f));
            _drawQuad(
            V3F(-i, j, 0.0f),\
            V3F(-i+1, j, 0.0f),\
            V3F( -i+1, j-1, 0.0f),\
            V3F( -i, j-1, 0.0f));

            _drawQuad(
            V3F(i, -j, 0.0f),\
            V3F(i-1, -j, 0.0f),\
            V3F( i-1, -j+1, 0.0f),\
            V3F( i, -j+1, 0.0f));

            _drawQuad(
            V3F(-i, -j, 0.0f),\
            V3F(-i+1, -j, 0.0f),\
            V3F( -i+1, -j+1, 0.0f),\
            V3F( -i, -j+1, 0.0f));
        }
    }
#undef V3F
}
void OpenGL::drawObject(const int j)
{
    glPushMatrix();
    glTranslatef(array[j].root_offset[0],array[j].root_offset[1],array[j].root_offset[2]);
    int index = array[j].curr*array[j].boneNum + 0;//assure bone zero is root

    CVector3f root(array[j].data[index].x * scale_factor,\
                   array[j].data[index].y * scale_factor,\
                   array[j].data[index].z * scale_factor);

    glColor3f(1,1,0);//yellow text
    textFont.setPixelSize(15);
    renderText(root.x, root.y,root.z + 1.7 * scale_factor,array[j].objectName,textFont);


    for(int i  = 0;i < array[j].boneNum;i++)
    {
        //init child point and parent point
        CVector3f child(array[j].data[index + i].x * scale_factor,\
                        array[j].data[index + i].y * scale_factor,\
                        array[j].data[index + i].z * scale_factor);
        CVector3f parent;
        if(array[j].parent_of[i] != -1)
        {
            int parent_index = array[j].parent_of[i];
            parent.setXYZ(
                    array[j].data[index + parent_index].x * scale_factor,\
                    array[j].data[index + parent_index].y * scale_factor,\
                    array[j].data[index + parent_index].z * scale_factor);
        }

        //draw
        if(array[j].parent_of[i] == -1)
        {
            if( showBoneNum )
            {
                glColor3f(1,1,0);//number color
                textFont.setPixelSize(8);
                renderText(child.x,child.y,child.z,QString::number(i),textFont);
            }
            else
            {
                //
                glColor3f(1,1,0);//yellow point
                _drawPoint(child,3.1);
            }
            continue;
        }


        if( showBoneNum )
        {
            glColor3f(1,1,0);//number color
            textFont.setPixelSize(9);
            renderText(child.x,child.y,child.z,QString::number(i),textFont);
        }
        else
        {
            //draw bone
            glColor3f(0.0f, 0.0f, 1.0f);//blue bone
            glLineWidth(3);//bone width
            _drawLine(parent,child);

            //draw joint
            glColor3f(1,1,0);//yellow point
            _drawPoint(parent,3.1);
        }


  }
  glPopMatrix();
}

void OpenGL::drawCoord()
{
    textFont.setPixelSize(15);
    glColor3f(1,1,0);//yellow coord

    _drawLine(CVector3f(0,0,0),CVector3f(1,0,0));//x-axis
    renderText(1,0,0,"X",textFont);

     _drawLine(CVector3f(0,0,0),CVector3f(0,1,0));//y-axis
    renderText(0,1,0,"Y",textFont);

    _drawLine(CVector3f(0,0,0),CVector3f(0,0,1));//z-axis
    renderText(0,0,1,"Z",textFont);

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
        if(array[j].valid == true)
        {
            drawObject(j);
        }
    }
    drawGround();

    if( showCoord )
    {
        drawCoord();
    }

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
}
