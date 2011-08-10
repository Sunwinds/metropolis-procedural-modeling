//#####################################################################
// Copyright 2010, Yu Lou.
//#####################################################################
#include "Mesh.hpp"
using namespace ProceduralModeling;
//####################################################################
// Function generateDisplayLists
//####################################################################
void Mesh::generateDisplayLists()
{
    for(int i=0;i<mUseMat.size();i++)
    {
        if((mUseMat.at(i).mIndex[1]-mUseMat.at(i).mIndex[0])>=mType*50)
        {
            GLuint l=glGenLists(1);
            glNewList(l,GL_COMPILE);
            if (mUseTexture) drawWithTexture(i); else drawWithoutTexture(i);
            glEndList();
            mUseMat[i].mDisplayList=l;
            mUseMat[i].mUseDisplayList=true;
        }
        else mUseMat[i].mUseDisplayList=false;
    }
}
//####################################################################
// Function draw
//####################################################################
void Mesh::draw(bool useMaterial)
{
    for(int i=0;i<mUseMat.size();i++)
    {
        int index=mUseMat.at(i).mMaterial;

        if(useMaterial&&index>=0) mMaterials.at(index).applyMaterial();

        if(mUseMat.at(i).mUseDisplayList) glCallList(mUseMat.at(i).mDisplayList);
        else if(mUseTexture) drawWithTexture(i);
        else drawWithoutTexture(i);
    }
}
//####################################################################
// Function findMaterial
//####################################################################
int Mesh::findMaterial(const QString& n)
{
    for(int i=0;i<mMaterials.size();i++) if(mMaterials.at(i).mName==n) return i;
    return -1;
}
//####################################################################
// Function loadMTLFile
//####################################################################
bool Mesh::loadMTLFile(char* n)
{
    QString name(n);
    int pos=name.lastIndexOf('/');

    if(pos>-1) name.remove(0,static_cast<uint>(pos+1));

    name=QString(getenv("PROC_MOD_BASE_DIR"))+"/Code/Resources/"+name;

    FILE* filePointer=fopen(name.toLatin1().constData(),"r");
	if(!filePointer){ printf("MTL file %s\n does not exist\n", name.toLatin1().constData()); return false;}

    float r,g,b,a,s;
    char buf[256];

    while(fscanf(filePointer,"%s",buf)!=EOF)
    {
        switch(buf[0])
        {
        case '#':
            fgets(buf, sizeof(buf), filePointer);
            break;
        case 'n':
            fgets(buf, sizeof(buf), filePointer);
            sscanf(buf, "%s %s", buf, buf);
            mMaterials.append(GLMaterial(QString(buf)));
            break;
        case 'N':
            if(fscanf(filePointer,"%f",&s)==1) mMaterials[mMaterials.size()-1].setShininess(s);
            else{ fclose(filePointer);  return false; }
            break;
        case 'K':
          switch(buf[1])
          {
            case 'd':
              if(fscanf(filePointer,"%f %f %f",&r,&g,&b)==3) mMaterials[mMaterials.size()-1].setMaterialParam(GL_DIFFUSE,r,g,b,1);
              else{ fclose(filePointer); return false; }
              break;
            case 's':
              if(fscanf(filePointer,"%f %f %f",&r,&g,&b)==3) mMaterials[mMaterials.size()-1].setMaterialParam(GL_SPECULAR,r,g,b,1);
              else{ fclose(filePointer); return false; }
              break;
            case 'a':
              if(fscanf(filePointer, "%f %f %f",&r,&g,&b)==3) mMaterials[mMaterials.size()-1].setMaterialParam(GL_AMBIENT,r,g,b,1);
              else{ fclose(filePointer); return false; }
              break;
            default: fgets(buf,sizeof(buf),filePointer); break;
          }
          break;
        case 'd':
        case 'T': if(fscanf(filePointer,"%f",&a)==1) mMaterials[mMaterials.size()-1].setAlpha(a); else{fclose(filePointer); return false;} break;
        default: fgets(buf, sizeof(buf), filePointer); break;
        }
    }
    fclose(filePointer);
    return true;
}
//####################################################################
// Function saveToObjFile
//####################################################################
void Mesh::saveToObjFile(const QString& path) const
{
    if(mType==DEFAULT) return;

    FILE* filePointer=fopen(path.toLatin1().constData(),"w");
    if(!filePointer){ printf("Failed to create %s\n", path.toLatin1().constData()); exit(-1); }

    fprintf(filePointer,"#%d vertices\n",mVertexList.size());

    for(int i=0;i<mVertexList.size();i++){ QVector3D v=mVertexList.at(i); fprintf(filePointer,"v %f %f %f\n",v.x(),v.y(),v.z()); }
    for(int i=0;i<mNormalList.size();i++){ QVector3D vn=mNormalList.at(i); fprintf(filePointer,"vn %f %f %f\n",vn.x(),vn.y(),vn.z()); }
    for(int i=0;i<mTexcoordList.size();i++){ QVector2D vt=mTexcoordList.at(i); fprintf(filePointer,"vt %f %f\n",vt.x(),vt.y()); }

    if (mSubMesh.empty())
    {
        fprintf(filePointer,"#%d faces\n",mIndexList.size()/mType);
        for(int i=0;i<mIndexList.size();i+=mType)
        {
            fprintf(filePointer,"f");
            for(int j=0;j<mType; j++) fprintf(filePointer," %d/%d/%d",mIndexList.at(i+j)+1,mIndexList.at(i+j)+1,mIndexList.at(i+j)+1);
            fprintf(filePointer,"\n");
        }
    }
    else
        for(int i=0;i<mSubMesh.size();i++)
        {
            fprintf(filePointer,"g %s\n",mSubMesh.at(i).mGroupName.toLatin1().constData());
            for(int j=mSubMesh.at(i).mIndex[0];j<mSubMesh.at(i).mIndex[1];j++)
            {
                if(mUseMat.at(j).mMaterial!=-1) fprintf(filePointer,"usemtl %s\n",mMaterials.at(mUseMat.at(j).mMaterial).mName.toLatin1().constData());
                for(int k=mUseMat.at(j).mIndex[0];k<mUseMat.at(j).mIndex[1];k+=mType)
                {
                    fprintf(filePointer,"f");
                    for(int h=0;h<mType;h++) fprintf(filePointer," %d/%d/%d",mIndexList.at(k+h)+1,mIndexList.at(k+h)+1,mIndexList.at(k+h)+1);
                    fprintf(filePointer, "\n");
                }
            }
        }

    fclose(filePointer);

    printf("Saved to %s\n",path.toLatin1().constData());
}
//####################################################################
// Function TriangleMesh
//####################################################################
TriangleMesh::TriangleMesh(const char* fileName,bool useTex,bool tricky)
 :Mesh(TRI,useTex)
{
	FILE* filePointer=fopen(fileName,"r");
	if (filePointer){ if(Load(filePointer,tricky)) mIsLoaded=true; fclose(filePointer); }
}
//####################################################################
// Function drawWithTexture
//####################################################################
void TriangleMesh::drawWithTexture(int index)
{
    glBegin(GL_TRIANGLES);
    for(int i=mUseMat.at(index).mIndex[0];i<mUseMat.at(index).mIndex[1];i++)
    {
        int v=mIndexList.at(i),n=mNormalIndexList.at(i);
        glNormal3f(mNormalList.at(n).x(),mNormalList.at(n).y(),mNormalList.at(n).z());
        glVertex3f(mVertexList.at(v).x(),mVertexList.at(v).y(),mVertexList.at(v).z());
    }
    glEnd();
}
//####################################################################
// Function drawWithoutTexture
//####################################################################
void TriangleMesh::drawWithoutTexture(int index)
{
    glBegin(GL_TRIANGLES);
    for (int i=mUseMat.at(index).mIndex[0];i<mUseMat.at(index).mIndex[1];i++)
    {
        int v=mIndexList.at(i),n=mNormalIndexList.at(i),t=mTexIndexList.at(i);
        glTexCoord2f(mTexcoordList.at(t).x(),mTexcoordList.at(t).y());
        glNormal3f(mNormalList.at(n).x(),mNormalList.at(n).y(),mNormalList.at(n).z());
        glVertex3f(mVertexList.at(v).x(),mVertexList.at(v).y(),mVertexList.at(v).z());
    }
    glEnd();
}
//####################################################################
// Function simpleLoad
//####################################################################
bool TriangleMesh::simpleLoad(FILE* filePointer, int& displayList)
{
    float x,y,z;
    int v1,v2,v3;
    int n1=0,n2=0,n3=0;
    int t;
    char buf[256];

    while(fscanf(filePointer,"%s",buf)!=EOF)
    {
        switch(buf[0])
        {
          case '#': fgets(buf,sizeof(buf),filePointer); break;
          case 'v':
            switch(buf[1])
            {
              case '\0':
                if(fscanf(filePointer,"%f %f %f",&x,&y,&z)==3){ QVector3D vertex(x,y,z); mVertexList.append(vertex); }
                else return false;
                break;
              case 'n':
                if(fscanf(filePointer,"%f %f %f",&x,&y,&z)==3){ QVector3D normal(x,y,z); normal.normalize(); mNormalList.append(normal); }
                else return false;
				break;
              default: fgets(buf, sizeof(buf),filePointer); break;
            }
            break;
          case 'f':
              {
                  bool correct=false;
                  if(fscanf(filePointer,"%s",buf)!=1) return false;

                  if(strstr(buf,"//"))
                  {
                      if(sscanf(buf,"%d//%d",&v1,&n1)==2 && fscanf(filePointer,"%d//%d",&v2,&n2)==2 && fscanf(filePointer,"%d//%d",&v3,&n3) == 2) correct=true;
                      else return false;
                  }
                  else if(sscanf(buf,"%d/%d/%d",&v1,&t,&n1)==3)
                  {
                      if(fscanf(filePointer,"%d/%d/%d",&v2,&t,&n2)==3 && fscanf(filePointer,"%d/%d/%d",&v3,&t,&n3)==3 ) correct=true;
                      else return false;
                  }
                  else if(sscanf(buf,"%d/%d",&v1,&t)==2)
                  {
                      if( fscanf(filePointer,"%d/%d",&v2,&t)==2 && fscanf(filePointer,"%d/%d",&v3,&t)==2) correct=true;
                      else return false;
                  }
                  else
                  {
                      if(sscanf(buf,"%d",&v1)==1 && fscanf(filePointer,"%d",&v2)==1 && fscanf(filePointer,"%d",&v3)==1) correct=true;
                      else return false;
                  }
                  if (correct)
                  {
                      mIndexList.append(v1-1); mIndexList.append(v2-1); mIndexList.append(v3-1);
                      mNormalIndexList.append(n1-1); mNormalIndexList.append(n2-1); mNormalIndexList.append(n3-1);
                  }
              }
              break;
          default:
            fgets(buf, sizeof(buf), filePointer);
            break;
        }
    }

    if(mNormalList.empty())
    {
        mNormalList.resize(mVertexList.size());
        for(int i=0;i<mIndexList.size()/3;i++)
        {
            QVector3D n=QVector3D::normal(mVertexList.at(mIndexList.at(3*i)), mVertexList.at(mIndexList.at(3*i+1)),mVertexList.at(mIndexList.at(3*i+2)));
            mNormalList[mIndexList.at(3*i)]+=n; mNormalList[mIndexList.at(3*i+1)]+=n; mNormalList[mIndexList.at(3*i+2)]+=n;
        }
        for(int i=0;i<mNormalList.size();i++) mNormalList[i].normalize();
    }

    if(mNormalIndexList.at(0)==-1) for(int i=0;i<mIndexList.size();i++) mNormalIndexList[i]=mIndexList[i];

    QVector3D min(FLT_MAX,FLT_MAX,FLT_MAX),max(-FLT_MAX,-FLT_MAX,-FLT_MAX);

    foreach(QVector3D v,mVertexList)
    {
        if(v.x()<min.x()) min.setX(v.x());
        if(v.y()<min.y()) min.setY(v.y());
        if(v.z()<min.z()) min.setZ(v.z());
        if(v.x()>max.x()) max.setX(v.x());
        if(v.y()>max.y()) max.setY(v.y());
        if(v.z()>max.z()) max.setZ(v.z());
    }

    QVector3D diag=max-min;
    QVector3D center=min+.5*diag;
    float maxLen=diag.x(),minLen=diag.x();

    if(maxLen<diag.y()) maxLen=diag.y(); else if(minLen>diag.y()) minLen=diag.y();
    if(maxLen<diag.z()) maxLen=diag.z(); else if(minLen>diag.z()) minLen=diag.z();

    for(int i=0;i<mVertexList.size();i++) mVertexList[i]=(mVertexList[i]-center)/maxLen;

    displayList=glGenLists(1);
    glNewList(displayList, GL_COMPILE);
    glBegin(GL_TRIANGLES);
    for(int i=0;i<mIndexList.size();i++)
    {
        int v=mIndexList.at(i),n=mNormalIndexList.at(i);
        glNormal3f(mNormalList.at(n).x(),mNormalList.at(n).y(),mNormalList.at(n).z());
        glVertex3f(mVertexList.at(v).x(),mVertexList.at(v).y(),mVertexList.at(v).z());
    }
    glEnd();
    glEndList();

    return true;
}
//####################################################################
// Function Load
//####################################################################
bool TriangleMesh::Load(FILE* filePointer,bool tricky)
{
	float x,y,z;
    float u,v;
	int v1,v2,v3;
    int t1=0,t2=0,t3=0;
    int n1=0,n2=0,n3=0;
	char buf[256];
    QVector3D min(FLT_MAX,FLT_MAX,FLT_MAX),max(-FLT_MAX,-FLT_MAX,-FLT_MAX);

	while(fscanf(filePointer,"%s", buf)!=EOF)
    {
        switch(buf[0])
        {
          case '#': fgets(buf,sizeof(buf),filePointer); break;
          case 'v':
            switch(buf[1])
            {
              case '\0':
                if(fscanf(filePointer,"%f %f %f",&x,&y,&z)==3){ QVector3D vertex(x,y,z); mVertexList.append(vertex); }
                else return false;
                break;
              case 'n':
                if(fscanf(filePointer,"%f %f %f",&x,&y,&z)==3){ QVector3D normal(x,y,z); mNormalList.append(normal); }
                else return false;
				break;
              case 't':
                if(fscanf(filePointer,"%f %f",&u,&v)==2){ QVector2D tex(u,v); mTexcoordList.append(tex); }
                else return false;
                break;
              default: fgets(buf,sizeof(buf),filePointer); break;
            }
            break;
          case 'f':
              {
                  bool correct=false;
                  if(fscanf(filePointer,"%s",buf)!=1) return false;

                  if(strstr(buf,"//"))
                  {
                      if(sscanf(buf,"%d//%d",&v1,&n1)==2  && fscanf(filePointer,"%d//%d",&v2,&n2)==2  && fscanf(filePointer,"%d//%d",&v3,&n3)==2) correct=true;
                      else return false;
                  }
                  else if(sscanf(buf,"%d/%d/%d",&v1,&t1,&n1)==3)
                  {
                      if(fscanf(filePointer,"%d/%d/%d",&v2,&t2,&n2)==3 && fscanf(filePointer,"%d/%d/%d",&v3,&t3,&n3)==3) correct=true;
                      else return false;
                  }
                  else if (sscanf(buf, "%d/%d",&v1,&t1)==2)
                  {
                      if(fscanf(filePointer,"%d/%d",&v2,&t2)==2 && fscanf(filePointer,"%d/%d",&v3,&t3)==2) correct=true;
                      else return false;
                  }
                  else
                  {
                      if(sscanf(buf,"%d",&v1)==1 && fscanf(filePointer,"%d",&v2)==1 && fscanf(filePointer,"%d",&v3)==1) correct=true;
                      else return false;
                  }
                  if (correct)
                  {
                      mIndexList.append(v1-1); mIndexList.append(v2-1); mIndexList.append(v3-1);
                      mTexIndexList.append(t1-1); mTexIndexList.append(t2-1);mTexIndexList.append(t3-1);
                      mNormalIndexList.append(n1-1); mNormalIndexList.append(n2-1); mNormalIndexList.append(n3-1);
                  }
              }
              break;
          case 'u':
            fgets(buf,sizeof(buf),filePointer);
            sscanf(buf,"%s %s",buf,buf);
            if (!mUseMat.empty())
                mUseMat[mUseMat.size()-1].mIndex[1]=mIndexList.size();
            mUseMat.append(UseMat());
            mUseMat[mUseMat.size()-1].mIndex[0]=mIndexList.size();
            mUseMat[mUseMat.size()-1].mMaterial=findMaterial(QString(buf));
            break;
          case 'g':
            fgets(buf,sizeof(buf),filePointer);
            sscanf(buf,"%s",buf);
            if(QString(buf)[0].isLetterOrNumber())
            {
                if (!mSubMesh.empty())
                    mSubMesh[mSubMesh.size()-1].mIndex[1] = mUseMat.size();
                mSubMesh.append(SubMesh(QString(buf)));
                mSubMesh[mSubMesh.size()-1].mIndex[0] = mUseMat.size();
            }
            break;
          case 'm':
            fgets(buf,sizeof(buf),filePointer);
            sscanf(buf,"%s %s",buf,buf);
            loadMTLFile(buf);
            break;
          default:
            fgets(buf,sizeof(buf),filePointer);
            break;
        }
    }

    if(mTexIndexList.at(0)==-1 && mUseTexture){ printf("Could not use texture without texture coordinates\n"); exit(-1); }

    foreach(QVector3D v,mVertexList)
    {
        if(v.x()<min.x()) min.setX(v.x());
        if(v.y()<min.y()) min.setY(v.y());
        if(v.z()<min.z()) min.setZ(v.z());
        if(v.x()>max.x()) max.setX(v.x());
        if(v.y()>max.y()) max.setY(v.y());
        if(v.z()>max.z()) max.setZ(v.z());
    }

    QVector3D diag=max-min;
    float len=diag.x();
    if(tricky) len=265;
    mMaxX=diag.x()/len; mMaxY=diag.y()/len; mMaxZ=diag.z()/len;

    for(int i=0;i<mVertexList.size();i++) mVertexList[i]=(mVertexList[i]-min)/len;

    if(mNormalList.empty())
    {
        mNormalList.resize(mVertexList.size());
        for(int i=0;i<mIndexList.size()/3;i++)
        {
            QVector3D normal=QVector3D::normal(mVertexList.at(mIndexList.at(3*i)), mVertexList.at(mIndexList.at(3*i+1)),mVertexList.at(mIndexList.at(3*i+2)));
            mNormalList[mIndexList.at(3*i)]+=normal; mNormalList[mIndexList.at(3*i+1)]+=normal; mNormalList[mIndexList.at(3*i+2)]+=normal;
        }
        for(int i=0;i<mNormalList.size();i++) mNormalList[i].normalize();
    }

    if(mNormalIndexList.at(0)==-1) for(int i=0;i<mIndexList.size();i++) mNormalIndexList[i]=mIndexList[i];
    if (mUseMat.empty()){ mUseMat.append(UseMat()); mUseMat[0].mIndex[0]=0; mUseMat[0].mIndex[1]=mIndexList.size(); }
    else mUseMat[mUseMat.size()-1].mIndex[1]=mIndexList.size();
    if (mSubMesh.empty()){ mSubMesh.append(SubMesh("")); mSubMesh[0].mIndex[0]=0; mSubMesh[0].mIndex[1]=mUseMat.size(); }
    else mSubMesh[mSubMesh.size()-1].mIndex[1]=mUseMat.size();

    generateDisplayLists();
    return true;
}
//####################################################################
// Function QuadMesh
//####################################################################
QuadMesh::QuadMesh(const char* fileName,bool useTex)
 :Mesh(QUAD,useTex)
{
	FILE* filePointer=fopen(fileName,"r");
	if(filePointer){if(Load(filePointer)) mIsLoaded=true; fclose(filePointer); }
}
//####################################################################
