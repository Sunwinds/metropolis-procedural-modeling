//#####################################################################
// Copyright 2009, Jerry Talton.
//#####################################################################
#include <QString>
#include "Math.hpp"
#include "ProceduralModel.hpp"
#include "Production.hpp"
#include "SymbolStringIterator.hpp"
using namespace ProceduralModeling;
//####################################################################
// Function loadFromFile
//####################################################################
bool ProceduralModel::loadFromFile(const QString& fileName)
{
    if(!mPlugin.load(fileName)) return false;

    mResourceHandler.init();
    mResourceHandler.setRenderWidget(mRenderWidget);
    mPlugin.setup(mSymbolTable,&mResourceHandler);

    mLogPriors.fill(1,mNumReplicas);
    mLogLikelihoods.fill(1,mNumReplicas);
    resetStats();

    for(uint i=0;i<mNumReplicas;i++)
    {
        mInverseTemperatures.push_back(1./pow(mTemperatureExponent,i));
        mModels.push_back(new DerivationTree());
        mDerivationDepthLimit=mPlugin.import(mModels[i]);
        generate(mModels[i],mDerivationDepthLimit,false);
    }

    return true;
}
//####################################################################
// Function Render
//####################################################################
void ProceduralModel::render(const ProductionList& string) const
{
    SymbolStringConstIterator it(string.begin(),mSymbolTable);
    while(it != string.end()){ it.symbol()->render(it.params()); ++it; }
}
//####################################################################
// Function GenerateMesh
//####################################################################
void ProceduralModel::generateMesh(const ProductionList& string, const QString& name)
{
    mPlugin.initOutputMesh(&mResourceHandler);

    SymbolStringConstIterator it(string.begin(),mSymbolTable);
    while(it != string.end()){ it.symbol()->saveToMesh(it.params()); ++it; }
    mPlugin.saveOutputMesh(&mResourceHandler, name);
}
//####################################################################
// Function voxelize
//####################################################################
void ProceduralModel::voxelize(const  ProductionList& string) const
{
    mResourceHandler.resetBBoxHelper();
    SymbolStringConstIterator it(string.begin(),mSymbolTable);
    while(it != string.end()){ it.symbol()->voxel(it.params()); ++it; }
}
//####################################################################
// Function generate
//####################################################################
void ProceduralModel::generate(DerivationTree* tree,const uint depthLimit,const bool matchLimitExactly) const
{
    uint derivationLevel=tree->treeDepth();
    bool isTerminalString=false;

    while((matchLimitExactly || !isTerminalString) && derivationLevel<=depthLimit)
    {
        isTerminalString=true;

        tree->derivation().push_back(ProductionList());

        ProductionList& oldString=tree->production(derivationLevel-1);
        ProductionList& newString=tree->production(derivationLevel);
        ProductionListIterator oldIterator=oldString.begin();
        uint jumpablesInLevel=0;

        while(oldIterator!=oldString.end())
        {
            const SymbolEntryList& oldStringSegment=oldIterator->symbolList();
            ChildrenIterators& oldChildren=oldIterator->children();
            uint oldStringSegmentLength=oldStringSegment.size();
            uint srcIndex=0,symbolNum=0;

            while(srcIndex < oldStringSegmentLength)
            {
                const Symbol* currSymbol=mSymbolTable[oldStringSegment[srcIndex].mID];
                if(!currSymbol->isDeterministic(oldStringSegment.data()+srcIndex)) isTerminalString=false;
                if(currSymbol->isJumpable(oldStringSegment.data()+srcIndex)) oldIterator->numJumpables()++;

                newString.push_back(Production(currSymbol->maxNumSymbols(),currSymbol->maxNumSymbolEntries()));
                oldChildren[symbolNum]=--newString.end();

                Production& newProduction=newString.back();
                SymbolEntryList& newSymbolList=newProduction.symbolList();

                uint size=currSymbol->produce(oldStringSegment.data()+srcIndex+1,newSymbolList.data(),newProduction.logProbability());

                newProduction.symbolList().resize(size);
                srcIndex+=currSymbol->numEntries();
                symbolNum++;
            }

            oldChildren.resize(symbolNum);
            jumpablesInLevel+=oldIterator->numJumpables();
            ++oldIterator;
        }

        tree->jumpablesInLevel().push_back(jumpablesInLevel);
        derivationLevel++;
    }

    tree->derivation().pop_back();
    tree->jumpablesInLevel().pop_back();
}
//####################################################################
// Function logProbParams
//####################################################################
float ProceduralModel::logProbParams(ProductionListConstIterator begin,ProductionListConstIterator end) const
{
    float logProb=0;
    SymbolStringConstIterator it(begin,mSymbolTable);
    while(it!=end){ logProb+=it.symbol()->logDiffusionProbability(it.params()); ++it; }
    return logProb;
}
//####################################################################
// Function initializeSimulation
//####################################################################
void ProceduralModel::initializeSimulation()
{
    mMaxPosteriorModel=mModels.first()->production();

    for(uint i=0;i<mNumReplicas;i++){ mLogPriors[i]=logPrior(i); mLogLikelihoods[i]=logLikelihood(mModels[i]->production()); }

    mLogMaxPosterior=mLogPriors.first()+mLogLikelihoods.first();
}
//####################################################################
// Function evolveChain
//####################################################################
void ProceduralModel::evolveChain()
{
    if(!mSimulationStarted){ initializeSimulation(); mSimulationStarted=true; }

    bool accepted=false;

    if(mNumReplicas>1)
    {
         if(Math::uniformZeroToOne()<mJumpProbability) for(int i=mNumReplicas-1;i>=0;i--) accepted=jumpMove(i);
        else for(int i=mNumReplicas-1;i>=0;i--) accepted=diffusionMove(i);

        uint index=Math::uniformInRange(0,mNumReplicas-2);

        float ji=replicaTemperature(index+1,index),ij=replicaTemperature(index,index+1);
        float ii=replicaTemperature(index,index),jj=replicaTemperature(index+1,index+1);
        float acceptanceProb=exp(ij+ji-ii-jj);

        if(Math::uniformZeroToOne()<acceptanceProb){ swapModels(index,index+1); accepted=accepted||index==0; }

        logReplicaExchange(accepted,index,ij,ji,ii,jj,acceptanceProb);
    }
    else accepted=Math::uniformZeroToOne()<mJumpProbability?jumpMove():diffusionMove();

    if(accepted){
        float newLogPosterior=mLogPriors.first()+mLogLikelihoods.first();
        if(newLogPosterior>mLogMaxPosterior){ mLogMaxPosterior=newLogPosterior; mMaxPosteriorModel=mModels.first()->production(); }
    }
}
//####################################################################
// Function resetModel
//####################################################################
void ProceduralModel::resetModel()
{
    for(uint i=0;i<mNumReplicas;i++)
    {
        mDerivationDepthLimit=mPlugin.import(mModels[i]);
        generate(mModels[i],mDerivationDepthLimit,false);
    }
    resetStats();
    mSimulationStarted=false;
}
//####################################################################
// Function setNumReplicas
//####################################################################
void ProceduralModel::setNumReplicas(const uint numReplicas)
{
    int oldNumReplicas=mNumReplicas;
    if(!mPlugin.isLoaded()) return;

    mNumReplicas=numReplicas;

    mLogPriors.resize(mNumReplicas);
    mLogLikelihoods.resize(mNumReplicas);
    mModels.resize(mNumReplicas);
    mInverseTemperatures.resize(mNumReplicas);

    for(uint i=oldNumReplicas;i<mNumReplicas;i++)
    {
        mLogPriors[i]=mLogLikelihoods[i]=1;
        mInverseTemperatures[i]=1./pow(mTemperatureExponent,i);
        mModels[i]=new DerivationTree();
        mDerivationDepthLimit=mPlugin.import(mModels[i]);
        generate(mModels[i],mDerivationDepthLimit,false);
    }
}
//####################################################################
// Function setTemperatureExponent
//####################################################################
void ProceduralModel::setTemperatureExponent(const float exponent)
{
    mTemperatureExponent=exponent;
    for(uint i=0;i<mNumReplicas;i++) mInverseTemperatures[i]=1./pow(mTemperatureExponent,i);
}
//####################################################################
// Function logPrior
//####################################################################
float ProceduralModel::logPrior(const uint index) const
{
    return mModels[index]->logProbability(false)+logProbParams(mModels[index]->production().begin(),mModels[index]->production().end());
}
//####################################################################
// Function diffusionMove
//####################################################################
bool ProceduralModel::diffusionMove(const uint index)
{
    ++mNumDiffusions[index];
    SymbolEntry* symbolEntry=randomTerminalInProduction(mModels[index]);
    const Symbol* diffSymbol=mSymbolTable[symbolEntry->mID];
    float* params=reinterpret_cast<float*>(symbolEntry+diffSymbol->paramOffset());
    uint numDiffusionParams=diffSymbol->numDiffusionParams();
    if(numDiffusionParams==0) return false;
    uint numFormalParams=diffSymbol->numFormalParams();

    float* oldParams=new float(numDiffusionParams+numFormalParams);
    memcpy(oldParams,params,(numDiffusionParams+numFormalParams)*sizeof(float));

    for(uint i=0;i<numDiffusionParams;i++) params[i]=Math::uniformZeroToOne();

    float newLogLikelihood=logLikelihood(mModels[index]->production());
    float likelihoodTerm=(newLogLikelihood-mLogLikelihoods[index]);

    float acceptanceProb=exp(mInverseTemperatures[index]*likelihoodTerm);

    bool accepted=Math::uniformZeroToOne()<acceptanceProb;

    if(accepted){
        mLogLikelihoods[index]=newLogLikelihood;
        float newLogProb=diffSymbol->logDiffusionProbability(params);
        float oldLogProb=diffSymbol->logDiffusionProbability(oldParams);
        mLogPriors[index]+=(newLogProb-oldLogProb);
    }
    else{ memcpy(params,oldParams,numDiffusionParams*sizeof(float)); }

    logDiffusionStats(index,accepted,exp(likelihoodTerm),newLogLikelihood,mLogLikelihoods[index],acceptanceProb);

    delete[] oldParams;

    return accepted;
}
//####################################################################
// Function jumpMove
//####################################################################
bool ProceduralModel::jumpMove(const uint index)
{
    SymbolEntry* symbolEntry=NULL;
    ProductionListIterator* symbolProductionIterator=NULL;

    uint treeLevel;
    if(mUseLevelBiasedPolicy) treeLevel=levelBiasedRandomTreeIndex(mModels[index],symbolEntry,symbolProductionIterator);
    else treeLevel=uniformRandomTreeIndex(mModels[index],symbolEntry,symbolProductionIterator);

    SubtreeIterators left,right;
    ProductionListIterator temp=*symbolProductionIterator;
    left.push_back(temp); right.push_back(++temp);
    setSubtreeIterators(*symbolProductionIterator,*symbolProductionIterator,mModels[index]->treeDepth()-treeLevel-1,left,right);

    DerivationTree randomSubtree;
    generateSubtree(symbolEntry,randomSubtree,mDerivationDepthLimit-treeLevel);
    dimensionMatching(randomSubtree.production(),mModels[index]->production());

    ProductionList proposal;
    proposal.insert(proposal.begin(),mModels[index]->production().begin(),left.last());
    proposal.insert(proposal.end(),randomSubtree.production().begin(),randomSubtree.production().end());
    proposal.insert(proposal.end(),right.last(),mModels[index]->production().end());

    uint newJumpables,oldJumpables;
    if(mUseLevelBiasedPolicy){ newJumpables=(treeLevel+randomSubtree.treeDepth()); oldJumpables=mModels[index]->treeDepth(); }
    else{
        uint oldSubtreeNumJumpables=DerivationTree::numJumpables(left,right,mModels[index]->treeDepth()==mDerivationDepthLimit);
        oldJumpables=mModels[index]->numJumpables();
        newJumpables=oldJumpables-oldSubtreeNumJumpables+randomSubtree.numJumpables()-1;
    }

    float logJumpableTerm=log(newJumpables-oldJumpables);
    float newLogLikelihood=logLikelihood(proposal);
    float oldLogLikelihood=mLogLikelihoods[index];
    float likelihoodTerm=newLogLikelihood-oldLogLikelihood;
    float oldLogParamProb=logProbParams(left.last(),right.last());
    float newLogParamProb=logProbParams(randomSubtree.production().begin(),randomSubtree.production().end());
    float paramTerm=newLogParamProb-oldLogParamProb;

    float acceptanceProb=exp(mInverseTemperatures[index]*(logJumpableTerm+likelihoodTerm+paramTerm));

    bool accepted=Math::uniformZeroToOne()<acceptanceProb;

    if(accepted)
    {
        float oldLogProb=DerivationTree::logProbability(left,right);
        float newLogProb=randomSubtree.logProbability(false);

        int levelMismatch=treeLevel+randomSubtree.treeDepth()-mModels[index]->treeDepth();
        if(levelMismatch<0) generate(&randomSubtree,mModels[index]->treeDepth()-treeLevel,true);
        else if(levelMismatch>0){
            generate(mModels[index],treeLevel+randomSubtree.treeDepth(),true);
            ProductionListIterator temp=right.back();
            setSubtreeIterators(left.back(),--temp,levelMismatch+1,left,right);
        }

        *symbolProductionIterator=randomSubtree.production(1).begin();
        uint level=1;
        for(;level<randomSubtree.treeDepth();level++)
        {
            ProductionList& currLevelList=mModels[index]->production(treeLevel+level);
            uint replacedJumpables=Production::numJumpablesInList(left[level-1],right[level-1]);
            currLevelList.erase(left[level-1],right[level-1]);
            currLevelList.splice(right[level-1],randomSubtree.production(level));
            if(level<randomSubtree.treeDepth()-1){
                mModels[index]->jumpablesInLevel(treeLevel+level)+=randomSubtree.jumpablesInLevel(level)-replacedJumpables;
                if(mModels[index]->levelIsDeterministic(treeLevel+level,mSymbolTable)) break;
            }
        }

        int levelsToRemove=mModels[index]->treeDepth()-(treeLevel+level+1);
        if(levelsToRemove>0) copyDiffusionParams(mModels[index]->production(treeLevel+level),proposal);
        for(int i=0;i<levelsToRemove;i++) mModels[index]->derivation().pop_back();
        mModels[index]->jumpablesInLevel().resize(mModels[index]->treeDepth()-1);

        mLogPriors[index]+=(newLogProb-oldLogProb)+(newLogParamProb-oldLogParamProb);
        mLogLikelihoods[index]=newLogLikelihood;
    }

    logJumpStats(index,accepted,treeLevel,exp(likelihoodTerm),newLogLikelihood,oldLogLikelihood,logJumpableTerm,newJumpables,oldJumpables,acceptanceProb);

	return accepted;
}
//####################################################################
// Function setSubtreeIterators
//####################################################################
void ProceduralModel::setSubtreeIterators(ProductionListIterator leftIt,ProductionListIterator rightIt,uint level,SubtreeIterators& left,SubtreeIterators& right)
{
    while(--level>0)
    {
        leftIt=leftIt->child(0); rightIt=rightIt->child(rightIt->numSymbols()-1);
        ProductionListIterator temp=rightIt;
        left.push_back(leftIt); right.push_back(++temp);
    }
}
//####################################################################
// Function generateSubtree
//####################################################################
void ProceduralModel::generateSubtree(SymbolEntry* symbolEntry,DerivationTree &newSubTree,const uint depthLimit)
{
    const Symbol* currSymbol=mSymbolTable[symbolEntry[0].mID];
    Production stubAxiom(currSymbol->maxNumSymbolEntries(),currSymbol->numEntries());
    newSubTree=DerivationTree(stubAxiom);
    memcpy(newSubTree.production().front().symbolList().data(),symbolEntry,currSymbol->numEntries()*sizeof(SymbolEntry));
    generate(&newSubTree,depthLimit,false);
}
//####################################################################
// Function uniformRandomTreeIndex
//####################################################################
void ProceduralModel::randomTreeIndexHelper(DerivationTree* model,const uint treeLevel,uint symbolIndex,SymbolEntry*& symbolEntry,ProductionListIterator*& symbolProductionIterator)
{
    ProductionListIterator it=model->production(treeLevel).begin();
    uint runningSum=it->numJumpables();
    while(runningSum < symbolIndex){ ++it; runningSum+=it->numJumpables(); }
    symbolIndex-=runningSum-it->numJumpables();

    SymbolEntryList& symbols=it->symbolList();
    uint jumpableSymbolNum=1,symbolNum=0,currIndex=0;

    while(!mSymbolTable[symbols[currIndex].mID]->isJumpable(symbols.data()+currIndex))
    { currIndex+=mSymbolTable[symbols[currIndex].mID]->numEntries(); symbolNum++; }

    while(jumpableSymbolNum < symbolIndex)
    {
        currIndex+=mSymbolTable[symbols[currIndex].mID]->numEntries();
        if(!mSymbolTable[symbols[currIndex].mID]->isJumpable(symbols.data()+currIndex)) jumpableSymbolNum++; symbolNum++;
    }

    symbolProductionIterator=&it->child(symbolNum);
    symbolEntry=symbols.data()+currIndex;
}
//####################################################################
// Function uniformRandomTreeIndex
//####################################################################
uint ProceduralModel::uniformRandomTreeIndex(DerivationTree* model,SymbolEntry*& symbolEntry,ProductionListIterator*& symbolProductionIterator)
{
    uint numJumpableProductions=model->numJumpables();

    uint symbolIndex=Math::uniformZeroToOne()*numJumpableProductions+1;

    uint treeLevel=0,runningSum=model->jumpablesInLevel(treeLevel);
    while(runningSum < symbolIndex){ treeLevel++; runningSum+=model->jumpablesInLevel(treeLevel); }
    symbolIndex-=runningSum-model->jumpablesInLevel(treeLevel);

    randomTreeIndexHelper(model,treeLevel,symbolIndex,symbolEntry,symbolProductionIterator);

    return treeLevel;
}
//####################################################################
// Function levelBiasedRandomTreeIndex
//####################################################################
uint ProceduralModel::levelBiasedRandomTreeIndex(DerivationTree* model,SymbolEntry*& symbolEntry,ProductionListIterator*& symbolProductionIterator)
{
    uint treeLevel;
    do{ treeLevel=Math::uniformZeroToOne()*(model->treeDepth()-1); } while(model->jumpablesInLevel(treeLevel)==0);
    uint symbolIndex=Math::uniformZeroToOne()*model->jumpablesInLevel(treeLevel)+1;

    randomTreeIndexHelper(model,treeLevel,symbolIndex,symbolEntry,symbolProductionIterator);

    return treeLevel;
}
//####################################################################
// Function randomTerminalInProduction
//####################################################################
SymbolEntry* ProceduralModel::randomTerminalInProduction(DerivationTree* model)
{
    uint numTerminals=0;
    SymbolStringConstIterator constIterator(model->production().begin(),mSymbolTable);
    while(constIterator != model->production().end()){ if(constIterator.isTerminal()) numTerminals++; ++constIterator; }

	if(!numTerminals) return NULL;

    uint symbolIndex=Math::uniformZeroToOne()*numTerminals+1,count=1;
    SymbolStringIterator iterator(model->production().begin(),mSymbolTable);
    while(!iterator.isTerminal()) ++iterator;
    while(count<symbolIndex){ ++iterator; if(iterator.isTerminal()) count++; }
    return iterator.symbolEntry();
}
//####################################################################
// Function copyDiffusionParams
//####################################################################
void ProceduralModel::copyDiffusionParams(ProductionList& dest,const ProductionList& source)
{
    SymbolStringConstIterator srcIt(source.begin(),mSymbolTable);
    SymbolStringIterator destIt(dest.begin(),mSymbolTable);
    while(srcIt != source.end()){ memcpy(destIt.params(),srcIt.params(),srcIt.symbol()->numDiffusionParams()*sizeof(float)); ++srcIt; ++destIt; }
}
//####################################################################
// Function dimensionMatching
//####################################################################
void ProceduralModel::dimensionMatching(ProductionList& newList,const ProductionList& oldList) const
{
    ProductionListConstIterator oldIt=oldList.begin();
    QVector<float> oldParams;

    while(oldIt!=oldList.end())
    {
        const SymbolEntryList& oldSegment=oldIt->symbolList();
        uint oldIndex=0,oldSegmentLength=oldSegment.size();
        do
        {
            const Symbol* oldSymbol=mSymbolTable[oldSegment[oldIndex].mID];
            uint numDiffParams=oldSymbol->numDiffusionParams(),paramOffset=oldSymbol->paramOffset();
            for(uint i=0;i<numDiffParams;i++) oldParams.push_back((oldSegment.data()+oldIndex+paramOffset)[i].mID);
            oldIndex+=oldSymbol->numEntries();
        }
        while(oldIndex<oldSegmentLength);

        ++oldIt;
    }

    ProductionListIterator newIt=newList.begin();
    uint oldParamIndex=0,numOldParams=oldParams.size();
    while(newIt!=newList.end())
    {
        SymbolEntryList& newSegment=newIt->symbolList();
        uint newIndex=0,newSegmentLength=newSegment.size();
        while(newIndex<newSegmentLength)
        {
            Symbol* newSymbol=mSymbolTable[newSegment[newIndex].mID];
            uint numDiffParams=newSymbol->numDiffusionParams(),paramOffset=newSymbol->paramOffset();
            for(uint i=0;i<numDiffParams;i++)
                if(oldParamIndex>=numOldParams) goto terminated;
                else (newSegment.data()+newIndex+paramOffset)[i].mID=oldParams[oldParamIndex++];
            newIndex+=newSymbol->numEntries();
        }
        ++newIt;
    }

  terminated: return;
}
//####################################################################
// Function modelStats
//####################################################################
void ProceduralModel::modelStats(const ProductionList& string,ulong& numSymbols,ulong& numFormalParams,ulong& numDiffusionParams) const
{
    numSymbols=numDiffusionParams=numFormalParams=0;
    SymbolStringConstIterator it(string.begin(),mSymbolTable);
    while(it != string.end()){ numDiffusionParams+=it.symbol()->numDiffusionParams(); numFormalParams+=it.symbol()->numFormalParams(); ++numSymbols; ++it; }
}
//####################################################################
// Function modelToString
//####################################################################
QByteArray ProceduralModel::modelToString(const ProductionList& string) const
{
    QString toReturn;
    ProductionListConstIterator iterator=string.begin();
    while(iterator != string.end())
    {
        const SymbolEntryList& stringSegment=iterator->symbolList();
        uint index=0,segmentLength=stringSegment.size();
        while(index<segmentLength)
        {
            const Symbol* currSymbol=mSymbolTable[stringSegment[index].mID];
            uint paramOffset=currSymbol->paramOffset();
            const float* params=reinterpret_cast<const float*>(stringSegment.data()+index+paramOffset);
            toReturn+=currSymbol->symbolName();
            if(paramOffset>1) toReturn+="::"+QString::number((stringSegment.data()+index)[1].mFlags)+"::";
            if(currSymbol->numParams()>0){
                uint numDiffParams=currSymbol->numDiffusionParams(),numFormParams=currSymbol->numFormalParams(),i=0;
                if(numDiffParams){
                    toReturn+="<";
                    for(;i<numDiffParams;i++) toReturn+=QString::number(params[i])+",";
                    toReturn.chop(1); toReturn+=">";
                }
                if(numFormParams){
                    toReturn+="(";
                    for(;i<numDiffParams+numFormParams;i++) toReturn+=QString::number(params[i])+",";
                    toReturn.chop(1); toReturn+=")";
                }
            }
            index+=currSymbol->numEntries();
        }
        ++iterator;
        toReturn+=" ";
    }
    toReturn+="\n";
    return toReturn.toAscii();
}
//####################################################################
// Function saveToFile
//####################################################################
bool ProceduralModel::saveToFile(QFile& file) const
{
    if(!file.open(QIODevice::ReadWrite)) return false;

    QDataStream stream(&file);
    stream.setVersion(QDataStream::Qt_4_6);
    stream<<mNumReplicas;
    for(uint i=0;i<mNumReplicas;i++) stream<<*mModels[i];
    stream<<mLogPriors<<mLogLikelihoods<<mLogMaxPosterior<<mMaxPosteriorModel;
    return true;
}
//####################################################################
// Function readFromFile
//####################################################################
bool ProceduralModel::readFromFile(QFile& file,const bool relinkIterators)
{
    if(!file.open(QIODevice::ReadOnly)) return false;

    QDataStream stream(&file);
    stream.setVersion(QDataStream::Qt_4_6);
    stream>>mNumReplicas;

    for(uint i=0;i<mNumReplicas;i++){ mModels.push_back(new DerivationTree()); stream>>*mModels[i]; }

    stream>>mLogPriors>>mLogLikelihoods>>mLogMaxPosterior>>mMaxPosteriorModel;

    if(relinkIterators) foreach(DerivationTree* model,mModels)
        for(uint i=0;i<model->treeDepth();i++)
        {
            ProductionListIterator currIterator=model->production(i).begin();
            ProductionListIterator nextIterator=i+1<model->treeDepth()?model->production(i+1).begin():model->production(i).begin();
            while(currIterator != model->production(i).end())
            {
                const SymbolEntryList& stringSegment=currIterator->symbolList();
                uint index=0,segmentLength=stringSegment.size();
                currIterator->children().clear();
                while(index < segmentLength)
                {
                    const Symbol* currSymbol=mSymbolTable[stringSegment[index].mID];
                    index+=currSymbol->numEntries();
                    currIterator->children().push_back(nextIterator++);
                }
                ++currIterator;
            }
        }

    mSimulationStarted=true;
    resetStats();
    return true;
}
//####################################################################
// Function resetStats
//####################################################################
void ProceduralModel::resetStats()
{
    mNumDiffusions.fill(0,mNumReplicas);
    mNumDiffusionsAccepted.fill(0,mNumReplicas);
    mNumJumps.fill(0,mNumReplicas);
    mNumJumpsAccepted.fill(0,mNumReplicas);
    mNumSwaps=mNumSwapsAccepted=0;
}
//####################################################################
// Function writeStringAsAxiom
//####################################################################
bool ProceduralModel::writeStringAsAxiom(const ProductionList& string,QFile& file) const
{
    if(!file.open(QIODevice::ReadWrite)) return false;

    QDataStream stream(&file);
    stream.setVersion(QDataStream::Qt_4_6);

    QVector<SymbolEntry> newAxiom;
    ProductionListConstIterator iterator=string.begin();
    while(iterator != string.end())
    {
        const SymbolEntryList& stringSegment=iterator->symbolList();
        uint index=0,segmentLength=stringSegment.size();

        while(index < segmentLength)
        {
            const Symbol* currSymbol=mSymbolTable[stringSegment[index].mID];
            for(uint i=0;i<currSymbol->numEntries();i++) newAxiom.push_back(stringSegment[index+i]);
            index+=currSymbol->numEntries();
        }
        ++iterator;
    }

    stream<<newAxiom;
    return true;
}
//####################################################################
// Function readStringAsAxiom
//####################################################################
bool ProceduralModel::readStringAsAxiom(QFile& file)
{
    if(!file.open(QIODevice::ReadOnly)) return false;

    QDataStream stream(&file);
    stream.setVersion(QDataStream::Qt_4_6);

    QVector<SymbolEntry> newAxiom;
    stream>>newAxiom;

    uint numSymbols=0;

    uint index=0;

    while(index<newAxiom.size())
    {
        const Symbol* currSymbol=mSymbolTable[newAxiom[index].mID];
        numSymbols++;
        index+=currSymbol->numEntries();
    }

    mModels.first()=new DerivationTree(Production(numSymbols,index));
    SymbolEntryList& list=mModels.first()->production(0).begin()->symbolList();
    for(uint i=0;i<newAxiom.size();i++) list[i]=newAxiom[i];

    resetStats();
    generate(mModels.first(),mDerivationDepthLimit,false);
    mSimulationStarted=false;

    return true;
}
//####################################################################
// Function logDiffusionStats
//####################################################################
void ProceduralModel::logDiffusionStats(uint index,bool accepted,float likelihoodTerm,float newLogLikelihood,float oldLogLikelihood,float acceptanceProb)
{
    mNumDiffusions[index]++;
    if(accepted) mNumDiffusionsAccepted[index]++;

    if(mLogStats)
    {
        QString outputString;
        QTextStream stream(&outputString);

        stream.setRealNumberPrecision(15);
        stream<<"##### Diffusion Move #####"<<endl;
        stream<<(accepted?"Accepted":"Rejected")<<endl;
        stream.setFieldWidth(18);
        stream<<"Likelihood: "<<likelihoodTerm<<newLogLikelihood<<oldLogLikelihood<<endl;
        stream<<"Prior: "<<exp(mLogPriors.first())<<mLogPriors.first()<<endl;
        stream<<"Acceptance: "<<acceptanceProb<<endl<<endl;

        std::cout<<qPrintable(outputString);
    }
}
//####################################################################
// Function logJumpStats
//####################################################################
void ProceduralModel::logJumpStats(uint index,bool accepted,uint treeLevel,float likelihoodTerm,float newLogLikelihood,float oldLogLikelihood,float jumpableTerm,uint newJumpables,uint oldJumpables,float acceptanceProb)
{
    mNumJumps[index]++;
    if(accepted) mNumJumpsAccepted[index]++;

    if(mLogStats)
    {
        QString outputString;
        QTextStream stream(&outputString);

        stream.setRealNumberPrecision(15);
        stream<<"##### Jump Move #####"<<endl;
        stream<<(accepted?"Accepted":"Rejected")<<endl;
        stream<<"TreeLevel: "<<treeLevel<<endl;
        stream.setFieldWidth(18);
        stream<<"Likelihood: "<<likelihoodTerm<<newLogLikelihood<<oldLogLikelihood<<endl;
        stream<<"Prior: "<<exp(mLogPriors.first())<<mLogPriors.first()<<endl;
        stream<<"Jumpable: "<<jumpableTerm<<newJumpables<<oldJumpables<<endl;
        stream<<"Acceptance: "<<acceptanceProb<<endl<<endl;

        std::cout<<qPrintable(outputString);
    }
}
//####################################################################
// Function logReplicaExchange
//####################################################################
void ProceduralModel::logReplicaExchange(bool accepted,uint index,float ij,float ji,float ii,float jj,float prob)
{
    mNumSwaps++;
    if(accepted) mNumSwapsAccepted++;

    if(mLogStats)
    {
        QString outputString;
        QTextStream stream(&outputString);
        stream.setRealNumberPrecision(15);
        stream<<"Swap: "<<index<<" with "<<index+1<<endl;
        stream<<"ij: "<<ij;
        stream<<"ji: "<<ji;
        stream<<"ii: "<<ii;
        stream<<"jj: "<<jj;
        stream<<"probability: "<<prob<<endl;
        stream<<"accepted: "<<accepted<<endl;

        std::cout<<qPrintable(outputString);
    }
}
//####################################################################
