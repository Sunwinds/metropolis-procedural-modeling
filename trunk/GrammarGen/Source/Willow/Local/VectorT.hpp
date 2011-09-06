/**************************************************************************
 **************************************************************************/

#pragma once

#include <string.h>
#include <assert.h>
#include <vector>
#include <float.h>
#include <math.h>

#ifdef WIN
#include <omp.h>
#endif
#ifdef _MSC_VER
   #define FORCE_INLINE __forceinline
#else
   #define FORCE_INLINE inline
#endif


#ifdef _DEBUG
#define DASSERT(cond) if (!(cond)) assert(0);
#else
#define DASSERT(cond) do {} while (0)
#endif

///////////////////////////////////////////////////////////////////////////
template <typename T, int dim> class VectorT
	{
	private:
		T vec[dim];

	public:
		FORCE_INLINE VectorT (void)
		{
			for (int i = 0; i < dim; i++)
				vec[i] = (T)0;
		}
		FORCE_INLINE VectorT (const VectorT &v)
		{
			for (int i = 0; i < dim; i++)
				vec[i] = v[i];
		}
		FORCE_INLINE VectorT (T val1, T val2)
		{
			vec[0] = val1;
			vec[1] = dim > 1 ? val2 : (T)(0);
			for (int i = 2; i < dim; i++)
				vec[i] = (T)0;
		}
		FORCE_INLINE VectorT (T val1, T val2, T val3)
		{
			vec[0] = val1;
			vec[1] = dim > 1 ? val2 : (T)(0);
			if (dim > 2)
				vec[2] = val3;
			for (int i = 3; i < dim; i++)
				vec[i] = (T)0;
		}
		FORCE_INLINE VectorT (T val1, T val2, T val3, T val4)
		{
			vec[0] = val1;
			vec[1] = dim > 1 ? val2 : (T)(0);
			if (dim > 2)
				vec[2] = val3;
			if (dim > 3)
				vec[3] = val4;
			//for (int i = 4; i < dim; i++)
			//    vec[i] = (T)0;
		}
		FORCE_INLINE VectorT (T *val, int _dim = 3)
		{
			DASSERT (val != NULL);
			for (int i = 0; i < _dim; i++)
				vec[i] = val[i];
		}
		FORCE_INLINE int dimension () const { return dim; }

		FORCE_INLINE T operator[] (int index) const
		{
			DASSERT (index >= 0 && index < dim);
			return vec[index];
		}
		FORCE_INLINE T &operator[] (int index)
		{
			DASSERT (index >= 0 && index < dim);
			return vec[index];
		}
		FORCE_INLINE VectorT operator*= (const T val)
		{
			for(int i = 0; i < dim; i++)
				vec[i] *= val;
			return *this;
		}
		FORCE_INLINE VectorT operator- (void)
		{
			VectorT res;
			for(int i = 0; i < dim; i++)
				res[i] = -vec[i];
			return res;
		}
		FORCE_INLINE VectorT operator*= (const VectorT v)
		{
			for(int i = 0; i < dim; i++)
				vec[i] *= v[i];
			return *this;
		}
		// Construct from a vector of a different type
		template<typename T2, int dim2> VectorT<T,dim>(const VectorT<T2,dim2> &v)
		{
			if (dim < dim2)
				for (int i = 0; i < dim; i++)
					vec[i] = (T)v[i];
			else {
				int i;
				for (i = 0; i < dim2; i++)
					vec[i] = (T)v[i];
				for (; i < dim; i++)
					vec[i] = (T)0;
			}
		}
		FORCE_INLINE VectorT operator+= (const VectorT v)
		{
			for(int i = 0; i < dim; i++)
				vec[i] += v[i];
			return *this;
		}
		FORCE_INLINE VectorT operator-= (const VectorT v)
		{
			for(int i = 0; i < dim; i++)
				vec[i] -= v[i];
			return *this;
		}
		FORCE_INLINE bool operator!= (const VectorT v)
		{
			for(int i = 0; i < dim; i++)
				if (vec[i] != v[i])
					return true;
			return false;
		}
		FORCE_INLINE bool operator== (const VectorT v)
		{
			for(int i = 0; i < dim; i++)
				if (vec[i] != v[i])
					return false;
			return true;
		}
		// Dot product
		FORCE_INLINE T dot (const VectorT &v)
		{
			T res = (T)0;
			for(int i = 0; i < dim; i++)
				res += vec[i] * v[i];
			return res;
		}
		// Length
		FORCE_INLINE T length ()
		{
			T res = dot (*this);
			return sqrt (res);
		}
		// Length
		FORCE_INLINE T lengthSq ()
		{
			return dot (*this);
		}
		// Normalize
		VectorT &normalize () {
			T len = length ();
			if (len != 0)
				*this *= (T)1.0 / len;
			return *this;
		}
		VectorT<T,3> cross (const VectorT<T,dim> &v2) const
		{
			VectorT<T,3> res;
			if (dim > 2)
			{
				res[0] = vec[1] * v2[2] - vec[2] * v2[1];
				res[1] = vec[2] * v2[0] - vec[0] * v2[2];
				res[2] = vec[0] * v2[1] - vec[1] * v2[0];
			}
			return res;
		}
		FORCE_INLINE VectorT operator/= (VectorT v)
		{
			for(int i = 0; i < v.dimension(); i++)
				vec[i] /= v[i];
			return *this;
		}
	};


