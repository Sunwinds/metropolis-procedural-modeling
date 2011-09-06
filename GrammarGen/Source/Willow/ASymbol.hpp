//#####################################################################
// Copyright 2009, Jerry Talton.
// This software is governed by the license contained in LICENSE.
//#####################################################################
#ifndef _A_SYMBOL_HPP_
#define _A_SYMBOL_HPP_

#include "Arrays.hpp"
#include "Distributions.hpp"
#include "Symbol.hpp"

namespace ProceduralModeling{

//#####################################################################
// Class ASymbol
//#####################################################################
class ASymbol : public Symbol
{
public:
    ASymbol(Symbol** symbolTable)
     :Symbol(false,10,37,0,5,symbolTable)
    {}

    ASymbol(const bool isTerminal,const uint maxNumSymbols,const uint maxNumSymbolEntries,const uint numDiffusionParams,const uint numFormalParams,Symbol** symbolTable)
     :Symbol(isTerminal,maxNumSymbols,maxNumSymbolEntries,numDiffusionParams,numFormalParams,symbolTable)
    {}

    virtual uint produce(const SymbolEntry* input,SymbolEntry* output,float& logProbability) const
    {
        const float* params=reinterpret_cast<const float*>(input+1);

        if(input[0].isInvalid()){ output[0].mID=101; memcpy(output+1,input,sizeof(SymbolEntry)*6); logProbability=0; return 7; }

        bool cond[14]={
            params[4]<0.3||params[0]==0.0,
            true,
            params[4]>=1.3&&params[0]>3.0,
            params[4]>=1.3&&params[0]>3.0,
            params[4]>=1.3&&params[0]>3.0,
            params[4]>=1.3&&params[0]>3.0,
            params[4]>=1.3&&params[0]>3.0,
            params[4]>=0.3&&params[0]>3.0,
            params[4]>=1.3&&params[0]>0.0&&params[0]<3.0+1.0,
            params[4]>=1.3&&params[0]>0.0&&params[0]<3.0+1.0,
            params[4]>=1.3&&params[0]>0.0&&params[0]<3.0+1.0,
            params[4]>=1.3&&params[0]>0.0&&params[0]<3.0+1.0,
            params[4]>=1.3&&params[0]>0.0&&params[0]<3.0+1.0,
            params[4]>=0.3&&params[0]>0.0&&params[0]<3.0+1.0
        };

        float prob[14]={
            cond[0]?Math::clampNonNegative(1.0):0,
            cond[1]?Math::clampNonNegative(0.001):0,
            cond[2]?Math::clampNonNegative((0.9+(1.0-0.9)*params[4]/(exp(log(2.0)*params[0])))*0.4):0,
            cond[3]?Math::clampNonNegative((0.9+(1.0-0.9)*params[4]/(exp(log(2.0)*params[0])))*0.3):0,
            cond[4]?Math::clampNonNegative((0.9+(1.0-0.9)*params[4]/(exp(log(2.0)*params[0])))*0.2):0,
            cond[5]?Math::clampNonNegative((0.9+(1.0-0.9)*params[4]/(exp(log(2.0)*params[0])))*0.1):0,
            cond[6]?Math::clampNonNegative((0.9+(1.0-0.9)*params[4]/(exp(log(2.0)*params[0])))*0.05):0,
            cond[7]?Math::clampNonNegative(1.0-(0.9+(1.0-0.9)*params[4]/(exp(log(2.0)*params[0])))):0,
            cond[8]?Math::clampNonNegative((0.9+(1.0-0.9)*params[4]/(exp(log(2.0)*params[0])))*0.4):0,
            cond[9]?Math::clampNonNegative((0.9+(1.0-0.9)*params[4]/(exp(log(2.0)*params[0])))*0.3):0,
            cond[10]?Math::clampNonNegative((0.9+(1.0-0.9)*params[4]/(exp(log(2.0)*params[0])))*0.2):0,
            cond[11]?Math::clampNonNegative((0.9+(1.0-0.9)*params[4]/(exp(log(2.0)*params[0])))*0.1):0,
            cond[12]?Math::clampNonNegative((0.9+(1.0-0.9)*params[4]/(exp(log(2.0)*params[0])))*0.05):0,
            cond[13]?Math::clampNonNegative(1.0-(0.9+(1.0-0.9)*params[4]/(exp(log(2.0)*params[0])))):0
        };

        float probSum=0;
        for(uint i=0;i<14;i++) probSum+=prob[i]; assert(probSum>0);
        for(uint i=0;i<14;i++) prob[i]/=probSum;

        uint selector=discreteSample(14,prob[0],prob[1],prob[2],prob[3],prob[4],prob[5],prob[6],prob[7],prob[8],prob[9],prob[10],prob[11],prob[12],prob[13]);

        switch(selector)
        {
            case 0:
                output[0].mID=103;
                output[2].mParam=params[1];
                output[3].mParam=4.0;
                output[1].mFlags=mSymbolTable[103]->setFlags(reinterpret_cast<float*>(output+2));
                output[4].mID=200;
                output[5].mParam=0.6;
                output[6].mParam=0.3;
                output[7].mParam=0.1;
                logProbability=log(prob[0]);
                return 8;
            break;
            case 1:
                output[0].mID=203;
                logProbability=log(prob[1]);
                return 1;
            break;
            case 2:
                output[0].mID=204;
                output[1].mParam=0.0;
                output[2].mParam=0.25;
                output[3].mParam=0.0;
                output[4].mID=304;
                output[5].mParam=Math::uniformZeroToOne();
                output[6].mParam=params[2];
                output[7].mID=201;
                output[8].mParam=0.005*exp(log(params[4])/1.7);
                output[9].mID=305;
                output[10].mParam=Math::uniformZeroToOne();
                output[11].mParam=params[0];
                output[12].mParam=params[1];
                output[13].mID=0;
                output[14].mID=306;
                output[15].mParam=Math::uniformZeroToOne();
                output[16].mParam=params[0];
                output[17].mParam=params[3];
                output[18].mID=101;
                output[20].mParam=params[0]-1.0;
                output[21].mParam=params[1]*0.89;
                output[22].mParam=params[2];
                output[23].mParam=params[3];
                output[24].mParam=(params[4]-1.0)*0.5;
                output[19].mFlags=mSymbolTable[101]->setFlags(reinterpret_cast<float*>(output+20));
                output[25].mID=1;
                output[26].mID=307;
                output[27].mParam=Math::uniformZeroToOne();
                output[28].mParam=params[0];
                output[29].mParam=params[3];
                output[30].mID=101;
                output[32].mParam=params[0]-1.0;
                output[33].mParam=params[1]*0.87;
                output[34].mParam=params[2];
                output[35].mParam=params[3];
                output[36].mParam=(params[4]-1.0)*0.5;
                output[31].mFlags=mSymbolTable[101]->setFlags(reinterpret_cast<float*>(output+32));
                logProbability=log(prob[2]);
                return 37;
            break;
            case 3:
                output[0].mID=204;
                output[1].mParam=0.0;
                output[2].mParam=0.25;
                output[3].mParam=0.0;
                output[4].mID=308;
                output[5].mParam=Math::uniformZeroToOne();
                output[6].mParam=params[2];
                output[7].mID=201;
                output[8].mParam=0.005*exp(log(params[4])/1.7);
                output[9].mID=309;
                output[10].mParam=Math::uniformZeroToOne();
                output[11].mParam=params[0];
                output[12].mParam=params[1];
                output[13].mID=0;
                output[14].mID=310;
                output[15].mParam=Math::uniformZeroToOne();
                output[16].mParam=params[0];
                output[17].mParam=params[3];
                output[18].mID=101;
                output[20].mParam=params[0]-1.0;
                output[21].mParam=params[1]*0.89;
                output[22].mParam=params[2];
                output[23].mParam=params[3];
                output[24].mParam=(params[4]-1.0)*0.6;
                output[19].mFlags=mSymbolTable[101]->setFlags(reinterpret_cast<float*>(output+20));
                output[25].mID=1;
                output[26].mID=311;
                output[27].mParam=Math::uniformZeroToOne();
                output[28].mParam=params[0];
                output[29].mParam=params[3];
                output[30].mID=101;
                output[32].mParam=params[0]-1.0;
                output[33].mParam=params[1]*0.87;
                output[34].mParam=params[2];
                output[35].mParam=params[3];
                output[36].mParam=(params[4]-1.0)*0.4;
                output[31].mFlags=mSymbolTable[101]->setFlags(reinterpret_cast<float*>(output+32));
                logProbability=log(prob[3]);
                return 37;
            break;
            case 4:
                output[0].mID=204;
                output[1].mParam=0.0;
                output[2].mParam=0.25;
                output[3].mParam=0.0;
                output[4].mID=312;
                output[5].mParam=Math::uniformZeroToOne();
                output[6].mParam=params[2];
                output[7].mID=201;
                output[8].mParam=0.005*exp(log(params[4])/1.7);
                output[9].mID=313;
                output[10].mParam=Math::uniformZeroToOne();
                output[11].mParam=params[0];
                output[12].mParam=params[1];
                output[13].mID=0;
                output[14].mID=314;
                output[15].mParam=Math::uniformZeroToOne();
                output[16].mParam=params[0];
                output[17].mParam=params[3];
                output[18].mID=101;
                output[20].mParam=params[0]-1.0;
                output[21].mParam=params[1]*0.89;
                output[22].mParam=params[2];
                output[23].mParam=params[3];
                output[24].mParam=(params[4]-1.0)*0.7;
                output[19].mFlags=mSymbolTable[101]->setFlags(reinterpret_cast<float*>(output+20));
                output[25].mID=1;
                output[26].mID=315;
                output[27].mParam=Math::uniformZeroToOne();
                output[28].mParam=params[0];
                output[29].mParam=params[3];
                output[30].mID=101;
                output[32].mParam=params[0]-1.0;
                output[33].mParam=params[1]*0.87;
                output[34].mParam=params[2];
                output[35].mParam=params[3];
                output[36].mParam=(params[4]-1.0)*0.3;
                output[31].mFlags=mSymbolTable[101]->setFlags(reinterpret_cast<float*>(output+32));
                logProbability=log(prob[4]);
                return 37;
            break;
            case 5:
                output[0].mID=204;
                output[1].mParam=0.0;
                output[2].mParam=0.25;
                output[3].mParam=0.0;
                output[4].mID=316;
                output[5].mParam=Math::uniformZeroToOne();
                output[6].mParam=params[2];
                output[7].mID=201;
                output[8].mParam=0.005*exp(log(params[4])/1.7);
                output[9].mID=317;
                output[10].mParam=Math::uniformZeroToOne();
                output[11].mParam=params[0];
                output[12].mParam=params[1];
                output[13].mID=0;
                output[14].mID=318;
                output[15].mParam=Math::uniformZeroToOne();
                output[16].mParam=params[0];
                output[17].mParam=params[3];
                output[18].mID=101;
                output[20].mParam=params[0]-1.0;
                output[21].mParam=params[1]*0.89;
                output[22].mParam=params[2];
                output[23].mParam=params[3];
                output[24].mParam=(params[4]-1.0)*0.8;
                output[19].mFlags=mSymbolTable[101]->setFlags(reinterpret_cast<float*>(output+20));
                output[25].mID=1;
                output[26].mID=319;
                output[27].mParam=Math::uniformZeroToOne();
                output[28].mParam=params[0];
                output[29].mParam=params[3];
                output[30].mID=101;
                output[32].mParam=params[0]-1.0;
                output[33].mParam=params[1]*0.87;
                output[34].mParam=params[2];
                output[35].mParam=params[3];
                output[36].mParam=(params[4]-1.0)*0.2;
                output[31].mFlags=mSymbolTable[101]->setFlags(reinterpret_cast<float*>(output+32));
                logProbability=log(prob[5]);
                return 37;
            break;
            case 6:
                output[0].mID=204;
                output[1].mParam=0.0;
                output[2].mParam=0.25;
                output[3].mParam=0.0;
                output[4].mID=320;
                output[5].mParam=Math::uniformZeroToOne();
                output[6].mParam=params[2];
                output[7].mID=201;
                output[8].mParam=0.005*exp(log(params[4])/1.7);
                output[9].mID=321;
                output[10].mParam=Math::uniformZeroToOne();
                output[11].mParam=params[0];
                output[12].mParam=params[1];
                output[13].mID=0;
                output[14].mID=322;
                output[15].mParam=Math::uniformZeroToOne();
                output[16].mParam=params[0];
                output[17].mParam=params[3];
                output[18].mID=101;
                output[20].mParam=params[0]-1.0;
                output[21].mParam=params[1]*0.89;
                output[22].mParam=params[2];
                output[23].mParam=params[3];
                output[24].mParam=(params[4]-1.0)*0.9;
                output[19].mFlags=mSymbolTable[101]->setFlags(reinterpret_cast<float*>(output+20));
                output[25].mID=1;
                output[26].mID=323;
                output[27].mParam=Math::uniformZeroToOne();
                output[28].mParam=params[0];
                output[29].mParam=params[3];
                output[30].mID=101;
                output[32].mParam=params[0]-1.0;
                output[33].mParam=params[1]*0.87;
                output[34].mParam=params[2];
                output[35].mParam=params[3];
                output[36].mParam=(params[4]-1.0)*0.1;
                output[31].mFlags=mSymbolTable[101]->setFlags(reinterpret_cast<float*>(output+32));
                logProbability=log(prob[6]);
                return 37;
            break;
            case 7:
                output[0].mID=204;
                output[1].mParam=0.0;
                output[2].mParam=0.5*0.25;
                output[3].mParam=0.0;
                output[4].mID=324;
                output[5].mParam=Math::uniformZeroToOne();
                output[6].mParam=params[2];
                output[7].mID=201;
                output[8].mParam=0.005*exp(log(params[4])/1.7);
                output[9].mID=325;
                output[10].mParam=Math::uniformZeroToOne();
                output[11].mParam=params[0];
                output[12].mParam=params[1];
                output[13].mID=326;
                output[14].mParam=Math::uniformZeroToOne();
                output[15].mParam=params[0];
                output[16].mID=101;
                output[18].mParam=params[0]-1.0;
                output[19].mParam=params[1]*0.87;
                output[20].mParam=params[2];
                output[21].mParam=params[3];
                output[22].mParam=params[4]-1.0;
                output[17].mFlags=mSymbolTable[101]->setFlags(reinterpret_cast<float*>(output+18));
                logProbability=log(prob[7]);
                return 23;
            break;
            case 8:
                output[0].mID=204;
                output[1].mParam=0.0;
                output[2].mParam=-0.03;
                output[3].mParam=0.0;
                output[4].mID=327;
                output[5].mParam=Math::uniformZeroToOne();
                output[6].mParam=params[2];
                output[7].mID=201;
                output[8].mParam=0.005*exp(log(params[4])/1.7);
                output[9].mID=328;
                output[10].mParam=Math::uniformZeroToOne();
                output[11].mParam=params[0];
                output[12].mParam=params[1];
                output[13].mID=0;
                output[14].mID=329;
                output[15].mParam=Math::uniformZeroToOne();
                output[16].mParam=params[0];
                output[17].mParam=params[3];
                output[18].mID=101;
                output[20].mParam=params[0]-1.0;
                output[21].mParam=params[1]*0.89;
                output[22].mParam=params[2];
                output[23].mParam=params[3];
                output[24].mParam=(params[4]-1.0)*0.5;
                output[19].mFlags=mSymbolTable[101]->setFlags(reinterpret_cast<float*>(output+20));
                output[25].mID=1;
                output[26].mID=330;
                output[27].mParam=Math::uniformZeroToOne();
                output[28].mParam=params[0];
                output[29].mParam=params[3];
                output[30].mID=101;
                output[32].mParam=params[0]-1.0;
                output[33].mParam=params[1]*0.87;
                output[34].mParam=params[2];
                output[35].mParam=params[3];
                output[36].mParam=(params[4]-1.0)*0.5;
                output[31].mFlags=mSymbolTable[101]->setFlags(reinterpret_cast<float*>(output+32));
                logProbability=log(prob[8]);
                return 37;
            break;
            case 9:
                output[0].mID=204;
                output[1].mParam=0.0;
                output[2].mParam=-0.03;
                output[3].mParam=0.0;
                output[4].mID=331;
                output[5].mParam=Math::uniformZeroToOne();
                output[6].mParam=params[2];
                output[7].mID=201;
                output[8].mParam=0.005*exp(log(params[4])/1.7);
                output[9].mID=332;
                output[10].mParam=Math::uniformZeroToOne();
                output[11].mParam=params[0];
                output[12].mParam=params[1];
                output[13].mID=0;
                output[14].mID=333;
                output[15].mParam=Math::uniformZeroToOne();
                output[16].mParam=params[0];
                output[17].mParam=params[3];
                output[18].mID=101;
                output[20].mParam=params[0]-1.0;
                output[21].mParam=params[1]*0.89;
                output[22].mParam=params[2];
                output[23].mParam=params[3];
                output[24].mParam=(params[4]-1.0)*0.6;
                output[19].mFlags=mSymbolTable[101]->setFlags(reinterpret_cast<float*>(output+20));
                output[25].mID=1;
                output[26].mID=334;
                output[27].mParam=Math::uniformZeroToOne();
                output[28].mParam=params[0];
                output[29].mParam=params[3];
                output[30].mID=101;
                output[32].mParam=params[0]-1.0;
                output[33].mParam=params[1]*0.87;
                output[34].mParam=params[2];
                output[35].mParam=params[3];
                output[36].mParam=(params[4]-1.0)*0.4;
                output[31].mFlags=mSymbolTable[101]->setFlags(reinterpret_cast<float*>(output+32));
                logProbability=log(prob[9]);
                return 37;
            break;
            case 10:
                output[0].mID=204;
                output[1].mParam=0.0;
                output[2].mParam=-0.03;
                output[3].mParam=0.0;
                output[4].mID=335;
                output[5].mParam=Math::uniformZeroToOne();
                output[6].mParam=params[2];
                output[7].mID=201;
                output[8].mParam=0.005*exp(log(params[4])/1.7);
                output[9].mID=336;
                output[10].mParam=Math::uniformZeroToOne();
                output[11].mParam=params[0];
                output[12].mParam=params[1];
                output[13].mID=0;
                output[14].mID=337;
                output[15].mParam=Math::uniformZeroToOne();
                output[16].mParam=params[0];
                output[17].mParam=params[3];
                output[18].mID=101;
                output[20].mParam=params[0]-1.0;
                output[21].mParam=params[1]*0.89;
                output[22].mParam=params[2];
                output[23].mParam=params[3];
                output[24].mParam=(params[4]-1.0)*0.7;
                output[19].mFlags=mSymbolTable[101]->setFlags(reinterpret_cast<float*>(output+20));
                output[25].mID=1;
                output[26].mID=338;
                output[27].mParam=Math::uniformZeroToOne();
                output[28].mParam=params[0];
                output[29].mParam=params[3];
                output[30].mID=101;
                output[32].mParam=params[0]-1.0;
                output[33].mParam=params[1]*0.87;
                output[34].mParam=params[2];
                output[35].mParam=params[3];
                output[36].mParam=(params[4]-1.0)*0.3;
                output[31].mFlags=mSymbolTable[101]->setFlags(reinterpret_cast<float*>(output+32));
                logProbability=log(prob[10]);
                return 37;
            break;
            case 11:
                output[0].mID=204;
                output[1].mParam=0.0;
                output[2].mParam=-0.03;
                output[3].mParam=0.0;
                output[4].mID=339;
                output[5].mParam=Math::uniformZeroToOne();
                output[6].mParam=params[2];
                output[7].mID=201;
                output[8].mParam=0.005*exp(log(params[4])/1.7);
                output[9].mID=340;
                output[10].mParam=Math::uniformZeroToOne();
                output[11].mParam=params[0];
                output[12].mParam=params[1];
                output[13].mID=0;
                output[14].mID=341;
                output[15].mParam=Math::uniformZeroToOne();
                output[16].mParam=params[0];
                output[17].mParam=params[3];
                output[18].mID=101;
                output[20].mParam=params[0]-1.0;
                output[21].mParam=params[1]*0.89;
                output[22].mParam=params[2];
                output[23].mParam=params[3];
                output[24].mParam=(params[4]-1.0)*0.8;
                output[19].mFlags=mSymbolTable[101]->setFlags(reinterpret_cast<float*>(output+20));
                output[25].mID=1;
                output[26].mID=342;
                output[27].mParam=Math::uniformZeroToOne();
                output[28].mParam=params[0];
                output[29].mParam=params[3];
                output[30].mID=101;
                output[32].mParam=params[0]-1.0;
                output[33].mParam=params[1]*0.87;
                output[34].mParam=params[2];
                output[35].mParam=params[3];
                output[36].mParam=(params[4]-1.0)*0.2;
                output[31].mFlags=mSymbolTable[101]->setFlags(reinterpret_cast<float*>(output+32));
                logProbability=log(prob[11]);
                return 37;
            break;
            case 12:
                output[0].mID=204;
                output[1].mParam=0.0;
                output[2].mParam=-0.03;
                output[3].mParam=0.0;
                output[4].mID=343;
                output[5].mParam=Math::uniformZeroToOne();
                output[6].mParam=params[2];
                output[7].mID=201;
                output[8].mParam=0.005*exp(log(params[4])/1.7);
                output[9].mID=344;
                output[10].mParam=Math::uniformZeroToOne();
                output[11].mParam=params[0];
                output[12].mParam=params[1];
                output[13].mID=0;
                output[14].mID=345;
                output[15].mParam=Math::uniformZeroToOne();
                output[16].mParam=params[0];
                output[17].mParam=params[3];
                output[18].mID=101;
                output[20].mParam=params[0]-1.0;
                output[21].mParam=params[1]*0.89;
                output[22].mParam=params[2];
                output[23].mParam=params[3];
                output[24].mParam=(params[4]-1.0)*0.9;
                output[19].mFlags=mSymbolTable[101]->setFlags(reinterpret_cast<float*>(output+20));
                output[25].mID=1;
                output[26].mID=346;
                output[27].mParam=Math::uniformZeroToOne();
                output[28].mParam=params[0];
                output[29].mParam=params[3];
                output[30].mID=101;
                output[32].mParam=params[0]-1.0;
                output[33].mParam=params[1]*0.87;
                output[34].mParam=params[2];
                output[35].mParam=params[3];
                output[36].mParam=(params[4]-1.0)*0.1;
                output[31].mFlags=mSymbolTable[101]->setFlags(reinterpret_cast<float*>(output+32));
                logProbability=log(prob[12]);
                return 37;
            break;
            case 13:
                output[0].mID=204;
                output[1].mParam=0.0;
                output[2].mParam=-0.03;
                output[3].mParam=0.0;
                output[4].mID=347;
                output[5].mParam=Math::uniformZeroToOne();
                output[6].mParam=params[2];
                output[7].mID=201;
                output[8].mParam=0.005*exp(log(params[4])/1.7);
                output[9].mID=348;
                output[10].mParam=Math::uniformZeroToOne();
                output[11].mParam=params[0];
                output[12].mParam=params[1];
                output[13].mID=349;
                output[14].mParam=Math::uniformZeroToOne();
                output[15].mParam=params[0];
                output[16].mID=101;
                output[18].mParam=params[0]-1.0;
                output[19].mParam=params[1]*0.87;
                output[20].mParam=params[2];
                output[21].mParam=params[3];
                output[22].mParam=params[4]-1.0;
                output[17].mFlags=mSymbolTable[101]->setFlags(reinterpret_cast<float*>(output+18));
                logProbability=log(prob[13]);
                return 23;
            break;
        }
        return -1;
    }

