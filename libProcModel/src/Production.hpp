//#####################################################################
// Copyright 2009, Jerry Talton.
//#####################################################################
#ifndef __PRODUCTION__
#define __PRODUCTION__

#include "Common.hpp"

namespace ProceduralModeling{

//####################################################################
// Class Production
//####################################################################
class Production
{
    typedef std::list<Production> ProductionList;
    typedef std::list<Production>::iterator ProductionListIterator;
    typedef ProductionList::const_iterator ProductionListConstIterator;
    typedef QVector<ProductionListIterator> ChildrenIterators;

protected:
    float mLogProbability;
    uint mNumJumpables;
    float mSumProposalProbs;
    SymbolEntryList mSymbols;
    ChildrenIterators mChildren;

public:
    Production()
     :mLogProbability(0),mNumJumpables(0)
    {}

    Production(const int maxSymbols,const int maxSymbolListSize)
     :mLogProbability(0),mNumJumpables(0)
    { mChildren.resize(maxSymbols); mSymbols.resize(maxSymbolListSize); }

    ~Production()
    {}

    const float& logProbability() const
    { return mLogProbability; }

    float& logProbability()
    { return mLogProbability; }

    const SymbolEntryList& symbolList() const
    { return mSymbols; }

    SymbolEntryList& symbolList()
    { return mSymbols; }

    QVector<ProductionListIterator>& children()
    { return mChildren; }

    const QVector<ProductionListIterator>& children() const
    { return mChildren; }

    const ProductionListIterator& child(const uint i) const
    { return mChildren[i]; }

    ProductionListIterator& child(const uint i)
    { return mChildren[i]; }

    uint numSymbols() const
    { return mChildren.size(); }

    uint& numJumpables()
    { return mNumJumpables; }

    uint numJumpables() const
    { return mNumJumpables; }

    static uint numJumpablesInList(ProductionListConstIterator begin,ProductionListConstIterator end)
    { uint numJumpables=0; while(begin!=end){ numJumpables+=begin->numJumpables(); ++begin; } return numJumpables; }

    static uint numJumpablesInList(const ProductionList& list)
    { return numJumpablesInList(list.begin(),list.end()); }

    friend QDataStream& operator<<(QDataStream& out,const Production& production)
    { return out<<production.mLogProbability<<production.mNumJumpables<<production.mSymbols; }

    friend QDataStream& operator>>(QDataStream& in,Production& production)
    { return in>>production.mLogProbability>>production.mNumJumpables>>production.mSymbols; }

    friend QDataStream& operator<<(QDataStream& out,const ProductionList& list)
    { out<<static_cast<uint>(list.size()); for(ProductionListConstIterator it=list.begin();it!=list.end();it++) out<<*it; return out; }

    friend QDataStream& operator>>(QDataStream& in,ProductionList& list)
    { uint size; in>>size; list.clear(); for(uint i=0;i<size;i++){ Production curr; in>>curr; list.push_back(curr); } return in; }

//####################################################################
};

typedef std::list<Production> ProductionList;
typedef ProductionList::iterator ProductionListIterator;
typedef ProductionList::const_iterator ProductionListConstIterator;
typedef QVector<ProductionListIterator> SubtreeIterators;
typedef QVector<ProductionListIterator> ChildrenIterators;

//#####################################################################
} // namespace ProceduralModeling

#endif //__PRODUCTION__
