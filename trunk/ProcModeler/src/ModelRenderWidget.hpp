//#####################################################################
// Copyright 2009, Jerry Talton.
//#####################################################################
#ifndef __MODEL_RENDER_WIDGET__
#define __MODEL_RENDER_WIDGET__

#include "ProceduralModel.hpp"
#include "ModelSimulation.hpp"
#include "VoxelizationWidget.hpp"
#include <QGLWidget>
#include <QSize>
#include <QLineEdit>

using ProceduralModeling::ProceduralModel;
using ProceduralModeling::ProductionList;

class ModelSimulation;

//####################################################################
// Class LogDisplay
//####################################################################
class LogDisplay
{
public:
    LogDisplay():elapsedIterationsDisplay(NULL),elapsedSecondsDisplay(NULL),iterationsRateDisplay(NULL),totalIterationsDisplay(NULL),
                swapAcceptanceRateDisplay(NULL),diffusionAcceptanceRateDisplay(NULL),jumpAcceptanceRateDisplay(NULL){}
    ~LogDisplay(){ delete elapsedIterationsDisplay; delete elapsedSecondsDisplay; delete iterationsRateDisplay; delete totalIterationsDisplay;
                    delete swapAcceptanceRateDisplay; delete diffusionAcceptanceRateDisplay; delete jumpAcceptanceRateDisplay;}
    void reset(int totalIterations);
    int refreshRate;
    QLineEdit *elapsedIterationsDisplay;
    QLineEdit *elapsedSecondsDisplay;
    QLineEdit *iterationsRateDisplay;
    QLineEdit *totalIterationsDisplay;
    QLineEdit *swapAcceptanceRateDisplay;
    QLineEdit *diffusionAcceptanceRateDisplay;
    QLineEdit *jumpAcceptanceRateDisplay;
};
//####################################################################
// Class ModelRenderWidget
//####################################################################
class ModelRenderWidget : public QGLWidget
{
    Q_OBJECT

    enum SaveFlags{ SAVE_IMAGE=1<<0,SAVE_OBJ=1<<1,SAVE_MODEL=1<<2,SAVE_ALL=1<<3 };

protected:
    ProceduralModel* mModel;
    QGLFramebufferObject* mModelBuffer;
    float mTheta,mPhi,mX,mY,mZ;
	float mAspectRatio;
    QPoint mLastMousePosition;
    uint mRenderLevel;
    bool mCtrlHeld,mShowMaxPosterior,mRunning,mShowLastModel;
    VoxelizationWidget* mVoxelWidget;
    uint mImageLogRate,mObjLogRate,mModelLogRate,mRefreshRate;
    float mSavedTheta,mSavedPhi,mSavedX,mSavedY,mSavedZ;
    LogDisplay mLogDisplay;

public:
    ModelRenderWidget(QWidget* parent=NULL)
        :QGLWidget(parent),mModel(NULL),mModelBuffer(NULL),mTheta(0),mPhi(PI_OVER_TWO),mX(0),mY(0),mZ(-20),mAspectRatio(1),mRenderLevel(0),
         mCtrlHeld(false),mShowMaxPosterior(false),mRunning(false),mShowLastModel(false),mVoxelWidget(NULL),mImageLogRate(2000),
         mObjLogRate(5000),mModelLogRate(1000),mRefreshRate(1),mSavedTheta(0),mSavedPhi(PI_OVER_TWO),mSavedX(0),mSavedY(0),mSavedZ(-20)
    {}

    ~ModelRenderWidget()
    { delete mModelBuffer; }

    uint imageLogRate() const
    { return mImageLogRate; }

    void setImageLogRate(uint imageLogRate)
    { mImageLogRate=imageLogRate; }

    uint objLogRate() const
    { return mObjLogRate; }

    void setObjLogRate(uint objLogRate)
    { mObjLogRate=objLogRate; }

    uint modelLogRate() const
    { return mModelLogRate; }

