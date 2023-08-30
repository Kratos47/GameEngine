//----------------------------------------------------------------------------
// Copyright Ed Keenan 2019
//----------------------------------------------------------------------------
// MemTracker 
//    v.2.1.0
//    v.2.2 - print to console
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
//    v.2.26  - any order for the link, removed FileIO.cpp and PerformanceTimer.cpp
//    v.2.27  - atomics in Timer 
//    v.2.28  - remove dynamic cast, warnings on atomic
//---------------------------------------------------------------------------- 

#ifndef MEM_TRACKER_H
#define MEM_TRACKER_H

// ---------------------------------------------------------------------------
// One instructions to rule them all:
//
//   To Enable: 
//             #define MEM_TRACKER_ENABLED
//   To Disable: 
//             comment out #define MEM_TRACKER_ENABLED
//
//   (1) Project Settings:
//      You need to set your project settings
//          Character Set -> Use Multi-Byte Character Set
//
//      Include MemTracker.h to every file:
//           Change your project settings
//               Go to C / C++ tab
//               Go to Advanced
//                      set Forced Include : MemTracker.h
//               This will add this file automagically to all files (great!)
//
//   (2) Modify main.cpp properties:
//      Add MemTrackerMain.h to the first line of main.cpp not the function
//          Has to be the first line of main.cpp 
//               To check static classes/allocations
//               This header will initialize memory tracking as well
//          To accomplish this 
//               set Forced Include on main.cpp: MemTrackerMain.h not MemTracker.h 
//
//  Placement new changes:
//      No memory tracking __SHOULD__ take place on placement new
//      So you need to tell the Memory Tracking system to ignore this placement new
//      Add 3 lines around placement new to allow it to compile
//
//      example:  Dog *pDog = new(address) Dog();  // original (new way below)
//
//      PLACEMENT_NEW_BEGIN    <----- Add 1/3 --------------
//      #undef new             <----- Add 2/3 --------------
//
//              Dog *pDog = new(address) Dog();
//
//      PLACEMENT_NEW_END      <----- Add 3/3 --------------
//
// ---------------------------------------------------------------------------

// Comment out this line to turn off memory tracking
#define MEM_TRACKER_ENABLED

// General includes
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

// Printing buffer size
#define MemTraceBuffSize 256

// Singleton helper class
class MemTrace
{
public:
	// displays a printf to the output window
	static void out(const char * const fmt, ...) 
	{
		MemTrace *pTrace = MemTrace::privGetInstance();
		assert(pTrace);

		pTrace->mtx.lock();

			va_list args;

		#pragma warning( push )
			#pragma warning( disable : 26492 )
			#pragma warning( disable : 26481 )
			va_start(args, fmt);
		#pragma warning( pop )

			vsprintf_s(&pTrace->privBuff[0], MemTraceBuffSize, fmt, args);
			OutputDebugString(&pTrace->privBuff[0]);

			//va_end(args);
			args = static_cast<va_list> (nullptr);

		pTrace->mtx.unlock();
	}

	// Big Six
	MemTrace() noexcept
	{
		memset(&privBuff[0], 0, MemTraceBuffSize);
	}
	MemTrace(const MemTrace &) = delete;
	MemTrace(MemTrace &&) = delete;
	MemTrace & operator = (const MemTrace &) = delete;
	MemTrace & operator = (MemTrace &&) = delete;
	~MemTrace() = default;

private:
	static MemTrace *privGetInstance() noexcept
	{
		// This is where its actually stored (BSS section)
		static MemTrace helper;
		return &helper;
	}
	char privBuff[MemTraceBuffSize];
	std::mutex mtx;
};

// Deprecated in v.2.23
	//Placement new - Macro Trick
	//template<typename T, typename... Ts>
	//inline T * _MemTrackerPlacement(void * Where, Ts... args)
	//{
	//	return new(Where) T(args...);
	//}

// Deprecated in v.2.23
	//Need to use this if MemTracking is enabled
	//#define placement_new(WHERE, TYPE, ...) _MemTrackerPlacement<TYPE>(WHERE, __VA_ARGS__)
	
#define PLACEMENT_NEW_BEGIN __pragma(push_macro("new")) 
#define PLACEMENT_NEW_END __pragma(pop_macro("new"))

#if defined(MEM_TRACKER_ENABLED)
	#define MEMORY_TRACKING_ENABLE_STRING "enabled         "
#else
	#define MEMORY_TRACKING_ENABLE_STRING "--> DISABLED <--"
#endif

#if defined(_DEBUG) && defined(MEM_TRACKER_ENABLED)
	#define _CRTDBG_MAP_ALLOC  
	#include <stdlib.h>  
	#include <crtdbg.h>  

	#define new new( _NORMAL_BLOCK , __FILE__ , __LINE__ )

#else
#pragma warning( push )
	#pragma warning( disable : 4820 )
	#pragma warning( disable : 4577 )
	#pragma warning( disable : 4987 )
	#include <new>
	#pragma warning( pop )
	#define new  new
#endif



