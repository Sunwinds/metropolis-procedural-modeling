//#####################################################################
// Copyright 2009, Jerry Talton.
//#####################################################################
#include "GrammarPlugin.hpp"
using namespace ProceduralModeling;
//####################################################################
// Function load
//####################################################################
bool GrammarPlugin::load(const QString& path)
{
#if PROCEDURAL_PLATFORM == PLATFORM_WINDOWS
    mHandle=LoadLibrary(path.toStdWString().c_str());
#elif PROCEDURAL_PLATFORM == PLATFORM_MAC || PROCEDURAL_PLATFORM == PLATFORM_LINUX
    mHandle=dlopen(path.toStdString().c_str(), RTLD_LAZY | RTLD_GLOBAL);
#endif
    if(!mHandle) return false;

    mAxiom=(AxiomFunction)symbol(mHandle,"axiom");
    mAxiomNumSymbols=(AxiomNumSymbolsFunction)symbol(mHandle,"axiomNumSymbols");
    mAxiomNumSymbolEntries=(AxiomNumSymbolEntriesFunction)symbol(mHandle,"axiomNumSymbolEntries");
    mCreateSymbolTable=(CreateSymbolTableFunction)symbol(mHandle,"createSymbolTable");
    mDestroySymbolTable=(DestroySymbolTableFunction)symbol(mHandle,"destroySymbolTable");
    mNumSymbols=(NumSymbolsInTableFunction)symbol(mHandle,"numSymbolsInTable");
    mDerivationDepthLimit=(DerivationDepthLimitFunction)symbol(mHandle,"derivationDepthLimit");
	mSetupGraphics=(SetupGraphicsFunction)symbol(mHandle,"setupGraphics");
    mInitOutputMesh=(InitOutputMesh)symbol(mHandle,"initOutputMesh");
    mSaveOutputMesh=(SaveOutputMesh)symbol(mHandle,"saveOutputMesh");
    mLoaded=true;

    return mAxiom && mAxiomNumSymbols && mAxiomNumSymbolEntries && mCreateSymbolTable && mDestroySymbolTable && mNumSymbols
                && mDerivationDepthLimit && mSetupGraphics && mInitOutputMesh && mSaveOutputMesh;
}
//####################################################################
// Function destroy
//####################################################################
void GrammarPlugin::destroy()
{
    if(mSymbolTable) mDestroySymbolTable(&mSymbolTable);
}
//####################################################################
// Function symbol
//####################################################################
void* GrammarPlugin::symbol(DL_HANDLE handle,const QString& name)
{
#if PROCEDURAL_PLATFORM == PLATFORM_WINDOWS
    return (void*)GetProcAddress(handle, name.toStdString().c_str());
#elif PROCEDURAL_PLATFORM == PLATFORM_MAC || PROCEDURAL_PLATFORM == PLATFORM_LINUX
    return (void*)dlsym(handle, name.toStdString().c_str());
#endif
}
//####################################################################
// Function setup
//####################################################################
void GrammarPlugin::setup(Symbol**& symbolTable,ResourceHandler* resourceHandler)
{
    symbolTable=mSymbolTable=mCreateSymbolTable();
	mSetupGraphics(mSymbolTable,resourceHandler);
}
//####################################################################
// Function import
//####################################################################
uint GrammarPlugin::import(DerivationTree* derivationTree)
{
    Production axiom(mAxiomNumSymbols(),mAxiomNumSymbolEntries());
    *derivationTree=DerivationTree(axiom);
    mAxiom(derivationTree->production().front().symbolList().data(),mSymbolTable);
    return mDerivationDepthLimit();
}
//####################################################################
// Function initOutputMesh
//####################################################################
void GrammarPlugin::initOutputMesh(ResourceHandler* resourceHandler)
{
    mInitOutputMesh(resourceHandler);
}
//####################################################################
// Function saveToOutputMesh
//####################################################################
void GrammarPlugin::saveOutputMesh(ResourceHandler* resourceHandler,const QString& name)
{
    mSaveOutputMesh(resourceHandler,name);
}
//####################################################################
