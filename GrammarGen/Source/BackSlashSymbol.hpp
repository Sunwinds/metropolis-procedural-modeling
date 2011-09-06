//#####################################################################
// Copyright 2009, Jerry Talton.
// This software is governed by the license contained in LICENSE.
//#####################################################################
#ifndef _BACK_SLASH_SYMBOL_HPP_
#define _BACK_SLASH_SYMBOL_HPP_

#include "Symbol.hpp"

namespace ProceduralModeling{

//#####################################################################
// Class BackSlashSymbol
//#####################################################################
class BackSlashSymbol : public Symbol
{
public:
    BackSlashSymbol(Symbol** symbolTable)
     :Symbol(true,1,2,0,1,symbolTable)
    {}

    BackSlashSymbol(const bool isTerminal,const uint maxNumSymbols,const uint maxNumSymbolEntries,const uint numDiffusionParams,const uint numFormalParams,Symbol** symbolTable)
     :Symbol(isTerminal,maxNumSymbols,maxNumSymbolEntries,numDiffusionParams,numFormalParams,symbolTable)
    {}

    virtual void render(const float* params) const
    {
        glRotatef(-params[0],0,1,0);
    }

    virtual uint produce(const SymbolEntry* input,SymbolEntry* output,float& logProbability) const
    {
        output[0].mID=6;
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
        mResourceHandler->getOutputMesh()->curDirTrans().rotate(-params[0], 0, 1, 0);
    }

    virtual const char* symbolName() const
    { return "\\"; }

//#####################################################################
};

//#####################################################################
} // namespace ProceduralModeling

#endif //_BACK_SLASH_SYMBOL_HPP_
