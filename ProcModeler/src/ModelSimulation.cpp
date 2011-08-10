//#####################################################################
// Copyright 2009, Jared Duke
//#####################################################################
#include "ModelSimulation.hpp"
#include "ModelRenderWidget.hpp"
#include <QFileDialog>
#include <QTextStream>
#include <iostream>

QDir ModelSimulation::mBaseDir=QDir::current();
//####################################################################
// Function ModelSimulation
//####################################################################
ModelSimulation::ModelSimulation(QString name,QString libraryName,QString pathName,unsigned long numIterations,ModelRenderWidget* modelWidget)
	: mName(name),mLibraryName(libraryName),mPathName(pathName),mNumIterations(numIterations),mModelWidget(modelWidget)
{
}
//####################################################################
// Function ~ModelSimulation
//####################################################################
ModelSimulation::~ModelSimulation()
{
	
}
//####################################################################
// Function newSimulation
//####################################################################
ModelSimulation* ModelSimulation::newSimulation(QString name,QDir dir,QFile& library,ModelRenderWidget* modelWidget)
{
	QFileInfo libraryInfo(library);
	library.copy(dir.absolutePath()+QString("/")+libraryInfo.fileName());
	return new ModelSimulation(name,libraryInfo.fileName(),dir.absolutePath(),(unsigned long)0,modelWidget);
}
//####################################################################
// Function loadSimulation
//####################################################################
ModelSimulation* ModelSimulation::loadSimulation(QFile& simFile,ModelRenderWidget* modelWidget)
{
    QString qString; QDir qDir;
    if(!simFile.open(QIODevice::ReadOnly)) return NULL;

    QTextStream fileStream(&simFile);

    QString name(fileStream.readLine());
    QString libraryName(fileStream.readLine());
    QString dummyPathName(fileStream.readLine()); // It was a bad idea to store the path, but now we need to keep to preserve backward compatibility =/
    QString pathName(QFileInfo(simFile).absolutePath());

    fileStream>>*modelWidget;

    unsigned long numIterations=0;
    do {
        unsigned long tempNumIterations;
        numIterations=tempNumIterations;
        fileStream>>tempNumIterations;
    } while(!fileStream.atEnd());

    simFile.close();

    return new ModelSimulation(name,libraryName,pathName,numIterations,modelWidget);
}
//####################################################################
// Function save
//####################################################################
void ModelSimulation::save()
{
    QFile simFile(mPathName+"/"+mName+".wsim");
    if(!simFile.open(QIODevice::WriteOnly|QIODevice::Truncate)) return;
    QTextStream fileStream(&simFile);
    fileStream<<mName<<endl<<mLibraryName<<endl<<mPathName<<endl;
    fileStream<<*mModelWidget<<endl;
    simFile.close();
}
//####################################################################
// Function updateg
//####################################################################
void ModelSimulation::update(const unsigned long& numIterations)
{
    mNumIterations=numIterations;
    QFile simFile(mPathName+"/"+mName+".wsim");
    if(!simFile.open(QIODevice::WriteOnly|QIODevice::Append)) return;
    QTextStream fileStream(&simFile);
    fileStream<<mNumIterations<<endl;
    simFile.close();
}
//####################################################################
