//#####################################################################
// Copyright 2009, Jerry Talton.
// This software is governed by the license contained in LICENSE.
//#####################################################################
#ifndef _LEAFCONNECTOR_SYMBOL_HPP_
#define _LEAFCONNECTOR_SYMBOL_HPP_

#include "Arrays.hpp"
#include "Distributions.hpp"
#include "Symbol.hpp"

namespace ProceduralModeling{

//#####################################################################
// Class LeafconnectorSymbol
//#####################################################################
class LeafconnectorSymbol : public Symbol
{
public:
    LeafconnectorSymbol(Symbol** symbolTable)
     :Symbol(true,1,2,0,1,symbolTable)
    {}

    LeafconnectorSymbol(const bool isTerminal,const uint maxNumSymbols,const uint maxNumSymbolEntries,const uint numDiffusionParams,const uint numFormalParams,Symbol** symbolTable)
     :Symbol(isTerminal,maxNumSymbols,maxNumSymbolEntries,numDiffusionParams,numFormalParams,symbolTable)
    {}

    virtual uint produce(const SymbolEntry* input,SymbolEntry* output,float& logProbability) const
    {
        output[0].mID=210;
        memcpy(output+1,input,sizeof(SymbolEntry)*1);
        logProbability=0;
        return 2;
    }

    virtual ushort setFlags(const float* params) const
    {
        return SymbolEntry::DETERMINISTIC;
    }

    virtual void render(const float* params) const
    {
        mResourceHandler->bindShader(mShader);
        glBegin(GL_LINES); glVertex3f(0,0,0); glVertex3f(0,params[0],0); glEnd(); glTranslatef(0,params[0],0);
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
    { return "Leafconnector"; }

//#####################################################################
};

//#####################################################################
} // namespace ProceduralModeling

#endif //_LEAFCONNECTOR_SYMBOL_HPP_