template <typename T, int dim> FORCE_INLINE VectorT<T,dim> operator* (const VectorT<T,dim> &v, const T val)
{
    VectorT<T,dim> res (v);
    res *= val;
    return res;
}
template <typename T, int dim> FORCE_INLINE VectorT<T,dim> operator* (const VectorT<T,dim> &v1, const VectorT<T,dim> &v2)
{
    VectorT<T,dim> res (v1);
    res *= v2;
    return res;
}
template <typename T, int dim> FORCE_INLINE VectorT<T,dim> operator+ (const VectorT<T,dim> &v1, const VectorT<T,dim> &v2)
{
    VectorT<T,dim> res (v1);
    res += v2;
    return res;
}
template <typename T, int dim> FORCE_INLINE VectorT<T,dim> operator- (const VectorT<T,dim> &v1, const VectorT<T,dim> &v2)
{
    VectorT<T,dim> res (v1);
    res -= v2;
    return res;
}
template <typename T, int dim> FORCE_INLINE VectorT<T,dim> dot (const VectorT<T,dim> &v1, const VectorT<T,dim> &v2)
{
    VectorT<T,dim> res (v1);
    res.dot (v2);
    return res;
}
template <typename T, int dim> FORCE_INLINE  VectorT<T,3> cross (const VectorT<T,dim> &v1, const VectorT<T,dim> &v2)
{
    return v1.cross (v2);
}
template<typename T, int dim> VectorT<T,dim> operator/ (VectorT<T,dim> p, VectorT<T,dim> v)
{
    VectorT<T,dim> res (p);
    res /= v;
    return res;
}

///////////////////////////////////////////////////////////////////////////

template <typename T> class BBox
	{
	public:
		VectorT<T,3> min, max;
		BBox (VectorT<T,3> _min, VectorT<T,3> _max)
		{
			DASSERT (_min.dimension() == _max.dimension());
			DASSERT (_min[0]<=_max[0] && _min[1]<=_max[1] && _min[2]<=_max[2]);
			this->min = _min;
			this->max = _max;
		}
		BBox (VectorT<T,3> minmax)
		{
			this->min = minmax;
			this->max = minmax;
		}

		BBox () {}

		FORCE_INLINE int dimension (void) { return min.dimension(); }

		bool inside (VectorT<T,3> pt)
		{
			for (int i = 0; i < 3; i++)
				if (pt[i] < min[i] || pt[i] > max[i])
					return false;
			return true;
		}
		void resize (VectorT<T,3> pt)
		{
			for (int i = 0; i < 3; i++) {
				if (pt[i] < min[i])
					min[i] = pt[i];
				if (pt[i] > max[i])
					max[i] = pt[i];
			}
		}

		void clamp(VectorT<T,3>& pt)
		{
			for (int i = 0; i < 3; i++) {
				if (pt[i] < min[i])
					pt[i] = min[i];
				if (pt[i] > max[i])
					pt[i] = max[i];
			}
		}
	};

///////////////////////////////////////////////////////////////////////////

// define our float - you can change it to double
typedef float ourfloat;

typedef VectorT<ourfloat,3> Vectorf;
typedef BBox<ourfloat> BBoxf;

typedef std::vector<Vectorf> Vectors;

/////////////////////////////////////////////////////////////////////////////////
template <typename T> class Array3d
	{
		T   *data;

	public:
		VectorT<int,3> size;

		Array3d (VectorT<int,3> _size, bool reset = false) : size(_size)
		{
			DASSERT (size.dimension() >= 3 &&
					 size[0] != 0 && size[1] != 0 && size[2] != 0);
			data = new T[size[0] * size[1] * size[2]];
			if (reset)
				memset (data, 0, size[0] * size[1] * size[2] * sizeof(T));
		}

		Array3d (const Array3d& a) : size(a.size), data(new T[a.size[0] * a.size[1] * a.size[2]])
		{
			memcpy (data, a.data, a.size[0] * a.size[1] * a.size[2] * sizeof(T));
		}

		~Array3d()
		{
			if(data)
				delete []data;
		}

		FORCE_INLINE T* operator()(int x, int y = 0, int z = 0)
		{
			DASSERT (x >= 0 && x < size[0] &&
					 y >= 0 && y < size[1] &&
					 z >= 0 && z < size[2]);
			return (data + size[1] * ( (size[0] * z) + y) + x);
		}

	private:
		Array3d& operator=(const Array3d&);
	};
