//#####################################################################
// Copyright 2009, Jerry Talton.
// This software is governed by the license contained in LICENSE.
//#####################################################################
#ifndef _PLUS_13_SYMBOL_HPP_
#define _PLUS_13_SYMBOL_HPP_

#include "Distributions.hpp"
#include "PlusSymbol.hpp"

namespace ProceduralModeling{

//#####################################################################
// Class Plus_13Symbol
//#####################################################################
class Plus_13Symbol : public PlusSymbol
{
public:
    Plus_13Symbol(Symbol** symbolTable)
     :PlusSymbol(true,1,2,1,0,symbolTable)
    {}

    virtual void render(const float* params) const
    {
        float newParams[1]={
            Distributions::UniformSample(-60.0,60.0,params[0])};
        PlusSymbol::render(newParams);
    }

    virtual uint produce(const SymbolEntry* input,SymbolEntry* output,float& logProbability) const
    {
        output[0].mID=351;
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
        return log(Distributions::UniformEval(-60.0,60.0,params[0]));
    }

    virtual void initGraphics()
    {
        PlusSymbol::initGraphics();
    }

    virtual void saveToMesh(const float* params) const
    {
        float newParams[1]={
            Distributions::UniformSample(-60.0,60.0,params[0])};
        PlusSymbol::saveToMesh(newParams);
    }

    virtual void voxel(const float* params) const
    {
        float newParams[1]={
            Distributions::UniformSample(-60.0,60.0,params[0])};
        PlusSymbol::voxel(newParams);
    }

    virtual const char* symbolName() const
    { return "Plus_13"; }

//#####################################################################
};

//#####################################################################
} // namespace ProceduralModeling

#endif //_PLUS_13_SYMBOL_HPP_
