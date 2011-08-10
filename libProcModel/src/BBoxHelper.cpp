//#####################################################################
// Copyright 2010, Yu Lou, Jerry Talton.
//#####################################################################
#include "BBoxHelper.hpp"
using namespace ProceduralModeling;
//####################################################################
// Function BBoxHelper
//####################################################################
BBoxHelper::BBoxHelper(int w,int h,float x1,float x2,float y1,float y2,float z1,float z2)
:mWidth(w),mHeight(h),mMinX(x1),mMinY(y1),mMinZ(z1)
{
    mBBoxInfo=new bitset<BBOX_DEPTH>*[w];
    for(int i=0;i<w;i++) mBBoxInfo[i]=new bitset<BBOX_DEPTH>[h];
    mRangeX=x2-mMinX;mRangeY=y2-mMinY;mRangeZ=z2-mMinZ;
}
//####################################################################
// Function ~BBoxHelper
//####################################################################
BBoxHelper::~BBoxHelper()
{
    if (!mBBoxInfo) return;
    for(int i=0;i<mWidth;i++) delete[] mBBoxInfo[i];
    delete[] mBBoxInfo;
}
//####################################################################
// Function saveToBBoxInfo
//####################################################################
void BBoxHelper::saveToBBoxInfo(float x1, float x2, float y1, float y2, float z1, float z2)
{
	int ix1=Math::round((x1-mMinX)/mRangeX*mWidth);
    ix1=mWidth>ix1?ix1:mWidth;
    int ix2=Math::round((x2-mMinX)/mRangeX*mWidth);
    ix2=mWidth>ix2?ix2:mWidth;

    int iy1=Math::round((y1-mMinY)/mRangeY*mHeight);
    iy1=mHeight>iy1?iy1:mHeight;
    int iy2=Math::round((y2-mMinY)/mRangeY*mHeight);
    iy2=mHeight>iy2?iy2:mHeight;

    int iz1=Math::round((z1-mMinZ)/mRangeZ*BBOX_DEPTH);
    iz1=BBOX_DEPTH>iz1?iz1:BBOX_DEPTH;
    int iz2=Math::round((z2-mMinZ)/mRangeZ*BBOX_DEPTH);
    iz2=BBOX_DEPTH>iz2?iz2:BBOX_DEPTH;

    std::bitset<BBOX_DEPTH> t;
    for(int i=iz1;i<iz2;i++) t.set(i);
    for(int i=ix1;i<ix2;i++) for(int j=iy1;j<iy2;j++) mBBoxInfo[i][j]|=t;
}
//####################################################################
// Function saveToMesh
//####################################################################
void BBoxHelper::saveToMesh()
{
    QString path=QString(getenv("PROC_MOD_BASE_DIR"))+"Code/Resources/"+"voxelX.obj";
    FILE* fp=fopen(path.toLatin1().constData(),"w");

    int idx=1;
    for(int i=0;i<mWidth;i++) for(int j=0;j<mHeight;j++) for(int k=0;k<BBOX_DEPTH;k++) if(mBBoxInfo[i][j].test(k)){
        fprintf(fp,"v %d %d %d\n",i,j,k);
        fprintf(fp,"v %d %d %d\n",i+1,j,k);
        fprintf(fp,"v %d %d %d\n",i,j+1,k);
        fprintf(fp,"v %d %d %d\n",i,j,k+1);
        fprintf(fp,"v %d %d %d\n",i+1,j+1,k);
        fprintf(fp,"v %d %d %d\n",i,j+1,k+1);
        fprintf(fp,"v %d %d %d\n",i+1,j,k+1);
        fprintf(fp,"v %d %d %d\n",i+1,j+1,k+1);

        fprintf(fp,"f %d %d %d %d\n",idx,idx+2,idx+4,idx+1);
        fprintf(fp,"f %d %d %d %d\n",idx+1,idx+4,idx+7,idx+6);
        fprintf(fp,"f %d %d %d %d\n",idx+5,idx+3,idx+6,idx+7);
        fprintf(fp,"f %d %d %d %d\n",idx+5,idx+2,idx+0,idx+3);
        fprintf(fp,"f %d %d %d %d\n",idx+2,idx+5,idx+7,idx+4);
        fprintf(fp,"f %d %d %d %d\n",idx,idx+1,idx+6,idx+3);

        idx += 8;
    }

    fclose(fp);
    printf("Save to voxelX.obj\n");
}
//####################################################################
// Function saveToFile
//####################################################################
void BBoxHelper::saveToFile(const QString& path)
{
    FILE* fp=fopen(path.toLatin1().constData(),"w");
    if(!fp){ printf("Failed to load %s\n",path.toLatin1().constData());exit(-1);}

    int count=0;
    for(int i=0;i<mWidth;i++) for(int j=0;j<mHeight;j++) for(int k=0;k<BBOX_DEPTH;k++)
    if(mBBoxInfo[i][j].test(k)){ fputc('1',fp); count++;} else fputc('0',fp);

    fclose(fp);
    printf("count1: %d\n",count);
}
//####################################################################
// Function reset
//####################################################################
void BBoxHelper::reset()
{
    for(int i=0;i<mWidth;i++) for(int j=0;j<mHeight;j++) mBBoxInfo[i][j].reset();
    mTransformMatrix.setToIdentity();
}
//####################################################################
