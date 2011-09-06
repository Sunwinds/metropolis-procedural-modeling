//#####################################################################
// Copyright 2009, Jerry Talton.
// This software is governed by the license contained in LICENSE.
//#####################################################################
#ifndef _EXCLAMATION_SYMBOL_HPP_
#define _EXCLAMATION_SYMBOL_HPP_

#include "Symbol.hpp"

namespace ProceduralModeling{

//#####################################################################
// Class ExclamationSymbol
//#####################################################################
class ExclamationSymbol : public Symbol
{
public:
    ExclamationSymbol(Symbol** symbolTable)
     :Symbol(true,1,2,0,1,symbolTable)
    {}

    ExclamationSymbol(const bool isTerminal,const uint maxNumSymbols,const uint maxNumSymbolEntries,const uint numDiffusionParams,const uint numFormalParams,Symbol** symbolTable)
     :Symbol(isTerminal,maxNumSymbols,maxNumSymbolEntries,numDiffusionParams,numFormalParams,symbolTable)
    {}

    virtual void render(const float* params) const
    {
        glLineWidth(params[0]);
    }

    virtual uint produce(const SymbolEntry* input,SymbolEntry* output,float& logProbability) const
    {
        output[0].mID=9;
        output[1].mParam=input[0].mParam;
        logProbability=0;
        return 2;
    }

    virtual ushort setFlags(const float* params) const
    {
        return SymbolEntry::DETERMINISTIC;
    }

    virtual const char* symbolName() const
    { return "!"; }

//#####################################################################
};

//#####################################################################
} // namespace ProceduralModeling

#endif //_EXCLAMATION_SYMBOL_HPP_
