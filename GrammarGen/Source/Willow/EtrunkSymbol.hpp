//#####################################################################
// Copyright 2009, Jerry Talton.
// This software is governed by the license contained in LICENSE.
//#####################################################################
#ifndef _ETRUNK_SYMBOL_HPP_
#define _ETRUNK_SYMBOL_HPP_

#include "Arrays.hpp"
#include "Distributions.hpp"
#include "Symbol.hpp"

namespace ProceduralModeling{

//#####################################################################
// Class EtrunkSymbol
//#####################################################################
class EtrunkSymbol : public Symbol
{
public:
    EtrunkSymbol(Symbol** symbolTable)
     :Symbol(true,1,1,0,0,symbolTable)
    {}

    EtrunkSymbol(const bool isTerminal,const uint maxNumSymbols,const uint maxNumSymbolEntries,const uint numDiffusionParams,const uint numFormalParams,Symbol** symbolTable)
     :Symbol(isTerminal,maxNumSymbols,maxNumSymbolEntries,numDiffusionParams,numFormalParams,symbolTable)
    {}

    virtual uint produce(const SymbolEntry* input,SymbolEntry* output,float& logProbability) const
    {
        output[0].mID=203;
        logProbability=0;
        return 1;
    }

    virtual ushort setFlags(const float* params) const
    {
        return SymbolEntry::DETERMINISTIC;
    }

    virtual void render(const float* params) const
    {

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
    { return "Etrunk"; }

//#####################################################################
};

//#####################################################################
} // namespace ProceduralModeling

#endif //_ETRUNK_SYMBOL_HPP_
