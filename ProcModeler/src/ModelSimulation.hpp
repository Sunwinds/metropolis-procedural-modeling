//#####################################################################
// Copyright 2009, Jared Duke
//#####################################################################
#ifndef __MODEL_SIMULATION__
#define __MODEL_SIMULATION__

#include <QString>
#include <QDir>
#include <stdlib.h>

class ModelRenderWidget;

//####################################################################
// Class ModelSimulation
//####################################################################
class ModelSimulation
{

protected:
    QString mName,mLibraryName,mPathName;
    unsigned long mNumIterations;
    ModelRenderWidget* mModelWidget;
    static QDir mBaseDir;

public:
    QString pathName() const
    { return QString(mPathName); }

    QString libraryPath() const
    { return mPathName+"/"+mLibraryName; }

    QString modelPath() const
    { return mPathName+"/"+"siggraph_s"+QString::number(mNumIterations)+".psim"; }

    static QString getBasePath() 
    { return QString(getenv("PROC_MOD_BASE_DIR"))+"/Simulations/"; }

    QString name() const
    { return QString(mName); }

    unsigned long getNumIterations() const
    { return mNumIterations; }

public:
    ModelSimulation(QString name,QString libraryName,QString pathName,unsigned long numIterations,ModelRenderWidget* modelWidget);
    ~ModelSimulation();
    
    static ModelSimulation* newSimulation(QString name,QDir dir,QFile& library,ModelRenderWidget* modelWidget);
    static ModelSimulation* loadSimulation(QFile& simFile,ModelRenderWidget* modelWidget);
    
    void save();
    void update(const unsigned long& numIterations);
//####################################################################
};

#endif //__MODEL_SIMULATION__