#ifdef MEM_TRACKER_ENABLED

	#ifdef _DEBUG

		#define MEM_TRACKER_PROCESS_END const int _leakCount = _CrtDumpMemoryLeaks(); \
										MemTrace::out("\n"); \
										MemTrace::out("--------------------------------\n"); \
										printf("\n"); \
										printf("--------------------------------\n"); \
										if (_leakCount) { \
										MemTrace::out(">>> Memory Tracking: FAIL <<<<<<\n"); \
										printf(">>> Memory Tracking: FAIL <<<<<<\n"); }\
										else {\
										MemTrace::out("    Memory Tracking: passed \n"); \
										printf("    Memory Tracking: passed \n"); }\
										MemTrace::out("--------------------------------\n"); \
										MemTrace::out("    Memory Tracking: end()      \n"); \
										MemTrace::out("--------------------------------\n"); \
										printf("--------------------------------\n"); \
										printf("    Memory Tracking: end()      \n"); \
										printf("--------------------------------\n"); \
										printf("\n"); 

		#define MEM_TRACKER_PROCESS		MemTrace::out("\n"); \
										MemTrace::out("****************************************\n"); \
										MemTrace::out("**      Framework: %s               **\n", FRAMEWORK_VER); \
										MemTrace::out("**   C++ Compiler: %d          **\n", _MSC_FULL_VER); \
										MemTrace::out("**    Windows SDK: %s       **\n", WINDOWS_TARGET_PLATFORM); \
										MemTrace::out("**   Mem Tracking: %s   **\n", MEMORY_TRACKING_ENABLE_STRING); \
										MemTrace::out("****************************************\n"); \
										MemTrace::out("\n");\
										printf("\n"); \
										printf("****************************************\n"); \
										printf("**      Framework: %s               **\n", FRAMEWORK_VER); \
										printf("**   C++ Compiler: %d          **\n", _MSC_FULL_VER); \
										printf("**    Windows SDK: %s       **\n", WINDOWS_TARGET_PLATFORM); \
										printf("**   Mem Tracking: %s   **\n", MEMORY_TRACKING_ENABLE_STRING); \
										printf("****************************************\n"); \
										printf("\n");\
										MemTrace::out("--------------------------------\n"); \
										MemTrace::out("    Memory Tracking: start()    \n"); \
										MemTrace::out("--------------------------------\n"); \
										MemTrace::out("\n"); \
										printf("--------------------------------\n"); \
										printf("    Memory Tracking: start()    \n"); \
										printf("--------------------------------\n"); \
										printf("\n");
									
	#else
		#define MEM_TRACKER_PROCESS		MemTrace::out("\n"); \
										MemTrace::out("****************************************\n"); \
										MemTrace::out("**      Framework: %s               **\n", FRAMEWORK_VER); \
										MemTrace::out("**   C++ Compiler: %d          **\n", _MSC_FULL_VER); \
										MemTrace::out("**    Windows SDK: %s       **\n", WINDOWS_TARGET_PLATFORM); \
										MemTrace::out("**   Mem Tracking: %s   **\n", MEMORY_TRACKING_ENABLE_STRING); \
										MemTrace::out("****************************************\n"); \
										MemTrace::out("\n");\
										printf("\n"); \
										printf("****************************************\n"); \
										printf("**      Framework: %s               **\n", FRAMEWORK_VER); \
										printf("**   C++ Compiler: %d          **\n", _MSC_FULL_VER); \
										printf("**    Windows SDK: %s       **\n", WINDOWS_TARGET_PLATFORM); \
										printf("**   Mem Tracking: %s   **\n", MEMORY_TRACKING_ENABLE_STRING); \
										printf("****************************************\n"); \
										printf("\n");
	  #define MEM_TRACKER_PROCESS_END

	#endif

#else
	#define MEM_TRACKER_PROCESS		MemTrace::out("\n"); \
										MemTrace::out("****************************************\n"); \
										MemTrace::out("**      Framework: %s               **\n", FRAMEWORK_VER); \
										MemTrace::out("**   C++ Compiler: %d          **\n", _MSC_FULL_VER); \
										MemTrace::out("**    Windows SDK: %s       **\n", WINDOWS_TARGET_PLATFORM); \
										MemTrace::out("**   Mem Tracking: %s   **\n", MEMORY_TRACKING_ENABLE_STRING); \
										MemTrace::out("****************************************\n"); \
										MemTrace::out("\n");\
										printf("\n"); \
										printf("****************************************\n"); \
										printf("**      Framework: %s               **\n", FRAMEWORK_VER); \
										printf("**   C++ Compiler: %d          **\n", _MSC_FULL_VER); \
										printf("**    Windows SDK: %s       **\n", WINDOWS_TARGET_PLATFORM); \
										printf("**   Mem Tracking: %s   **\n", MEMORY_TRACKING_ENABLE_STRING); \
										printf("****************************************\n"); \
										printf("\n");
	#define MEM_TRACKER_END
	#define MEM_TRACKER_PROCESS_END
#endif

// Note: MemTrackerMain.h
//        Has to be the first line of main.cpp not main()
//		  main.cpp has to be first in link in project
//        reorder manually .vcxproj to insure that this is true


#define MEM_TRACKER_BEGIN \
	static struct _StaticMem \
	{ \
		 _StaticMem() { MEM_TRACKER_PROCESS } \
		~_StaticMem() { MEM_TRACKER_PROCESS_END } \
		 _StaticMem(const _StaticMem &) = delete; \
		 _StaticMem(_StaticMem &&) = delete; \
		 _StaticMem &operator = (const _StaticMem &) = delete; \
         _StaticMem &operator = (_StaticMem &&) = delete; \
	} _StaticMemInst;


//#endif

#endif

// ---  End of File ---------------
