#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    create();

    this->grabKeyboard();

    this->setAcceptDrops(true);


    this->setFixedSize(1000,670);
    this->setWindowTitle("Motion Player");
}

void MainWindow::setInitData(const int index,const int sliderValue,const int sliderMax,const int dialValue)
{
    //set slider init value
    sb[index].setMaximum(sliderMax-1);
    sb[index].setValue(sliderValue);

    dl[0].setValue(dialValue);
}
void MainWindow::setStatusFrameA(int v)
{
    sl[10].setText(QString::number(v));
}
void MainWindow::setStatusFrameB(int v)
{
    sl[12].setText(QString::number(v));
}
void MainWindow::setStatusOffsetValue(const float *offset,const int index)
{
    if(index)
    {
        for(int i = 0;i < 3;i++)
            sl[i + 6].setText(QString::number(offset[i]));

    }
    else
    {
        for(int i = 0;i < 3;i++)
            sl[i + 2].setText(QString::number(offset[i]));
    }
}

void MainWindow::setSliderValue(const int index, const int v)
{
    sb[index].setValue(v);
}


void MainWindow::keyPressEvent(QKeyEvent *e)
{
    switch( e->key() ){
    case Qt::Key_A:
        glWidget->x_angle -= 0.04f;
        break;
    case Qt::Key_D:
        glWidget->x_angle += 0.04f;
        break;
    case Qt::Key_W:
        if(glWidget->y_angle <= 1.5)
           glWidget->y_angle +=0.04f;
        break;
    case Qt::Key_S:
        glWidget->y_angle -=0.04f;
        break;
    case Qt::Key_Up:
         glWidget->radio -= 0.04f;
        break;
    case Qt::Key_Down:
        glWidget->radio += 0.04f;
        break;

    case Qt::Key_J:
        if(cb[0].isChecked())
        {
            glWidget->array[0].root_offset[0] -= 0.2f;
            setStatusOffsetValue(glWidget->array[0].root_offset,0);
        }
        if(cb[1].isChecked())
        {
            glWidget->array[1].root_offset[0] -= 0.2f;
            setStatusOffsetValue(glWidget->array[1].root_offset,1);
        }
        break;
    case Qt::Key_L:
        if(cb[0].isChecked())
        {
            glWidget->array[0].root_offset[0] += 0.2f;
            setStatusOffsetValue(glWidget->array[0].root_offset,0);
        }
        if(cb[1].isChecked())
        {
            glWidget->array[1].root_offset[0] += 0.2f;
            setStatusOffsetValue(glWidget->array[1].root_offset,1);
        }
        break;
    case Qt::Key_K:
        if(cb[0].isChecked())
        {
            glWidget->array[0].root_offset[1] -= 0.2f;
            setStatusOffsetValue(glWidget->array[0].root_offset,0);
        }
        if(cb[1].isChecked())
        {
            glWidget->array[1].root_offset[1] -= 0.2f;
            setStatusOffsetValue(glWidget->array[1].root_offset,1);
        }
        break;
    case Qt::Key_I:
        if(cb[0].isChecked())
        {
            glWidget->array[0].root_offset[1] += 0.2f;
            setStatusOffsetValue(glWidget->array[0].root_offset,0);
        }
        if(cb[1].isChecked())
        {
            glWidget->array[1].root_offset[1] += 0.2f;
            setStatusOffsetValue(glWidget->array[1].root_offset,1);
        }
        break;
    case Qt::Key_Left:
        if(cb[0].isChecked() && glWidget->array[0].curr > 0)
        {
            glWidget->array[0].curr -= 1;
            sb[0].setValue(sb[0].value()-1);

        }
        if(cb[1].isChecked() && glWidget->array[1].curr > 0)
        {
            glWidget->array[1].curr -= 1;
            sb[1].setValue(sb[1].value()-1);
        }
        break;
    case Qt::Key_Right:
        if(cb[0].isChecked() && glWidget->array[0].curr < glWidget->array[0].frameNum-1)
        {
            glWidget->array[0].curr += 1;
            sb[0].setValue(sb[0].value()+1);
        }
        if(cb[1].isChecked() && glWidget->array[1].curr < glWidget->array[1].frameNum-1)
        {
            glWidget->array[1].curr += 1;
            sb[1].setValue(sb[1].value()+1);
        }
        break;
    case Qt::Key_Space:
        if(cb[0].isChecked() && glWidget->array[0].valid)
        {
            glWidget->array[0].pause = (!glWidget->array[0].pause);
        }
        if(cb[1].isChecked()&& glWidget->array[1].valid)
        {
            glWidget->array[1].pause = (!glWidget->array[1].pause);
        }
        if(glWidget->array[0].pause == true && glWidget->array[1].pause == true)
        {
            glWidget->timer.stop();
        }
        else
            glWidget->timer.start();
        break;
    case Qt::Key_X:
        if(cb[0].isChecked())
        {
            glWidget->destroyObject(0);
        }
        if(cb[1].isChecked())
        {
            glWidget->destroyObject(1);
        }
        break;



    default:
        break;
    }

    glWidget->updateGL();
}


