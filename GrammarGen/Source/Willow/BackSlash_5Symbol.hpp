//#####################################################################
// Copyright 2009, Jerry Talton.
// This software is governed by the license contained in LICENSE.
//#####################################################################
#ifndef _BACKSLASH_5_SYMBOL_HPP_
#define _BACKSLASH_5_SYMBOL_HPP_

#include "Distributions.hpp"
#include "BackSlashSymbol.hpp"

namespace ProceduralModeling{

//#####################################################################
// Class BackSlash_5Symbol
//#####################################################################
class BackSlash_5Symbol : public BackSlashSymbol
{
public:
    BackSlash_5Symbol(Symbol** symbolTable)
     :BackSlashSymbol(true,1,3,1,1,symbolTable)
    {}

    virtual void render(const float* params) const
    {
        float newParams[1]={
            params[1]+Distributions::UniformSample(-45.0,45.0,params[0])};
        BackSlashSymbol::render(newParams);
    }

    virtual uint produce(const SymbolEntry* input,SymbolEntry* output,float& logProbability) const
    {
        output[0].mID=320;
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
        return log(Distributions::UniformEval(-45.0,45.0,params[0]));
    }

    virtual void initGraphics()
    {
        BackSlashSymbol::initGraphics();
    }

    virtual void saveToMesh(const float* params) const
    {
        float newParams[1]={
            params[1]+Distributions::UniformSample(-45.0,45.0,params[0])};
        BackSlashSymbol::saveToMesh(newParams);
    }

    virtual void voxel(const float* params) const
    {
        float newParams[1]={
            params[1]+Distributions::UniformSample(-45.0,45.0,params[0])};
        BackSlashSymbol::voxel(newParams);
    }

    virtual const char* symbolName() const
    { return "BackSlash_5"; }

//#####################################################################
};

//#####################################################################
} // namespace ProceduralModeling

#endif //_BACKSLASH_5_SYMBOL_HPP_
