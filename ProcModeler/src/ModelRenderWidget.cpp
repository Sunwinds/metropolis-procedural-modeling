//#####################################################################
// Copyright 2009, Jerry Talton.
//#####################################################################
#include "ModelRenderWidget.hpp"
#include "ModelSimulation.hpp"
#include <time.h>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QGLFramebufferObject>
#include <QMatrix4x4>
void LogDisplay::reset(int totalIterations)
{
    elapsedIterationsDisplay->setText(QString::number(0));
    elapsedSecondsDisplay->setText(QString::number(0.0));
    iterationsRateDisplay->setText(QString::number(0.0));
    totalIterationsDisplay->setText(QString::number(totalIterations));
    swapAcceptanceRateDisplay->setText(QString::number(0.0));
    diffusionAcceptanceRateDisplay->setText(QString::number(0.0));
    jumpAcceptanceRateDisplay->setText(QString::number(0.0));
}
//####################################################################
// Function initializeGL
//####################################################################
void ModelRenderWidget::initializeGL()
{
    glClearColor(0,0,0,0);
    glShadeModel(GL_FLAT);
    glEnable(GL_DEPTH_TEST);
    glewInit();
}
//####################################################################
// Function renderModel
//####################################################################
void ModelRenderWidget::renderModel(const ProductionList& model)
{
    makeCurrent();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	gluPerspective(45,mAspectRatio,.1,1000);

    glTranslatef(mX,mY,0);
    gluLookAt(0,0,mZ,0,0,0,0,1,0);
    glRotatef(mTheta*100,1.0,0.0,0.0);
    glRotatef(mPhi*100,0.0,1.0,0.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glColor3f(1,1,1);

    mModel->render(model);
}
//####################################################################
// Function paintGL
//####################################################################
void ModelRenderWidget::paintGL()
{
    if(!mModel || mShowLastModel) return;

    if(mShowMaxPosterior) renderModel(mModel->maxPosteriorModel());
    else renderModel(mModel->model(mRenderLevel));
}
//####################################################################
// Function resizeGL
//####################################################################
void ModelRenderWidget::resizeGL(int width,int height)
{
    delete mModelBuffer;mModelBuffer=new QGLFramebufferObject(width,height,QGLFramebufferObject::Depth);
    glViewport(0,0,width,height);

	mAspectRatio=(float)width/(float)height;
}
//####################################################################
// Function evolveChain
//####################################################################
void ModelRenderWidget::evolveChain(unsigned long& iteration,QString outputPath,ModelSimulation* simulation)
{
    mRunning=true;
    clock_t start,finish;
    unsigned long startIteration=iteration;
    mModel->resetStats();
    float elapsedSeconds;
    unsigned long elapsedIterations;

    start=clock();
    while(true)
    {
        QCoreApplication::processEvents();
        if(!mRunning){ saveModelState(iteration,outputPath,simulation,SAVE_ALL); break; }
        mModel->evolveChain();
        mRenderLevel=mModel->modelTreeDepth()-1;

        if(iteration%mRefreshRate==0) updateGL();
        if(iteration%mImageLogRate==0) saveModelState(iteration,outputPath,simulation,SAVE_IMAGE);
        if(iteration%mObjLogRate==0) saveModelState(iteration,outputPath,simulation,SAVE_OBJ);
        if(iteration%mModelLogRate==0) saveModelState(iteration,outputPath,simulation,SAVE_MODEL);

        iteration++;
        if (iteration % mLogDisplay.refreshRate == 0)
        {
            finish=clock();
            elapsedSeconds=(finish-start)/(float)CLOCKS_PER_SEC;
            elapsedIterations=iteration-startIteration;
            mLogDisplay.elapsedIterationsDisplay->setText(QString::number(elapsedIterations));
            mLogDisplay.elapsedSecondsDisplay->setText(QString::number(elapsedSeconds));
            mLogDisplay.iterationsRateDisplay->setText(QString::number(elapsedIterations/elapsedSeconds));
            mLogDisplay.totalIterationsDisplay->setText(QString::number(iteration));
            mLogDisplay.swapAcceptanceRateDisplay->setText(QString::number(mModel->swapAcceptanceRate()));
            mLogDisplay.diffusionAcceptanceRateDisplay->setText(QString::number(mModel->diffusionAcceptanceRate(0)));
            mLogDisplay.jumpAcceptanceRateDisplay->setText(QString::number(mModel->jumpAcceptanceRate(0)));
        }
    }
    finish=clock();

    elapsedSeconds=(finish-start)/(float)CLOCKS_PER_SEC;
    elapsedIterations=iteration-startIteration;
    std::cout<<"Number of iterations: "<<elapsedIterations<<std::endl;
    std::cout<<"Elapsed time (in seconds): "<<elapsedSeconds<<std::endl;
    std::cout<<"Iterations per second: "<<elapsedIterations/elapsedSeconds<<std::endl;
    std::cout<<"Total number of iterations: "<<iteration<<std::endl;
    std::cout<<"Swap Accept Rate: "<<mModel->swapAcceptanceRate()<<std::endl;
    for(uint i=0;i<mModel->numReplicas();i++){
        std::cout<<"Diffusion Acceptance Rate "<<i<<": "<<mModel->diffusionAcceptanceRate(i)<<std::endl;
        std::cout<<"Jump Acceptance Rate "<<i<<": "<<mModel->jumpAcceptanceRate(i)<<std::endl;
    }
}
//####################################################################
// Function stepModel
//####################################################################
void ModelRenderWidget::stepModel()
{
    mRunning=true;
    mShowLastModel=true;

	bool loggingOn=mModel->useLogging();

	if(!loggingOn) mModel->toggleLogging();
    mModel->evolveChain();
    mRenderLevel=mModel->modelTreeDepth()-1;
	if(!loggingOn) mModel->toggleLogging();

    updateGL();

    mShowLastModel=false;
    mRunning=false;
}
//####################################################################
// Function wheelEvent
//####################################################################
void ModelRenderWidget::wheelEvent(QWheelEvent* event)
{
    mZ+=event->delta()/100.;
    event->accept();
    updateGL();
}
//####################################################################
// Function mousePressEvent
//####################################################################
void ModelRenderWidget::mousePressEvent(QMouseEvent* event)
{
    mLastMousePosition=event->pos();
}
//####################################################################
// Function mouseMoveEvent
//####################################################################
void ModelRenderWidget::mouseMoveEvent(QMouseEvent* event)
{
    int dx=event->x()-mLastMousePosition.x();
    int dy=event->y()-mLastMousePosition.y();

    if(mCtrlHeld){ mX+=.025*dx; mY-=.025*dy; }
    else{ mTheta+=-.01*dy; mPhi+=.01*dx; }

    mLastMousePosition=event->pos();
    updateGL();
    if (mVoxelWidget) mVoxelWidget->setCamera(mTheta,mPhi,mZ);
}
//####################################################################
// Function getModelImage
//####################################################################
QImage ModelRenderWidget::getModelImage(const ProductionList& model)
{
    mModelBuffer->bind();
    renderModel(model);
    mModelBuffer->release();
    if(mShowLastModel) renderModel(model);
    return mModelBuffer->toImage();
}
//####################################################################
// Function getModelTexture
//####################################################################
void ModelRenderWidget::readModel(const ProductionList& model)
{
    mModelBuffer->bind();
    renderModel(model);
    glReadPixels(0,0,mModelBuffer->size().width(),mModelBuffer->size().height(),GL_RGBA,GL_UNSIGNED_BYTE,NULL);
    mModelBuffer->release();
    if(mShowLastModel) renderModel(model);
}
//####################################################################
// Function saveToObj
//####################################################################
void ModelRenderWidget::saveToObj(QString path)
{
    if(!mModel) return;

    if(mShowMaxPosterior) mModel->generateMesh(mModel->maxPosteriorModel(),path+".obj");
    else mModel->generateMesh(mModel->model(mRenderLevel),path+".obj");
}
//####################################################################
// Function saveModelState
//####################################################################
void ModelRenderWidget::saveModelState(unsigned long& iter,QString outputPath,ModelSimulation* simulation,unsigned int saveFlags)
{
    QString baseString(outputPath+"/"+QString("siggraph_s")+QString::number(iter));

    if(SAVE_MODEL&saveFlags||SAVE_ALL&saveFlags) { QFile file(baseString+".psim"); mModel->saveToFile(file); if(simulation) simulation->update(iter); }
    if(SAVE_OBJ&saveFlags||SAVE_ALL&saveFlags) saveToObj(baseString);
    if(SAVE_IMAGE&saveFlags||SAVE_ALL&saveFlags) grabFrameBuffer().save(baseString+".png","PNG");
}
//####################################################################
// Function printCamera
//####################################################################
void ModelRenderWidget::printCamera()
{
    QVector4D camera(0,0,0,1);
    QVector4D up(0,1,0,0);
    QVector4D at(0,0,1,0);

    QMatrix4x4 modelView;
    modelView.setToIdentity();
    modelView.translate(mX,mY,0);
    modelView.lookAt(QVector3D(0,0,mZ),QVector3D(0,0,0),QVector3D(0,1,0));
    modelView.rotate(mTheta*100,1.0,0.0,0.0);
    modelView.rotate(mPhi*100,0.0,1.0,0.0);

	qDebug()<<modelView*camera;
    qDebug()<<modelView*up;
	qDebug()<<modelView*at;
}
//####################################################################