void MainWindow::create()
{
    glWidget  = new OpenGL;


    hbl = new QHBoxLayout();//contain vblCpL gblCpR
    vblCpL = new QVBoxLayout;//sb[0],sb[1]
    gblCpR = new QGridLayout;//rb[0],rb[1],


    //ctrl panel left
    sb[0].setOrientation(Qt::Horizontal);
    sb[1].setOrientation(Qt::Horizontal);

    vblCpL->addWidget(&sb[0]);
    vblCpL->addWidget(&sb[1]);
    vblCpL->setMargin(10);




    //ctrl panel right
    cb[0].setText("A");
    cb[1].setText("B");
    gblCpR->addWidget(&cb[0],1,1);
    gblCpR->addWidget(&cb[1],2,1);//check box


    bn[0].setText("OpenFile");
    bn[1].setText("OpenFile");
    gblCpR->addWidget(&bn[0],1,2);
    gblCpR->addWidget(&bn[1],2,2);//push button


    dl[0].setNotchesVisible(true);
    dl[0].setMinimum(0);
    dl[0].setMaximum(150);


    dl[1].setNotchesVisible(true);
    dl[1].setMinimum(200);
    dl[1].setMaximum(2000);


    dl[2].setNotchesVisible(true);
    dl[2].setMinimum(1);
    dl[2].setMaximum(10);//set dial 2,line width
    dl[2].setValue(5);

    gblCpR->addWidget(&dl[0],1,3,2,1);
    gblCpR->addWidget(&dl[1],1,4,2,1);
    gblCpR->addWidget(&dl[2],1,5,2,1);



    hbl->addLayout(vblCpL);
    hbl->addLayout(gblCpR);

    statusBar = new QStatusBar;
    statusBar->setSizeGripEnabled(false);


    //
    glWidget->setFixedSize(1000,575);
    statusBar->setFixedSize(1000,15);
    for(int i = 1;i < 13;i++)
    {
        statusBar->addPermanentWidget(&sl[i]);
    }
    sl[1].setText("OFFSET:");
    sl[5].setText("OFFSET:");
    for(int i = 0;i < 3;i++)
    {
        sl[2+i].setText(QString::number(glWidget->array[0].root_offset[i]));
        sl[6+i].setText(QString::number(glWidget->array[1].root_offset[i]));
    }

    sl[9].setText("Frame:");
    sl[10].setText(QString::number(glWidget->array[0].curr));
    sl[11].setText("Frame:");
    sl[12].setText(QString::number(glWidget->array[1].curr));


    vbl = new QVBoxLayout();

    vbl->addWidget(glWidget);
    vbl->addLayout(hbl);
    vbl->addWidget(statusBar);
    vbl->setMargin(0);

    centralWidget = new QWidget;
    centralWidget->setLayout(vbl);
    this->setCentralWidget(centralWidget);




    QObject::connect(&bn[0],SIGNAL(clicked()),this,SLOT(openFileA()));//open file button
    QObject::connect(&bn[1],SIGNAL(clicked()),this,SLOT(openFileB()));

    QObject::connect(&sb[0],SIGNAL(sliderMoved(int)),glWidget,SLOT(setCurrFrameA(int)));//set current frame
    QObject::connect(&sb[1],SIGNAL(sliderMoved(int)),glWidget,SLOT(setCurrFrameB(int)));
    QObject::connect(&sb[0],SIGNAL(valueChanged(int)),this,SLOT(setStatusFrameA(int)));//set status bar
    QObject::connect(&sb[1],SIGNAL(valueChanged(int)),this,SLOT(setStatusFrameB(int)));

    QObject::connect(&dl[0],SIGNAL(valueChanged(int)),glWidget,SLOT(setTimeInterval(int)));
    QObject::connect(&dl[1],SIGNAL(valueChanged(int)),glWidget,SLOT(setScaleFactor(int)));
    QObject::connect(&dl[2],SIGNAL(valueChanged(int)),glWidget,SLOT(setGroundSize(int)));

    QObject::connect(glWidget,SIGNAL(initDataChanged(int,int,int,int)),this,SLOT(setInitData(int,int,int,int)));
    QObject::connect(glWidget,SIGNAL(sliderValueChanged(int,int)),this,SLOT(setSliderValue(int,int)));

    QObject::connect(&sb[0],SIGNAL(sliderPressed()),&(glWidget->timer),SLOT(stop()));//timer operation
    QObject::connect(&sb[0],SIGNAL(sliderReleased()),&(glWidget->timer),SLOT(start()));
    QObject::connect(&sb[1],SIGNAL(sliderPressed()),&(glWidget->timer),SLOT(stop()));
    QObject::connect(&sb[1],SIGNAL(sliderReleased()),&(glWidget->timer),SLOT(start()));

    QObject::connect(this,SIGNAL(statusStr(QString,int)),statusBar,SLOT(message(QString,int)));
    QObject::connect(glWidget,SIGNAL(statusStr(QString,int)),statusBar,SLOT(message(QString,int)));

    statusStr("Ready!");

}
void MainWindow::openFileA()
{
    openFile(true);
}

void MainWindow::openFileB()
{
    openFile(false);
}
void MainWindow::openFile(bool isA)
{
    glWidget->timer.stop();
    QString s = QFileDialog::getOpenFileName(
            this,
            "Open File Dialog",
            "/",
            "Files (*.ase *.caf *.bvh *.trc *.bz)");
    if(s.size() < 5)
    {
        glWidget->timer.start();
        return ;
    }

    QString suffix;
    if( s.toUpper().endsWith("ASE"))
        suffix = "ASE";
    else if(s.toUpper().endsWith("CAF"))
        suffix = "CAF";
    else if(s.toUpper().endsWith("BVH"))
        suffix = "BVH";
    else if(s.toUpper().endsWith("TRC"))
        suffix = "TRC";
    else if(s.toUpper().endsWith("BZ"))
        suffix = "BZ";

    statusStr(s);

    glWidget->getObjectData(s.toStdString().c_str(),suffix,!isA);

    glWidget->timer.start();;
}
MainWindow::~MainWindow()
{
    delete ui;
}
