//#####################################################################
// Copyright 2009, Jerry Talton.
// This software is governed by the license contained in LICENSE.
//#####################################################################
#ifndef _POLYGON_SYMBOL_HPP_
#define _POLYGON_SYMBOL_HPP_

#include "Arrays.hpp"
#include "Distributions.hpp"
#include "Symbol.hpp"

namespace ProceduralModeling{

//#####################################################################
// Class PolygonSymbol
//#####################################################################
class PolygonSymbol : public Symbol
{
public:
    PolygonSymbol(Symbol** symbolTable)
     :Symbol(true,1,4,0,3,symbolTable)
    {}

    PolygonSymbol(const bool isTerminal,const uint maxNumSymbols,const uint maxNumSymbolEntries,const uint numDiffusionParams,const uint numFormalParams,Symbol** symbolTable)
     :Symbol(isTerminal,maxNumSymbols,maxNumSymbolEntries,numDiffusionParams,numFormalParams,symbolTable)
    {}

    virtual uint produce(const SymbolEntry* input,SymbolEntry* output,float& logProbability) const
    {
        output[0].mID=208;
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
        mResourceHandler->bindShader(mShader);
		float width_parameter = getWidth();

		glBegin (GL_TRIANGLE_STRIP);

		glVertex3f (-0.5*width_parameter*2, 0.0, 0);
		glVertex3f (-0.5*width_parameter*2, params[0], 0);
		glVertex3f (0.5*width_parameter*2, 0.0, 0);
		glVertex3f (0.5*width_parameter*2, params[0], 0);

		glEnd();
		glTranslatef(0,params[0],0);
    }

    virtual void initGraphics()
    {

    }

    virtual void saveToMesh(const float* params) const
    {

    }

    virtual void voxel(const float* params) const
    {

    }

    virtual const char* symbolName() const
    { return "Polygon"; }

//#####################################################################
};

//#####################################################################
} // namespace ProceduralModeling

#endif //_POLYGON_SYMBOL_HPP_
