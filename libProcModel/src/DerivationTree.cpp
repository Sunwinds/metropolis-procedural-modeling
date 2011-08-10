//#####################################################################
// Copyright 2009, Jerry Talton.
//#####################################################################
#include "DerivationTree.hpp"
#include "Math.hpp"
#include "Symbol.hpp"
#include "SymbolStringIterator.hpp"
using namespace ProceduralModeling;
//####################################################################
// Function treeStats
//####################################################################
float DerivationTree::logProbability(const bool includeAxiom) const
{
    float logProb=0;
    for(uint i=includeAxiom?0:1;i<(uint)mDerivation.size();i++)
    {
        ProductionListConstIterator it=mDerivation.at(i).begin();
        ProductionListConstIterator end=mDerivation.at(i).end();
        while(it!=end){ float p=it->logProbability(); assert(!Math::isNan(p)); logProb+=p; assert(!Math::isNan(logProb)); ++it;}
    }
    return logProb;
}
//####################################################################
// Function logProbability
//####################################################################
float DerivationTree::logProbability(const SubtreeIterators& left,const SubtreeIterators& right)
{
    float logProb=0;
    for(uint i=0;i<(uint)left.size();i++)
    {
        ProductionListConstIterator it=left[i];
        ProductionListConstIterator end=right[i];
        while(it!=end){ float p=it->logProbability(); assert(!Math::isNan(p)); logProb+=p; assert(!Math::isNan(logProb)); ++it; }
    }
    return logProb;
}
//####################################################################
// Function numJumpableSymbols
//####################################################################
uint DerivationTree::numJumpables(const SubtreeIterators& left,const SubtreeIterators& right,const bool includeLastLevel)
{
    uint numJumpables=0,length=includeLastLevel?left.size():(left.size()-1);
    for(uint i=0;i<length;i++)
    {
        ProductionListConstIterator it=left[i];
        ProductionListConstIterator end=right[i];
        while(it!=end){ numJumpables+=it->numJumpables(); ++it; }
    }
    return numJumpables;
}
//####################################################################
bool DerivationTree::levelIsDeterministic(const uint level,Symbol** const symbolTable) const
{
    ProductionListConstIterator plIt=mDerivation.at(level).begin(),plEnd=mDerivation.at(level).end();
    SymbolStringConstIterator it(plIt,symbolTable);
    while(it!=plEnd){ if(!it.isDeterministic()) return false; ++it; }
    return true;
}
//####################################################################
