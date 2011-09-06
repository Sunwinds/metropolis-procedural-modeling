//#####################################################################
// Copyright 2009, Jerry Talton.
// This software is governed by the license contained in LICENSE.
//#####################################################################
#ifndef _BACKSLASH_17_SYMBOL_HPP_
#define _BACKSLASH_17_SYMBOL_HPP_

#include "Distributions.hpp"
#include "BackSlashSymbol.hpp"

namespace ProceduralModeling{

//#####################################################################
// Class BackSlash_17Symbol
//#####################################################################
class BackSlash_17Symbol : public BackSlashSymbol
{
public:
    BackSlash_17Symbol(Symbol** symbolTable)
     :BackSlashSymbol(true,1,2,1,0,symbolTable)
    {}

    virtual void render(const float* params) const
    {
        float newParams[1]={
            137.5+Distributions::UniformSample(-45.0,45.0,params[0])};
        BackSlashSymbol::render(newParams);
    }

    virtual uint produce(const SymbolEntry* input,SymbolEntry* output,float& logProbability) const
    {
        output[0].mID=356;
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
        return log(Distributions::UniformEval(-45.0,45.0,params[0]));
    }

    virtual void initGraphics()
    {
        BackSlashSymbol::initGraphics();
    }

    virtual void saveToMesh(const float* params) const
    {
        float newParams[1]={
            137.5+Distributions::UniformSample(-45.0,45.0,params[0])};
        BackSlashSymbol::saveToMesh(newParams);
    }

    virtual void voxel(const float* params) const
    {
        float newParams[1]={
            137.5+Distributions::UniformSample(-45.0,45.0,params[0])};
        BackSlashSymbol::voxel(newParams);
    }

    virtual const char* symbolName() const
    { return "BackSlash_17"; }

//#####################################################################
};

//#####################################################################
} // namespace ProceduralModeling

#endif //_BACKSLASH_17_SYMBOL_HPP_