    virtual ushort setFlags(const float* params) const
    {
        uint conditionals=0;
        if(params[4]<0.3||params[0]==0.0) conditionals++;
        if(true) conditionals++;
        if(params[4]>=1.3&&params[0]>3.0) conditionals++;
        if(params[4]>=1.3&&params[0]>3.0) conditionals++;
        if(params[4]>=1.3&&params[0]>3.0) conditionals++;
        if(params[4]>=1.3&&params[0]>3.0) conditionals++;
        if(params[4]>=1.3&&params[0]>3.0) conditionals++;
        if(params[4]>=0.3&&params[0]>3.0) conditionals++;
        if(params[4]>=1.3&&params[0]>0.0&&params[0]<3.0+1.0) conditionals++;
        if(params[4]>=1.3&&params[0]>0.0&&params[0]<3.0+1.0) conditionals++;
        if(params[4]>=1.3&&params[0]>0.0&&params[0]<3.0+1.0) conditionals++;
        if(params[4]>=1.3&&params[0]>0.0&&params[0]<3.0+1.0) conditionals++;
        if(params[4]>=1.3&&params[0]>0.0&&params[0]<3.0+1.0) conditionals++;
        if(params[4]>=0.3&&params[0]>0.0&&params[0]<3.0+1.0) conditionals++;

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
    { return "A"; }

//#####################################################################
};

//#####################################################################
} // namespace ProceduralModeling

#endif //_A_SYMBOL_HPP_
