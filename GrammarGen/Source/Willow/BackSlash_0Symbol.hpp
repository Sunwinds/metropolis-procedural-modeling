//#####################################################################
// Copyright 2009, Jerry Talton.
// This software is governed by the license contained in LICENSE.
//#####################################################################
#ifndef _BACKSLASH_0_SYMBOL_HPP_
#define _BACKSLASH_0_SYMBOL_HPP_

#include "Distributions.hpp"
#include "BackSlashSymbol.hpp"

namespace ProceduralModeling{

//#####################################################################
// Class BackSlash_0Symbol
//#####################################################################
class BackSlash_0Symbol : public BackSlashSymbol
{
public:
    BackSlash_0Symbol(Symbol** symbolTable)
     :BackSlashSymbol(true,1,3,2,0,symbolTable)
    {}

    virtual void render(const float* params) const
    {
        float newParams[1]={
            137.5+Distributions::NormalSample(0.0,30.0,params[0],params[1])};
        BackSlashSymbol::render(newParams);
    }

    virtual uint produce(const SymbolEntry* input,SymbolEntry* output,float& logProbability) const
    {
        output[0].mID=301;
        memcpy(output+1,input,sizeof(SymbolEntry)*2);
        logProbability=0;
        return 3;
    }

    virtual ushort setFlags(const float* params) const
    {
        return SymbolEntry::DETERMINISTIC;
    }

    virtual float logDiffusionProbability(const float* params) const
    {
        return log(Distributions::NormalEval(0.0,30.0,params[0],params[1]));
    }

    virtual void initGraphics()
    {
        BackSlashSymbol::initGraphics();
    }

    virtual void saveToMesh(const float* params) const
    {
        float newParams[1]={
            137.5+Distributions::NormalSample(0.0,30.0,params[0],params[1])};
        BackSlashSymbol::saveToMesh(newParams);
    }

    virtual void voxel(const float* params) const
    {
        float newParams[1]={
            137.5+Distributions::NormalSample(0.0,30.0,params[0],params[1])};
        BackSlashSymbol::voxel(newParams);
    }

    virtual const char* symbolName() const
    { return "BackSlash_0"; }

//#####################################################################
};

//#####################################################################
} // namespace ProceduralModeling

#endif //_BACKSLASH_0_SYMBOL_HPP_
