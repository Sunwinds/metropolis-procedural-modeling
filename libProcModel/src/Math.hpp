//#####################################################################
// Copyright 2009, Jerry Talton.
// This software is governed by the license contained in LICENSE.
//#####################################################################
#ifndef _MATH_HPP_
#define _MATH_HPP_

#include <float.h>
#include <cmath>
#include <cstdlib>

#define PI_OVER_TWO (1.57079633)
#define TWO_PI (6.28318531)
#define HALF_LOG_TWO_PI (0.399089934)
#define SQRT_TWO_PI (2.50662827)

namespace ProceduralModeling{

namespace Math{

template<class T>
inline bool isNan(const T x)
{ return x!=x; }

template<class T>
inline T min(const T a,const T b)
{return a < b ? a : b;}

template<class T>
inline T min(const T a,const T b,const T c)
{return min(a,min(b,c));}

template<class T>
inline T max(const T a,const T b)
{return a > b ? a : b;}

template<class T>
inline T max(const T a,const T b,const T c)
{return max(a,max(b,c));}

template<class T>
inline int round(const T x)
{ return int(x+.5f); }

inline float sqr(const float x)
{ return x*x; }

inline float uniformZeroToOne()
{ return (float)rand()/((float)RAND_MAX+1.f); }

inline int uniformInRange(const int min,const int max)
{ return min+int((max-min+1)*uniformZeroToOne()); }

inline float clampNonNegative(const float x)
{ return x>0?x:0; }

//#####################################################################
} // namespace Math

//#####################################################################
} // namespace ProceduralModeling

#endif //_MATH_HPP_