    void setModelLogRate(uint modelLogRate)
    { mModelLogRate=modelLogRate; }

    void resetRenderLevel()
    { mRenderLevel=mModel->modelTreeDepth()-1; }

    void incrementRenderLevel()
    { mRenderLevel=mRenderLevel+1<mModel->modelTreeDepth()?mRenderLevel+1:mRenderLevel; }

    void decrementRenderLevel()
    { mRenderLevel=mRenderLevel>0?mRenderLevel-1:mRenderLevel; }

    void setProceduralModel(ProceduralModel* model)
    { mModel=model; mRenderLevel=mModel?mModel->modelTreeDepth()-1:0; updateGL(); }

    void printMaxPosterior()
    { std::cout<<mModel->modelToString(mModel->maxPosteriorModel()).constData()<<std::flush; }

    void printModelLevel()
    { std::cout<<mModel->modelToString(mModel->model(mRenderLevel)).constData()<<std::flush; }

    void renderLastModel(const ProductionList& model)
    { if(mShowLastModel) renderModel(model); }

    void controlKeyHeld(const bool pressed)
    { mCtrlHeld=pressed; }

    bool showMaxPosteriorModel() const
    { return mShowMaxPosterior; }

    void toggleShowMaxPosteriorModel()
    { mShowMaxPosterior=!mShowMaxPosterior; updateGL(); }

    void abort()
    { mRunning=false; }

    bool isRunning() const
    { return mRunning; }

    void setVoxelWidget(VoxelizationWidget* voxelWidget)
    { mVoxelWidget=voxelWidget; }

    void setCamera(float theta,float phi,float z)
    { mTheta=theta; mPhi=phi; updateGL(); }

    friend QTextStream& operator<<(QTextStream& out,const ModelRenderWidget& widget)
	{ return out<<widget.mTheta<<" "<<widget.mPhi<<" "<<" "<<widget.mX<<" "<<widget.mY<<" "<<widget.mZ<<" "<<widget.mRenderLevel; }

    friend QTextStream& operator>>(QTextStream& in,ModelRenderWidget& widget)
    { return in>>widget.mTheta>>widget.mPhi>>widget.mX>>widget.mY>>widget.mZ>>widget.mRenderLevel; widget.updateGL(); }

    void outputVoxel()
    { if (!mModel) return; mModel->voxelize(mModel->model(mRenderLevel)); mModel->getResourceHandler().getBBoxHelper()->saveToMesh(); }

    void outputVoxelFile(const QString& path)
    { if (!mModel) return; mModel->voxelize(mModel->maxPosteriorModel()); mModel->getResourceHandler().getBBoxHelper()->saveToFile(path); }

    void saveCamera()
    { mSavedTheta=mTheta; mSavedPhi=mPhi; mSavedX=mX; mSavedY=mY; mSavedZ=mZ; }

    void loadCamera()
    { mTheta=mSavedTheta; mPhi=mSavedPhi; mX=mSavedX; mY=mSavedY; mZ=mSavedZ; updateGL(); }

    LogDisplay& logDisplay()
    { return mLogDisplay; }

public:
    void evolveChain(unsigned long& iteration,QString outputPath=QString("."),ModelSimulation* simulation=NULL);
    void stepModel();
    QImage getModelImage(const ProductionList& model);
    void readModel(const ProductionList& model);
    void saveToObj(QString path=QString("quad"));
    void printCamera();

protected:
    void renderModel(const ProductionList& model);
    void saveModelState(unsigned long& iter,QString outputPath,ModelSimulation* simulation,unsigned int saveFlags=SAVE_ALL);
    virtual void initializeGL();
    virtual void paintGL();
    virtual void resizeGL(int width,int height);
    virtual void wheelEvent(QWheelEvent* event);
    virtual void mousePressEvent(QMouseEvent* event);
    virtual void mouseMoveEvent(QMouseEvent* event);

//####################################################################
};

#endif //__MODEL_RENDER_WIDGET__
