/**
 * @file		atoms.h
 * @brief
 * @author	Jeong Hoon (Sian) Choi
 * @version	1.0.0
 * @date		2023-11-26
 */
	 
//#pragma once
//#pragma GCC diagnostic ignored "-Wstringop-truncation"
//#pragma comment(lib, "libpthread.so")

#ifndef _HEADER_ATOMSH_
#define _HEADER_ATOMSH_

/* OS dependent */
#define OS_WINDOWS	0
#define OS_LINUX		1

#ifdef _WIN32
#define _TARGET_OS	OS_WINDOWS
#else
	#define _TARGET_OS	OS_LINUX
#endif

/* Include */
#include <random>

#if __has_include(<iostream>)
#include <iostream>
#endif

#if _TARGET_OS == OS_WINDOWS
// Windows header
#include <Windows.h>

#elif _TARGET_OS == OS_LINUX
// Linux header
#include <sys/types.h>
#include <unistd.h>

/* C & CPP */
/*
#ifdef __cplusplus
extern "C" {
#endif
*/

/* defines typedef & constant */

/* Declaration */

/* ASM codes */
// extern "C" int func(int x, int y);

/* Data structures definition - struct & class */

namespace spondlab {
	enum class ATOM {
		Li, Be, B, C, N, O, F, Ne,
		Na, Mg, Al, Si, P, S, Cl, Ar,
		K, Ca, Sc, Ti, V, Cr, Mn, Fe,
		Co, Ni, Cu, Zn, Ga, Ge, As, Se,
		Br, Kr, Rb, Sr, Y, Zr, Nb, Mo,
		Tc, Ru, Rh, Pd, Ag, Cd, In, Sn,
		Sb, Te, I, Xe, Cs, Ba, La, Ce,
		Pr, Nd, Sm, Eu, Gd, Tb, Dy, Ho,
		Er, Tm, Yb, Lu, Hf, Ta, W, Re,
		Os, Ir, Pt, Au, Hg, Tl, Pb, Bi
	};

	class Atoms {
	public:
		Atoms(unsigned int, unsigned int);

		virtual ~Atoms(void)=default;		
	private:
		const unsigned int min, max;
		
		static std::mt19937 rd;
	};
}


/*
#ifdef __cplusplus
}
#endif
*/

#endif // OS dependency

/* Inline & Template Define Header */
//#include "sample.hpp"

#endif // Header duplicate
