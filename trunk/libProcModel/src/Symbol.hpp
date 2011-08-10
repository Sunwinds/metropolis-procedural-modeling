//#####################################################################
// Copyright 2009, Jerry Talton.
// This software is governed by the license contained in LICENSE.
//#####################################################################
#ifndef _SYMBOL_HPP_
#define _SYMBOL_HPP_

#include "Common.hpp"
#include <ostream>
#include <stdarg.h>
#include <QtOpenGL>
#include "Math.hpp"
#include "ResourceHandler.hpp"

namespace ProceduralModeling{

using std::ostream;

//#####################################################################
// Class Symbol
//#####################################################################
class PROCEDURAL_EXPORT Symbol
{
protected:
    const bool mTerminal;
    const uint mMaxNumSymbols,mMaxNumSymbolEntries;
    const uint mNumDiffusionParams,mNumFormalParams;
    uint mShader,mMesh;
    QVector<uint> mTextures;
    ResourceHandler* mResourceHandler;
    Symbol** mSymbolTable;

public:
    Symbol(const bool terminal,const uint maxNumSymbols,const uint maxNumSymbolEntries,const uint numDiffusionParams,const uint numFormalParams,Symbol** symbolTable)
        :mTerminal(terminal),mMaxNumSymbols(maxNumSymbols),mMaxNumSymbolEntries(maxNumSymbolEntries),mNumDiffusionParams(numDiffusionParams),
        mNumFormalParams(numFormalParams),mShader(-1),mMesh(-1),mResourceHandler(NULL),mSymbolTable(symbolTable)
    {}

	virtual ~Symbol()
    {}

    bool isTerminal() const
    { return mTerminal; }

    bool isDeterministic(const SymbolEntry* listEntry) const
    { return mTerminal || listEntry[1].isInvalid(); }

    bool isJumpable(const SymbolEntry* listEntry) const
    { return !mTerminal && listEntry[1].isJumpable(); }

    uint maxNumSymbols() const
    { return mMaxNumSymbols; }

    uint maxNumSymbolEntries() const
    { return mMaxNumSymbolEntries; }

    uint numDiffusionParams() const
    { return mNumDiffusionParams; }

    uint numFormalParams() const
    { return mNumFormalParams; }

    uint numParams() const
    { return mNumDiffusionParams+mNumFormalParams; }

    uint paramOffset() const
    { return mTerminal?1:2; }

    uint numEntries() const
    { return 1+numParams()+(mTerminal?0:1); }

    void setResourceHandler(ResourceHandler* resourceHandler)
    { mResourceHandler = resourceHandler; }

	void appendTexture(const QString &fileName)
	{ int index=mResourceHandler->appendTexture(fileName); if(index>=-1) mTextures.append(index); }

	void initShader(const QString& vs,const QString& fs)
	{ mShader=mResourceHandler->initShader(vs,fs); }

	void loadModel(const QString& fileName, bool useTexture=false, bool tricky = false)
	{ mMesh=mResourceHandler->loadModel(fileName, useTexture, tricky);  }

    virtual float logDiffusionProbability(const float* params) const
    { return 0; }

    friend ostream& operator<<(ostream& os,const Symbol* m)
    { os<<m->symbolName(); return os; }

	virtual void initGraphics()
    {}

    virtual void saveToMesh(const float* params) const
    {}

    virtual void voxel(const float* params) const
    {}

protected:
    static ushort discreteSample(ushort numIndices, ...)
    { va_list vList; va_start(vList,numIndices);
    float accum=0,sample=Math::uniformZeroToOne();
    for(ushort i=0;i<numIndices;i++){ accum+=va_arg(vList,double); if(sample<=accum){ return i; va_end(vList); }}
    return numIndices-1; va_end(vList); }

public:
    virtual void render(const float* params) const=0;
    virtual uint produce(const SymbolEntry* input,SymbolEntry* output,float& probability) const=0;
    virtual const char* symbolName() const=0;
    virtual ushort setFlags(const float* params) const=0;

//#####################################################################
};

//#####################################################################
} // namespace ProceduralModeling

#endif //_SYMBOL_HPP_
