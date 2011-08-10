//#####################################################################
// Copyright 2009, Jerry Talton.
// This software is governed by the license contained in LICENSE.
//#####################################################################
#ifndef _MINUS_SYMBOL_HPP_
#define _MINUS_SYMBOL_HPP_

#include "Symbol.hpp"

namespace ProceduralModeling{

//#####################################################################
// Class MinusSymbol
//#####################################################################
class MinusSymbol : public Symbol
{
public:
    MinusSymbol(Symbol** symbolTable)
     :Symbol(true,1,2,0,1,symbolTable)
    {}

    MinusSymbol(const bool isTerminal,const uint maxNumSymbols,const uint maxNumSymbolEntries,const uint numDiffusionParams,const uint numFormalParams,Symbol** symbolTable)
     :Symbol(isTerminal,maxNumSymbols,maxNumSymbolEntries,numDiffusionParams,numFormalParams,symbolTable)
    {}

    virtual void render(const float* params) const
    {
        glRotatef(-params[0],0,0,1);
    }

    virtual uint produce(const SymbolEntry* input,SymbolEntry* output,float& logProbability) const
    {
        output[0].mID=3;
        output[1].mParam=input[0].mParam;
        logProbability=0;
        return 2;
    }

    virtual ushort setFlags(const float* params) const
    {
        return SymbolEntry::DETERMINISTIC;
    }

    virtual void saveToMesh(const float* params) const
    {
        mResourceHandler->getOutputMesh()->curDirTrans().rotate(-params[0],0,0,1);
    }

    virtual const char* symbolName() const
    { return "-"; }

//#####################################################################
};

//#####################################################################
} // namespace ProceduralModeling

#endif //_MINUS_SYMBOL_HPP_
