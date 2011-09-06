//#####################################################################
// Copyright 2009, Yu Lou.
//#####################################################################
#include "BuildingMesh.hpp"
#include "Math.hpp"

using namespace ProceduralModeling;

void BuildingMesh::init()
{
    mEnableSaveToMesh = false;
    mSubObject.clear();
    mTransformation.setToIdentity();
    memset(&(mGrids[0][0][0]), 0, sizeof(int)*16*16*16);
}

void BuildingMesh::initInternal(const QString& path, int w, int h, int d)
{
    FILE* fp = fopen(path.toLatin1().constData(), "r");
    if (fp == NULL)
    {
        printf("Failed to load %s\n", path.toLatin1().constData());
        exit(-1);
    }

    int*** data = new int**[w];
    mInternal = new int**[w];
    for (int i = 0;i < w; i++)
    {
        data[i] = new int*[h];
        mInternal[i] = new int*[h];
        for (int j = 0;j < h; j++)
        {
            data[i][j] = new int[d];
            memset(data[i][j], 0, sizeof(int)*d);
            mInternal[i][j] = new int[d];
            memset(mInternal[i][j], 0, sizeof(int)*d);
        }
    }

    for (int i = 0;i  < w; i++)
        for (int j = 0;j < h; j++)
            for (int k = 0;k < d; k++)
                data[i][j][k] = fgetc(fp)-'0';

    for (int i = 1;i  < w-1; i++)
        for (int j = 1;j < h-1; j++)
            for (int k = 1;k < d-1; k++)
                if (data[i][j][k-1]+data[i][j][k+1]+data[i][j-1][k]+data[i][j+1][k]+data[i-1][j][k]+data[i+1][j][k] == 6)
                    data[i][j][k] = 1;

    int count = 0;
    for (int i = 1;i < w-1; i++)
        for (int j = 1;j < h-1; j++)
            for (int k = 1; k < d-1; k++)
                if (data[i][j][k-1]+data[i][j][k+1]+data[i][j-1][k]+data[i][j+1][k]+data[i-1][j][k]+data[i+1][j][k] == 6)
                    mInternal[i][j][k] = 1;
    fclose(fp);
    for (int i = 0;i < w; i++)
    {
        for (int j = 0; j < h; j++)
            delete[] data[i][j];
        delete[] data[i];
    }
    delete[] data;
}

bool BuildingMesh::internalPart(float x, float y, float z)
{
	int ix = Math::round(x/0.4);
    ix = ix < 0 ? 0 : ix;
    ix = ix > 15 ? 15 : ix;
    int iy = Math::round(y/0.4);
    iy = iy < 0 ? 0 : iy;
    iy = iy > 15 ? 15 : iy;
    int iz = Math::round(z/0.4);
    iz = iz < 0 ? 0 : iz;
    iz = iz > 15 ? 15 : iz;
    if (mInternal[ix][iy][iz])
        return true;
    return false;
}

void BuildingMesh::addObj(TriangleMesh* mMesh, const QString& mName)
{
    mEnableSaveToMesh = true;
/*    QVector3D mPosition[8];
    mPosition[0] = mTransformation.map(QVector4D(0.0, 0.0, 0.0, 1.0)).toVector3D();
    mPosition[1] = mTransformation.map(QVector4D(mMesh->getMaxX(), 0.0, 0.0, 1.0)).toVector3D();
    mPosition[2] = mTransformation.map(QVector4D(0.0, mMesh->getMaxY(), 0.0, 1.0)).toVector3D();
    mPosition[3] = mTransformation.map(QVector4D(mMesh->getMaxX(), mMesh->getMaxY(), 0.0, 1.0)).toVector3D();
    mPosition[4] = mTransformation.map(QVector4D(0.0, 0.0, mMesh->getMaxZ(), 1.0)).toVector3D();
    mPosition[5] = mTransformation.map(QVector4D(mMesh->getMaxX(), 0.0, mMesh->getMaxZ(), 1.0)).toVector3D();
    mPosition[6] = mTransformation.map(QVector4D(0.0, mMesh->getMaxY(), mMesh->getMaxZ(), 1.0)).toVector3D();
    mPosition[7] = mTransformation.map(QVector4D(mMesh->getMaxX(), mMesh->getMaxY(), mMesh->getMaxZ(), 1.0)).toVector3D();
    int intern = 0;
    for (int i = 0;i < 8; i++)
        if (internalPart(mPosition[i].x(), mPosition[i].y(), mPosition[i].z()))
            intern++;
    if (intern >= 8)
        return;*/
    for (int i = 0;i < mSubObject.size(); i++)
    {
        if (QString::compare(mSubObject.at(i).mName, mName) == 0)
        {
            mSubObject[i].mTransformation.append(mTransformation);
            return;
        }
    }
    SubObj obj;
    obj.mTransformation.append(mTransformation);
    obj.mName = mName;
    obj.mMesh = mMesh;
    mSubObject.append(obj);
}

