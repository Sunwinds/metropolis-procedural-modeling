//#####################################################################
// Copyright 2009, Jerry Talton.
//#####################################################################
#ifdef USE_CUDA
  #include <GL/glew.h>
  #include "SketchBasedCUDAProceduralModel.hpp"
#endif
#include <QMenuBar>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QStatusBar>
#include "ModelRenderWidget.hpp"
#include "ModelWindow.hpp"
#include "ModelSimulation.hpp"
#include "SketchingWidget.hpp"
#include "SketchBasedProceduralModel.hpp"
#include "VoxelizationWidget.hpp"
#include "VoxelBasedProceduralModel.hpp"
#include "MondrianProceduralModel.hpp"

#define P_IMAGEWIDGET (static_cast<SketchingWidget*>(mTargetWidget))
#define P_VOXELWIDGET (static_cast<VoxelizationWidget*>(mTargetWidget))
#define P_VOXELPROCMODEL (static_cast<VoxelBasedProceduralModel*>(mModel))
#ifdef USE_CUDA
#define P_SKETCHMODEL (static_cast<SketchBasedCUDAProceduralModel*>(mModel))
#else
#define P_SKETCHMODEL (static_cast<SketchBasedProceduralModel*>(mModel))
#endif
#define P_MONDRIANMODEL (static_cast<MondrianProceduralModel*>(mModel))

#define VOXWIDTH 16
#define VOXHEIGHT 16

