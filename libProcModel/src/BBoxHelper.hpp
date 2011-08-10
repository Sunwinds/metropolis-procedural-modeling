//#####################################################################
// Copyright 2009, Yu Lou, Jerry Talton.
//#####################################################################
#ifndef __BOUNDINGBOX_HELPER_HPP__
#define __BOUNDINGBOX_HELPER_HPP__

#define BBOX_DEPTH 16
#define NEIGHBOR 1

#include "Common.hpp"
#include "Math.hpp"
#include <QStack>
#include <QMatrix4x4>
#include <bitset>

using std::bitset;

namespace ProceduralModeling{

//####################################################################
// Class BBoxHelper
//####################################################################
class PROCEDURAL_EXPORT BBoxHelper
{
public:
    int mWidth,mHeight;
    float mMinX,mMinY,mMinZ;
    float mRangeX,mRangeY,mRangeZ;
    bitset<BBOX_DEPTH>** mBBoxInfo;
    QMatrix4x4 mTransformMatrix;
    QStack<QMatrix4x4> mStack;

public:
    BBoxHelper(int w,int h,float x1,float x2,float y1,float y2,float z1,float z2);
    ~BBoxHelper();
    void saveToBBoxInfo(float x1,float x2,float y1,float y2,float z1,float z2);
    void saveToMesh();
    void saveToFile(const QString& path);
    void reset();
//####################################################################
};

//####################################################################
} // namespace ProceduralModeling

#endif // __BOUNDINGBOX_HELPER__
