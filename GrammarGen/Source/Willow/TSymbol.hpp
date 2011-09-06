//#####################################################################
// Copyright 2009, Jerry Talton.
// This software is governed by the license contained in LICENSE.
//#####################################################################
#ifndef _T_SYMBOL_HPP_
#define _T_SYMBOL_HPP_

#include "Arrays.hpp"
#include "Distributions.hpp"
#include "Symbol.hpp"

namespace ProceduralModeling{

//#####################################################################
// Class TSymbol
//#####################################################################
class TSymbol : public Symbol
{
public:
    TSymbol(Symbol** symbolTable)
     :Symbol(false,11,42,0,2,symbolTable)
    {}

    TSymbol(const bool isTerminal,const uint maxNumSymbols,const uint maxNumSymbolEntries,const uint numDiffusionParams,const uint numFormalParams,Symbol** symbolTable)
     :Symbol(isTerminal,maxNumSymbols,maxNumSymbolEntries,numDiffusionParams,numFormalParams,symbolTable)
    {}

    virtual uint produce(const SymbolEntry* input,SymbolEntry* output,float& logProbability) const
    {
        const float* params=reinterpret_cast<const float*>(input+1);

        if(input[0].isInvalid()){ output[0].mID=103; memcpy(output+1,input,sizeof(SymbolEntry)*3); logProbability=0; return 4; }

        bool cond[12]={
            params[1]>2.0,
            params[1]>2.0,
            params[1]>2.0,
            params[1]>0.0&&params[1]<3.0,
            params[1]>0.0&&params[1]<3.0,
            params[1]>0.0&&params[1]<3.0,
            params[1]>0.0&&params[1]<3.0,
            params[1]==0.0,
            params[1]==0.0,
            params[1]==0.0,
            params[1]==0.0,
            true
        };

        float prob[12]={
            cond[0]?Math::clampNonNegative(0.45):0,
            cond[1]?Math::clampNonNegative(0.45):0,
            cond[2]?Math::clampNonNegative(0.1):0,
            cond[3]?Math::clampNonNegative(1.0/4.0):0,
            cond[4]?Math::clampNonNegative(1.0/4.0):0,
            cond[5]?Math::clampNonNegative(1.0/4.0):0,
            cond[6]?Math::clampNonNegative(1.0/4.0):0,
            cond[7]?Math::clampNonNegative(1.0/4.0):0,
            cond[8]?Math::clampNonNegative(1.0/4.0):0,
            cond[9]?Math::clampNonNegative(1.0/4.0):0,
            cond[10]?Math::clampNonNegative(1.0/4.0):0,
            cond[11]?Math::clampNonNegative(0.02):0
        };

        float probSum=0;
        for(uint i=0;i<12;i++) probSum+=prob[i]; assert(probSum>0);
        for(uint i=0;i<12;i++) prob[i]/=probSum;

        uint selector=discreteSample(12,prob[0],prob[1],prob[2],prob[3],prob[4],prob[5],prob[6],prob[7],prob[8],prob[9],prob[10],prob[11]);

        switch(selector)
        {
            case 0:
                output[0].mID=352;
                output[1].mParam=Math::uniformZeroToOne();
                output[2].mID=206;
                output[3].mParam=0.0;
                output[4].mParam=0.3;
                output[5].mParam=0.0;
                output[6].mID=204;
                output[7].mParam=0.0;
                output[8].mParam=-0.5+params[1]*0.1;
                output[9].mParam=0.0;
                output[10].mID=201;
                output[11].mParam=0.005;
                output[12].mID=202;
                output[13].mParam=params[0];
                output[14].mParam=0.0;
                output[15].mParam=0.0;
                output[16].mParam=1.0;
                output[17].mID=201;
                output[18].mParam=0.005*8.0;
                output[19].mID=200;
                output[20].mParam=0.1;
                output[21].mParam=0.7;
                output[22].mParam=0.0;
                output[23].mID=207;
                output[24].mParam=0.0;
                output[25].mParam=10.0;
                output[26].mParam=0.0;
                output[27].mID=102;
                output[29].mParam=2.0;
                output[30].mParam=3.0;
                output[31].mParam=7.0;
                output[32].mParam=1.0;
                output[33].mParam=-0.5;
                output[28].mFlags=mSymbolTable[102]->setFlags(reinterpret_cast<float*>(output+29));
                output[34].mID=200;
                output[35].mParam=0.6;
                output[36].mParam=0.3;
                output[37].mParam=0.1;
                output[38].mID=103;
                output[40].mParam=params[0];
                output[41].mParam=params[1]-1.0;
                output[39].mFlags=mSymbolTable[103]->setFlags(reinterpret_cast<float*>(output+40));
                logProbability=log(prob[0]);
                return 42;
            break;
            case 1:
                output[0].mID=353;
                output[1].mParam=Math::uniformZeroToOne();
                output[2].mID=206;
                output[3].mParam=0.0;
                output[4].mParam=0.3;
                output[5].mParam=0.0;
                output[6].mID=204;
                output[7].mParam=0.0;
                output[8].mParam=-0.5+params[1]*0.1;
                output[9].mParam=0.0;
                output[10].mID=201;
                output[11].mParam=0.005;
                output[12].mID=202;
                output[13].mParam=params[0];
                output[14].mParam=0.0;
                output[15].mParam=0.0;
                output[16].mParam=1.0;
                output[17].mID=201;
                output[18].mParam=0.005*8.0;
                output[19].mID=200;
                output[20].mParam=0.1;
                output[21].mParam=0.7;
                output[22].mParam=0.0;
                output[23].mID=207;
                output[24].mParam=0.0;
                output[25].mParam=10.0;
                output[26].mParam=0.0;
                output[27].mID=102;
                output[29].mParam=3.0;
                output[30].mParam=4.0;
                output[31].mParam=8.0;
                output[32].mParam=2.0;
                output[33].mParam=-0.7;
                output[28].mFlags=mSymbolTable[102]->setFlags(reinterpret_cast<float*>(output+29));
                output[34].mID=200;
                output[35].mParam=0.6;
                output[36].mParam=0.3;
                output[37].mParam=0.1;
                output[38].mID=103;
                output[40].mParam=params[0];
                output[41].mParam=params[1]-1.0;
                output[39].mFlags=mSymbolTable[103]->setFlags(reinterpret_cast<float*>(output+40));
                logProbability=log(prob[1]);
                return 42;
            break;
            case 2:
                output[0].mID=354;
                output[1].mParam=Math::uniformZeroToOne();
                output[2].mID=206;
                output[3].mParam=0.0;
                output[4].mParam=0.3;
                output[5].mParam=0.0;
                output[6].mID=204;
                output[7].mParam=0.0;
                output[8].mParam=-0.5+params[1]*0.1;
                output[9].mParam=0.0;
                output[10].mID=201;
                output[11].mParam=0.005;
                output[12].mID=202;
                output[13].mParam=params[0];
                output[14].mParam=0.0;
                output[15].mParam=0.0;
                output[16].mParam=1.0;
                output[17].mID=103;
                output[19].mParam=params[0];
                output[20].mParam=params[1]-1.0;
                output[18].mFlags=mSymbolTable[103]->setFlags(reinterpret_cast<float*>(output+19));
                logProbability=log(prob[2]);
                return 21;
            break;
            case 3:
                output[0].mID=355;
                output[1].mParam=Math::uniformZeroToOne();
                output[2].mID=206;
                output[3].mParam=0.0;
                output[4].mParam=0.3;
                output[5].mParam=0.0;
                output[6].mID=204;
                output[7].mParam=0.0;
                output[8].mParam=-0.5+params[1]*0.1;
                output[9].mParam=0.0;
                output[10].mID=201;
                output[11].mParam=0.005;
                output[12].mID=202;
                output[13].mParam=params[0]/2.0;
                output[14].mParam=0.0;
                output[15].mParam=0.0;
                output[16].mParam=1.0;
                output[17].mID=201;
                output[18].mParam=0.005*8.0;
                output[19].mID=200;
                output[20].mParam=0.1;
                output[21].mParam=0.7;
                output[22].mParam=0.0;
                output[23].mID=207;
                output[24].mParam=0.0;
                output[25].mParam=10.0;
                output[26].mParam=0.0;
                output[27].mID=102;
                output[29].mParam=2.0;
                output[30].mParam=3.0;
                output[31].mParam=7.0;
                output[32].mParam=1.0;
                output[33].mParam=-0.5;
                output[28].mFlags=mSymbolTable[102]->setFlags(reinterpret_cast<float*>(output+29));
                output[34].mID=200;
                output[35].mParam=0.6;
                output[36].mParam=0.3;
                output[37].mParam=0.1;
                output[38].mID=103;
                output[40].mParam=params[0];
                output[41].mParam=params[1]-1.0;
                output[39].mFlags=mSymbolTable[103]->setFlags(reinterpret_cast<float*>(output+40));
                logProbability=log(prob[3]);
                return 42;
            break;
            case 4:
                output[0].mID=356;
                output[1].mParam=Math::uniformZeroToOne();
                output[2].mID=206;
                output[3].mParam=0.0;
                output[4].mParam=0.3;
                output[5].mParam=0.0;
                output[6].mID=204;
                output[7].mParam=0.0;
                output[8].mParam=-0.3+params[1]*0.1;
                output[9].mParam=0.0;
                output[10].mID=201;
                output[11].mParam=0.005;
                output[12].mID=202;
                output[13].mParam=params[0]/2.0;
                output[14].mParam=0.0;
                output[15].mParam=0.0;
                output[16].mParam=1.0;
                output[17].mID=201;
                output[18].mParam=0.005*8.0;
                output[19].mID=200;
                output[20].mParam=0.1;
                output[21].mParam=0.7;
                output[22].mParam=0.0;
                output[23].mID=207;
                output[24].mParam=0.0;
                output[25].mParam=10.0;
                output[26].mParam=0.0;
                output[27].mID=102;
                output[29].mParam=3.0;
                output[30].mParam=4.0;
                output[31].mParam=8.0;
                output[32].mParam=2.0;
                output[33].mParam=-0.7;
                output[28].mFlags=mSymbolTable[102]->setFlags(reinterpret_cast<float*>(output+29));
                output[34].mID=200;
                output[35].mParam=0.6;
                output[36].mParam=0.3;
                output[37].mParam=0.1;
                output[38].mID=103;
                output[40].mParam=params[0];
                output[41].mParam=params[1]-1.0;
                output[39].mFlags=mSymbolTable[103]->setFlags(reinterpret_cast<float*>(output+40));
                logProbability=log(prob[4]);
                return 42;
            break;
            case 5:
                output[0].mID=357;
                output[1].mParam=Math::uniformZeroToOne();
                output[2].mID=206;
                output[3].mParam=0.0;
                output[4].mParam=0.3;
                output[5].mParam=0.0;
                output[6].mID=204;
                output[7].mParam=0.0;
                output[8].mParam=-0.5+params[1]*0.1;
                output[9].mParam=0.0;
                output[10].mID=201;
                output[11].mParam=0.005;
                output[12].mID=202;
                output[13].mParam=params[0]/2.0;
                output[14].mParam=0.0;
                output[15].mParam=0.0;
                output[16].mParam=1.0;
                output[17].mID=201;
                output[18].mParam=0.005*8.0;
                output[19].mID=200;
                output[20].mParam=0.1;
                output[21].mParam=0.7;
                output[22].mParam=0.0;
                output[23].mID=207;
                output[24].mParam=0.0;
                output[25].mParam=10.0;
                output[26].mParam=0.0;
                output[27].mID=102;
                output[29].mParam=4.0;
                output[30].mParam=5.0;
                output[31].mParam=9.0;
                output[32].mParam=3.0;
                output[33].mParam=-0.9;
                output[28].mFlags=mSymbolTable[102]->setFlags(reinterpret_cast<float*>(output+29));
                output[34].mID=200;
                output[35].mParam=0.6;
                output[36].mParam=0.3;
                output[37].mParam=0.1;
                output[38].mID=103;
                output[40].mParam=params[0];
                output[41].mParam=params[1]-1.0;
                output[39].mFlags=mSymbolTable[103]->setFlags(reinterpret_cast<float*>(output+40));
                logProbability=log(prob[5]);
                return 42;
            break;
            case 6:
                output[0].mID=358;
                output[1].mParam=Math::uniformZeroToOne();
                output[2].mID=206;
                output[3].mParam=0.0;
                output[4].mParam=0.3;
                output[5].mParam=0.0;
                output[6].mID=204;
                output[7].mParam=0.0;
                output[8].mParam=-0.5+params[1]*0.1;
                output[9].mParam=0.0;
                output[10].mID=201;
                output[11].mParam=0.005;
                output[12].mID=202;
                output[13].mParam=params[0]/2.0;
                output[14].mParam=0.0;
                output[15].mParam=0.0;
                output[16].mParam=1.0;
                output[17].mID=201;
                output[18].mParam=0.005*8.0;
                output[19].mID=200;
                output[20].mParam=0.1;
                output[21].mParam=0.7;
                output[22].mParam=0.0;
                output[23].mID=207;
                output[24].mParam=0.0;
                output[25].mParam=10.0;
                output[26].mParam=0.0;
                output[27].mID=102;
                output[29].mParam=4.0;
                output[30].mParam=5.0;
                output[31].mParam=10.0;
                output[32].mParam=4.0;
                output[33].mParam=-1.0;
                output[28].mFlags=mSymbolTable[102]->setFlags(reinterpret_cast<float*>(output+29));
                output[34].mID=200;
                output[35].mParam=0.6;
                output[36].mParam=0.3;
                output[37].mParam=0.1;
                output[38].mID=103;
                output[40].mParam=params[0];
                output[41].mParam=params[1]-1.0;
                output[39].mFlags=mSymbolTable[103]->setFlags(reinterpret_cast<float*>(output+40));
                logProbability=log(prob[6]);
                return 42;
            break;
            case 7:
                output[0].mID=201;
                output[1].mParam=0.005*8.0;
                output[2].mID=200;
                output[3].mParam=0.1;
                output[4].mParam=0.7;
                output[5].mParam=0.0;
                output[6].mID=207;
                output[7].mParam=0.0;
                output[8].mParam=10.0;
                output[9].mParam=0.0;
                output[10].mID=102;
                output[12].mParam=2.0;
                output[13].mParam=3.0;
                output[14].mParam=7.0;
                output[15].mParam=1.0;
                output[16].mParam=-0.5;
                output[11].mFlags=mSymbolTable[102]->setFlags(reinterpret_cast<float*>(output+12));
                output[17].mID=200;
                output[18].mParam=0.6;
                output[19].mParam=0.3;
                output[20].mParam=0.1;
                output[21].mID=203;
                logProbability=log(prob[7]);
                return 22;
            break;
            case 8:
                output[0].mID=201;
                output[1].mParam=0.005*8.0;
                output[2].mID=200;
                output[3].mParam=0.1;
                output[4].mParam=0.7;
                output[5].mParam=0.0;
                output[6].mID=207;
                output[7].mParam=0.0;
                output[8].mParam=10.0;
                output[9].mParam=0.0;
                output[10].mID=102;
                output[12].mParam=3.0;
                output[13].mParam=4.0;
                output[14].mParam=8.0;
                output[15].mParam=2.0;
                output[16].mParam=-0.7;
                output[11].mFlags=mSymbolTable[102]->setFlags(reinterpret_cast<float*>(output+12));
                output[17].mID=200;
                output[18].mParam=0.6;
                output[19].mParam=0.3;
                output[20].mParam=0.1;
                output[21].mID=203;
                logProbability=log(prob[8]);
                return 22;
            break;
            case 9:
                output[0].mID=201;
                output[1].mParam=0.005*8.0;
                output[2].mID=200;
                output[3].mParam=0.1;
                output[4].mParam=0.7;
                output[5].mParam=0.0;
                output[6].mID=207;
                output[7].mParam=0.0;
                output[8].mParam=10.0;
                output[9].mParam=0.0;
                output[10].mID=102;
                output[12].mParam=4.0;
                output[13].mParam=5.0;
                output[14].mParam=9.0;
                output[15].mParam=3.0;
                output[16].mParam=-0.9;
                output[11].mFlags=mSymbolTable[102]->setFlags(reinterpret_cast<float*>(output+12));
                output[17].mID=200;
                output[18].mParam=0.6;
                output[19].mParam=0.3;
                output[20].mParam=0.1;
                output[21].mID=203;
                logProbability=log(prob[9]);
                return 22;
            break;
            case 10:
                output[0].mID=201;
                output[1].mParam=0.005*8.0;
                output[2].mID=200;
                output[3].mParam=0.1;
                output[4].mParam=0.7;
                output[5].mParam=0.0;
                output[6].mID=207;
                output[7].mParam=0.0;
                output[8].mParam=10.0;
                output[9].mParam=0.0;
                output[10].mID=102;
                output[12].mParam=4.0;
                output[13].mParam=5.0;
                output[14].mParam=10.0;
                output[15].mParam=4.0;
                output[16].mParam=-1.0;
                output[11].mFlags=mSymbolTable[102]->setFlags(reinterpret_cast<float*>(output+12));
                output[17].mID=200;
                output[18].mParam=0.6;
                output[19].mParam=0.3;
                output[20].mParam=0.1;
                output[21].mID=203;
                logProbability=log(prob[10]);
                return 22;
            break;
            case 11:
                output[0].mID=203;
                logProbability=log(prob[11]);
                return 1;
            break;
        }
        return -1;
    }

    virtual ushort setFlags(const float* params) const
    {
        uint conditionals=0;
        if(params[1]>2.0) conditionals++;
        if(params[1]>2.0) conditionals++;
        if(params[1]>2.0) conditionals++;
        if(params[1]>0.0&&params[1]<3.0) conditionals++;
        if(params[1]>0.0&&params[1]<3.0) conditionals++;
        if(params[1]>0.0&&params[1]<3.0) conditionals++;
        if(params[1]>0.0&&params[1]<3.0) conditionals++;
        if(params[1]==0.0) conditionals++;
        if(params[1]==0.0) conditionals++;
        if(params[1]==0.0) conditionals++;
        if(params[1]==0.0) conditionals++;
        if(true) conditionals++;

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
    { return "T"; }

//#####################################################################
};

//#####################################################################
} // namespace ProceduralModeling

#endif //_T_SYMBOL_HPP_
