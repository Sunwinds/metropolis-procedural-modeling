//#####################################################################
// Copyright 2009, Jerry Talton.
//#####################################################################
#include <iostream>
#include <QMouseEvent>
#include <QFileDialog>
#include "SketchingWidget.hpp"
#include "Math.hpp"
#include <queue>
#include <QStatusBar>
#include <QToolBar>
#include <QMenuBar>
#include <QInputDialog>
#include <QColorDialog>
using ProceduralModeling::Math::sqr;
//####################################################################
// Function SketchingWidget
//####################################################################
SketchingWidget::SketchingWidget(QWidget* parent)
 :QWidget(parent),mPainter(),mImage(1,1,QImage::Format_ARGB32),mBrushSize(10),mBrushColor(Qt::white)
{
    mMainWindow=static_cast<QMainWindow*>(parent);
    mImage.fill(0xFF000000);
    updateGLImage();
    setCursor(QCursor(Qt::CrossCursor));
}
//####################################################################
// Function paintEvent
//####################################################################
void SketchingWidget::paintEvent(QPaintEvent *event)
{
    mPainter.begin(this);
    mPainter.drawImage(0,0,mImage,0,0,width(),height());
    mPainter.end();
    //updateGLImage();
}
//####################################################################
// Function wheelEvent
//####################################################################
void SketchingWidget::wheelEvent(QWheelEvent* event)
{
    float numDegrees=event->delta()/50.;
    mBrushSize=mBrushSize+numDegrees<1?1:mBrushSize+numDegrees;
}
//####################################################################
// Function mousePressEvent
//####################################################################
void SketchingWidget::mousePressEvent(QMouseEvent* event)
{
    if(event->button()==Qt::LeftButton){ mLastMousePosition=event->pos(); drawBrush(mLastMousePosition); update(); }
    else event->ignore();
}
//####################################################################
// Function mouseMoveEvent
//####################################################################
void SketchingWidget::mouseMoveEvent(QMouseEvent* event)
{
    if(event->pos()!=mLastMousePosition){ mLastMousePosition=event->pos(); drawBrush(mLastMousePosition); update(); }
    else event->ignore();
}
//####################################################################
// Function drawBrush
//####################################################################
void SketchingWidget::drawBrush(const QPoint& pos)
{
    mPainter.begin(&mImage);
    setBrush();
    mPainter.save();
    mPainter.translate(pos);
    mPainter.drawEllipse(-mBrushSize/2,-mBrushSize/2,mBrushSize,mBrushSize);
    mPainter.restore();
    mPainter.end();
    updateGLImage();
}
//####################################################################
// Function fastMarchSketch
//####################################################################
void SketchingWidget::fastMarchSketch(QImage& image)
{
    uint width=image.width(),height=image.height();

    float maxDistance=1e10;
    float** buffer=new float*[width];
    for(uint i=0;i<width;i++){ buffer[i]=new float[height]; for(uint j=0;j<height;j++) buffer[i][j]=maxDistance; }

    for(uint j=0;j<height;j++){ QRgb* scanLine=reinterpret_cast<QRgb*>(image.scanLine(j)); for(uint i=0;i<width;i++) if(*(scanLine+i)!=0xFF000000) buffer[i][j]=0; }

    for(int i=0;i<width;i++) for(int j=0;j<height;j++) buffer[i][j]=qMin(buffer[i][j],newMarchValue(buffer,maxDistance,i,j));
    for(int i=0;i<width;i++) for(int j=height-1;j>=0;j--) buffer[i][j]=qMin(buffer[i][j],newMarchValue(buffer,maxDistance,i,j));
    for(int i=width-1;i>=0;i--) for(int j=0;j<height;j++) buffer[i][j]=qMin(buffer[i][j],newMarchValue(buffer,maxDistance,i,j));
    for(int i=width-1;i>=0;i--) for(int j=height-1;j>=0;j--) buffer[i][j]=qMin(buffer[i][j],newMarchValue(buffer,maxDistance,i,j));

    float varTerm=sqrt(sqr(width)+sqr(height));
    for(uint i=0;i<width;i++) for(uint j=0;j<height;j++){ float val=exp(-sqr(buffer[i][j]/(15*varTerm)))*255; image.setPixel(i,j,qRgb(val,val,val)); }

    for(uint i=0;i<width;i++) delete[] buffer[i]; delete[] buffer;
}
//####################################################################
// Function newMArchValue
//####################################################################
float SketchingWidget::newMarchValue(float** buffer,const float maxDistance,const uint i,const uint j) const
{
    if(buffer[i][j]==0) return 0;
    float width=mImage.width(),height=mImage.height(),h=1,uxmin,uymin;
    if(i==0) uxmin=buffer[i+1][j]; else if(i==width-1) uxmin=buffer[i-1][j]; else uxmin=qMin(buffer[i-1][j],buffer[i+1][j]);
    if(j==0) uymin=buffer[i][j+1]; else if(j==height-1) uymin=buffer[i][j-1]; else uymin=qMin(buffer[i][j-1],buffer[i][j+1]);
    if(fabs(uxmin-uymin)>=h) return qMin(uxmin,uymin)+h;
    else return .5*(uxmin+uymin+sqrt(2*sqr(h)-sqr(uxmin-uymin)));
}
//####################################################################
// Function setupMenuAndButton
//####################################################################
void SketchingWidget::setupMenuAndButton()
{
    QMenu* imageMenu=mMainWindow->menuBar()->addMenu(tr("Image"));

    QAction* loadImageAct=new QAction(tr("Load from File..."),mMainWindow);
    loadImageAct->setShortcut(tr("Ctrl+i"));
    loadImageAct->setStatusTip(tr("Load a target image from a file"));
    connect(loadImageAct,SIGNAL(triggered()),this,SLOT(loadImageFromFile()));
    imageMenu->addAction(loadImageAct);

    QAction* clearImageAct=new QAction(tr("Clear"), mMainWindow);
    clearImageAct->setStatusTip(tr("Clear SKetch image"));
    connect(clearImageAct,SIGNAL(triggered()),this,SLOT(clear()));
    imageMenu->addAction(clearImageAct);

    QAction* pickBrushColorAct=new QAction(tr("PickColor"), mMainWindow);
    pickBrushColorAct->setStatusTip(tr("Pick Brush Color"));
    connect(pickBrushColorAct,SIGNAL(triggered()),this,SLOT(pickColor()));
    imageMenu->addAction(pickBrushColorAct);
}
//####################################################################
// Function loadImageFromFile
//####################################################################
void SketchingWidget::loadImageFromFile()
{
    QString fileName=QFileDialog::getOpenFileName(mMainWindow,tr("Open Image"),".",tr("Image Files (*.png *.jpg *.bmp)"));
    if (fileName.isEmpty()) return;
    mImage.load(fileName);
    mMainWindow->resize(2*mImage.width(),mImage.height()+mMainWindow->iconSize().height()+mMainWindow->menuBar()->height()+mMainWindow->statusBar()->height());
    refresh();
}
//####################################################################
// Function pickColor
//####################################################################
void SketchingWidget::pickColor()
{
    QColorDialog dialog(mBrushColor, mMainWindow);
    dialog.setOption(QColorDialog::ShowAlphaChannel, true);
    if (dialog.exec() == QDialog::Accepted)
        mBrushColor=dialog.selectedColor();
}
//####################################################################
