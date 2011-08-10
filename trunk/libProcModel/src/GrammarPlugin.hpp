//#####################################################################
// Copyright 2009, Jerry Talton.
//#####################################################################
#ifndef __GRAMMAR_PLUGIN__
#define __GRAMMAR_PLUGIN__

#include "Common.hpp"
#include "ResourceHandler.hpp"
#include "DerivationTree.hpp"
#include <QGLWidget>
#include <QString>

#if PROCEDURAL_PLATFORM == PLATFORM_WINDOWS
# define WIN32_LEAN_AND_MEAN
# include <windows.h>
struct HINSTANCE__;
# define DL_HANDLE struct HINSTANCE__*
#elif PROCEDURAL_PLATFORM == PLATFORM_MAC || PROCEDURAL_PLATFORM == PLATFORM_LINUX
# include <dlfcn.h>
# define DL_HANDLE void*
#endif

namespace ProceduralModeling{

class Symbol;

//####################################################################
// Class GrammarPlugin
//####################################################################
class PROCEDURAL_EXPORT GrammarPlugin
{
    typedef void(*AxiomFunction)(SymbolEntry*,Symbol**);
    typedef uint(*AxiomNumSymbolsFunction)();
    typedef uint(*AxiomNumSymbolEntriesFunction)();
    typedef Symbol**(*CreateSymbolTableFunction)();
    typedef void(*DestroySymbolTableFunction)(Symbol*** table);
    typedef uint(*NumSymbolsInTableFunction)();
    typedef uint(*DerivationDepthLimitFunction)();
	typedef void(*SetupGraphicsFunction)(Symbol** table,ResourceHandler* resourceHandler);
    typedef void(*InitOutputMesh)(ResourceHandler* resourceHandler);
    typedef void(*SaveOutputMesh)(ResourceHandler* resourceHandler, const QString& name);

protected:
    DL_HANDLE mHandle;
    AxiomFunction mAxiom;
    AxiomNumSymbolsFunction mAxiomNumSymbols;
    AxiomNumSymbolEntriesFunction mAxiomNumSymbolEntries;
    CreateSymbolTableFunction mCreateSymbolTable;
    DestroySymbolTableFunction mDestroySymbolTable;
    NumSymbolsInTableFunction mNumSymbols;
    DerivationDepthLimitFunction mDerivationDepthLimit;
	SetupGraphicsFunction mSetupGraphics;
    InitOutputMesh mInitOutputMesh;
    SaveOutputMesh mSaveOutputMesh;
    Symbol** mSymbolTable;
    bool mLoaded;

public:
    GrammarPlugin()
     :mHandle(NULL),mAxiom(NULL),mAxiomNumSymbols(NULL),mAxiomNumSymbolEntries(NULL),mCreateSymbolTable(NULL),mDestroySymbolTable(NULL),
      mNumSymbols(NULL),mDerivationDepthLimit(NULL),mSetupGraphics(NULL),mInitOutputMesh(NULL),mSaveOutputMesh(NULL),mSymbolTable(NULL),
      mLoaded(false)
    {}

    bool isLoaded() const
    { return mLoaded; }

public:
    bool load(const QString& path);
    uint import(DerivationTree* derivationTree);
    void setup(Symbol**& symbolTable,ResourceHandler* resourceHandler);
    void destroy();
    void initOutputMesh(ResourceHandler* resourceHandler);
    void saveOutputMesh(ResourceHandler* resourceHandler,const QString& name);

protected:
    static void* symbol(DL_HANDLE handle,const QString& name);

//####################################################################
};

//#####################################################################
} // namespace ProceduralModeling

#endif //__GRAMMAR_PLUGIN__
