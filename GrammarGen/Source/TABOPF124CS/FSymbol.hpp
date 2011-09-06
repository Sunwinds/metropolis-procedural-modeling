//#####################################################################
// Copyright 2009, Jerry Talton.
// This software is governed by the license contained in LICENSE.
//#####################################################################
#ifndef _F_SYMBOL_HPP_
#define _F_SYMBOL_HPP_

#include "Arrays.hpp"
#include "Distributions.hpp"
#include "Symbol.hpp"

namespace ProceduralModeling{

//#####################################################################
// Class FSymbol
//#####################################################################
class FSymbol : public Symbol
{
public:
    FSymbol(Symbol** symbolTable)
     :Symbol(false,11,23,0,1,symbolTable)
    {}

    FSymbol(const bool isTerminal,const uint maxNumSymbols,const uint maxNumSymbolEntries,const uint numDiffusionParams,const uint numFormalParams,Symbol** symbolTable)
     :Symbol(isTerminal,maxNumSymbols,maxNumSymbolEntries,numDiffusionParams,numFormalParams,symbolTable)
    {}

    virtual uint produce(const SymbolEntry* input,SymbolEntry* output,float& logProbability) const
    {
        const float* params=reinterpret_cast<const float*>(input+1);

        float prob[2]={
            Math::clampNonNegative(1.0-params[0]/10.0),
            Math::clampNonNegative(params[0]/10.0)
        };

        float probSum=0;
        for(uint i=0;i<2;i++) probSum+=prob[i]; assert(probSum>0);
        for(uint i=0;i<2;i++) prob[i]/=probSum;

        uint selector=discreteSample(2,prob[0],prob[1]);

        switch(selector)
        {
            case 0:
                output[0].mID=100;
                output[2].mParam=params[0]+1.0;
                output[1].mFlags=mSymbolTable[100]->setFlags(reinterpret_cast<float*>(output+2));
                output[3].mID=0;
                output[4].mID=3;
                output[5].mParam=22.5;
                output[6].mID=100;
                output[8].mParam=params[0]+1.0;
                output[7].mFlags=mSymbolTable[100]->setFlags(reinterpret_cast<float*>(output+8));
                output[9].mID=1;
                output[10].mID=100;
                output[12].mParam=params[0]+1.0;
                output[11].mFlags=mSymbolTable[100]->setFlags(reinterpret_cast<float*>(output+12));
                output[13].mID=0;
                output[14].mID=2;
                output[15].mParam=22.5;
                output[16].mID=100;
                output[18].mParam=params[0]+1.0;
                output[17].mFlags=mSymbolTable[100]->setFlags(reinterpret_cast<float*>(output+18));
                output[19].mID=1;
                output[20].mID=100;
                output[22].mParam=params[0]+1.0;
                output[21].mFlags=mSymbolTable[100]->setFlags(reinterpret_cast<float*>(output+22));
                logProbability=log(prob[0]);
                return 23;
            break;
            case 1:
                output[0].mID=200;
                logProbability=log(prob[1]);
                return 1;
            break;
        }
        return -1;
    }

    virtual ushort setFlags(const float* params) const
    {
        return SymbolEntry::CLEAR;
    }

    virtual void render(const float* params) const
    {
	glBegin(GL_LINES); glVertex3f(0,0,0); glVertex3f(0,1,0); glEnd(); glTranslatef(0,1,0);
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
    { return "F"; }

//#####################################################################
};

//#####################################################################
} // namespace ProceduralModeling

#endif //_F_SYMBOL_HPP_
