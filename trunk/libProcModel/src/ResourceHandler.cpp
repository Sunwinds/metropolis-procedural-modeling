//#####################################################################
// Copyright 2010, Yu Lou, Jerry Talton.
//#####################################################################
#include "ResourceHandler.hpp"
using namespace ProceduralModeling;
//####################################################################
// Function destroy
//####################################################################
void ResourceHandler::destroy()
{
    for(uint i=0;i<mShaders.size();i++) delete mShaders[i];
    mShaders.clear(); mTextures.clear();
    foreach(TriangleMesh* mesh,mMeshes) delete mesh;
    mMeshes.clear(); mShaderFiles.clear(); mTextureFiles.clear(); mMeshFiles.clear();
}
//####################################################################
// Function initShader
//####################################################################
int ResourceHandler::initShader(const QString& vs,const QString& fs)
{
    int index=checkDuplicate(1,vs);
    if(index>-1) return index;

    QGLShaderProgram* shader=new QGLShaderProgram(mRenderWidget->context());
    QGLShader *vert,*frag;
    if (!vs.isEmpty()){vert=new QGLShader(QGLShader::Vertex);vert->compileSourceFile(getResourceBaseDir()+vs);shader->addShader(vert);}
    if (!fs.isEmpty()){frag=new QGLShader(QGLShader::Fragment);frag->compileSourceFile(getResourceBaseDir()+fs);shader->addShader(frag);}
    shader->link(); mShaders.append(shader); mShaderFiles.append(vs);
    return mShaders.size()-1;
}
//####################################################################
// Function bindShader
//####################################################################
void ResourceHandler::bindShader(uint idx)
{
    if (mCurShader==idx || idx >= (int)mShaders.size()) return;
    if (mCurShader < mShaders.size()) mShaders.at(mCurShader)->release();
    mCurShader=idx;
    mShaders.at(idx)->bind();
}
//####################################################################
// Function appendTexture
//####################################################################
int ResourceHandler::appendTexture(const QString& fileName)
{
    int index=checkDuplicate(2,fileName);
    if(index>-1) return index;

    QImage image(getResourceBaseDir()+fileName);
    if(!image.isNull()){ mTextures.append(mRenderWidget->bindTexture(image)); mTextureFiles.append(fileName); return mTextures.size()-1; }
    else{ printf("The texture %s is not loaded!\n", fileName.toLatin1().constData()); exit(-1); };
}
//####################################################################
// Function loadModel
//####################################################################
int ResourceHandler::loadModel(const QString& fileName, bool useTex, bool tricky)
{
	int index=checkDuplicate(3,fileName);
    if(index>-1) return index;

    QString path=getResourceBaseDir()+fileName;
    TriangleMesh* mesh=new TriangleMesh(path.toLatin1().constData(), useTex, tricky);
    if (mesh->isLoaded()) { mMeshes.append(mesh); mMeshFiles.append(fileName); return mMeshes.size()-1; }
    else{ printf("The mesh %s is not loaded!\n", path.toLatin1().constData()); exit(-1); }
}
//####################################################################
// Function checkDuplicate
//####################################################################
int ResourceHandler::checkDuplicate(uint type,const QString& s) const
{
    switch (type)
    {
      case 1: for(int i=0;i<mShaderFiles.size();i++) if(mShaderFiles[i]==s) return i; break;
      case 2: for(int i=0;i<mTextureFiles.size();i++) if(mTextureFiles[i]==s) return i; break;
      case 3: for(int i=0;i<mMeshFiles.size();i++) if(mMeshFiles[i]==s) return i; break;
    };
    return -1;
}
//####################################################################
// Function enableLighting
//####################################################################
void ResourceHandler::enableLighting()
{
    if (!glIsEnabled(GL_LIGHT0))
        return;
    if(mShaders.empty())
        glEnable(GL_LIGHTING);
    else
    { foreach(QGLShaderProgram* shader,mShaders) shader->setUniformValue("enableLighting",1); }
}
//####################################################################
// Function disableLighting
//####################################################################
void ResourceHandler::disableLighting()
{
    if (!glIsEnabled(GL_LIGHT0))
        return;
    if(mShaders.empty())
        glDisable(GL_LIGHTING);
    else
    { foreach(QGLShaderProgram* shader,mShaders) shader->setUniformValue("enableLighting",0); }
}
//####################################################################
