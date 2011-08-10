//#####################################################################
// Copyright 2009, Jerry Talton.
// This software is governed by the license contained in LICENSE.
//#####################################################################
#ifndef _PIPE_SYMBOL_HPP_
#define _PIPE_SYMBOL_HPP_

#include "Symbol.hpp"

namespace ProceduralModeling{

//#####################################################################
// Class PipeSymbol
//#####################################################################
class PipeSymbol : public Symbol
{
public:
    PipeSymbol(Symbol** symbolTable)
     :Symbol(true,1,1,0,0,symbolTable)
    {}

    PipeSymbol(const bool isTerminal,const uint maxNumSymbols,const uint maxNumSymbolEntries,const uint numDiffusionParams,const uint numFormalParams,Symbol** symbolTable)
     :Symbol(isTerminal,maxNumSymbols,maxNumSymbolEntries,numDiffusionParams,numFormalParams,symbolTable)
    {}

    virtual void render(const float* params) const
    {
        glRotatef(180,0,1,0);
    }

    virtual uint produce(const SymbolEntry* input,SymbolEntry* output,float& logProbability) const
    {
        output[0].mID=8;
        logProbability=0;
        return 1;
    }

    virtual ushort setFlags(const float* params) const
    {
        return SymbolEntry::DETERMINISTIC;
    }

    virtual const char* symbolName() const
    { return "|"; }

//#####################################################################
};

//#####################################################################
} // namespace ProceduralModeling

#endif //_PIPE_SYMBOL_HPP_
