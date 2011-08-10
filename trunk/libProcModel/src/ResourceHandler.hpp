//#####################################################################
// Copyright 2009, Yu Lou, Jerry Talton.
//#####################################################################
#ifndef __RESOURCE_HANDLER_HPP__
#define __RESOURCE_HANDLER_HPP__

#include "Common.hpp"
#include <QtOpenGL>
#include <QGLWidget>
#include "Mesh.hpp"
#include "BBoxHelper.hpp"

namespace ProceduralModeling{

typedef QVector<QString> FileVector;
typedef QVector<GLuint> TextureVector;
typedef QVector<QGLShaderProgram*> ShaderVector;
typedef QVector<TriangleMesh*> MeshVector;
enum OutputMeshType { MESH, TREE, BUILDING, CITY };

//####################################################################
// Class ResourceHandler
//####################################################################
class PROCEDURAL_EXPORT ResourceHandler
{
private:
    QGLWidget* mRenderWidget;
    ShaderVector mShaders;
    uint mCurShader;
    FileVector mShaderFiles,mTextureFiles,mMeshFiles;
    TextureVector mTextures;
    MeshVector mMeshes;
    Mesh* mOutputMesh;
    OutputMeshType mOutputMeshType;
    BBoxHelper* mBBoxHelper;

public:
    ResourceHandler()
      :mRenderWidget(NULL),mOutputMesh(NULL),mBBoxHelper(NULL)
    { init(); }

    ~ResourceHandler()
    { destroy(); delete mOutputMesh; delete mBBoxHelper; mBBoxHelper=NULL; }

    void init()
    { destroy(); mCurShader=-1; mRenderWidget=NULL; }

    void setRenderWidget(QGLWidget* widget)
    { mRenderWidget=widget;}

    QGLWidget* getRenderWidget()
    { return mRenderWidget; }

    TriangleMesh* getModel(int idx)
    { return mMeshes.at(idx); }

    QGLShaderProgram* getShader(int idx)
    { return mShaders.at(idx); }

    void unbindShader()
    { mShaders.at(mCurShader)->release(); }

    void bindTexture(uint idx)
    { glBindTexture(GL_TEXTURE_2D, mTextures.at(idx)); }

    void initOutputMesh(Mesh* mesh, OutputMeshType type)
    { mOutputMesh=mesh; mOutputMeshType=type; }

    Mesh* getOutputMesh()
    { return mOutputMesh; }

    OutputMeshType getOutputMeshType()
    { return mOutputMeshType; }

    void initBBoxHelper(int w,int h,float x1,float x2,float y1,float y2,float z1,float z2)
    { mBBoxHelper=new BBoxHelper(w,h,x1,x2,y1,y2,z1,z2); }

    int getBBoxWidth() const
    { return mBBoxHelper->mWidth; }

    int getBBoxHeight() const
    { return mBBoxHelper->mHeight; }

    void resetBBoxHelper() const
    { if(!mBBoxHelper) return; mBBoxHelper->reset(); }

    BBoxHelper* getBBoxHelper()
    { return mBBoxHelper; }

    bitset<BBOX_DEPTH>** getBBoxInfo() const
    { return mBBoxHelper->mBBoxInfo; }

    QMatrix4x4& getBBoxTransMat()
    { return mBBoxHelper->mTransformMatrix; }

    void pushBBoxMatrix()
    { mBBoxHelper->mStack.push(mBBoxHelper->mTransformMatrix); }

    void popBBoxMatrix()
    { mBBoxHelper->mTransformMatrix = mBBoxHelper->mStack.pop(); }

private:
    static QString getResourceBaseDir()
    { return QString(getenv("PROC_MOD_BASE_DIR"))+"/Code/Resources/"; }

public:
    int initShader(const QString& vs,const QString& fs);
    void bindShader(uint idx);
    int appendTexture(const QString& fileName);
    int loadModel(const QString& fileName, bool useTex, bool tricky);
    void enableLighting();
    void disableLighting();

private:
    int checkDuplicate(uint type,const QString& s) const;
    void destroy();

//####################################################################
};

//####################################################################
} // namespace ProceduralModeling

#endif // __RESOURCE_HANDLER__
