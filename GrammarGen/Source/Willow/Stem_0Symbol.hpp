//#####################################################################
// Copyright 2009, Jerry Talton.
// This software is governed by the license contained in LICENSE.
//#####################################################################
#ifndef _STEM_0_SYMBOL_HPP_
#define _STEM_0_SYMBOL_HPP_

#include "Distributions.hpp"
#include "StemSymbol.hpp"

namespace ProceduralModeling{

//#####################################################################
// Class Stem_0Symbol
//#####################################################################
class Stem_0Symbol : public StemSymbol
{
public:
    Stem_0Symbol(Symbol** symbolTable)
     :StemSymbol(true,1,2,1,0,symbolTable)
    {}

    virtual void render(const float* params) const
    {
        float newParams[4]={
            Distributions::UniformSample(1.0*0.6,1.0,params[0]),
            0.0,
            12.0,
            1.0};
        StemSymbol::render(newParams);
    }

    virtual uint produce(const SymbolEntry* input,SymbolEntry* output,float& logProbability) const
    {
        output[0].mID=300;
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
        return log(Distributions::UniformEval(1.0*0.6,1.0,params[0]));
    }

    virtual void initGraphics()
    {
        StemSymbol::initGraphics();
    }

    virtual void saveToMesh(const float* params) const
    {
        float newParams[4]={
            Distributions::UniformSample(1.0*0.6,1.0,params[0]),
            0.0,
            12.0,
            1.0};
        StemSymbol::saveToMesh(newParams);
    }

    virtual void voxel(const float* params) const
    {
        float newParams[4]={
            Distributions::UniformSample(1.0*0.6,1.0,params[0]),
            0.0,
            12.0,
            1.0};
        StemSymbol::voxel(newParams);
    }

    virtual const char* symbolName() const
    { return "Stem_0"; }

//#####################################################################
};

//#####################################################################
} // namespace ProceduralModeling

#endif //_STEM_0_SYMBOL_HPP_