void BuildingMesh::addWall(float p1, float p2)
{
    Wall wall;
    wall.mNormal = mTransformation.mapVector(QVector3D(0, 0, -1));
    wall.mPosition[0] = mTransformation.map(QVector4D(p1, 0, 0, 1)).toVector3D();
    wall.mPosition[1] = mTransformation.map(QVector4D(0, 0, 0, 1)).toVector3D();
    wall.mPosition[2] = mTransformation.map(QVector4D(0, p2, 0, 1)).toVector3D();
    wall.mPosition[3] = mTransformation.map(QVector4D(p1, p2, 0, 1)).toVector3D();
/*    if (internalPart(wall.mPosition[0].x(),wall.mPosition[0].y(),wall.mPosition[0].z()) &&
        internalPart(wall.mPosition[1].x(),wall.mPosition[1].y(),wall.mPosition[1].z()) &&
        internalPart(wall.mPosition[2].x(),wall.mPosition[2].y(),wall.mPosition[2].z()) &&
        internalPart(wall.mPosition[3].x(),wall.mPosition[3].y(),wall.mPosition[3].z()))
        return;*/
    mWalls.append(wall);
}

void BuildingMesh::addRooftop(float p1, float p2, float offset)
{
#define GRID_HEIGHT 0.4
/*    Rooftop rooftop;
    rooftop.mNormal = mTransformation.mapVector(QVector3D(0, 1, 0));
    rooftop.mPosition[0] = mTransformation.map(QVector4D(p1, offset, 0, 1)).toVector3D();
    rooftop.mPosition[1] = mTransformation.map(QVector4D(0, offset, 0, 1)).toVector3D();
    rooftop.mPosition[2] = mTransformation.map(QVector4D(0, offset, p2, 1)).toVector3D();
    rooftop.mPosition[3] = mTransformation.map(QVector4D(p1, offset, p2, 1)).toVector3D();*/
/*    if (internalPart(rooftop.mPosition[0].x(),rooftop.mPosition[0].y(),rooftop.mPosition[0].z()) &&
        internalPart(rooftop.mPosition[1].x(),rooftop.mPosition[1].y(),rooftop.mPosition[1].z()) &&
        internalPart(rooftop.mPosition[2].x(),rooftop.mPosition[2].y(),rooftop.mPosition[2].z()) &&
        internalPart(rooftop.mPosition[3].x(),rooftop.mPosition[3].y(),rooftop.mPosition[3].z()))
        return;*/
//    mRooftops.append(rooftop);
    QVector3D pos = mTransformation.map(QVector4D(0, 0, 0, 1)).toVector3D();
    QVector3D pos2 = mTransformation.map(QVector4D(p1, 0, p2, 1)).toVector3D();
    int y = floor((pos.y()-GRID_HEIGHT/2.0f)/GRID_HEIGHT);    
    int x1 = floor((pos.x()+0.2)/0.4);
    int x2 = floor((pos2.x()-0.2)/0.4);
    int z1 = floor((pos.z()+0.2)/0.4);
    int z2 = floor((pos2.z()-0.2)/0.4);
    for (int i = x1;i <= x2; i++)
        for (int j = z1; j <= z2; j++)
            mGrids[i][y][j] = 1;
}

void BuildingMesh::pushState()
{
    mMatrixState.push(mTransformation);
}

void BuildingMesh::popState()
{
    mTransformation = mMatrixState.pop();
}

QMatrix4x4& BuildingMesh::curDirTrans()
{
    return mTransformation;
}

