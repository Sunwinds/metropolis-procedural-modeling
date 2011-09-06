//#####################################################################
// Copyright 2009, Jerry Talton.
// This software is governed by the license contained in LICENSE.
//#####################################################################
#ifndef _TURNPERPTOVEC_SYMBOL_HPP_
#define _TURNPERPTOVEC_SYMBOL_HPP_

#include "Arrays.hpp"
#include "Distributions.hpp"
#include "Symbol.hpp"

namespace ProceduralModeling{

//#####################################################################
// Class TurnperptovecSymbol
//#####################################################################
class TurnperptovecSymbol : public Symbol
{
public:
    TurnperptovecSymbol(Symbol** symbolTable)
     :Symbol(true,1,4,0,3,symbolTable)
    {}

    TurnperptovecSymbol(const bool isTerminal,const uint maxNumSymbols,const uint maxNumSymbolEntries,const uint numDiffusionParams,const uint numFormalParams,Symbol** symbolTable)
     :Symbol(isTerminal,maxNumSymbols,maxNumSymbolEntries,numDiffusionParams,numFormalParams,symbolTable)
    {}

    virtual uint produce(const SymbolEntry* input,SymbolEntry* output,float& logProbability) const
    {
        output[0].mID=206;
        memcpy(output+1,input,sizeof(SymbolEntry)*3);
        logProbability=0;
        return 4;
    }

    virtual ushort setFlags(const float* params) const
    {
        return SymbolEntry::DETERMINISTIC;
    }

    virtual void render(const float* params) const
    {
		float matrix[16];
		glGetFloatv(GL_MODELVIEW_MATRIX, matrix);

		Vectorf y(matrix[4], matrix[5], matrix[6]);
		Vectorf dir(params[0], params[1], params[2]);
		Vectorf perpDir = (dir.cross (y)).cross(dir).normalize();
		perpDir *= dir.length();

		// The rest is turtovec code
		Vectorf x (matrix[0], matrix[1], matrix[2]);
		// add params to y
		Vectorf newy = y + perpDir;
		newy.normalize();
		// old x and new y form a plane, get its normal
		Vectorf normal = x.cross(newy);
		// newx
		*(Vectorf *)(matrix + 0) = newy.cross(normal).normalize();
		// newy
		*(Vectorf *)(matrix + 4) = newy;
		// newz
		*(Vectorf *)(matrix + 8) = -newy.cross(*(Vectorf *)(matrix + 0));

		glMatrixMode(GL_MODELVIEW_MATRIX);
		glLoadMatrixf(matrix);

    }

    virtual void initGraphics()
    {

    }

    virtual void saveToMesh(const float* params) const
    {
        Mesh* mesh = mResourceHandler->getOutputMesh();
        QMatrix4x4 matrix = mesh->curDirTrans();
        qreal* data = matrix.data();

        QVector3D y(data[4], data[5], data[6]);
        QVector3D dir(params[0], params[1], params[2]);
        QVector3D perpDir = QVector3D::normal(QVector3D::crossProduct(dir,y),dir);
        perpDir *= dir.length();

        QVector3D x(data[0], data[1], data[2]);
        QVector3D newy = y+perpDir;
        newy.normalize();

        QVector3D nor = QVector3D::normal(x, newy);
        QVector3D newx = QVector3D::normal(newy, nor);
        QVector3D newz = -QVector3D::crossProduct(newy, newx);
        mesh->curDirTrans().setColumn(0, QVector4D(newx, data[3]));
        mesh->curDirTrans().setColumn(1, QVector4D(newy, data[7]));
        mesh->curDirTrans().setColumn(2, QVector4D(newz, data[11]));
    }

    virtual void voxel(const float* params) const
    {

    }

    virtual const char* symbolName() const
    { return "Turnperptovec"; }

//#####################################################################
};

//#####################################################################
} // namespace ProceduralModeling

#endif //_TURNPERPTOVEC_SYMBOL_HPP_
