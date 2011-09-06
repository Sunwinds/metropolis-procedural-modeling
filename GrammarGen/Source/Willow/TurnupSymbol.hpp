//#####################################################################
// Copyright 2009, Jerry Talton.
// This software is governed by the license contained in LICENSE.
//#####################################################################
#ifndef _TURNUP_SYMBOL_HPP_
#define _TURNUP_SYMBOL_HPP_

#include "Arrays.hpp"
#include "Distributions.hpp"
#include "Symbol.hpp"

namespace ProceduralModeling{

//#####################################################################
// Class TurnupSymbol
//#####################################################################
class TurnupSymbol : public Symbol
{
public:
    TurnupSymbol(Symbol** symbolTable)
     :Symbol(true,1,4,0,3,symbolTable)
    {}

    TurnupSymbol(const bool isTerminal,const uint maxNumSymbols,const uint maxNumSymbolEntries,const uint numDiffusionParams,const uint numFormalParams,Symbol** symbolTable)
     :Symbol(isTerminal,maxNumSymbols,maxNumSymbolEntries,numDiffusionParams,numFormalParams,symbolTable)
    {}

    virtual uint produce(const SymbolEntry* input,SymbolEntry* output,float& logProbability) const
    {
        output[0].mID=207;
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

        Vectorf newZ(matrix[8]+params[0], matrix[9]+params[1], matrix[10]+params[2]);
        newZ.normalize();
        Vectorf y(matrix[4], matrix[5], matrix[6]);
		Vectorf newX = (y.cross (newZ)).normalize();

		// old x and new y form a plane, get its normal
		// newx
		*(Vectorf *)(matrix + 0) = newX;
		// newz
		*(Vectorf *)(matrix + 8) = -y.cross(*(Vectorf *)(matrix + 0));

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
        QVector3D up(data[8]+params[0], data[9]+params[1], data[10]+params[2]);
        up.normalize();
        QVector3D y(data[4], data[5], data[6]);
        QVector3D newx = QVector3D::normal(y, up);
        QVector3D newz = -QVector3D::crossProduct(y, newx);

        mesh->curDirTrans().setColumn(0, QVector4D(newx, data[3]));
        mesh->curDirTrans().setColumn(2, QVector4D(newz, data[11]));
    }

    virtual void voxel(const float* params) const
    {

    }

    virtual const char* symbolName() const
    { return "Turnup"; }

//#####################################################################
};

//#####################################################################
} // namespace ProceduralModeling

#endif //_TURNUP_SYMBOL_HPP_
