//#####################################################################
// Copyright 2009, Jerry Talton.
// This software is governed by the license contained in LICENSE.
//#####################################################################
#ifndef _MINUS_0_SYMBOL_HPP_
#define _MINUS_0_SYMBOL_HPP_

#include "Distributions.hpp"
#include "MinusSymbol.hpp"

namespace ProceduralModeling{

//#####################################################################
// Class Minus_0Symbol
//#####################################################################
class Minus_0Symbol : public MinusSymbol
{
public:
    Minus_0Symbol(Symbol** symbolTable)
     :MinusSymbol(true,1,2,1,0,symbolTable)
    {}

    virtual void render(const float* params) const
    {
        float newParams[1]={
            Distributions::UniformSample(60.0,90.0,params[0])};
        MinusSymbol::render(newParams);
    }

    virtual uint produce(const SymbolEntry* input,SymbolEntry* output,float& logProbability) const
    {
        output[0].mID=302;
        memcpy(output+1,input,sizeof(SymbolEntry)*1);
        logProbability=0;
        return 2;
    }

    virtual ushort setFlags(const float* params) const
    {
        return SymbolEntry::DETERMINISTIC;
    }

    virtual float logDiffusionProbability(const float* params) const
    {
        return log(Distributions::UniformEval(60.0,90.0,params[0]));
    }

    virtual void initGraphics()
    {
        MinusSymbol::initGraphics();
    }

    virtual void saveToMesh(const float* params) const
    {
        float newParams[1]={
            Distributions::UniformSample(60.0,90.0,params[0])};
        MinusSymbol::saveToMesh(newParams);
    }

    virtual void voxel(const float* params) const
    {
        float newParams[1]={
            Distributions::UniformSample(60.0,90.0,params[0])};
        MinusSymbol::voxel(newParams);
    }

    virtual const char* symbolName() const
    { return "Minus_0"; }

//#####################################################################
};

//#####################################################################
} // namespace ProceduralModeling

#endif //_MINUS_0_SYMBOL_HPP_
