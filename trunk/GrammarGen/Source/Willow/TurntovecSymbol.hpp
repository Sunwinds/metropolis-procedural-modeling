//#####################################################################
// Copyright 2009, Jerry Talton.
// This software is governed by the license contained in LICENSE.
//#####################################################################
#ifndef _TURNTOVEC_SYMBOL_HPP_
#define _TURNTOVEC_SYMBOL_HPP_

#include "Arrays.hpp"
#include "Distributions.hpp"
#include "Symbol.hpp"
#include "VectorT.hpp"

namespace ProceduralModeling{

//#####################################################################
// Class TurntovecSymbol
//#####################################################################
class TurntovecSymbol : public Symbol
{
public:
    TurntovecSymbol(Symbol** symbolTable)
     :Symbol(true,1,4,0,3,symbolTable)
    {}

    TurntovecSymbol(const bool isTerminal,const uint maxNumSymbols,const uint maxNumSymbolEntries,const uint numDiffusionParams,const uint numFormalParams,Symbol** symbolTable)
     :Symbol(isTerminal,maxNumSymbols,maxNumSymbolEntries,numDiffusionParams,numFormalParams,symbolTable)
    {}

    virtual uint produce(const SymbolEntry* input,SymbolEntry* output,float& logProbability) const
    {
        output[0].mID=204;
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

		Vectorf x (matrix[0], matrix[1], matrix[2]);
		// add params to y
		Vectorf newy (matrix[4] + params[0], matrix[5] + params[1], matrix[6] + params[2]);
		newy.normalize();
		// old x and new y form a plane, get its normal
		Vectorf normal = x.cross(newy);
		// newx
		*(Vectorf *)(matrix + 0) = (newy.cross(normal)).normalize();
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

        QVector3D x(data[0], data[1], data[2]);
        QVector3D newy(data[4]+params[0], data[5]+params[1], data[6]+params[2]);
        newy.normalize();
        QVector3D nor = QVector3D::crossProduct(x, newy);
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
    { return "Turntovec"; }

//#####################################################################
};

//#####################################################################
} // namespace ProceduralModeling

#endif //_TURNTOVEC_SYMBOL_HPP_
