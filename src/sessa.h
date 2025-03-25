/**
 * @file		sessa.h
 * @brief   Spondlab Sessa Interface
 * @author	Jeong Hoon (Sian) Choi
 * @version	1.0.0
 * @date		2023-12-27
 */
	 
//#pragma once
//#pragma GCC diagnostic ignored "-Wstringop-truncation"
//#pragma comment(lib, "libpthread.so")

#ifndef _HEADER_SESSAH_
#define _HEADER_SESSAH_

/* OS dependent */
#define OS_WINDOWS	0
#define OS_LINUX		1

#ifdef _WIN32
#define _TARGET_OS	OS_WINDOWS
#else
	#define _TARGET_OS	OS_LINUX
#endif

/* Include */

#include <utility>
#include <algorithm>
#include <numeric>
#include <memory>

#include <thread>
#include <mutex>
#include <condition_variable>

#include <filesystem>
#include <sstream>
#include <string>
#include <string_view>

#include "atoms.h"

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

/* MACRO functions */
#ifndef SWAP
template <typename T>
void SWAP(T& a, T& b) {
	T tmp=a;
	a=b;
	b=tmp;
}
#endif

#ifndef MIN
#define MIN(a, b)	(a > b ? b : a)
#endif

#ifndef MAX
#define MAX(a, b) (a > b ? a : b)
#endif

/* Attributes */
/*
[[noreturn]]
[[deprecated]]
[[fallthrough]]
[[nodiscard]]
[[maybe_unused]]
*/

/* Declaration */

/* ASM codes */
// extern "C" int func(int x, int y);

/* Data structures definition - struct & class */

namespace spondlab {
	class Sessa {
	public:
		Sessa(std::string_view);

		virtual ~Sessa(void)=default;

	private:
		void open(void);

		void close(void);

		void setting(void);
		
		const std::string sessaPath;
	};
}

#endif // OS dependency

/* Inline & Template Define Header */
//#include "sample.hpp"

#endif // Header duplicate