void BuildingMesh::processRoofs(QVector<Rooftop>& rooftops) const
{
#define ROOF_HEIGHT 0.05
#define INDENT_SIZE 0.03
#define SIZEINX 4
#define SIZEINY 4
#define SIZEINZ 4
    for (int x = 0; x < SIZEINX; x++)
    {
        float baseX = x*0.4;
        for (int y = 0;y < SIZEINY; y++)
        {
            float baseY = y*0.4;
            for (int z = 0;z < SIZEINZ; z++)
            {
                float baseZ = z*0.4;
                if (!mGrids[x][y][z])
                    continue;
                Rooftop roof;
                for (int i = 0;i < 4; i++)
                    roof.topPos[i].setY(baseY+0.4+ROOF_HEIGHT);
                roof.bottomPos[0] = QVector3D(baseX, baseY+0.4, baseZ);
                roof.bottomPos[1] = QVector3D(baseX+0.4, baseY+0.4, baseZ);
                roof.bottomPos[2] = QVector3D(baseX, baseY+0.4, baseZ+0.4);
                roof.bottomPos[3] = QVector3D(baseX+0.4, baseY+0.4, baseZ+0.4);

                if (z == 0 || mGrids[x][y][z-1] == 0)
                {
                    roof.topPos[0].setZ(baseZ+INDENT_SIZE);
                    roof.topPos[1].setZ(baseZ+INDENT_SIZE);
                    roof.side[0] = true;
                }
                else
                {
                    roof.topPos[0].setZ(baseZ-INDENT_SIZE);
                    roof.topPos[1].setZ(baseZ-INDENT_SIZE);
                    roof.side[0] = false;
                }

                if (z == SIZEINZ-1 || mGrids[x][y][z+1] == 0)
                {
                    roof.topPos[2].setZ(baseZ+0.4-INDENT_SIZE);
                    roof.topPos[3].setZ(baseZ+0.4-INDENT_SIZE);
                    roof.side[1] = true;
                }
                else
                {
                    roof.topPos[2].setZ(baseZ+0.4+INDENT_SIZE);
                    roof.topPos[3].setZ(baseZ+0.4+INDENT_SIZE);
                    roof.side[1] = false;
                }

                if (x == 0 || mGrids[x-1][y][z] == 0)
                {
                    roof.topPos[0].setX(baseX+INDENT_SIZE);
                    roof.topPos[2].setX(baseX+INDENT_SIZE);
                    roof.side[2] = true;
                }
                else
                {
                    roof.topPos[0].setX(baseX-INDENT_SIZE);
                    roof.topPos[2].setX(baseX-INDENT_SIZE);
                    roof.side[2] = false;
                }

                if (x == SIZEINX-1 || mGrids[x+1][y][z] == 0)
                {
                    roof.topPos[1].setX(baseX+0.4-INDENT_SIZE);
                    roof.topPos[3].setX(baseX+0.4-INDENT_SIZE);
                    roof.side[3] = true;
                }
                else
                {
                    roof.topPos[1].setX(baseX+0.4+INDENT_SIZE);
                    roof.topPos[3].setX(baseX+0.4+INDENT_SIZE);
                    roof.side[3] = false;
                }

                if (x > 0 && z > 0 && mGrids[x-1][y][z] && mGrids[x][y][z-1] && !mGrids[x-1][y][z-1])
                {
                    roof.topPos[0].setX(baseX+INDENT_SIZE);
                    roof.topPos[0].setZ(baseZ+INDENT_SIZE);
                }

                if (x < SIZEINX-1 && z > 0 && mGrids[x+1][y][z] && mGrids[x][y][z-1] && !mGrids[x+1][y][z-1])
                {
                    roof.topPos[1].setX(baseX+0.4-INDENT_SIZE);
                    roof.topPos[1].setZ(baseZ+INDENT_SIZE);
                }

                if (x > 0 && z < SIZEINZ-1 && mGrids[x-1][y][z] && mGrids[x][y][z+1] && !mGrids[x-1][y][z+1])
                {
                    roof.topPos[2].setX(baseX+INDENT_SIZE);
                    roof.topPos[2].setZ(baseZ+0.4-INDENT_SIZE);
                }

                if (x < SIZEINX-1 && z < SIZEINZ-1 && mGrids[x+1][y][z] && mGrids[x][y][z+1] && !mGrids[x+1][y][z+1])
                {
                    roof.topPos[3].setX(baseX+0.4-INDENT_SIZE);
                    roof.topPos[3].setZ(baseZ+0.4-INDENT_SIZE);
                }
                rooftops.append(roof);
            }
        }
    }    
}