//####################################################################
// Function ModelWindow
//####################################################################
ModelWindow::ModelWindow(WidgetType target)
 :mType(target),mRenderWidget(new ModelRenderWidget(this)),mModel(NULL),mSimulation(NULL),mModelLoaded(false),mLoggingAct(NULL),mLevelAct(NULL),mMaxPosteriorAct(NULL),mGlobalLogSB(NULL),
  mNumReplicasSB(NULL),mTemperatureExponentSB(NULL),mFalsePositiveSB(NULL),mFalseNegativeSB(NULL),mLikelihoodMultiplierSB(NULL),mVoxelMissingSB(NULL),mEmptyHittingSB(NULL),mLikelihoodFactorSB(NULL)
{
    setupMenuAndButton();
    switch(mType)
    {
      case IMAGE:
        mTargetWidget=new SketchingWidget(this);
        break;
      case VOXEL:
        mTargetWidget=new VoxelizationWidget(this);
        P_VOXELWIDGET->setSize(VOXWIDTH,VOXHEIGHT);
        mRenderWidget->setVoxelWidget(P_VOXELWIDGET);
        P_VOXELWIDGET->setRenderWidget(mRenderWidget);
        break;
      case MA:
        mTargetWidget=new SketchingWidget(this);
        break;
      default: break;
    }
    mModel=newModel();
    move(50, 50);

    mControlPanel = new QDockWidget("Control Panel", this);
    mControlPanel->setFloating(true);
    mControlPanel->setFeatures(QDockWidget::DockWidgetFloatable|QDockWidget::DockWidgetClosable|QDockWidget::NoDockWidgetFeatures);
    mControlPanel->move(870, 100);
    mControlPanel->setWidget(new QWidget());
    mControlPanel->widget()->setLayout(new QVBoxLayout());

    setupSimMenuAndButton();
    setupStatDisplayPanel();

    setMinimumSize(320,240);
    resize(800,600);

    QWidget* centralWidget=new QWidget();

    QHBoxLayout* layout=new QHBoxLayout();
    layout->addWidget(mTargetWidget,100);
    layout->addStretch(1);
    layout->addWidget(mRenderWidget,100);

    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

    statusBar()->show();

    refreshToggles();
}
//####################################################################
// Function ~ModelWindow
//####################################################################
ModelWindow::~ModelWindow()
{
    delete mRenderWidget;
    delete mModel;
    delete mSimulation;
    delete mControlPanel;
}
//####################################################################
// Function createCheckBox
//####################################################################
QCheckBox* ModelWindow::createCheckBox(const char* name, QAction* action)
{
    QCheckBox* checkBox = new QCheckBox(tr(name));
    connect(checkBox, SIGNAL(clicked()), action, SLOT(trigger()));
    connect(action, SIGNAL(triggered(bool)), checkBox, SLOT(setChecked(bool)));
    checkBox->setChecked(action->isChecked());
//    connect(action, SIGNAL(triggered()), checkBox, SLOT(click()));
    return checkBox;
}
//####################################################################
// Function setupMenuAndButton
//####################################################################
void ModelWindow::setupMenuAndButton()
{
    QMenu* modelMenu=menuBar()->addMenu(tr("Model"));

    QAction* openModelAct=new QAction(tr("Open..."),this);
    openModelAct->setShortcut(QKeySequence::New);
    openModelAct->setIconVisibleInMenu(false);
    openModelAct->setStatusTip(tr("Open a procedural model"));
    connect(openModelAct,SIGNAL(triggered()),this,SLOT(openModel()));
    modelMenu->addAction(openModelAct);

    QAction* saveModelAct=new QAction(tr("Save mesh..."),this);
    saveModelAct->setStatusTip(tr("Save the output mesh"));
    connect(saveModelAct,SIGNAL(triggered()),this,SLOT(saveMesh()));
    modelMenu->addAction(saveModelAct);

    QAction* saveAxiomAct=new QAction(tr("Save as axiom..."),this);
    saveAxiomAct->setShortcut(tr("Alt+a"));
    saveAxiomAct->setStatusTip(tr("Save in axiom format for loading in parent grammar"));
    connect(saveAxiomAct,SIGNAL(triggered()),this,SLOT(saveModelAsAxiom()));
    modelMenu->addAction(saveAxiomAct);

    QAction* loadAxiomAct=new QAction(tr("Load as axiom..."),this);
    loadAxiomAct->setShortcut(tr("Ctrl+a"));
    loadAxiomAct->setStatusTip(tr("Load axiom format"));
    connect(loadAxiomAct,SIGNAL(triggered()),this,SLOT(loadModelAsAxiom()));
    modelMenu->addAction(loadAxiomAct);

    QAction* snapShotAct=new QAction(tr("Snapshot..."),this);
    snapShotAct->setShortcut(tr("Alt+i"));
    connect(snapShotAct,SIGNAL(triggered()),this,SLOT(snapShot()));
    modelMenu->addAction(snapShotAct);

    QMenu* workspaceMenu=menuBar()->addMenu(tr("&Workspace"));

    QAction* loadSimWorkspaceAct=new QAction(tr("Load..."),this);
    loadSimWorkspaceAct->setShortcut(QKeySequence::Open);
    loadSimWorkspaceAct->setStatusTip(tr("Load a saved simulation workspace"));
    connect(loadSimWorkspaceAct,SIGNAL(triggered()),this,SLOT(loadSimWorkspace()));
    workspaceMenu->addAction(loadSimWorkspaceAct);

    QAction* resetSimWorkspaceAct=new QAction(tr("Reset..."),this);
    resetSimWorkspaceAct->setShortcut(QKeySequence::Close);
    resetSimWorkspaceAct->setStatusTip(tr("Reset the simulation workspace"));
    connect(resetSimWorkspaceAct,SIGNAL(triggered()),this,SLOT(resetSimWorkspace()));
    workspaceMenu->addAction(resetSimWorkspaceAct);

    QToolBar* mMainToolBar=addToolBar(tr("MainToolBar"));
    mMainToolBar->addAction(openModelAct);
}
//####################################################################
// Function setupSimMenuAndButton
//####################################################################
void ModelWindow::setupSimMenuAndButton()
{
    QMenu* simMenu=menuBar()->addMenu(tr("&Simulation"));

    simMenu->addAction(mControlPanel->toggleViewAction());

    QAction* loadSimAct=new QAction(tr("Load..."),this);
    loadSimAct->setStatusTip(tr("Load a saved simulation"));
    connect(loadSimAct,SIGNAL(triggered()),this,SLOT(loadSim()));
    simMenu->addAction(loadSimAct);

    QAction* saveSimAct=new QAction(tr("Save..."),this);
    saveSimAct->setStatusTip(tr("Save a simulation"));
    connect(saveSimAct,SIGNAL(triggered()),this,SLOT(saveSim()));
    simMenu->addAction(saveSimAct);

    QAction* loggingAct=new QAction(tr("Use logging..."),this);
    loggingAct->setStatusTip(tr("Use logging"));
    loggingAct->setCheckable(true);
    connect(loggingAct,SIGNAL(triggered()),this,SLOT(logging()));
    mLoggingAct=loggingAct;

    QAction* maxPosteriorAct=new QAction(tr("Show max..."),this);
    maxPosteriorAct->setStatusTip(tr("Show max posterior model"));
    maxPosteriorAct->setCheckable(true);
    connect(maxPosteriorAct,SIGNAL(triggered()),this,SLOT(maxPosterior()));
    mMaxPosteriorAct=maxPosteriorAct;

    QAction* levelAct=new QAction(tr("Bias non-terminal level..."),this);
    levelAct->setStatusTip(tr("Avoid exponential selection bias"));
    levelAct->setCheckable(true);
    levelAct->setChecked(mModel->useLevelBiasedPolicy());
    connect(levelAct,SIGNAL(triggered()),this,SLOT(levelBias()));
    mLevelAct=levelAct;

    QAction* globalLogRateAct=new QAction(tr("Set global logging rate..."),this);
    connect(globalLogRateAct,SIGNAL(triggered()),this,SLOT(setGlobalLogRate()));

    QAction* setNumReplicasAct=new QAction(tr("Number of replicas in exchange..."),this);
    setNumReplicasAct->setStatusTip(tr("Set the number of replicas to be used in the parallel tempering exchange."));
    connect(setNumReplicasAct,SIGNAL(triggered()),this,SLOT(setNumReplicas()));

    QAction* setTemperatureExponentAct=new QAction(tr("Set the temperature exponent..."),this);
    setTemperatureExponentAct->setStatusTip(tr("Set the temperature exponent to control the geometric series in the parallel tempering exchange."));
    connect(setTemperatureExponentAct,SIGNAL(triggered()),this,SLOT(setTemperatureExponent()));

    QToolBar* mSimToolBar=addToolBar(tr("MainToolBar"));

    QAction* increRenderLevelAct=new QAction(tr("RenderLevel++"),this);
    increRenderLevelAct->setStatusTip(tr("Increment Render Level"));
    connect(increRenderLevelAct,SIGNAL(triggered()),this,SLOT(incrementRenderLevel()));
    mSimToolBar->addAction(increRenderLevelAct);

    QAction* decreRenderLevelAct=new QAction(tr("RenderLevel--"),this);
    decreRenderLevelAct->setStatusTip(tr("Decrement Render Level"));
    connect(decreRenderLevelAct,SIGNAL(triggered()),this,SLOT(decrementRenderLevel()));
    mSimToolBar->addAction(decreRenderLevelAct);

    QAction* printModelLevelAct=new QAction(tr("PrintCurLevel"),this);
    printModelLevelAct->setStatusTip(tr("Print Current Level Proc Model"));
    connect(printModelLevelAct,SIGNAL(triggered()),this,SLOT(printModelLevel()));
    mSimToolBar->addAction(printModelLevelAct);

    QAction* printMaxPosteriorAct=new QAction(tr("PrintMax"),this);
    printMaxPosteriorAct->setStatusTip(tr("Print MaxPosterior Proc Model"));
    connect(printMaxPosteriorAct,SIGNAL(triggered()),this,SLOT(printMaxPosterior()));
    mSimToolBar->addAction(printMaxPosteriorAct);

    QAction* saveCameraAct=new QAction(tr("SaveCamera"),this);
    saveCameraAct->setStatusTip(tr("Save Current Camera Setting"));
    connect(saveCameraAct,SIGNAL(triggered()),this,SLOT(saveCamera()));
    mSimToolBar->addAction(saveCameraAct);

    QAction* loadCameraAct=new QAction(tr("LoadCamera"),this);
    loadCameraAct->setStatusTip(tr("Load Saved Camera Setting"));
    connect(loadCameraAct,SIGNAL(triggered()),this,SLOT(loadCamera()));
    mSimToolBar->addAction(loadCameraAct);

    QAction* printModelStatsAct=new QAction(tr("PrintModelStats"),this);
    printModelStatsAct->setStatusTip(tr("Print model statistics"));
    connect(printModelStatsAct,SIGNAL(triggered()),this,SLOT(printModelStats()));
    mSimToolBar->addAction(printModelStatsAct);

    QGroupBox* simControlGroup=new QGroupBox(tr("General Control"));
    QGridLayout* simControlLayout=new QGridLayout();

    int row=0;
    QCheckBox* loggingCheckBox=createCheckBox("Use logging",loggingAct);
    simControlLayout->addWidget(loggingCheckBox, row,0);

    QCheckBox* maxPosteriorBox=createCheckBox("Show max posterior model",maxPosteriorAct);
    simControlLayout->addWidget(maxPosteriorBox,row,1);
    row++;

    QCheckBox* levelCheckBox=createCheckBox("Bias non-terminal level",levelAct);
    simControlLayout->addWidget(levelCheckBox,row,0);
    row++;

	QPushButton* toggleSimButton=new QPushButton(tr("Toggle simulation start/stop..."));
    connect(toggleSimButton,SIGNAL(clicked()),this,SLOT(toggleSim()));
    simControlLayout->addWidget(toggleSimButton,row,0);

    QPushButton* resetLikelihoodButton=new QPushButton(tr("Reset model likelihood..."));
    connect(resetLikelihoodButton,SIGNAL(clicked()),this,SLOT(resetModelLikelihood()));
    simControlLayout->addWidget(resetLikelihoodButton,row,1);
    row++;

    QLabel* globalLogLabel= new QLabel(tr("Set global logging rate"));
    simControlLayout->addWidget(globalLogLabel,row,0);
    mGlobalLogSB=new QSpinBox();
    mGlobalLogSB->setRange(1,10000);
    mGlobalLogSB->setSingleStep(100);
    mGlobalLogSB->setValue(mRenderWidget->objLogRate());
    connect(mGlobalLogSB,SIGNAL(valueChanged(int)),this,SLOT(setGlobalLogRate(int)));
    simControlLayout->addWidget(mGlobalLogSB,row,1);
    row++;

    QLabel* numReplicasLabel= new QLabel(tr("Set the number of replicas"));
    simControlLayout->addWidget(numReplicasLabel,row,0);
    mNumReplicasSB=new QSpinBox();
    mNumReplicasSB->setRange(1,10000);
    mNumReplicasSB->setSingleStep(1);
    connect(mNumReplicasSB,SIGNAL(valueChanged(int)),this,SLOT(setNumReplicas(int)));
    simControlLayout->addWidget(mNumReplicasSB,row,1);
    row++;

    QLabel* temperatureExponentLabel= new QLabel(tr("Set the temperature exponent"));
    simControlLayout->addWidget(temperatureExponentLabel,row,0);
    mTemperatureExponentSB=new QDoubleSpinBox();
    mTemperatureExponentSB->setRange(1,10);
    mTemperatureExponentSB->setSingleStep(0.1);
    connect(mTemperatureExponentSB,SIGNAL(valueChanged(double)),this,SLOT(setTemperatureExponent(double)));
    simControlLayout->addWidget(mTemperatureExponentSB,row,1);
    row++;
    resetPTparam(1, 1.0);

    simControlGroup->setLayout(simControlLayout);
    mControlPanel->widget()->layout()->addWidget(simControlGroup);

    switch(mType)
    {
        case IMAGE:
        {
            QGroupBox* imageSimControlGroup = new QGroupBox(tr("Image Simulation Control"));
            QGridLayout* imageSimControlLayout = new QGridLayout();

            row=0;
            QLabel* falsePositiveLabel= new QLabel(tr("Set the false positive multiplier"));
            imageSimControlLayout->addWidget(falsePositiveLabel,row,0);
            mFalsePositiveSB=new QDoubleSpinBox();
            mFalsePositiveSB->setRange(-50,50);
            mFalsePositiveSB->setSingleStep(1.0);
            mFalsePositiveSB->setValue(3.0);
            connect(mFalsePositiveSB,SIGNAL(valueChanged(double)),this,SLOT(setFalsePositiveMultiplier(double)));
            imageSimControlLayout->addWidget(mFalsePositiveSB,row,1);
            row++;

            QLabel* falseNegativeLabel= new QLabel(tr("Set the false negative multiplier"));
            imageSimControlLayout->addWidget(falseNegativeLabel,row,0);
            mFalseNegativeSB=new QDoubleSpinBox();
            mFalseNegativeSB->setRange(-50,50);
            mFalseNegativeSB->setSingleStep(1.0);
            mFalseNegativeSB->setValue(1.0);
            connect(mFalseNegativeSB,SIGNAL(valueChanged(double)),this,SLOT(setFalseNegativeMultiplier(double)));
            imageSimControlLayout->addWidget(mFalseNegativeSB,row,1);
            row++;

            QLabel* likelihoodMultiplierLabel= new QLabel(tr("Set the likelihood multiplier"));
            imageSimControlLayout->addWidget(likelihoodMultiplierLabel,row,0);
            mLikelihoodMultiplierSB=new QDoubleSpinBox();
            mLikelihoodMultiplierSB->setRange(-1000,1000);
            mLikelihoodMultiplierSB->setSingleStep(1.0);
            mLikelihoodMultiplierSB->setValue(25.0);
            connect(mLikelihoodMultiplierSB,SIGNAL(valueChanged(double)),this,SLOT(setLikelihoodMultiplier(double)));
            imageSimControlLayout->addWidget(mLikelihoodMultiplierSB,row,1);
            row++;

            imageSimControlGroup->setLayout(imageSimControlLayout);
            mControlPanel->widget()->layout()->addWidget(imageSimControlGroup);

            P_IMAGEWIDGET->setupMenuAndButton();
        }
        break;
        case VOXEL:
        {
            QGroupBox* voxelSimControlGroup = new QGroupBox(tr("Voxel Simulation Control"));
            QGridLayout* voxelSimControlLayout = new QGridLayout();

            row=0;
            QLabel* voxelMissingLabel= new QLabel(tr("Set the voxel missing factor"));
            voxelSimControlLayout->addWidget(voxelMissingLabel,row,0);
            mVoxelMissingSB=new QDoubleSpinBox();
            mVoxelMissingSB->setRange(0,50);
            mVoxelMissingSB->setSingleStep(0.1);
            mVoxelMissingSB->setValue(1.0);
            connect(mVoxelMissingSB,SIGNAL(valueChanged(double)),this,SLOT(setVoxelMissingFactor(double)));
            voxelSimControlLayout->addWidget(mVoxelMissingSB,row,1);
            row++;

            QLabel* emptyHittingLabel= new QLabel(tr("Set the empty hitting factor"));
            voxelSimControlLayout->addWidget(emptyHittingLabel,row,0);
            mEmptyHittingSB=new QDoubleSpinBox();
            mEmptyHittingSB->setRange(0,50);
            mEmptyHittingSB->setSingleStep(0.1);
            mEmptyHittingSB->setValue(1.0);
            connect(mEmptyHittingSB,SIGNAL(valueChanged(double)),this,SLOT(setEmptyHittingFactor(double)));
            voxelSimControlLayout->addWidget(mEmptyHittingSB,row,1);
            row++;

            QLabel* likelihoodFactorLabel= new QLabel(tr("Set the likelihood scaling factor"));
            voxelSimControlLayout->addWidget(likelihoodFactorLabel,row,0);
            mLikelihoodFactorSB=new QDoubleSpinBox();
            mLikelihoodFactorSB->setRange(0,10);
            mLikelihoodFactorSB->setSingleStep(0.1);
            mLikelihoodFactorSB->setValue(1.0);
            connect(mLikelihoodFactorSB,SIGNAL(valueChanged(double)),this,SLOT(setLikelihoodFactor(double)));
            voxelSimControlLayout->addWidget(mLikelihoodFactorSB,row,1);
            row++;

            voxelSimControlGroup->setLayout(voxelSimControlLayout);
            mControlPanel->widget()->layout()->addWidget(voxelSimControlGroup);

            P_VOXELWIDGET->setupMenuAndButton();
        }
        break;
        default: break;
    }
}
//####################################################################
// Function newModel
//####################################################################
void ModelWindow::setupStatDisplayPanel()
{
    QGroupBox* statGroup=new QGroupBox(tr("Statistic"));
    QGridLayout* statLayout=new QGridLayout();

    int row=0;
    QLabel* logRefreshRateLabel= new QLabel(tr("Set the logging refresh rate"));
    statLayout->addWidget(logRefreshRateLabel,row,0);
    QSpinBox* logRefreshSB=new QSpinBox();
    logRefreshSB->setRange(1,10000);
    logRefreshSB->setSingleStep(100);
    logRefreshSB->setValue(500);
    mRenderWidget->logDisplay().refreshRate=500;
    connect(logRefreshSB,SIGNAL(valueChanged(int)),this,SLOT(setLoggingRefreshRate(int)));
    statLayout->addWidget(logRefreshSB,row,1);
    row++;

    QLabel* elapsedIterationsLabel= new QLabel(tr("Number of iterations:"));
    statLayout->addWidget(elapsedIterationsLabel,row,0);
    QLineEdit* elapsedIterationsDisplay=new QLineEdit();
    elapsedIterationsDisplay->setReadOnly(true);
    elapsedIterationsDisplay->setAlignment(Qt::AlignRight);
    mRenderWidget->logDisplay().elapsedIterationsDisplay=elapsedIterationsDisplay;
    statLayout->addWidget(elapsedIterationsDisplay,row,1);
    row++;

    QLabel* elapsedSecondsLabel= new QLabel(tr("Elapsed time (in seconds):"));
    statLayout->addWidget(elapsedSecondsLabel,row,0);
    QLineEdit* elapsedSecondsDisplay=new QLineEdit();
    elapsedSecondsDisplay->setReadOnly(true);
    elapsedSecondsDisplay->setAlignment(Qt::AlignRight);
    mRenderWidget->logDisplay().elapsedSecondsDisplay=elapsedSecondsDisplay;
    statLayout->addWidget(elapsedSecondsDisplay,row,1);
    row++;

    QLabel* iterationsRateLabel= new QLabel(tr("Iterations per second:"));
    statLayout->addWidget(iterationsRateLabel,row,0);
    QLineEdit* iterationsRateDisplay=new QLineEdit();
    iterationsRateDisplay->setReadOnly(true);
    iterationsRateDisplay->setAlignment(Qt::AlignRight);
    mRenderWidget->logDisplay().iterationsRateDisplay=iterationsRateDisplay;
    statLayout->addWidget(iterationsRateDisplay,row,1);
    row++;

    QLabel* totalIterationsLabel= new QLabel(tr("Total number of iterations:"));
    statLayout->addWidget(totalIterationsLabel,row,0);
    QLineEdit* totalIterationsDisplay=new QLineEdit();
    totalIterationsDisplay->setReadOnly(true);
    totalIterationsDisplay->setAlignment(Qt::AlignRight);
    mRenderWidget->logDisplay().totalIterationsDisplay=totalIterationsDisplay;
    statLayout->addWidget(totalIterationsDisplay,row,1);
    row++;

    QLabel* swapAcceptanceRateLabel= new QLabel(tr("Swap Accept Rate (%):"));
    statLayout->addWidget(swapAcceptanceRateLabel,row,0);
    QLineEdit* swapAcceptanceRateDisplay=new QLineEdit();
    swapAcceptanceRateDisplay->setReadOnly(true);
    swapAcceptanceRateDisplay->setAlignment(Qt::AlignRight);
    mRenderWidget->logDisplay().swapAcceptanceRateDisplay=swapAcceptanceRateDisplay;
    statLayout->addWidget(swapAcceptanceRateDisplay,row,1);
    row++;

    QLabel* diffusionAcceptanceRateLabel= new QLabel(tr("Diffusion Acceptance Rate 0 (%):"));
    statLayout->addWidget(diffusionAcceptanceRateLabel,row,0);
    QLineEdit* diffusionAcceptanceRateDisplay=new QLineEdit();
    diffusionAcceptanceRateDisplay->setReadOnly(true);
    diffusionAcceptanceRateDisplay->setAlignment(Qt::AlignRight);
    mRenderWidget->logDisplay().diffusionAcceptanceRateDisplay=diffusionAcceptanceRateDisplay;
    statLayout->addWidget(diffusionAcceptanceRateDisplay,row,1);
    row++;

    QLabel* jumpAcceptanceRateLabel= new QLabel(tr("Jump Acceptance Rate 0 (%):"));
    statLayout->addWidget(jumpAcceptanceRateLabel,row,0);
    QLineEdit* jumpAcceptanceRateDisplay=new QLineEdit();
    jumpAcceptanceRateDisplay->setReadOnly(true);
    jumpAcceptanceRateDisplay->setAlignment(Qt::AlignRight);
    mRenderWidget->logDisplay().jumpAcceptanceRateDisplay=jumpAcceptanceRateDisplay;
    statLayout->addWidget(jumpAcceptanceRateDisplay,row,1);
    row++;

    mRenderWidget->logDisplay().reset(0);
    statGroup->setLayout(statLayout);
    mControlPanel->widget()->layout()->addWidget(statGroup);
}
//####################################################################
// Function resetPTparam
//####################################################################
void ModelWindow::resetPTparam(int numReplicas, float temperatureExponent)
{
    mNumReplicasSB->setValue(numReplicas);
    mTemperatureExponentSB->setValue(temperatureExponent);
}
//####################################################################
// Function newModel
//####################################################################
ProceduralModel* ModelWindow::newModel() const
{
    if(mType==IMAGE)
#ifdef USE_CUDA
        return new SketchBasedCUDAProceduralModel(mRenderWidget,*P_IMAGEWIDGET);
#else
        return new SketchBasedProceduralModel(mRenderWidget,*P_IMAGEWIDGET);
#endif
    else if(mType==VOXEL) return new VoxelBasedProceduralModel(mRenderWidget,*P_VOXELWIDGET);
    else if(mType==MA) return new MondrianProceduralModel(mRenderWidget,*P_IMAGEWIDGET);
    else return new ProceduralModel(mRenderWidget);
}
//####################################################################
// Function toggleSim
//####################################################################
void ModelWindow::toggleSim()
{
	if(mModelLoaded && !mRenderWidget->isRunning())
	{
		saveSimWorkspace(); if (!mSimulation) return;
		QString pathName=mSimulation->pathName();
		mModel->getResourceHandler().disableLighting();
		mRenderWidget->updateGL();
		mRenderWidget->evolveChain(mNumIterations,pathName,mSimulation);
	}
	else
	{
		mModel->getResourceHandler().enableLighting();
		mRenderWidget->updateGL();
		mRenderWidget->abort();
	}
}
//####################################################################
// Function keyPressEvent
//####################################################################
void ModelWindow::keyPressEvent(QKeyEvent* event)
{
    switch(event->key())
    {
      case Qt::Key_Space:
        toggleSim();
        break;
      case Qt::Key_Control: mRenderWidget->controlKeyHeld(true); break;
      case Qt::Key_S: mRenderWidget->setCamera(-4.5,0,-10); break;
      case Qt::Key_L: if(mModelLoaded&&!mRenderWidget->isRunning()) { mModel->getResourceHandler().disableLighting(); mRenderWidget->stepModel(); } break;
      case Qt::Key_X: mModel->resetModel(); mRenderWidget->resetRenderLevel(); mRenderWidget->updateGL(); break;
      default: event->ignore(); break;
    };
}
//####################################################################
// Function keyReleaseEvent
//####################################################################
void ModelWindow::keyReleaseEvent(QKeyEvent* event)
{
    switch(event->key())
    {
      case Qt::Key_Control: mRenderWidget->controlKeyHeld(false); break;
      default: event->ignore(); break;
    };
}
//####################################################################
// Function openModel
//####################################################################
void ModelWindow::openModel()
{
    char* procModBaseDir=getenv("PROC_MOD_BASE_DIR");
    QString fileFilter="Model (*." + QString(PROCEDURAL_PLUGIN_EXTENSION) + ")";
# if PROCEDURAL_PLATFORM == PLATFORM_MAC
    QString modelPath=QFileDialog::getOpenFileName(this,"Select grammar plugin",QString(procModBaseDir)+QString("/Code/GrammarGen/Plugins/*"),fileFilter);
# else
    QString modelPath=QFileDialog::getOpenFileName(this,"Select grammar plugin",QString(procModBaseDir)+QString("/Code/GrammarGen/Plugins"),fileFilter);
# endif
    if(modelPath.isEmpty()) return;
    openModel(modelPath);    
    resetPTparam(mModel->numReplicas(),mModel->temperatureExponent());
    mRenderWidget->logDisplay().reset(mNumIterations);
    delete mSimulation; mSimulation=NULL;
}
//####################################################################
// Function openModel
//####################################################################
void ModelWindow::openModel(QString modelPath)
{
    mModelPluginPath=modelPath;
    mRenderWidget->setProceduralModel(NULL);
    delete mModel; mModel=newModel(); mModel->setWidget(mRenderWidget);
    if(!mModel->loadFromFile(mModelPluginPath)) exit(-1);
    mRenderWidget->setProceduralModel(mModel);
    mNumIterations=0; mModelLoaded=true;
    refreshToggles();    
}
//####################################################################
// Function saveMesh
//####################################################################
void ModelWindow::saveMesh()
{
    QString fileFilter="Mesh Obj Files (*.obj)";
    QString fileName=QFileDialog::getSaveFileName(this,"Save mesh as",ModelSimulation::getBasePath()+".*",fileFilter);
    if(!fileName.isEmpty()) mRenderWidget->saveToObj(fileName);
}
//####################################################################
// Function openSimulation
//####################################################################
void ModelWindow::loadSimWorkspace()
{
    delete mSimulation; mSimulation=NULL;

    QString fileFilter="Simulation Workspace files (*.wsim)";
    QString fileName=QFileDialog::getOpenFileName(this,"Select simulation workspace", ModelSimulation::getBasePath()+".*",fileFilter);
    if(fileName.isEmpty()) return;

    QFile simFile(fileName);
    mSimulation=ModelSimulation::loadSimulation(simFile,mRenderWidget); if(!mSimulation) exit(-1);

    if(mType==IMAGE){
        P_IMAGEWIDGET->loadImage(QString(mSimulation->pathName())+"/targetImage.png");
        const QImage& image=P_IMAGEWIDGET->image();
        resize(2*image.width(),image.height()+iconSize().height()+menuBar()->height()+statusBar()->height());
        P_IMAGEWIDGET->refresh();
    }
    else if(mType==VOXEL){ }
//    { P_VOXELWIDGET->loadModel(QString(mSimulation->pathName())+"/targetModel.obj"); }

    openModel(mSimulation->libraryPath());
    mNumIterations=mSimulation->getNumIterations();
    QFile modelFile(mSimulation->modelPath());
    mModel->readFromFile(modelFile);
    resetPTparam(mModel->numReplicas(),mModel->temperatureExponent());
    mRenderWidget->logDisplay().reset(mNumIterations);
    mRenderWidget->updateGL();
}
//####################################################################
// Function saveSimWorkspace
//####################################################################
void ModelWindow::saveSimWorkspace()
{
    if(mSimulation||loadNewSimWorkspace())
    {
        mSimulation->save();
        if(mType==VOXEL){ QFile model(P_VOXELWIDGET->getFilePath()); model.copy(QString(mSimulation->pathName())+"/targetModel.obj"); }
        else if(mType==IMAGE) P_IMAGEWIDGET->saveImage(QString(mSimulation->pathName())+"/targetImage.png");
    }
}
//####################################################################
// Function newSimWorkspace
//####################################################################
bool ModelWindow::loadNewSimWorkspace()
{
    QString simPath;
    do
    {
        simPath=QFileDialog::getSaveFileName(this,"Save simulation workspace",ModelSimulation::getBasePath(),tr("Workspace (*.wsim)"));
        if(simPath.isEmpty()) return false;
        else if(QDir().exists(simPath)) QMessageBox::critical(this,"Invalid Name", "The specified workspace name is invalid.");
        else break;
    }
    while(true);

    QDir dir(simPath); QFile library(mModelPluginPath);
    if(!QDir().mkpath(simPath)) { QMessageBox::critical(this,"Invalid Directory", "The simulation workspace directory could not be created."); return false; }

    delete mSimulation; mSimulation=NULL;
    mSimulation=ModelSimulation::newSimulation(dir.dirName(),dir,library,mRenderWidget);
    return true;
}
//####################################################################
// Function loadSim
//####################################################################
void ModelWindow::loadSim()
{
    QString fileFilter="Simulation files (*.psim)";
    QString fileName=QFileDialog::getOpenFileName(this,"Select grammar plugin",ModelSimulation::getBasePath()+".*",fileFilter);
    QFile simFile(fileName);
    mModel->readFromFile(simFile);
    resetPTparam(mModel->numReplicas(),mModel->temperatureExponent());
    mRenderWidget->logDisplay().reset(mNumIterations);
    mRenderWidget->updateGL();
}
//####################################################################
// Function saveSim
//####################################################################
void ModelWindow::saveSim()
{
    QString fileFilter="Simulation files (*.psim)";
    QString fileName=QFileDialog::getSaveFileName(this,"Save sim as",ModelSimulation::getBasePath()+".*",fileFilter);
    QFile simFile(fileName);
    mModel->saveToFile(simFile);
}
//####################################################################
// Function setNumReplicas
//####################################################################
void ModelWindow::setNumReplicas(int val)
{
    if(!mModel || !mModelLoaded) return;
    mModel->setNumReplicas(val);
}
//####################################################################
// Function setNumReplicas
//####################################################################
void ModelWindow::setNumReplicas()
{
    bool ok;
    if(!mModel) return;
    int numReplicas=QInputDialog::getInt(this,tr("Parallel Tempering"),tr("Number of replicas:"),mModel->numReplicas(),1,10000,1,&ok);
    if(!ok) return;
    setNumReplicas(numReplicas);
    mNumReplicasSB->setValue(numReplicas);
}
//####################################################################
// Function setTemperatureExponent
//####################################################################
void ModelWindow::setTemperatureExponent(double val)
{
    if(!mModel || !mModelLoaded) return;
    mModel->setTemperatureExponent(val);
}
//####################################################################
// Function setTemperatureExponent
//####################################################################
void ModelWindow::setTemperatureExponent()
{
    bool ok;
    if(!mModel) return;
    double exponent=QInputDialog::getDouble(this,tr("Parallel Tempering"),tr("Temperature profile exponent:"),mModel->temperatureExponent(),1,10,1,&ok);
    if(!ok) return;
    setTemperatureExponent(exponent);
    mTemperatureExponentSB->setValue(exponent);
}
//####################################################################
// Function setGlobalLogRate
//####################################################################
void ModelWindow::setGlobalLogRate(int val)
{
    if(!mModel) return;

    mRenderWidget->setImageLogRate(val);
    mRenderWidget->setObjLogRate(val);
    mRenderWidget->setModelLogRate(val);
}
//####################################################################
// Function setGlobalLogRate
//####################################################################
void ModelWindow::setGlobalLogRate()
{
    bool ok;
    if(!mModel) return;
    int rate=QInputDialog::getInt(this,tr("OBJ+Image+PSim Logging Rate"),tr("Iterations per output:"),mRenderWidget->objLogRate(),1,10000,1,&ok);
    if(!ok) return;
    setGlobalLogRate(rate);
    mGlobalLogSB->setValue(rate);
}
//####################################################################
// Function saveModelAsAxiom
//####################################################################
void ModelWindow::saveModelAsAxiom()
{
    QString fileFilter="Axiom files (*.axiom)";
    QString fileName=QFileDialog::getSaveFileName(this,"Save axiom as",ModelSimulation::getBasePath()+".*",fileFilter);
    QFile axiomFile(fileName);
    mModel->writeStringAsAxiom(mModel->maxPosteriorModel(),axiomFile);
}
//####################################################################
// Function loadModelAsAxiom
//####################################################################
void ModelWindow::loadModelAsAxiom()
{
    QString fileFilter="Axiom files (*.axiom)";
    QString fileName=QFileDialog::getOpenFileName(this,"Load axiom as",ModelSimulation::getBasePath()+".*",fileFilter);
    QFile axiomFile(fileName);
    mModel->readStringAsAxiom(axiomFile);
    mRenderWidget->resetRenderLevel();
    mRenderWidget->updateGL();
}
//####################################################################
// Function resetSimWorkspace
//####################################################################
void ModelWindow::resetSimWorkspace()
{
    mRenderWidget->setProceduralModel(NULL);
    delete mModel; mModel=newModel();
    delete mSimulation; mSimulation=NULL;
    mNumIterations=0; mModelLoaded=false;
    if(mType==IMAGE) P_IMAGEWIDGET->clear();
    mModelPluginPath.clear();
    mRenderWidget->updateGL();
    refreshToggles();
 }
