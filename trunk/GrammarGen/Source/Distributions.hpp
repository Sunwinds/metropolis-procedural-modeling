//#####################################################################
// Copyright 2009, Jerry Talton.
// This software is governed by the license contained in LICENSE.
//#####################################################################
#ifndef _DISTRIBUTIONS_HPP_
#define _DISTRIBUTIONS_HPP_

#include "Common.hpp"
#include "Math.hpp"

namespace ProceduralModeling{

//#####################################################################
// Class Distributions
//#####################################################################
class Distributions
{
public:
    static float UniformSample(const float min,const float max,const float sample)
    { assert(min<=max); return min+sample*(max-min); }

    static float NormalSample(const float mean,const float variance,const float sampleOne,const float sampleTwo)
    { assert(variance>=0); return mean+sqrt(variance)*sqrt(-2*log(sampleOne))*cos(TWO_PI*sampleTwo); }

    static float UniformEval(const float min,const float max,const float sample)
    { assert(min!=max); return 1/(max-min); }

    static float NormalEval(const float mean,const float variance,const float sampleOne,const float sampleTwo)
    { assert(variance>=0); return pow(sampleOne,Math::sqr(cos(2*M_PI*sampleTwo)))/(SQRT_TWO_PI*sqrt(variance)); }

//#####################################################################
};

//#####################################################################
} // namespace ProceduralModeling

#endif //_DISTRIBUTIONS_HPP_