void BuildingMesh::saveToObjFile(const QString& path) const
{
    int idx = path.lastIndexOf('/');
    QString mtlPath(path);
    mtlPath = mtlPath.remove(idx+1, path.length()-idx);
    mtlPath.append("bld.mtl");
    // Write out material file first
    FILE* fp = fopen(mtlPath.toLatin1().constData(), "w");
    if (fp == NULL)
    {
        printf("Failed to create %s\n", mtlPath.toLatin1().constData());
        exit(-1);
    }
    for (int i = 0; i < mSubObject.size(); i++)
    {
        SubObj obj = mSubObject.at(i);
        for (int j = 0;j < obj.mMesh->getMaterials().size(); j++)
        {
            GLMaterial mat = obj.mMesh->getMaterials().at(j);
            fprintf(fp, "newmtl %s_%s\n", obj.mName.toLatin1().constData(), mat.mName.toLatin1().constData());
            fprintf(fp, "Ka  %f %f %f\n", mat.mAmbient[0], mat.mAmbient[1], mat.mAmbient[2]);
            fprintf(fp, "Kd  %f %f %f\n", mat.mDiffuse[0], mat.mDiffuse[1], mat.mDiffuse[2]);
            fprintf(fp, "Ks  %f %f %f\n", mat.mSpecular[0], mat.mSpecular[1], mat.mSpecular[2]);
            fprintf(fp, "d  %f\n", mat.mAmbient[3]);
            fprintf(fp, "Ns  %f\n", mat.mShininess);
            fprintf(fp, "illum  2\n");
        }
    }

/*        fprintf(fp, "newmtl wall_mat\n");
    fprintf(fp, "Ka  0.3, 0.3, 0.3\n");
    fprintf(fp, "Kd  0.9, 0.8, 0.6\n");
    fprintf(fp, "Ks  0.9, 0.9, 0.9\n");
    fprintf(fp, "d  1.0\n");
    fprintf(fp, "Ns  10\n");
    fprintf(fp, "illum  2\n");*/

    fprintf(fp, "newmtl rooftop_mat\n");
    fprintf(fp, "Ka  0.3, 0.3, 0.3\n");
    fprintf(fp, "Kd  0.9, 0.8, 0.6\n");
    fprintf(fp, "Ks  0.9, 0.9, 0.9\n");
    fprintf(fp, "d  1.0\n");
    fprintf(fp, "Ns  10\n");
    fprintf(fp, "illum  2\n");
    fclose(fp);

    // Obj file
    fp = fopen(path.toLatin1().constData(), "w");
    if (fp == NULL)
    {
        printf("Failed to create %s\n", path.toLatin1().constData());
        exit(-1);
    }

    fprintf(fp, "mtllib ./bld.mtl\n");

    int vertexCount = 1, texCount = 1, mNormalCount = 1;
#if 1
    for (int i = 0; i < mSubObject.size() ;i++)
    {
        SubObj obj = mSubObject.at(i);
        TriangleMesh* mMesh = obj.mMesh;

        for(int l = 0;l < mMesh->getTexcoordList().size(); l++)
        {
            QVector2D vt = mMesh->getTexcoordList().at(l);
            fprintf(fp, "vt %f %f\n", vt.x(), vt.y());
        }

        for (int j = 0; j < obj.mTransformation.size(); j++)
        {
            QMatrix4x4 mTransformation = obj.mTransformation.at(j);

            for(int l = 0;l < mMesh->getVertexList().size(); l++)
            {
                QVector3D vo = mMesh->getVertexList().at(l);
                QVector3D v = mTransformation.map(QVector4D(vo, 1.0)).toVector3D();
                fprintf(fp, "v %f %f %f\n", v.x(), v.y(), v.z());
            }

            for(int l = 0;l < mMesh->getNormalList().size(); l++)
            {
                QVector3D vno = mMesh->getNormalList().at(l);
                QVector3D vn = mTransformation.mapVector(vno);
                vn.normalize();
                fprintf(fp, "vn %f %f %f\n", vn.x(), vn.y(), vn.z());
            }

            for (int k = 0; k < mMesh->getSubMesh().size(); k++)
            {
                SubMesh subMesh = mMesh->getSubMesh().at(k);

                // Write faces
                fprintf(fp, "g %s_%s_%d\n", obj.mName.toLatin1().constData(), subMesh.mGroupName.toLatin1().constData(), j);
                for (int l = subMesh.mIndex[0];l < subMesh.mIndex[1]; l++)
                {
                    UseMat useMat = mMesh->getUseMat().at(l);
                    if (useMat.mMaterial != -1)
                        fprintf(fp, "usemtl %s_%s\n", obj.mName.toLatin1().constData(), mMesh->getMaterials().at(useMat.mMaterial).mName.toLatin1().constData());

                    for (int t = useMat.mIndex[0]; t < useMat.mIndex[1]; t += 3)
                    {
                        fprintf(fp, "f");
                        for (int h = 0;h < 3; h++)
                            fprintf(fp, " %d/%d/%d", vertexCount+mMesh->getIndexList().at(t+h), texCount+mMesh->getTexIndexList().at(t+h),
                                                    mNormalCount+mMesh->getNormalIndexList().at(t+h));
                        fprintf(fp, "\n");
                    }
                }
            }
            vertexCount += mMesh->getVertexList().size();
            mNormalCount += mMesh->getNormalList().size();
        }
        texCount += mMesh->getTexcoordList().size();
    }
#endif
    // Wall
/*        fprintf(fp, "g\n");
    for (int i = 0;i < mWalls.size(); i++)
        for (int j = 0; j < 4; j++)
            fprintf(fp, "v %f %f %f\n", mWalls.at(i).mPosition[j].x(),  mWalls.at(i).mPosition[j].y(),  mWalls.at(i).mPosition[j].z());

    fprintf(fp, "vt %f %f\n", 0.0, 0.0);
    fprintf(fp, "vt %f %f\n", 1.0, 0.0);
    fprintf(fp, "vt %f %f\n", 1.0, 1.0);
    fprintf(fp, "vt %f %f\n", 0.0, 1.0);

    for (int i = 0;i < mWalls.size(); i++)
        fprintf(fp, "vn %f %f %f\n", mWalls.at(i).mNormal.x(),  mWalls.at(i).mNormal.y(),  mWalls.at(i).mNormal.z());

    fprintf(fp, "g wall\n");
    fprintf(fp, "usemtl wall_mat\n");
    for (int i = 0;i < mWalls.size(); i++)
    {
        fprintf(fp, "f");
        for (int j = 0;j < 4; j++)
            fprintf(fp, " %d/%d/%d", vertexCount+i*4+j, texCount+j, mNormalCount+i);
        fprintf(fp, "\n");
    }
    vertexCount += mWalls.size()*4;
    texCount += 4;
    mNormalCount += mWalls.size();*/

    QVector<Rooftop> rooftops;
    processRoofs(rooftops);
    // Rooftop
    fprintf(fp, "g\n");
    for (int i = 0;i < (int)rooftops.size(); i++)
    {
        for (int j = 0; j < 4; j++)
            fprintf(fp, "v %f %f %f\n", rooftops.at(i).bottomPos[j].x(),  rooftops.at(i).bottomPos[j].y(),  rooftops.at(i).bottomPos[j].z());
        for (int j = 0; j < 4; j++)
            fprintf(fp, "v %f %f %f\n", rooftops.at(i).topPos[j].x(),  rooftops.at(i).topPos[j].y(),  rooftops.at(i).topPos[j].z());
    }

    QVector3D dir(ROOF_HEIGHT, INDENT_SIZE, 0);
    dir.normalize();
    fprintf(fp, "vn %f %f %f\n", 0., 1., 0.);
    fprintf(fp, "vn %f %f %f\n", 0., dir.y(), -dir.x());
    fprintf(fp, "vn %f %f %f\n", 0., dir.y(), dir.x());
    fprintf(fp, "vn %f %f %f\n", -dir.x(), dir.y(), 0.0);    
    fprintf(fp, "vn %f %f %f\n", dir.x(), dir.y(), 0.0);

    fprintf(fp, "g rooftop\n");
    for (int i = 0;i < (int)rooftops.size(); i++)
    {
        fprintf(fp, "f %d//%d %d//%d %d//%d %d//%d\n", vertexCount+6, mNormalCount, vertexCount+7, mNormalCount, vertexCount+5, mNormalCount, vertexCount+4, mNormalCount);
        if (rooftops.at(i).side[0])
            fprintf(fp, "f %d//%d %d//%d %d//%d %d//%d\n", vertexCount+4, mNormalCount+1, vertexCount+5, mNormalCount+1, vertexCount+1, mNormalCount+1, vertexCount+0, mNormalCount+1);
        if (rooftops.at(i).side[1])
            fprintf(fp, "f %d//%d %d//%d %d//%d %d//%d\n", vertexCount+7, mNormalCount+2, vertexCount+6, mNormalCount+2, vertexCount+2, mNormalCount+2, vertexCount+3, mNormalCount+2);
        if (rooftops.at(i).side[2])
            fprintf(fp, "f %d//%d %d//%d %d//%d %d//%d\n", vertexCount+6, mNormalCount+3, vertexCount+4, mNormalCount+3, vertexCount+0, mNormalCount+3, vertexCount+2, mNormalCount+3);
        if (rooftops.at(i).side[3])
            fprintf(fp, "f %d//%d %d//%d %d//%d %d//%d\n", vertexCount+5, mNormalCount+4, vertexCount+7, mNormalCount+4, vertexCount+3, mNormalCount+4, vertexCount+1, mNormalCount+4);
        vertexCount += 8;
    }

    fclose(fp);

    printf("Saved to %s\n", path.toLatin1().constData());
}
