//-----------------------------------------------------------------------------
// Copyright Ed Keenan 2019
//----------------------------------------------------------------------------- 
// Trace 
//    v.2.1.0
//    v.2.5
//    v.2.6   - Baseline
//    v.2.6.1 - fixed warning TestRegistry
//    v.2.7   - Baseline
//    v.2.8   - warning 5039 windows.h
//    v.2.9   - fence
//    v.2.10  - msc_ver 1914, win 10.0.17
//    v.2.11  - warning fixes 
//    v.2.12  - mutex on all trace::out
//    v.2.13  - fixed warnings on mutex and default copy const / assignment
//    v.2.14  - FileIO remove new... so you can overload new/delete
//    v.2.15  - number tests pass reformat
//    v.2.16  - fixing static analysis issues
//    v.2.17  - missing deleted copy constructor and assignment operator in macro
//    v.2.18  - teardown issue
//    v.2.19  - WinSDK, Compiler, Framework version reporting
//    v.2.20  - WinSDK, Compiler, Framework version reporting in Release mode
//    v.2.21  - x64 info header
//    v.2.22  - Timer renamed to PerformanceTimer 
//    v.2.23  - Mem Tracking now works with placement and alignment new
//    v.2.24  - new tracking with file names and line numbers
//    v.2.25  - fixing collision with unused_var() macros in align16
//    v.2.26  - any order for the link, removed FileIO.cpp and PerformanceTimer.cpp
//    v.2.27  - atomics in Timer 
//    v.2.28  - remove dynamic cast, warnings on atomic
//----------------------------------------------------------------------------- 

#ifndef DEBUG_OUTPUT_H
#define DEBUG_OUTPUT_H

#include <assert.h>
#include <stdio.h>

// Windows.h include
// many warnings - need to wrap for Wall warnings
#pragma warning( push )
	#pragma warning( disable : 4820 )
	#pragma warning( disable : 4668 )
	#pragma warning( disable : 5039 )
	#include <Windows.h>
#pragma warning( pop ) 

#pragma warning( push )
	#pragma warning( disable : 4365 )
	#pragma warning( disable : 4623 )
	#pragma warning( disable : 4625 )
	#pragma warning( disable : 4626 )
	#pragma warning( disable : 4820 )
	#pragma warning( disable : 5026 )
	#pragma warning( disable : 5027 )
	#pragma warning( disable : 5039 )
	#include <mutex>
#pragma warning( pop ) 

// NOTE: you need to set your project settings
//       Character Set -> Use Multi-Byte Character Set

//#define TraceBuffSize 256

// Singleton helper class
class Trace
{
public:
	static const unsigned int TraceBuffSize = 256;

	// displays a printf to the output window
	static void out(const char * const fmt, ...)
	{
		Trace *pTrace = Trace::privGetInstance();
		assert(pTrace);

		pTrace->mtx.lock();

			va_list args;

		#pragma warning( push )
			#pragma warning( disable : 26492 )
			#pragma warning( disable : 26481 )
			va_start(args, fmt);
		#pragma warning( pop )

			vsprintf_s(&pTrace->privBuff[0], TraceBuffSize, fmt, args);
			OutputDebugString(&pTrace->privBuff[0]);

			//va_end(args);
			args = static_cast<va_list> (nullptr);

		pTrace->mtx.unlock();
	}

	// Big four
	Trace() noexcept
	{
		memset(&privBuff[0], 0x0, TraceBuffSize);
	} 
	Trace(const Trace &) = delete;
	Trace(Trace &&) = delete;
	Trace & operator = (const Trace &) = delete;
	Trace & operator = (Trace &&) = delete;
	~Trace() = default;

private:
	static Trace *privGetInstance() noexcept
	{
		// This is where its actually stored (BSS section)
		static Trace helper;
		return &helper;
	}
	char privBuff[TraceBuffSize];
	std::mutex mtx;
};

#endif

// ---  End of File ---------------
