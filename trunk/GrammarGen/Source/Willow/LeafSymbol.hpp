//#####################################################################
// Copyright 2009, Jerry Talton.
// This software is governed by the license contained in LICENSE.
//#####################################################################
#ifndef _LEAF_SYMBOL_HPP_
#define _LEAF_SYMBOL_HPP_

#include "Arrays.hpp"
#include "Distributions.hpp"
#include "Symbol.hpp"

namespace ProceduralModeling{

//#####################################################################
// Class LeafSymbol
//#####################################################################
class LeafSymbol : public Symbol
{
public:
    LeafSymbol(Symbol** symbolTable)
     :Symbol(true,1,3,0,2,symbolTable)
    {}

    LeafSymbol(const bool isTerminal,const uint maxNumSymbols,const uint maxNumSymbolEntries,const uint numDiffusionParams,const uint numFormalParams,Symbol** symbolTable)
     :Symbol(isTerminal,maxNumSymbols,maxNumSymbolEntries,numDiffusionParams,numFormalParams,symbolTable)
    {}

    virtual uint produce(const SymbolEntry* input,SymbolEntry* output,float& logProbability) const
    {
        output[0].mID=211;
        memcpy(output+1,input,sizeof(SymbolEntry)*2);
        logProbability=0;
        return 3;
    }

    virtual ushort setFlags(const float* params) const
    {
        return SymbolEntry::DETERMINISTIC;
    }

    virtual void render(const float* params) const
    {
        mResourceHandler->bindShader(mShader);
        glBegin(GL_POLYGON);
		glNormal3f(0,0,1);
        float shift=.1*params[0],x=.5*params[0],y=params[1];
		glVertex3f(-x,-shift,0);
        glVertex3f(x,-shift,0);
		glVertex3f(x,y-shift,0);
        glVertex3f(-x,y-shift,0);
		glEnd();
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
    { return "Leaf"; }

//#####################################################################
};

//#####################################################################
} // namespace ProceduralModeling

#endif //_LEAF_SYMBOL_HPP_