//####################################################################
// Function snapShot
//####################################################################
void ModelWindow::snapShot()
{
    QString fileFilter="Image (*.png)";
    QString path=mSimulation?mSimulation->pathName():getenv("PROC_MOD_BASE_DIR");
    QString imagePath=QFileDialog::getSaveFileName(this,"Save model snapshot",path,fileFilter);
    if(!imagePath.isEmpty()) mRenderWidget->grabFrameBuffer(true).save(imagePath);
}
//####################################################################
// Function logging
//####################################################################
void ModelWindow::logging()
{
    if(mModel) mModel->toggleLogging();
}
//####################################################################
// Function maxPosterior
//####################################################################
void ModelWindow::maxPosterior()
{
    if(mRenderWidget) mRenderWidget->toggleShowMaxPosteriorModel();
}
//####################################################################
// Function levelBias
//####################################################################
void ModelWindow::levelBias()
{
    if(mModel) mModel->toggleLevelPolicy();
}
//####################################################################
// Function resetModelLikelihood()
//####################################################################
void ModelWindow::resetModelLikelihood()
{
    if(mModel && mModelLoaded) mModel->initializeSimulation();
}
//####################################################################
// Function refreshToggles
//####################################################################
void ModelWindow::refreshToggles()
{
    if(!mModel) return;
    mLevelAct->setChecked(mModel->useLevelBiasedPolicy());
    mLoggingAct->setChecked(mModel->useLogging());
    mMaxPosteriorAct->setChecked(mRenderWidget->showMaxPosteriorModel());
    mNumReplicasSB->setValue(mModel->numReplicas());
    mTemperatureExponentSB->setValue(mModel->temperatureExponent());
    if(mType==IMAGE)
    {   mFalsePositiveSB->setValue(P_SKETCHMODEL->falsePositiveMultiplier());
        mFalseNegativeSB->setValue(P_SKETCHMODEL->falseNegativeMultiplier());
        mLikelihoodMultiplierSB->setValue(P_SKETCHMODEL->likelihoodMultiplier());
    }
    else if(mType==VOXEL)
    {
        mVoxelMissingSB->setValue(P_VOXELPROCMODEL->getVoxelMissingFactor());
        mEmptyHittingSB->setValue(P_VOXELPROCMODEL->getEmptyHittingFactor());
        mLikelihoodFactorSB->setValue(P_VOXELPROCMODEL->getLikelihoodFactor());
    }
}
//####################################################################
// Function incrementRenderLevel
//####################################################################
void ModelWindow::incrementRenderLevel()
{
    if(mModel && mModelLoaded) { mRenderWidget->incrementRenderLevel(); mRenderWidget->updateGL(); }
}
//####################################################################
// Function incrementRenderLevel
//####################################################################
void ModelWindow::decrementRenderLevel()
{
    if(mModel && mModelLoaded) { mRenderWidget->decrementRenderLevel(); mRenderWidget->updateGL(); }
}
//####################################################################
// Function incrementRenderLevel
//####################################################################
void ModelWindow::printModelLevel()
{
    if(mModel && mModelLoaded) { mRenderWidget->printModelLevel(); }
}
//####################################################################
// Function incrementRenderLevel
//####################################################################
void ModelWindow::printMaxPosterior()
{
    if(mModel && mModelLoaded) { mRenderWidget->printMaxPosterior(); }
}
//####################################################################
// Function saveCamera
//####################################################################
void ModelWindow::saveCamera()
{
    if(mModel && mModelLoaded) { mRenderWidget->saveCamera(); mRenderWidget->printCamera(); }
}
//####################################################################
// Function loadCamera
//####################################################################
void ModelWindow::loadCamera()
{
    if(mModel && mModelLoaded) { mRenderWidget->loadCamera(); }
}
//####################################################################
// Function printModelStats
//####################################################################
void ModelWindow::printModelStats()
{
    if(mModel && mModelLoaded)
    {
        const ProductionList& string=mMaxPosteriorAct->isChecked()?mModel->maxPosteriorModel():mModel->currentModel();
        ulong numSymbols,numDiffusionParams,numFormalParams;
        mModel->modelStats(string,numSymbols,numDiffusionParams,numFormalParams);
        qDebug()<<"Number of symbols: "<<numSymbols;
        qDebug()<<"Number of diffusion parameters: "<<numDiffusionParams;
        qDebug()<<"Number of formal parameters: "<<numFormalParams;
        qDebug()<<"Depth of tree: "<<mModel->modelTreeDepth();
    }
}
//####################################################################
// Function setFalsePositiveMultiplier
//####################################################################
void ModelWindow::setFalsePositiveMultiplier(double val)
{
    if(!mModel) return;
    P_SKETCHMODEL->setFalsePositiveMultiplier(val);
    resetModelLikelihood();
}
//####################################################################
// Function setFalsePositiveMultiplier
//####################################################################
void ModelWindow::setFalsePositiveMultiplier()
{
    bool ok; if(!mModel) return;
    double multiplier=QInputDialog::getDouble(this,tr("False Positive"),tr("Penalty Multiplier:"),P_SKETCHMODEL->falsePositiveMultiplier(),-50,50,5,&ok);
    if(!ok) return;
    setFalsePositiveMultiplier(multiplier);
    mFalsePositiveSB->setValue(multiplier);
}
//####################################################################
// Function setFalseNegativeMultiplier
//####################################################################
void ModelWindow::setFalseNegativeMultiplier(double val)
{
    if(!mModel) return;
    P_SKETCHMODEL->setFalseNegativeMultiplier(val);
    resetModelLikelihood();
}
//####################################################################
// Function setFalseNegativeMultiplier
//####################################################################
void ModelWindow::setFalseNegativeMultiplier()
{
    bool ok; if(!mModel) return;
    double multiplier=QInputDialog::getDouble(this,tr("False Negative"),tr("Penalty Multiplier:"),P_SKETCHMODEL->falseNegativeMultiplier(),-50,50,5,&ok);
    if(!ok) return;
    setFalseNegativeMultiplier(multiplier);
    mFalseNegativeSB->setValue(multiplier);
}
//####################################################################
// Function setLikelihoodMultiplier
//####################################################################
void ModelWindow::setLikelihoodMultiplier(double val)
{
    if(!mModel) return;
    P_SKETCHMODEL->setLikelihoodMultiplier(val);
    resetModelLikelihood();
}
//####################################################################
// Function setLikelihoodMultiplier
//####################################################################
void ModelWindow::setLikelihoodMultiplier()
{
    bool ok; if(!mModel) return;
    double multiplier=QInputDialog::getDouble(this,tr("Likelihood"),tr("Likelihood Multiplier(1 = Very Random, 50 = Not Random, Negative = Inverted Likelihood):"),P_SKETCHMODEL->likelihoodMultiplier(),-1000,1000,5,&ok);
    if(!ok) return;
    setLikelihoodMultiplier(multiplier);
    mLikelihoodMultiplierSB->setValue(multiplier);
}
//####################################################################
// Function toggleIgnoreColor
//####################################################################
void ModelWindow::toggleIgnoreColor()
{
    if(!mModel) return;
    P_SKETCHMODEL->toggleIgnoreColor();
    resetModelLikelihood();
}
//####################################################################
// Function setVoxelMissingFactor
//####################################################################
void ModelWindow::setVoxelMissingFactor(double val)
{
    if (!mModel) return;
    P_VOXELPROCMODEL->setVoxelMissingFactor(val);
    resetModelLikelihood();
}
//####################################################################
// Function setVoxelMissingFactor
//####################################################################
void ModelWindow::setVoxelMissingFactor()
{
    bool ok;
    if (!mModel) return;
    double factor=QInputDialog::getDouble(this,tr("Voxel Missing"),tr("Penalty Factor:"),P_VOXELPROCMODEL->getVoxelMissingFactor(),0,50,5,&ok);
    if (!ok) return;
    setVoxelMissingFactor(factor);
    mVoxelMissingSB->setValue(factor);
}
//####################################################################
// Function setEmptyHittingFactor
//####################################################################
void ModelWindow::setEmptyHittingFactor(double val)
{
    if (!mModel) return;
    P_VOXELPROCMODEL->setEmptyHittingFactor(val);
    resetModelLikelihood();
}
//####################################################################
// Function setEmptyHittingFactor
//####################################################################
void ModelWindow::setEmptyHittingFactor()
{
    bool ok;
    if (!mModel) return;
    double factor=QInputDialog::getDouble(this,tr("Empty Hitting"),tr("Penalty Factor:"),P_VOXELPROCMODEL->getEmptyHittingFactor(),0,50,5,&ok);
    if (!ok) return;
    setEmptyHittingFactor(factor);
    mEmptyHittingSB->setValue(factor);
}
//####################################################################
// Function setLikehoodFactor
//####################################################################
void ModelWindow::setLikelihoodFactor(double val)
{
    if (!mModel) return;
    P_VOXELPROCMODEL->setLikelihoodFactor(val);
    resetModelLikelihood();
}
//####################################################################
// Function setLikehoodFactor
//####################################################################
void ModelWindow::setLikelihoodFactor()
{
    bool ok;
    if (!mModel) return;
    double factor=QInputDialog::getDouble(this,tr("Likelihood"),tr("Likelihood Factor:"),P_VOXELPROCMODEL->getLikelihoodFactor(),0,10,5,&ok);
    if (!ok) return;
    setLikelihoodFactor(factor);
    mLikelihoodFactorSB->setValue(factor);
}
//####################################################################
// Function setLoggingRefreshRate
//####################################################################
void ModelWindow::setLoggingRefreshRate(int val)
{
    mRenderWidget->logDisplay().refreshRate=val;
}
//####################################################################
