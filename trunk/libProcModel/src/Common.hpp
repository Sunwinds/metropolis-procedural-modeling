#ifndef _COMMON_HPP_
#define _COMMON_HPP_

//#####################################################################
// Includes
//#####################################################################
#include <iostream>
#include <cassert>
#include <QVector>
#include <float.h>
#include "SymbolEntry.hpp"

//#####################################################################
// Generic typedefs
//#####################################################################
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;

namespace ProceduralModeling{

//#####################################################################
// Plugin defines
//#####################################################################
#define PLATFORM_WINDOWS 0
#define PLATFORM_LINUX 1
#define PLATFORM_MAC 2

#if defined(__WIN32__) || defined(_WIN32)
#pragma warning (disable: 4251)
#pragma warning (disable: 4661)
#pragma warning (disable : 4275)
# define PROCEDURAL_PLATFORM PLATFORM_WINDOWS
# define PROCEDURAL_PLUGIN_EXTENSION "dll"
#elif defined(__APPLE_CC__) || defined(__APPLE__)
# define PROCEDURAL_PLATFORM PLATFORM_MAC
# define PROCEDURAL_PLUGIN_EXTENSION "dylib"
# ifndef __MACOSX__
# define __MACOSX__
# endif
#else
# define PROCEDURAL_PLATFORM PLATFORM_LINUX
# define PROCEDURAL_PLUGIN_EXTENSION "so"
#endif

#ifdef NDEBUG
#define PROCEDURAL_DEBUG 0
#else
#define PROCEDURAL_DEBUG 1
#endif

#ifndef PROCEDURAL_EXPORT
# if PROCEDURAL_PLATFORM == PLATFORM_WINDOWS
# if defined(STATIC_LINKED)
# define PROCEDURAL_EXPORT
# else
# if defined(PROCEDURAL_BUILD)
# define PROCEDURAL_EXPORT __declspec(dllexport)
# else
# define PROCEDURAL_EXPORT __declspec(dllimport)
# endif
# endif
# define PROCEDURAL_PLUGIN_EXPORT __declspec(dllexport)
# else
# if defined(__GNUC__) && __GNUC__ >= 4
# define PROCEDURAL_EXPORT __attribute__ ((visibility("default")))
# define PROCEDURAL_PLUGIN_EXPORT __attribute__ ((visibility("default")))
# else
# define PROCEDURAL_EXPORT
# define PROCEDURAL_PLUGIN_EXPORT
# endif
# endif
#endif

#ifndef PROCEDURAL_FUNCTION_EXPORT
# if PROCEDURAL_PLATFORM == PLATFORM_WINDOWS
# if defined(STATIC_LINKED)
# define PROCEDURAL_FUNCTION_EXPORT
# else
# if defined(PROCEDURAL_BUILD)
# define PROCEDURAL_FUNCTION_EXPORT __declspec(dllexport)
# else
# define PROCEDURAL_FUNCTION_EXPORT __declspec(dllimport)
# endif
# endif
# else
# define PROCEDURAL_FUNCTION_EXPORT
# endif
#endif

#ifndef PROCEDURAL_EXPORT_C
# define PROCEDURAL_EXPORT_C extern "C" PROCEDURAL_EXPORT
#endif

#ifndef PROCEDURAL_PLUGIN_EXPORT_C
# define PROCEDURAL_PLUGIN_EXPORT_C extern "C" PROCEDURAL_PLUGIN_EXPORT
#endif

} // namespace ProceduralModeling

#endif // _COMMON_HPP_
