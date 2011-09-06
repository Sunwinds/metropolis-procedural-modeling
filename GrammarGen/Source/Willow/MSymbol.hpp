//#####################################################################
// Copyright 2009, Jerry Talton.
// This software is governed by the license contained in LICENSE.
//#####################################################################
#ifndef _M_SYMBOL_HPP_
#define _M_SYMBOL_HPP_

#include "Arrays.hpp"
#include "Distributions.hpp"
#include "Symbol.hpp"

namespace ProceduralModeling{

//#####################################################################
// Class MSymbol
//#####################################################################
class MSymbol : public Symbol
{
public:
    MSymbol(Symbol** symbolTable)
     :Symbol(false,9,27,0,3,symbolTable)
    {}

    MSymbol(const bool isTerminal,const uint maxNumSymbols,const uint maxNumSymbolEntries,const uint numDiffusionParams,const uint numFormalParams,Symbol** symbolTable)
     :Symbol(isTerminal,maxNumSymbols,maxNumSymbolEntries,numDiffusionParams,numFormalParams,symbolTable)
    {}

    virtual uint produce(const SymbolEntry* input,SymbolEntry* output,float& logProbability) const
    {
        const float* params=reinterpret_cast<const float*>(input+1);

        if(input[0].isInvalid()){ output[0].mID=100; memcpy(output+1,input,sizeof(SymbolEntry)*4); logProbability=0; return 5; }

        bool cond[2]={
            params[0]>0.0,
            params[0]>0.0
        };

        float prob[2]={
            cond[0]?Math::clampNonNegative(3.0/(params[0])):0,
            cond[1]?Math::clampNonNegative(1.0-3.0/params[0]):0
        };

        float probSum=0;
        for(uint i=0;i<2;i++) probSum+=prob[i]; assert(probSum>0);
        for(uint i=0;i<2;i++) prob[i]/=probSum;

        uint selector=discreteSample(2,prob[0],prob[1]);

        switch(selector)
        {
            case 0:
                output[0].mID=101;
                output[2].mParam=params[0];
                output[3].mParam=params[1]*0.75;
                output[4].mParam=137.5;
                output[5].mParam=1.0;
                output[6].mParam=params[2];
                output[1].mFlags=mSymbolTable[101]->setFlags(reinterpret_cast<float*>(output+2));
                logProbability=log(prob[0]);
                return 7;
            break;
            case 1:
                output[0].mID=301;
                output[1].mParam=Math::uniformZeroToOne();
                output[2].mParam=Math::uniformZeroToOne();
                output[3].mID=0;
                output[4].mID=302;
                output[5].mParam=Math::uniformZeroToOne();
                output[6].mID=7;
                output[7].mParam=90.0;
                output[8].mID=101;
                output[10].mParam=params[0];
                output[11].mParam=params[1]*0.75;
                output[12].mParam=137.5;
                output[13].mParam=1.0;
                output[14].mParam=(params[2]-1.0)*0.3;
                output[9].mFlags=mSymbolTable[101]->setFlags(reinterpret_cast<float*>(output+10));
                output[15].mID=1;
                output[16].mID=201;
                output[17].mParam=0.005*exp(log((params[2]-1.0))/1.7);
                output[18].mID=303;
                output[19].mParam=Math::uniformZeroToOne();
                output[20].mParam=params[0];
                output[21].mParam=params[1];
                output[22].mID=100;
                output[24].mParam=params[0]-1.0;
                output[25].mParam=params[1]*0.9;
                output[26].mParam=(params[2]-1.0)*0.7;
                output[23].mFlags=mSymbolTable[100]->setFlags(reinterpret_cast<float*>(output+24));
                logProbability=log(prob[1]);
                return 27;
            break;
        }
        return -1;
    }

    virtual ushort setFlags(const float* params) const
    {
        uint conditionals=0;
        if(params[0]>0.0) conditionals++;
        if(params[0]>0.0) conditionals++;

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
    { return "M"; }

//#####################################################################
};

//#####################################################################
} // namespace ProceduralModeling

#endif //_M_SYMBOL_HPP_
