//#####################################################################
// Copyright 2009, Jerry Talton.
// This software is governed by the license contained in LICENSE.
//#####################################################################
#ifndef _TURNZTOVEC_SYMBOL_HPP_
#define _TURNZTOVEC_SYMBOL_HPP_

#include "Arrays.hpp"
#include "Distributions.hpp"
#include "Symbol.hpp"
#include "VectorT.hpp"

namespace ProceduralModeling{

//#####################################################################
// Class TurnztovecSymbol
//#####################################################################
class TurnztovecSymbol : public Symbol
{
public:
    TurnztovecSymbol(Symbol** symbolTable)
     :Symbol(true,1,4,0,3,symbolTable)
    {}

    TurnztovecSymbol(const bool isTerminal,const uint maxNumSymbols,const uint maxNumSymbolEntries,const uint numDiffusionParams,const uint numFormalParams,Symbol** symbolTable)
     :Symbol(isTerminal,maxNumSymbols,maxNumSymbolEntries,numDiffusionParams,numFormalParams,symbolTable)
    {}

    virtual uint produce(const SymbolEntry* input,SymbolEntry* output,float& logProbability) const
    {
        output[0].mID=209;
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
        float xPos = matrix[12], zPos = matrix[14];

		Vectorf newz(matrix[8], matrix[9], matrix[10]);
		Vectorf dir(params[0], params[1], params[2]);
		// add params to z
		newz += newz.dot(dir) > 0 ? dir : -dir; // turn either face towards the vector
		newz.normalize();
		// old x and new z form a plane, get its normal
		Vectorf normal = ((Vectorf *)(matrix + 0))->cross(newz);
		// newx
		*(Vectorf *)(matrix + 0) = (newz.cross(normal)).normalize();
		// newy
		*(Vectorf *)(matrix + 4) = -newz.cross(*(Vectorf *)(matrix + 0));
		// newz
		*(Vectorf *)(matrix + 8) = newz;

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
        QVector3D newz(data[8], data[9], data[10]);
        QVector3D dir(params[0], params[1], params[2]);
        newz += QVector3D::dotProduct(newz, dir) > 0 ? dir : -dir;
        newz.normalize();
        QVector3D nor = QVector3D::crossProduct(x, newz);
        QVector3D newx = QVector3D::normal(newz, nor);
        QVector3D newy = -QVector3D::crossProduct(newz, newx);
        mesh->curDirTrans().setColumn(0, QVector4D(newx, data[3]));
        mesh->curDirTrans().setColumn(1, QVector4D(newy, data[7]));
        mesh->curDirTrans().setColumn(2, QVector4D(newz, data[11]));
    }

    virtual void voxel(const float* params) const
    {

    }

    virtual const char* symbolName() const
    { return "Turnztovec"; }

//#####################################################################
};

//#####################################################################
} // namespace ProceduralModeling

#endif //_TURNZTOVEC_SYMBOL_HPP_
