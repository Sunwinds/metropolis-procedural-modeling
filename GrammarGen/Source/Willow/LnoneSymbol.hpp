//#####################################################################
// Copyright 2009, Jerry Talton.
// This software is governed by the license contained in LICENSE.
//#####################################################################
#ifndef _LNONE_SYMBOL_HPP_
#define _LNONE_SYMBOL_HPP_

#include "Arrays.hpp"
#include "Distributions.hpp"
#include "Symbol.hpp"

namespace ProceduralModeling{

//#####################################################################
// Class LnoneSymbol
//#####################################################################
class LnoneSymbol : public Symbol
{
public:
    LnoneSymbol(Symbol** symbolTable)
     :Symbol(false,17,44,0,2,symbolTable)
    {}

    LnoneSymbol(const bool isTerminal,const uint maxNumSymbols,const uint maxNumSymbolEntries,const uint numDiffusionParams,const uint numFormalParams,Symbol** symbolTable)
     :Symbol(isTerminal,maxNumSymbols,maxNumSymbolEntries,numDiffusionParams,numFormalParams,symbolTable)
    {}

    virtual uint produce(const SymbolEntry* input,SymbolEntry* output,float& logProbability) const
    {
        if(input[0].isInvalid()){ output[0].mID=105; memcpy(output+1,input,sizeof(SymbolEntry)*3); logProbability=0; return 4; }

        const float* params=reinterpret_cast<const float*>(input+1);

        output[0].mID=200;
        output[1].mParam=0.1;
        output[2].mParam=0.7;
        output[3].mParam=0.0;
        output[4].mID=0;
        output[5].mID=5;
        output[6].mParam=130.0;
        output[7].mID=209;
        output[8].mParam=0.0;
        output[9].mParam=1.0;
        output[10].mParam=0.0;
        output[11].mID=204;
        output[12].mParam=0.0;
        output[13].mParam=-0.5;
        output[14].mParam=0.0;
        output[15].mID=210;
        output[16].mParam=params[0]*0.3*1.1;
        output[17].mID=211;
        output[18].mParam=params[0]*0.7;
        output[19].mParam=params[0]*0.7;
        output[20].mID=1;
        output[21].mID=0;
        output[22].mID=7;
        output[23].mParam=180.0;
        output[24].mID=5;
        output[25].mParam=130.0;
        output[26].mID=209;
        output[27].mParam=0.0;
        output[28].mParam=1.0;
        output[29].mParam=0.0;
        output[30].mID=204;
        output[31].mParam=0.0;
        output[32].mParam=-0.5;
        output[33].mParam=0.0;
        output[34].mID=210;
        output[35].mParam=params[0]*0.3*1.1;
        output[36].mID=211;
        output[37].mParam=params[0]*0.7;
        output[38].mParam=params[0]*0.7;
        output[39].mID=1;
        output[40].mID=200;
        output[41].mParam=0.6;
        output[42].mParam=0.3;
        output[43].mParam=0.1;
        logProbability=0;
        return 44;
    }

    virtual ushort setFlags(const float* params) const
    {
        return SymbolEntry::DETERMINISTIC | (params[1]<5.0 ?  SymbolEntry::CLEAR : SymbolEntry::INVALID);
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
    { return "Lnone"; }

//#####################################################################
};

//#####################################################################
} // namespace ProceduralModeling

#endif //_LNONE_SYMBOL_HPP_
