//#####################################################################
// Copyright 2009, Jerry Talton.
// This software is governed by the license contained in LICENSE.
//#####################################################################
#ifndef _C_SYMBOL_HPP_
#define _C_SYMBOL_HPP_

#include "Arrays.hpp"
#include "Distributions.hpp"
#include "Symbol.hpp"

namespace ProceduralModeling{

//#####################################################################
// Class CSymbol
//#####################################################################
class CSymbol : public Symbol
{
public:
    CSymbol(Symbol** symbolTable)
     :Symbol(false,5,17,0,5,symbolTable)
    {}

    CSymbol(const bool isTerminal,const uint maxNumSymbols,const uint maxNumSymbolEntries,const uint numDiffusionParams,const uint numFormalParams,Symbol** symbolTable)
     :Symbol(isTerminal,maxNumSymbols,maxNumSymbolEntries,numDiffusionParams,numFormalParams,symbolTable)
    {}

    virtual uint produce(const SymbolEntry* input,SymbolEntry* output,float& logProbability) const
    {
        const float* params=reinterpret_cast<const float*>(input+1);

        if(input[0].isInvalid()){ output[0].mID=102; memcpy(output+1,input,sizeof(SymbolEntry)*6); logProbability=0; return 7; }

        bool cond[4]={
            params[0]>0.0,
            params[0]<=0.0&&params[1]>0.0,
            params[0]<=0.0&&params[1]>0.0,
            params[1]<=0.0
        };

        float prob[4]={
            cond[0]?Math::clampNonNegative(1.0):0,
            cond[1]?Math::clampNonNegative(1.0-1.0/params[1]):0,
            cond[2]?Math::clampNonNegative(1.0/params[1]):0,
            cond[3]?Math::clampNonNegative(1.0):0
        };

        float probSum=0;
        for(uint i=0;i<4;i++) probSum+=prob[i]; assert(probSum>0);
        for(uint i=0;i<4;i++) prob[i]/=probSum;

        uint selector=discreteSample(4,prob[0],prob[1],prob[2],prob[3]);

        switch(selector)
        {
            case 0:
                output[0].mID=0;
                output[1].mID=350;
                output[2].mParam=Math::uniformZeroToOne();
                output[3].mID=104;
                output[5].mParam=0.4;
                output[6].mParam=params[2];
                output[7].mParam=params[3];
                output[8].mParam=params[4];
                output[4].mFlags=mSymbolTable[104]->setFlags(reinterpret_cast<float*>(output+5));
                output[9].mID=1;
                output[10].mID=102;
                output[12].mParam=params[0]-1.0;
                output[13].mParam=params[1]-1.0;
                output[14].mParam=params[2];
                output[15].mParam=params[3];
                output[16].mParam=params[4];
                output[11].mFlags=mSymbolTable[102]->setFlags(reinterpret_cast<float*>(output+12));
                logProbability=log(prob[0]);
                return 17;
            break;
            case 1:
                output[0].mID=0;
                output[1].mID=351;
                output[2].mParam=Math::uniformZeroToOne();
                output[3].mID=104;
                output[5].mParam=0.4;
                output[6].mParam=params[2];
                output[7].mParam=params[3];
                output[8].mParam=params[4];
                output[4].mFlags=mSymbolTable[104]->setFlags(reinterpret_cast<float*>(output+5));
                output[9].mID=1;
                output[10].mID=102;
                output[12].mParam=params[0]-1.0;
                output[13].mParam=params[1]-1.0;
                output[14].mParam=params[2];
                output[15].mParam=params[3];
                output[16].mParam=params[4];
                output[11].mFlags=mSymbolTable[102]->setFlags(reinterpret_cast<float*>(output+12));
                logProbability=log(prob[1]);
                return 17;
            break;
            case 2:
                output[0].mID=205;
                logProbability=log(prob[2]);
                return 1;
            break;
            case 3:
                output[0].mID=205;
                logProbability=log(prob[3]);
                return 1;
            break;
        }
        return -1;
    }

    virtual ushort setFlags(const float* params) const
    {
        uint conditionals=0;
        if(params[0]>0.0) conditionals++;
        if(params[0]<=0.0&&params[1]>0.0) conditionals++;
        if(params[0]<=0.0&&params[1]>0.0) conditionals++;
        if(params[1]<=0.0) conditionals++;

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
    { return "C"; }

//#####################################################################
};

//#####################################################################
} // namespace ProceduralModeling

#endif //_C_SYMBOL_HPP_
