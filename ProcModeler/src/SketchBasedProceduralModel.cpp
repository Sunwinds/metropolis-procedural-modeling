//#####################################################################
// Copyright 2009, Jerry Talton.
//#####################################################################
#include <QMenuBar>
#include <QInputDialog>
#include "SketchBasedProceduralModel.hpp"
using ProceduralModeling::ProductionList;
using ProceduralModeling::Math::sqr;

//#####################################################################
// Function likelihood
//#####################################################################
float SketchBasedProceduralModel::logLikelihood(const ProductionList& model) const
{
    const QImage& sketchImage=mSketchingWidget.image();
    QImage renderedImage=static_cast<ModelRenderWidget*>(mRenderWidget)->getModelImage(model);

    float logProb=0;

    uint width=sketchImage.width(),height=sketchImage.height();

    float multiplierSum=mFalseNegativeMultiplier+mFalsePositiveMultiplier;
    float falseNegMultiplier=mFalseNegativeMultiplier/multiplierSum; float falsePosMultiplier=mFalsePositiveMultiplier/multiplierSum;
    float finalLikelihoodMultiplier=mLikelihoodMultiplier*(-50.f)/sqrtf((float)(width*height));

    if(mIgnoreColor)
    {
        for(uint j=0;j<height;j++) {
            for(uint i=0;i<width;i++) {
                int targetVal=QColor::fromRgba(sketchImage.pixel(i,j)).value(); int sampleVal=QColor::fromRgba(renderedImage.pixel(i,j)).value();
                if(targetVal<mValueThreshold && sampleVal>mValueThreshold) logProb+=falsePosMultiplier;
                if(targetVal>mValueThreshold && sampleVal<mValueThreshold) logProb+=falseNegMultiplier;
            }
        }
    }
    else
    {
        for(uint j=0;j<height;j++) {
            for(uint i=0;i<width;i++) {
                float diff=(QColor::fromRgba(sketchImage.pixel(i,j)).valueF()-QColor::fromRgba(renderedImage.pixel(i,j)).valueF());float diff2=diff*diff;
                logProb+=diff<0.0?diff2*falsePosMultiplier:diff2*falseNegMultiplier;
            }
        }
    }

    return finalLikelihoodMultiplier*logProb;
}
//#####################################################################
