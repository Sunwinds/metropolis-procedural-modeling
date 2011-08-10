//#####################################################################
// Copyright 2009, Yu Lou, Jerry Talton.
//#####################################################################
#ifndef _MESH_HPP_
#define _MESH_HPP_

#include "Common.hpp"
#include <QtOpenGL>
#include <QVector>
#include <QVector2D>
#include <QVector3D>
#include <QStack>
#include <QMatrix4x4>

namespace ProceduralModeling{
//####################################################################
// Struct GLMaterial
//####################################################################
struct GLMaterial
{
public:
    QString mName;
    GLfloat mAmbient[4],mDiffuse[4],mSpecular[4],mEmission[4],mShininess;

public:
    GLMaterial(const QString& name="")
     :mName(name),mShininess(1)
    {
        mAmbient[0]=mAmbient[1]=mAmbient[2]=0.2; mAmbient[3]=1.0;
        mDiffuse[0]=mDiffuse[1]=mDiffuse[2]=0.8; mDiffuse[3]=1.0;
        mSpecular[0]=mSpecular[1]=mSpecular[2]=0; mSpecular[3]=1.0;
        mEmission[0]=mEmission[1]=mEmission[2]=0; mEmission[3]=1.0;
    }

    void setMaterialParam(GLenum pname,GLfloat r,GLfloat g,GLfloat b,GLfloat alpha)
    { GLfloat param[4]={r,g,b,alpha}; setMaterialParam(pname,param); }

    void setMaterialParam(GLenum pname,GLfloat param[])
    {
        switch (pname){
          case GL_AMBIENT: memmove(mAmbient,param,sizeof(GLfloat)*4); break;
          case GL_DIFFUSE: memmove(mDiffuse,param,sizeof(GLfloat)*4); break;
          case GL_SPECULAR: memmove(mSpecular,param,sizeof(GLfloat)*4); break;
          case GL_EMISSION: memmove(mEmission,param,sizeof(GLfloat)*4); break;
        }
    }

    void setAlpha(GLfloat alpha)
    { mAmbient[3]=mDiffuse[3]=mSpecular[3]=alpha; }

    void setShininess(GLfloat shin)
    { mShininess=shin/100.0*128.0; }

    void applyMaterial() const
    {
        glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,mAmbient);
        glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,mDiffuse);
        glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,mSpecular);
        glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,mShininess);
    }

//####################################################################
};

typedef enum mType {DEFAULT=0,TRI=3,QUAD=4} MeshType;
typedef QVector<QVector3D> VertexList;
typedef QVector<QVector3D> NormalList;
typedef QVector<QVector2D> TexCoordList;
typedef QVector<int> IndexList;

//####################################################################
// Struct SubMesh
//####################################################################
struct SubMesh
{
public:
    int mIndex[2];
    QString mGroupName;

public:
    SubMesh(const QString& name="default")
      :mGroupName(name)
    { mIndex[0]=mIndex[1]=-1; }

//####################################################################
};

//####################################################################
// Struct UseMat
//####################################################################
struct UseMat
{
public:
    bool mUseDisplayList;
    int mIndex[2];
    int mMaterial;
    GLuint mDisplayList;

public:
    UseMat()
      :mUseDisplayList(true),mMaterial(-1)
    { mIndex[0]=mIndex[1]=-1; }

//####################################################################
};

//####################################################################
// Class Mesh
//####################################################################
class PROCEDURAL_EXPORT Mesh
{
protected:
    int mType;
    bool mUseTexture,mEnableSaveToMesh,mIsLoaded;
    VertexList mVertexList;
	NormalList mNormalList;
    TexCoordList mTexcoordList;
	IndexList mIndexList,mNormalIndexList,mTexIndexList;
    QVector<GLMaterial> mMaterials;
    QVector<SubMesh> mSubMesh;
    QVector<UseMat> mUseMat;
    QMatrix4x4 mTransformation;

public:
    Mesh(int type=DEFAULT,bool useTex=false)
      :mType(type),mUseTexture(useTex),mEnableSaveToMesh(false),mIsLoaded(false)
    {}

    virtual ~Mesh()
    {}

    bool isLoaded() const
    { return mIsLoaded; }

    virtual bool Load(FILE* filePointer)
    { return false; }

    void enableSaveToMesh()
    { mEnableSaveToMesh=true; }

    bool isEnableSaveToMesh() const
    { return mEnableSaveToMesh; }

    virtual QMatrix4x4& curDirTrans()
    { return mTransformation; }

    virtual void pushState()
    {}

    virtual void popState()
    {}

    virtual void init()
    {}

protected:
    virtual void drawWithTexture(int index)
    {}

    virtual void drawWithoutTexture(int index)
    {}

public:
    void generateDisplayLists();
    void draw(bool useMaterial = true);
    int findMaterial(const QString& n);
    bool loadMTLFile(char* n);
    virtual void saveToObjFile(const QString& path) const;

//####################################################################
};

//####################################################################
// Class TriangleMesh
//####################################################################
class PROCEDURAL_EXPORT TriangleMesh : public Mesh
{
protected:
    float mMaxX,mMaxY,mMaxZ;

public:
    TriangleMesh(bool useTex=false)
      :Mesh(TRI,useTex)
    {}

	TriangleMesh(const char* fileName,bool useTex=false,bool tricky=false);

	~TriangleMesh()
    {}

    QVector<GLMaterial>& getMaterials()
    { return mMaterials; }

    QVector<SubMesh>& getSubMesh()
    { return mSubMesh; }

    QVector<UseMat>& getUseMat()
    { return mUseMat; }

    VertexList& getVertexList()
    { return mVertexList; }

    NormalList& getNormalList()
    { return mNormalList; }

    TexCoordList& getTexcoordList()
    { return mTexcoordList; }

    IndexList& getIndexList()
    { return mIndexList; }

    IndexList& getNormalIndexList()
    { return mNormalIndexList; }

    IndexList& getTexIndexList()
    { return mTexIndexList; }

    float getMaxX()
    { return mMaxX; }

    float getMaxY()
    { return mMaxY; }

    float getMaxZ()
    { return mMaxZ; }

    virtual QMatrix4x4& curDirTrans()
    { return mTransformation; }

    virtual void pushState()
    {}

    virtual void popState()
    {}

    virtual void saveToObj()
    {}

    virtual void init()
    {}

public:
    void drawWithTexture(int index);
    void drawWithoutTexture(int index);
    bool simpleLoad(FILE* filePointer,int& displayList);

protected:
	bool Load(FILE* filePointer,bool tricky);

//####################################################################
};

//####################################################################
// Class QuadMesh
//####################################################################
class PROCEDURAL_EXPORT QuadMesh : public Mesh
{
public:
    QuadMesh(bool useTex=false)
     :Mesh(QUAD,useTex)
    {
    }

    ~QuadMesh()
    {}

    void drawWithTexture(int index)
    {}

    void drawWithoutTexture(int index)
    {}

    virtual QMatrix4x4& curDirTrans()
    { return mTransformation; }

    virtual void pushState()
    {}

    virtual void popState()
    {}

    virtual void saveToObj()
    {}

    virtual void init()
    {}

public:
    QuadMesh(const char* fileName,bool useTex=false);

//####################################################################
};

//####################################################################
}  // namespace ProceduralModeling

#endif // __MESH__
