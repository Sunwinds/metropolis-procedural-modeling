//#####################################################################
// Copyright 2009, Jerry Talton.
// This software is governed by the license contained in LICENSE.
//#####################################################################
#ifndef _LEFT_BRACKET_SYMBOL_HPP_
#define _LEFT_BRACKET_SYMBOL_HPP_

#include "Symbol.hpp"

namespace ProceduralModeling{

//#####################################################################
// Class LeftBracketSymbol
//#####################################################################
class LeftBracketSymbol : public Symbol
{
public:
    LeftBracketSymbol(Symbol** symbolTable)
     :Symbol(true,1,1,0,0,symbolTable)
    {}

    LeftBracketSymbol(const bool isTerminal,const uint maxNumSymbols,const uint maxNumSymbolEntries,const uint numDiffusionParams,const uint numFormalParams,Symbol** symbolTable)
     :Symbol(isTerminal,maxNumSymbols,maxNumSymbolEntries,numDiffusionParams,numFormalParams,symbolTable)
    {}

    virtual void render(const float* params) const
    {
        glPushMatrix();
    }

    virtual uint produce(const SymbolEntry* input,SymbolEntry* output,float& logProbability) const
    {
        output[0].mID=0;
        logProbability=0;
        return 1;
    }

    virtual ushort setFlags(const float* params) const
    {
        return SymbolEntry::DETERMINISTIC;
    }

    virtual void saveToMesh(const float* params) const
    {
        mResourceHandler->getOutputMesh()->pushState();
    }

    virtual void voxel(const float* params) const
    {
        mResourceHandler->pushBBoxMatrix();
    }

    virtual const char* symbolName() const
    { return "["; }

//#####################################################################
};

//#####################################################################
} // namespace ProceduralModeling

#endif //_LEFT_BRACKET_SYMBOL_HPP_
