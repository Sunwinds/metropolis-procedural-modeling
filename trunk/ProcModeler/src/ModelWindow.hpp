//#####################################################################
// Copyright 2009, Jerry Talton.
//#####################################################################
#ifndef __MODEL_WINDOW__
#define __MODEL_WINDOW__

#include <QMainWindow>
#include <QDir>
#include <QCheckBox>
#include <QDoubleSpinBox>

typedef enum TargetWidget {IMAGE, VOXEL, MA} WidgetType;

class ModelRenderWidget;
class SketchingWidget;
class ProceduralModel;
class ModelSimulation;
class QAction;

//####################################################################
// Class ModelWindow
//####################################################################
class ModelWindow : public QMainWindow
{
    Q_OBJECT

protected:
    WidgetType mType;
    ModelRenderWidget* mRenderWidget;
    QWidget* mTargetWidget;
    QDockWidget* mControlPanel;
    ProceduralModel* mModel;
	ModelSimulation* mSimulation;
    unsigned long mNumIterations;
    QString mModelPluginPath;
    bool mModelLoaded;
    QAction *mLoggingAct,*mLevelAct,*mMaxPosteriorAct;
    QSpinBox *mGlobalLogSB,*mNumReplicasSB;
    QDoubleSpinBox *mTemperatureExponentSB;
    QDoubleSpinBox *mFalsePositiveSB,*mFalseNegativeSB,*mLikelihoodMultiplierSB;
    QDoubleSpinBox *mVoxelMissingSB,*mEmptyHittingSB,*mLikelihoodFactorSB;

public:
    ModelWindow(TargetWidget target=IMAGE);
    ~ModelWindow();

protected:
    void setupMenuAndButton();
    void setupSimMenuAndButton();
    QCheckBox* createCheckBox(const char* name, QAction* action);
    void setupStatDisplayPanel();
    void resetPTparam(int numReplicas, float temperatureExponent);
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void keyReleaseEvent(QKeyEvent* event);
	ProceduralModel* newModel() const;

private slots:
    void openModel();
    void openModel(QString modelPath);
    void loadSim();
    void saveSim();
    void saveMesh();
    bool loadNewSimWorkspace();
    void loadSimWorkspace();
    void saveSimWorkspace();
    void resetSimWorkspace();
    void setGlobalLogRate();
    void setGlobalLogRate(int);
    void resetModelLikelihood();
    void saveModelAsAxiom();
    void loadModelAsAxiom();
    void snapShot();
    void levelBias();
    void logging();
    void maxPosterior();
    void refreshToggles();
    void incrementRenderLevel();
    void decrementRenderLevel();
    void printModelLevel();
    void printMaxPosterior();
    void saveCamera();
    void loadCamera();
    void printModelStats();

    void setFalsePositiveMultiplier();
    void setFalsePositiveMultiplier(double);
    void setFalseNegativeMultiplier();
    void setFalseNegativeMultiplier(double);
    void setLikelihoodMultiplier();
    void setLikelihoodMultiplier(double);
    void toggleIgnoreColor();
    void toggleSim();
    void setVoxelMissingFactor();
    void setVoxelMissingFactor(double);
    void setEmptyHittingFactor();
    void setEmptyHittingFactor(double);
    void setLikelihoodFactor();
    void setLikelihoodFactor(double);
    void setNumReplicas();
    void setNumReplicas(int);
    void setTemperatureExponent();
    void setTemperatureExponent(double);
    void setLoggingRefreshRate(int);
//####################################################################
};

#endif // __MODEL_WINDOW__
