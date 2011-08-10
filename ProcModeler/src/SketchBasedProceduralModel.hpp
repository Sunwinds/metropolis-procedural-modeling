//#####################################################################
// Copyright 2009, Jerry Talton.
//#####################################################################
#ifndef __SKETCH_BASED_PROCEDURAL_MODEL__
#define __SKETCH_BASED_PROCEDURAL_MODEL__

#include "ModelRenderWidget.hpp"
#include "SketchingWidget.hpp"

//####################################################################
// Class SketchBasedProceduralModel
//####################################################################
class SketchBasedProceduralModel : public ProceduralModel
{
	typedef ProceduralModeling::ProductionList ProductionList;

protected:
    const SketchingWidget& mSketchingWidget;
    float mFalsePositiveMultiplier,mFalseNegativeMultiplier,mLikelihoodMultiplier;
    int mValueThreshold;
    bool mIgnoreColor;

public:
    SketchBasedProceduralModel(ModelRenderWidget* renderWidget,const SketchingWidget& sketchingWidget)
     :ProceduralModel(renderWidget),mSketchingWidget(sketchingWidget),mFalsePositiveMultiplier(3),mFalseNegativeMultiplier(1.0),
      mLikelihoodMultiplier(25.f),mValueThreshold(15),mIgnoreColor(true)
    {}

    float falsePositiveMultiplier() const
    { return mFalsePositiveMultiplier; }

    void setFalsePositiveMultiplier(const float multiplier)
    { mFalsePositiveMultiplier=multiplier; }

    float falseNegativeMultiplier() const
    { return mFalseNegativeMultiplier; }

    void setFalseNegativeMultiplier(const float multiplier)
    { mFalseNegativeMultiplier=multiplier; }

    float likelihoodMultiplier() const
    { return mLikelihoodMultiplier; }

    void setLikelihoodMultiplier(const float multiplier)
    { mLikelihoodMultiplier=multiplier; }

    void toggleIgnoreColor()
    { mIgnoreColor=!mIgnoreColor; }

    bool ignoreColor() const
    { return mIgnoreColor; }

protected:
    virtual float logLikelihood(const ProductionList& model) const;
//####################################################################
};

#endif //__SKETCH_BASED_PROCEDURAL_MODEL__
