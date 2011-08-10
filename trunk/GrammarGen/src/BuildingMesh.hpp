//#####################################################################
// Copyright 2009, Yu Lou.
//#####################################################################
#ifndef _BUILDING_MESH_HPP_
#define _BUILDING_MESH_HPP_

#include "Mesh.hpp"

using ProceduralModeling::TriangleMesh;
using ProceduralModeling::GLMaterial;
using ProceduralModeling::UseMat;
using ProceduralModeling::SubMesh;

struct SubObj
{
    QVector<QMatrix4x4> mTransformation;
    TriangleMesh* mMesh;
    QString mName;
};

struct Wall
{
    QVector3D mNormal;
    QVector3D mPosition[4];
};

struct Rooftop
{
//    QVector3D mNormal;
//    QVector3D mPosition[4];
    // z-1, z+1, x-1, x+1
    bool side[4];
    QVector3D bottomPos[4];
    QVector3D topPos[4];
};

class BuildingMesh : public TriangleMesh
{
protected:
    bool mEnableSaveToMesh;
    QStack<QMatrix4x4> mMatrixState;
    QVector<SubObj> mSubObject;
    QVector<Wall> mWalls;
    int*** mInternal;
    int mGrids[16][16][16];

public:
    BuildingMesh():mInternal(NULL){}
    ~BuildingMesh() { }
    void init();
    void initInternal(const QString& path, int w, int h, int d);
    bool internalPart(float x, float y, float z);
    void processRoofs(QVector<Rooftop>& rooftops) const;
    void addObj(TriangleMesh* mesh, const QString& name);
    void addWall(float p1, float p2);
    void addRooftop(float p1, float p2, float offset);
    void pushState();
    void popState();
    QMatrix4x4& curDirTrans();
    void saveToObjFile(const QString& path) const;
};

#endif
