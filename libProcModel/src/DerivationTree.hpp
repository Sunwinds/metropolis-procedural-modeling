//#####################################################################
// Copyright 2009, Jerry Talton.
//#####################################################################
#ifndef __DERIVATION_TREE__
#define __DERIVATION_TREE__

#include "Common.hpp"
#include "Production.hpp"

namespace ProceduralModeling{

class Symbol;

//####################################################################
// Class DerivationTree
//####################################################################
class DerivationTree
{
protected:
    QList<ProductionList> mDerivation;
    QVector<uint> mJumpables;

public:
    DerivationTree()
    {}

    DerivationTree(const Production& axiom)
    { mDerivation.push_back(ProductionList()); mDerivation.last().push_back(axiom); }

    uint numJumpables() const
    { uint sum=0; foreach(uint i,mJumpables) sum+=i; return sum; }

    QVector<uint>& jumpablesInLevel()
    { return mJumpables; }

    const QVector<uint>& jumpablesInLevel() const
    { return mJumpables; }

    uint& jumpablesInLevel(uint level)
    { return mJumpables[level]; }

    uint jumpablesInLevel(uint level) const
    { return mJumpables[level]; }

    QList<ProductionList>& derivation()
    { return mDerivation; }

    const QList<ProductionList>& derivation() const
    { return mDerivation; }

    ProductionList& production()
    { return mDerivation.last(); }

    const ProductionList& production() const
    { return mDerivation.last(); }

    ProductionList& production(const uint level)
    { return mDerivation[level]; }

    const ProductionList& production(const uint level) const
    { return mDerivation.at(level); }

    uint treeDepth() const
    { return mDerivation.size(); }

    friend QDataStream& operator<<(QDataStream& out,const DerivationTree& tree)
    { return out<<tree.mDerivation<<tree.mJumpables; }

    friend QDataStream& operator>>(QDataStream& in,DerivationTree& tree)
    { return in>>tree.mDerivation>>tree.mJumpables; }

public:
    float logProbability(const bool includeAxiom=true) const;
    static float logProbability(const SubtreeIterators& left,const SubtreeIterators& right);
    static uint numJumpables(const SubtreeIterators& left,const SubtreeIterators& right,const bool includeLastLevel=true);
    bool levelIsDeterministic(const uint level,Symbol** const symbolTable) const;

//####################################################################
};

//#####################################################################
} // namespace ProceduralModeling

#endif //__DERIVATION_TREE__
