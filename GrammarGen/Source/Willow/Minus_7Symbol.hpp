//#####################################################################
// Copyright 2009, Jerry Talton.
// This software is governed by the license contained in LICENSE.
//#####################################################################
#ifndef _MINUS_7_SYMBOL_HPP_
#define _MINUS_7_SYMBOL_HPP_

#include "Distributions.hpp"
#include "MinusSymbol.hpp"

namespace ProceduralModeling{

//#####################################################################
// Class Minus_7Symbol
//#####################################################################
class Minus_7Symbol : public MinusSymbol
{
public:
    Minus_7Symbol(Symbol** symbolTable)
     :MinusSymbol(true,1,4,1,2,symbolTable)
    {}

    virtual void render(const float* params) const
    {
        float newParams[1]={
            30.0+params[1]/12.0*Distributions::UniformSample(5.0,55.0,params[0])*params[2]};
        MinusSymbol::render(newParams);
    }

    virtual uint produce(const SymbolEntry* input,SymbolEntry* output,float& logProbability) const
    {
        output[0].mID=333;
        memcpy(output+1,input,sizeof(SymbolEntry)*3);
        logProbability=0;
        return 4;
    }

    virtual ushort setFlags(const float* params) const
    {
        return SymbolEntry::DETERMINISTIC;
    }

    virtual float logDiffusionProbability(const float* params) const
    {
        return log(Distributions::UniformEval(5.0,55.0,params[0]));
    }

    virtual void initGraphics()
    {
        MinusSymbol::initGraphics();
    }

    virtual void saveToMesh(const float* params) const
    {
        float newParams[1]={
            30.0+params[1]/12.0*Distributions::UniformSample(5.0,55.0,params[0])*params[2]};
        MinusSymbol::saveToMesh(newParams);
    }

    virtual void voxel(const float* params) const
    {
        float newParams[1]={
            30.0+params[1]/12.0*Distributions::UniformSample(5.0,55.0,params[0])*params[2]};
        MinusSymbol::voxel(newParams);
    }

    virtual const char* symbolName() const
    { return "Minus_7"; }

//#####################################################################
};

//#####################################################################
} // namespace ProceduralModeling

#endif //_MINUS_7_SYMBOL_HPP_
