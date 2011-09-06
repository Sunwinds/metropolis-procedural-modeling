//#####################################################################
// Copyright 2009, Jerry Talton.
// This software is governed by the license contained in LICENSE.
//#####################################################################
#ifndef _L_SYMBOL_HPP_
#define _L_SYMBOL_HPP_

#include "Arrays.hpp"
#include "Distributions.hpp"
#include "Symbol.hpp"

namespace ProceduralModeling{

//#####################################################################
// Class LSymbol
//#####################################################################
class LSymbol : public Symbol
{
public:
    LSymbol(Symbol** symbolTable)
     :Symbol(false,3,14,0,4,symbolTable)
    {}

    LSymbol(const bool isTerminal,const uint maxNumSymbols,const uint maxNumSymbolEntries,const uint numDiffusionParams,const uint numFormalParams,Symbol** symbolTable)
     :Symbol(isTerminal,maxNumSymbols,maxNumSymbolEntries,numDiffusionParams,numFormalParams,symbolTable)
    {}

    virtual uint produce(const SymbolEntry* input,SymbolEntry* output,float& logProbability) const
    {
        const float* params=reinterpret_cast<const float*>(input+1);

        if(input[0].isInvalid()){ output[0].mID=104; memcpy(output+1,input,sizeof(SymbolEntry)*5); logProbability=0; return 6; }

        bool cond[2]={
            params[1]>0.0,
            params[1]<3.0
        };

        float prob[2]={
            cond[0]?Math::clampNonNegative(1.0):0,
            cond[1]?Math::clampNonNegative(0.02):0
        };

        float probSum=0;
        for(uint i=0;i<2;i++) probSum+=prob[i]; assert(probSum>0);
        for(uint i=0;i<2;i++) prob[i]/=probSum;

        uint selector=discreteSample(2,prob[0],prob[1]);

        switch(selector)
        {
            case 0:
                output[0].mID=204;
                output[1].mParam=0.0;
                output[2].mParam=params[3];
                output[3].mParam=0.0;
                output[4].mID=208;
                output[5].mParam=params[0];
                output[6].mParam=params[1];
                output[7].mParam=params[2];
                output[8].mID=104;
                output[10].mParam=params[0];
                output[11].mParam=params[1]-1.0;
                output[12].mParam=params[2];
                output[13].mParam=params[3];
                output[9].mFlags=mSymbolTable[104]->setFlags(reinterpret_cast<float*>(output+10));
                logProbability=log(prob[0]);
                return 14;
            break;
            case 1:
                output[0].mID=205;
                logProbability=log(prob[1]);
                return 1;
            break;
        }
        return -1;
    }

    virtual ushort setFlags(const float* params) const
    {
        uint conditionals=0;
        if(params[1]>0.0) conditionals++;
        if(params[1]<3.0) conditionals++;

        switch(conditionals)
        {
            case 0: return SymbolEntry::INVALID; break;
            case 1: return SymbolEntry::DETERMINISTIC; break;
            default: return SymbolEntry::CLEAR; break;
        }
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
    { return "L"; }

//#####################################################################
};

//#####################################################################
} // namespace ProceduralModeling

#endif //_L_SYMBOL_HPP_
