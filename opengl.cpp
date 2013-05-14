#include "opengl.h"

OpenGL::OpenGL(QWidget *parent) :
    QGLWidget(parent)
{   
        x_angle = 0.0f;
        y_angle = 0.0f;
        radio = 10.0f;

        ground_size = 5;

        initializeGL();
        resizeGL(GL_SIZE_WIDTH, GL_SIZE_HEIGHT);


        QObject::connect(&timer,SIGNAL(timeout()),this,SLOT(commonUpdate()));
        timer.setInterval(30);


        //font set
        textFont.setBold(true);
        textFont.setPixelSize(21);

}


void OpenGL::setGroundSize(int v)
{
    ground_size = v;
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
            array[i].curr = 0;

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
    printf("set Interval\n");
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
void OpenGL::paintObject(const int j)
{
    glPushMatrix();
    glTranslatef(array[j].root_offset[0],array[j].root_offset[1],array[j].root_offset[2]);
    int index = array[j].curr*array[j].boneNum*3;

    glColor3f(1,1,0);//yellow text
    glLineWidth(3);
    renderText(
         array[j].data[index + 0],\
         array[j].data[index + 1],\
         array[j].data[index + 2]+1.7,array[j].name,textFont);



    for(int i  = 0;i < array[j].boneNum;i++)
    {
        //

        if(array[j].parent_of[i] == -1)
        {
            //
            glPointSize(3.1);
            glBegin(GL_POINTS);
            glColor3f(1,1,0);//yellow point
            glVertex3f(
                    array[j].data[index + i*3+0],\
                    array[j].data[index + i*3+1],\
                    array[j].data[index + i*3+2]);
            glEnd();
            continue;
        }

        int parent_index = array[j].parent_of[i]*3;
        int child_index = i*3;

        glColor3f(0.0f, 0.0f, 1.0f);//blue bone
        glBegin(GL_LINES);
        glVertex3f(
                array[j].data[index + parent_index+0],\
                array[j].data[index + parent_index+1],\
                array[j].data[index + parent_index+2]);
        glVertex3f(
                array[j].data[index + child_index ],\
                array[j].data[index + child_index +1],\
                array[j].data[index + child_index +2]);
        glEnd();


        //
        glPointSize(3.1);
        glBegin(GL_POINTS);
        glColor3f(1,1,0);//yellow point
        glVertex3f(
                array[j].data[index + parent_index+0],\
                array[j].data[index + parent_index+1],\
                array[j].data[index + parent_index+2]);
        glEnd();

  }
  glPopMatrix();
}
void OpenGL::paintCoord()
{
    glColor3f(1,1,0);//yellow coord
    glBegin(GL_LINES);
    glVertex3f(0,0,0);
    glVertex3f(1,0,0);//x-axis
    glEnd();

    glBegin(GL_LINES);
    glVertex3f(0,0,0);
    glVertex3f(0,1,0);//y-axis
    glEnd();

    glBegin(GL_LINES);
    glVertex3f(0,0,0);
    glVertex3f(0,0,1);//z-axis
    glEnd();

}
void OpenGL::paintCurve(const int j)
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
        int r_index = (i-1)*array[j].boneNum*3 + bone_id*3;
        int b_index = i*array[j].boneNum*3 + bone_id*3;



        glBegin(GL_LINES);
        glVertex3f(
                array[j].data[r_index+0],\
                array[j].data[r_index+1],\
                array[j].data[r_index+2]);
        glVertex3f(
                array[j].data[b_index ],\
                array[j].data[b_index +1],\
                array[j].data[b_index +2]);
        glEnd();
    }
}
      glPopMatrix();
}
void OpenGL::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer

    glLoadIdentity();									// Reset The Current Modelview Matrix
    glTranslatef(0,-2,0);//
    gluLookAt(radio*cos(x_angle)*cos(y_angle), radio*sin(x_angle)*cos(y_angle), radio*sin(y_angle),
        0,0,0,0.0f,0.0f,1.0f);





    for(int j = 0;j < 2;j++)//
    {
        if(array[j].valid == false)
            continue;

        paintObject(j);

        if(array[j].DRAW_CURVE)
        {
            paintCurve(j);
        }
    }
    drawGround();
   // paintCoord();
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
{}
