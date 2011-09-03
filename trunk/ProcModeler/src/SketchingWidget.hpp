//#####################################################################
// Copyright 2009, Jerry Talton.
//#####################################################################
#ifndef __SKETCHING_WIDGET__
#define __SKETCHING_WIDGET__

#include <QImage>
#include <QPainter>
#include <QResizeEvent>
#include <QWidget>
#include <QGLWidget>
#include <QTextStream>
#include <QMainWindow>

class QMouseEvent;

//####################################################################
// Class SketchingWidget
//####################################################################
class SketchingWidget : public QWidget
{
    Q_OBJECT

protected:
    QMainWindow* mMainWindow;
    QPainter mPainter;
    QImage mOriginalImage,mImage;
    QImage mGLImage;
    bool mGLImageFlag;
    float mBrushSize;
    QPoint mLastMousePosition;
    QColor mBrushColor;

public:
    const QImage& image() const
    { return mImage; }

    const QImage& glImage() const
    { return mGLImage; }

    void loadImage(const QString& fileName)
    { mImage.load(fileName); }

    void saveImage(const QString& fileName) const
    { mImage.save(fileName); }

    void refresh()
    { updateGLImage(); update(); }

    void fastMarchSketch()
    { fastMarchSketch(mImage); refresh(); }

    virtual void resizeEvent(QResizeEvent* event)
    { mImage=mImage.scaled(width(),height(),Qt::IgnoreAspectRatio,Qt::SmoothTransformation); refresh(); }

    void setBrush()
    { mPainter.setPen(QPen(QBrush(mBrushColor),mBrushSize,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin)); }

    virtual void updateGLImage()
    { mGLImage=QGLWidget::convertToGLFormat(mImage); setGLImageFlag(true); }

    bool glImageFlag() const
    { return mGLImageFlag; }

    void setGLImageFlag(bool glImageFlag)
    { mGLImageFlag=glImageFlag; }

public:
    SketchingWidget(QWidget* parent=NULL);
    void fastMarchSketch(QImage& image);
    virtual void setupMenuAndButton();

protected:
    virtual void paintEvent(QPaintEvent *event);
    virtual void wheelEvent(QWheelEvent* event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent* event);
    float newMarchValue(float** buffer,const float maxDistance,const uint i,const uint j) const;
    void drawBrush(const QPoint& pos);

public slots:
    void clear()
    { mImage.fill(0xFF000000); refresh(); }

protected slots:
    void loadImageFromFile();
    void pickColor();
//####################################################################
};

#endif //__MODEL_RENDER_WIDGET__
