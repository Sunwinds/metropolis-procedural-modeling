//#####################################################################
// Copyright 2010, Jerry Talton.
//#####################################################################
#ifndef __STRING_ITERATOR__
#define __STRING_ITERATOR__

#include "Production.hpp"

namespace ProceduralModeling{

//####################################################################
// Class SymbolStringIterator
//####################################################################
class SymbolStringIterator
{

private:
    ProductionListIterator mListIterator;
    uint mListIndex;
    Symbol** const mSymbolTable;

public:
    SymbolStringIterator(Symbol** const symbolTable)
     :mListIndex(0),mSymbolTable(symbolTable)
    {}

    SymbolStringIterator(const SymbolStringIterator& other)
     :mListIterator(other.mListIterator),mListIndex(other.mListIndex),mSymbolTable(other.mSymbolTable)
    {}

    SymbolStringIterator(const ProductionListIterator& it,Symbol** const symbolTable)
     :mListIterator(it),mListIndex(0),mSymbolTable(symbolTable)
    {}

    SymbolEntry* symbolEntry() const
    { return mListIterator->symbolList().data()+mListIndex; }

    Symbol* symbol() const
    { return mSymbolTable[mListIterator->symbolList()[mListIndex].mID]; }

    float* params() const
    { return reinterpret_cast<float*>(mListIterator->symbolList().data()+mListIndex+symbol()->paramOffset()); }

    Production& production() const
    { return *mListIterator; }

    bool isTerminal() const
    { return mSymbolTable[mListIterator->symbolList()[mListIndex].mID]->isTerminal(); }

    bool isDeterministic() const
    { return mSymbolTable[mListIterator->symbolList()[mListIndex].mID]->isDeterministic(mListIterator->symbolList().data()+mListIndex); }

    bool isJumpable() const
    { return mSymbolTable[mListIterator->symbolList()[mListIndex].mID]->isJumpable(mListIterator->symbolList().data()+mListIndex); }

    SymbolStringIterator& operator++()
    { int newIndex=mListIndex+symbol()->numEntries();
    if(newIndex<mListIterator->symbolList().size()) mListIndex=newIndex; else{++mListIterator; mListIndex=0;} return *this; }

    bool operator==(const SymbolStringIterator& other) const
    { return mListIterator==other.mListIterator && mListIndex==other.mListIndex; }

    bool operator!=(const SymbolStringIterator& other) const
    { return mListIterator!=other.mListIterator || mListIndex!=other.mListIndex; }

    bool operator!=(const ProductionListConstIterator& other) const
    { return mListIterator!=other; }

//#####################################################################
};

//####################################################################
// Class SymbolStringConstIterator
//####################################################################
class SymbolStringConstIterator
{

private:
    ProductionListConstIterator mListIterator;
    uint mListIndex;
    Symbol** const mSymbolTable;

public:
    SymbolStringConstIterator(Symbol** const symbolTable)
     :mListIndex(0),mSymbolTable(symbolTable)
    {}

    SymbolStringConstIterator(const SymbolStringConstIterator& other)
     :mListIterator(other.mListIterator),mListIndex(other.mListIndex),mSymbolTable(other.mSymbolTable)
    {}

    SymbolStringConstIterator(const ProductionListConstIterator& it,Symbol** const symbolTable)
     :mListIterator(it),mListIndex(0),mSymbolTable(symbolTable)
    {}

    const SymbolEntry* symbolEntry() const
    { return mListIterator->symbolList().data()+mListIndex; }

    const Symbol* symbol() const
    { return mSymbolTable[mListIterator->symbolList()[mListIndex].mID]; }

    const float* params() const
    { return reinterpret_cast<const float*>(mListIterator->symbolList().data()+mListIndex+symbol()->paramOffset()); }

    const Production& production() const
    { return *mListIterator; }

    bool isTerminal() const
    { return mSymbolTable[mListIterator->symbolList()[mListIndex].mID]->isTerminal(); }

    bool isDeterministic() const
    { return mSymbolTable[mListIterator->symbolList()[mListIndex].mID]->isDeterministic(mListIterator->symbolList().data()+mListIndex); }

    bool isJumpable() const
    { return mSymbolTable[mListIterator->symbolList()[mListIndex].mID]->isJumpable(mListIterator->symbolList().data()+mListIndex); }

    SymbolStringConstIterator& operator++()
    { int newIndex=mListIndex+symbol()->numEntries();
    if(newIndex<mListIterator->symbolList().size()) mListIndex=newIndex; else{++mListIterator; mListIndex=0;} return *this; }

    bool operator==(const SymbolStringConstIterator& other) const
    { return mListIterator==other.mListIterator && mListIndex==other.mListIndex; }

    bool operator!=(const SymbolStringConstIterator& other) const
    { return mListIterator!=other.mListIterator || mListIndex!=other.mListIndex; }

    bool operator!=(const ProductionListConstIterator& other) const
    { return mListIterator!=other; }

//#####################################################################
};

//#####################################################################
} // namespace ProceduralModeling

#endif //__STRING_ITERATOR__
