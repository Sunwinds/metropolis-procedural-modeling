//#####################################################################
// Copyright 2009, Jerry Talton.
//#####################################################################
#ifndef __PROCEDURAL_MODEL__
#define __PROCEDURAL_MODEL__

#include "Symbol.hpp"
#include "DerivationTree.hpp"
#include "GrammarPlugin.hpp"
#include "Production.hpp"

namespace ProceduralModeling{

//####################################################################
// Class ProceduralModel
//####################################################################
class PROCEDURAL_EXPORT ProceduralModel
{
protected:
    GrammarPlugin mPlugin;
    Symbol** mSymbolTable;
    QVector<DerivationTree*> mModels;
    QVector<float> mInverseTemperatures,mLogPriors,mLogLikelihoods;
    float mLogMaxPosterior,mJumpProbability,mTemperatureExponent;
    uint mDerivationDepthLimit,mNumReplicas;
    ProductionList mMaxPosteriorModel;
    QGLWidget* mRenderWidget;
    ResourceHandler mResourceHandler;
    QVector<ulong> mNumDiffusions,mNumDiffusionsAccepted,mNumJumps,mNumJumpsAccepted;
    ulong mNumSwaps,mNumSwapsAccepted;
    bool mLogStats,mSimulationStarted,mUseLevelBiasedPolicy;

public:
    ProceduralModel(QGLWidget* widget=NULL)
        :mSymbolTable(NULL),mLogMaxPosterior(0),mJumpProbability(.5),mTemperatureExponent(1.3),mDerivationDepthLimit(0),mNumReplicas(10),
        mRenderWidget(widget),mNumSwaps(0),mNumSwapsAccepted(0),mLogStats(false),mSimulationStarted(false),mUseLevelBiasedPolicy(true)
    {}

    ~ProceduralModel()
    { mPlugin.destroy(); foreach(DerivationTree* tree,mModels) delete tree; }

    virtual float logLikelihood(const ProductionList& model) const
    { return 0; }

    float logMaxPosterior() const
    { return mLogMaxPosterior; }

    const ProductionList& maxPosteriorModel() const
    { return mMaxPosteriorModel; }

    const ProductionList& currentModel() const
    { return mModels[0]->production(); }

    const ProductionList& model(const uint level,const uint index=0) const
    { return mModels[index]->production(level); }

    uint modelTreeDepth(const uint index=0) const
    { return mModels[index]->treeDepth(); }

    const ResourceHandler& getResourceHandler() const
    { return mResourceHandler; }

    ResourceHandler& getResourceHandler()
    { return mResourceHandler; }

    void setWidget(QGLWidget* widget)
    { mRenderWidget=widget; }

    void toggleLevelPolicy()
    { mUseLevelBiasedPolicy=!mUseLevelBiasedPolicy; }

    bool useLevelBiasedPolicy() const
    { return mUseLevelBiasedPolicy; }

    void toggleLogging()
    { mLogStats=!mLogStats; }

    bool useLogging() const
    { return mLogStats; }

    uint numReplicas() const
    { return mNumReplicas; }

    float temperatureExponent() const
    { return mTemperatureExponent; }

    float diffusionAcceptanceRate(const uint index) const
    { return mNumDiffusions[index]>0?(float)mNumDiffusionsAccepted[index]/mNumDiffusions[index]:0; }

    float jumpAcceptanceRate(const uint index) const
    { return mNumJumps[index]>0?(float)mNumJumpsAccepted[index]/mNumJumps[index]:0; }

    float swapAcceptanceRate() const
    { return mNumSwaps>0?(float)mNumSwapsAccepted/mNumSwaps:0; }

    float replicaTemperature(const uint replicaIndex,const uint temperatureIndex) const
    { return mInverseTemperatures[temperatureIndex]*(mLogPriors[replicaIndex]+mLogLikelihoods[replicaIndex]); }

    void swapModels(uint i,uint j)
    { DerivationTree* treeTemp=mModels[i]; mModels[i]=mModels[j]; mModels[j]=treeTemp;
    float temp=mLogPriors[i]; mLogPriors[i]=mLogPriors[j]; mLogPriors[j]=temp;
    temp=mLogLikelihoods[i]; mLogLikelihoods[i]=mLogLikelihoods[j]; mLogLikelihoods[j]=temp; }

public:
    virtual void initializeSimulation();
    virtual void evolveChain();
    bool loadFromFile(const QString& filename);
    void render(const ProductionList& string) const;
    void generateMesh(const ProductionList& string, const QString& filename);
    void voxelize(const  ProductionList& string) const;
    QByteArray modelToString(const ProductionList& string) const;
    bool saveToFile(QFile& file) const;
    bool readFromFile(QFile& file,const bool relinkIterators=true);
    bool writeStringAsAxiom(const ProductionList& string,QFile& file) const;
    bool readStringAsAxiom(QFile& file);
    void resetModel();
    void resetStats();
    void setTemperatureExponent(const float exponent);
    void setNumReplicas(const uint numReplicas);
    void modelStats(const ProductionList& string,ulong& numSymbols,ulong& numDiffusionParams,ulong& numFormalParams) const;

protected:
    void generate(DerivationTree* model,const uint depthLimit,const bool matchLimitExactly) const;
    float logPrior(const uint index=0) const;
    bool diffusionMove(const uint index=0);
    bool jumpMove(const uint index=0);
    float logProbParams(ProductionListConstIterator begin,ProductionListConstIterator end) const;
    uint uniformRandomTreeIndex(DerivationTree* model,SymbolEntry*& symbolEntry,ProductionListIterator*& symbolProductionIterator);
    uint levelBiasedRandomTreeIndex(DerivationTree* model,SymbolEntry*& symbolEntry,ProductionListIterator*& symbolProductionIterator);
    void randomTreeIndexHelper(DerivationTree* model,const uint treeLevel,uint symbolIndex,SymbolEntry*& symbolEntry,ProductionListIterator*& symbolProductionIterator);
    SymbolEntry* randomTerminalInProduction(DerivationTree* model);
    void generateSubtree(SymbolEntry* symbolEntry,DerivationTree& subtree,const uint depthLimit);
    void copyDiffusionParams(ProductionList& dest,const ProductionList& source);
    void dimensionMatching(ProductionList& newList,const ProductionList& oldList) const;
    void setSubtreeIterators(ProductionListIterator leftIt,ProductionListIterator rightIt,uint level,SubtreeIterators& left,SubtreeIterators& right);
    void logDiffusionStats(uint index,bool accepted,float likelihoodTerm,float newLogLikelihood,float oldLogLikelihood,float acceptanceProbability);
    void logJumpStats(uint index,bool accepted,uint treeLevel,float likelihoodTerm,float newLogLikelihood,float oldLogLikelihood,float nonTermTerm,
        uint newJumpables,uint oldJumpables,float acceptanceProbability);
    void logReplicaExchange(bool accepted,uint index,float ij,float ji,float ii,float jj,float prob);

//####################################################################
};

//#####################################################################
} // namespace ProceduralModeling

#endif //__PROCEDURAL_MODEL__
