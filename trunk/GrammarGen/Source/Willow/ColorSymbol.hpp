//#####################################################################
// Copyright 2009, Jerry Talton.
// This software is governed by the license contained in LICENSE.
//#####################################################################
#ifndef _COLOR_SYMBOL_HPP_
#define _COLOR_SYMBOL_HPP_

#include "Arrays.hpp"
#include "Distributions.hpp"
#include "Symbol.hpp"

namespace ProceduralModeling{

//#####################################################################
// Class ColorSymbol
//#####################################################################
class ColorSymbol : public Symbol
{
public:
    ColorSymbol(Symbol** symbolTable)
     :Symbol(true,1,4,0,3,symbolTable)
    {}

    ColorSymbol(const bool isTerminal,const uint maxNumSymbols,const uint maxNumSymbolEntries,const uint numDiffusionParams,const uint numFormalParams,Symbol** symbolTable)
     :Symbol(isTerminal,maxNumSymbols,maxNumSymbolEntries,numDiffusionParams,numFormalParams,symbolTable)
    {}

    virtual uint produce(const SymbolEntry* input,SymbolEntry* output,float& logProbability) const
    {
        output[0].mID=200;
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
		glColor3f (params[0], params[1], params[2]);
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
    { return "Color"; }

//#####################################################################
};

//#####################################################################
} // namespace ProceduralModeling

#endif //_COLOR_SYMBOL_HPP_
