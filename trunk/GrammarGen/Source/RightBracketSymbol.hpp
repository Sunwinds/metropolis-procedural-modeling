//#####################################################################
// Copyright 2009, Jerry Talton.
// This software is governed by the license contained in LICENSE.
//#####################################################################
#ifndef _RIGHT_BRACKET_SYMBOL_HPP_
#define _RIGHT_BRACKET_SYMBOL_HPP_

#include "Symbol.hpp"

namespace ProceduralModeling{

//#####################################################################
// Class RightBracketSymbol
//#####################################################################
class RightBracketSymbol : public Symbol
{
public:
    RightBracketSymbol(Symbol** symbolTable)
     :Symbol(true,1,1,0,0,symbolTable)
    {}

    RightBracketSymbol(const bool isTerminal,const uint maxNumSymbols,const uint maxNumSymbolEntries,const uint numDiffusionParams,const uint numFormalParams,Symbol** symbolTable)
     :Symbol(isTerminal,maxNumSymbols,maxNumSymbolEntries,numDiffusionParams,numFormalParams,symbolTable)
    {}

    virtual void render(const float* params) const
    {
        glPopMatrix();
    }

    virtual uint produce(const SymbolEntry* input,SymbolEntry* output,float& logProbability) const
    {
        output[0].mID=1;
        logProbability=0;
        return 1;
    }

    virtual ushort setFlags(const float* params) const
    {
        return SymbolEntry::DETERMINISTIC;
    }

    virtual void saveToMesh(const float* params) const
    {
        mResourceHandler->getOutputMesh()->popState();
    }

    virtual void voxel(const float* params) const
    {
        mResourceHandler->popBBoxMatrix();
    }

    virtual const char* symbolName() const
    { return "]"; }

//#####################################################################
};

//#####################################################################
} // namespace ProceduralModeling

#endif //_RIGHT_BRACKET_SYMBOL_HPP_
