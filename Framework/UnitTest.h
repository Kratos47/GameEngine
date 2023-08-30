//-----------------------------------------------------------------------------
// Copyright Ed Keenan 2019
//----------------------------------------------------------------------------- 
// UnitTest 
//    v.2.1.0
//    v.2.1.1 - fixed C5038 warning
//    v.2.2   - GetStats()
//    v.2.3   - CHECK_EQUAL() bug fix
//    v.2.4   - Print output to console and debug window
//    v.2.5   - Add to End of list
//    v.2.5.1 - Teardown - rework
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

#ifndef CPP_UNIT_H
#define CPP_UNIT_H

#include <assert.h>
#include <stdio.h>

//---------------------------------------------------------------------------------------
// UnitTrace - output to debug window
//             isolated from other systems
//
// NOTE: You need to set your project settings
//       Character Set -> Use Multi-Byte Character Set
//
//---------------------------------------------------------------------------------------

#pragma warning( push )
	#pragma warning( disable : 4365 )
	#include <atomic>
#pragma warning( pop ) 

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

#define UnitTraceBuffSize 256

// Singleton helper class
class UnitTrace
{
public:
	// displays a printf to the output window
	static void out(const char * const fmt, ...)
	{
		UnitTrace *pTrace = UnitTrace::privGetInstance();
		assert(pTrace);

		pTrace->mtx.lock();

			va_list args;

		#pragma warning( push )
			#pragma warning( disable : 26492 )
			#pragma warning( disable : 26481 )
			va_start(args, fmt);
		#pragma warning( pop )

			vsprintf_s(&pTrace->privBuff[0], UnitTraceBuffSize, fmt, args);
			OutputDebugString(&pTrace->privBuff[0]);

			//va_end(args);
			args = static_cast<va_list> (nullptr);

		pTrace->mtx.unlock();
	}

	// Big Six
	UnitTrace() noexcept
	{
		memset(&privBuff[0], 0x0, UnitTraceBuffSize);
	}
	UnitTrace(const UnitTrace &) = delete;
	UnitTrace(UnitTrace &&) = delete;
	UnitTrace & operator = (const UnitTrace &) = delete;
	UnitTrace & operator = (UnitTrace &&) = delete;
	~UnitTrace() = default;

private:
	static UnitTrace *privGetInstance() noexcept
	{
		// This is where its actually stored (BSS section)
		static UnitTrace helper;
		return &helper;
	}
	char privBuff[UnitTraceBuffSize];
	std::mutex mtx;
};

//---------------------------------------------------------------------------------------
// Utility class - for testing floats
//
// Needs <cmath> - fabs()
//
//---------------------------------------------------------------------------------------
#include <cmath>

class UnitUtility
{
public:
	static bool AreEqual(float a, float b, float epsilon = 0.001f) noexcept
	{
		return (fabs(a - b) < epsilon);
	}

	static bool AreEqual(double a, double b, double epsilon = 0.001f) noexcept
	{
		return (fabs(a - b) < epsilon);
	}
};

//---------------------------------------------------------------------------------------
// Unit Tests
//
// Needs this header
//
//---------------------------------------------------------------------------------------
struct UnitStats
{
	UnitStats() noexcept
		:testCount(0),
		testPass(0),
		testFail(0), 
		indvAsserts(0)
	{
	}

	// Big four
	UnitStats(const UnitStats &) = default;
	UnitStats & operator = (const UnitStats &) = default;
	~UnitStats() = default;

	// data: ------------------
	int testCount;
	int testPass;
	int testFail;
	int indvAsserts;
};
struct UnitData
{
	UnitData() noexcept
		: pMemberName(nullptr),
		pSourceFilePath(nullptr),
		sourceLineNumber(0),
		result(false),
		pad0(0),
		pad1(0),
		pad2(0)
	{
	}

	// Big four
	UnitData(const UnitData &) = default;
	UnitData & operator = (const UnitData &) = default;
	~UnitData() = default;

	// data: -----------------
	const char *pMemberName;
	const char *pSourceFilePath;
	int sourceLineNumber;
	bool result;
	char pad0;
	char pad1;
	char pad2;
};

class UnitSLink
{
public:
	UnitSLink() noexcept
		:_pNext(nullptr)
	{
	}
	// Big Six
	UnitSLink(const UnitSLink &) = delete;
	UnitSLink(UnitSLink &&) = delete;
	UnitSLink & operator = (const UnitSLink &) = delete;
	UnitSLink & operator = (UnitSLink &&) = delete;
	virtual ~UnitSLink() = default;

	static void AddToFront(UnitSLink *&pRoot, UnitSLink &rNode ) noexcept
	{
		if (pRoot == nullptr)
		{		
			pRoot = &rNode;
			assert(rNode._pNext == nullptr);
		}
		else
		{
			UnitSLink *pTmp = pRoot;
			pRoot = &rNode;
			rNode._pNext = pTmp;
		}
	}

	static void AddToEnd(UnitSLink *&pRoot, UnitSLink *pNode) noexcept
	{

		if (nullptr == pRoot)
		{
			pRoot = pNode;

			assert(pNode != nullptr);
			assert(pNode->_pNext == nullptr);
		}
		else
		{
			UnitSLink *pTmpX = pRoot;

			while (pTmpX != nullptr)
			{
				if (pTmpX->_pNext == nullptr)
				{
					// at the end
					pTmpX->_pNext = pNode;
					pNode->_pNext = nullptr;
				}

				pTmpX = pTmpX->_pNext;
			}
		}
		
	}

public:
	// Data
	UnitSLink *_pNext;
};

class Test : public UnitSLink
{
public:
	Test(const char * const pTestName) noexcept;

	// Big four
	Test() = delete;
	Test(const Test &) = delete;
	Test(Test &&) = delete;
	Test & operator = (const Test &) = delete;
	Test & operator = (Test &&) = delete;
	~Test() = default;

	virtual void run(UnitData &, UnitStats &) = 0;
	// For Tests with NO Teardowns... do nothing
	virtual void teardown() 
	{
	};

public:
	const char * const pName;
	Test *testFunc;
};

class TestRegistry
{
public:
	// Big four
	TestRegistry(const TestRegistry &) = delete;
	TestRegistry(TestRegistry &&) = delete;
	TestRegistry & operator = (const TestRegistry &) = delete;
	TestRegistry & operator = (TestRegistry &&) = delete;
	~TestRegistry() = default;

	static void AddTest(UnitSLink *pTest) noexcept
	{
		//UnitTrace::out("TestRegistry:AddTest(%s) \n", pTest->pName);
		TestRegistry &rRegistry = TestRegistry::privGetInstance();
		
		// add to End	
		UnitSLink::AddToEnd(rRegistry.pRoot, pTest);
	}
	static void RunTests()
	{
#ifdef _DEBUG

		UnitTrace::out("------------------- Testing DEBUG ------------------------\n");

	#if _M_X64
			const char * const mode = "x64 Debug";
	#else
			const char * const mode = "x86 Debug";
	#endif

#else

	#if _M_X64
			const char * const mode = "x64 Release";
	#else
			const char * const mode = "x86 Release";
	#endif

	#ifdef MR_FAST
			UnitTrace::out("------------------- Testing MR_FAST ----------------------\n");
	#else
			UnitTrace::out("------------------- Testing RELEASE ----------------------\n");
	#endif

#endif

		TestRegistry &rRegistry = TestRegistry::privGetInstance();
		UnitSLink *pTmp = rRegistry.pRoot;

		while (pTmp != nullptr)
		{
			rRegistry._UnitStats.testCount++;

			// downcast to the test
			Test *pTest = (Test *)(pTmp);

			assert(pTest);
			printf("Test:%s \n", pTest->pName);

			// Needed to be added - for fencing issues between tests
			// Release rearranges.. and affects timing

			// Forces a Fence... 
			atomic_thread_fence(std::memory_order_acq_rel);

			// run the test
			rRegistry._UnitData.result = true;
			
			assert(pTest->testFunc != nullptr);
			pTest->testFunc->run(rRegistry._UnitData, rRegistry._UnitStats);

			// Forces a Fence... 
			atomic_thread_fence(std::memory_order_acq_rel);

			// Always call the teardown
			pTest->testFunc->teardown();

			// Forces a Fence... 
			atomic_thread_fence(std::memory_order_acq_rel);

		
			if (rRegistry._UnitData.result == true)
			{
				rRegistry._UnitStats.testPass++;
			}
			else
			{
				rRegistry._UnitStats.testFail++;
			}

			// next test
			pTmp = pTmp->_pNext;
		}

		UnitTrace::out("\n");
		UnitTrace::out(" (%s) testPass: %d\n",mode, rRegistry._UnitStats.testPass);
		UnitTrace::out(" (%s) testFail: %d\n",mode, rRegistry._UnitStats.testFail);
		UnitTrace::out("\n");
		UnitTrace::out(" testCount: %d\n", rRegistry._UnitStats.testCount);
		UnitTrace::out("indvChecks: %d\n", rRegistry._UnitStats.indvAsserts);
		UnitTrace::out("\n");
		UnitTrace::out("-----------------\n");

		printf("\n");
		printf(" (%s) testPass: %d\n", mode, rRegistry._UnitStats.testPass);
		printf(" (%s) testFail: %d\n", mode, rRegistry._UnitStats.testFail);
		printf("\n");
		printf(" testCount: %d\n", rRegistry._UnitStats.testCount);
		printf("indvChecks: %d\n", rRegistry._UnitStats.indvAsserts);
		printf("\n");
		printf("-----------------\n");

	}

	static UnitStats GetStats() noexcept
	{
		TestRegistry &rRegistry = TestRegistry::privGetInstance();
		return rRegistry._UnitStats;
	}

private:
	TestRegistry() noexcept
	{
		this->pRoot = nullptr;
	}

	static TestRegistry &privGetInstance() noexcept
	{
		static TestRegistry tRegistry;
		return tRegistry;
	}

// Data: ------------------------
	UnitData _UnitData;
	UnitStats _UnitStats;
	UnitSLink *pRoot;
};


// a trick to create a c-string
#define STRING_ME(s) #s

// Allow conditional expressions that are constant.
#pragma warning( disable : 4127 )

// Increments total check count
// Creates a clickable format in the output window for failure
// Abort test on first fail
#define CHECK( condition ) \
{ \
	_UnitStats.indvAsserts++;\
	if( !( condition ) ) \
	{ \
		_UnitData.result = false;  \
		_UnitData.pMemberName = this->pName; \
		_UnitData.pSourceFilePath = __FILE__; \
		_UnitData.sourceLineNumber = __LINE__; \
		UnitTrace::out("%s(%d): %s \n", _UnitData.pSourceFilePath, _UnitData.sourceLineNumber, _UnitData.pMemberName ); \
		printf("%s(%d): %s \n", _UnitData.pSourceFilePath, _UnitData.sourceLineNumber, _UnitData.pMemberName ); \
		/*this->teardown();*/ \
		return; \
	} \
	else\
	{\
	}\
}


#define CHECK_EQUAL( value1, value2 ) \
{ \
	_UnitStats.indvAsserts++;\
	if( !( (value1) == (value2) ) ) \
	{ \
		_UnitData.result = false;  \
		_UnitData.pMemberName = this->pName; \
		_UnitData.pSourceFilePath = __FILE__; \
		_UnitData.sourceLineNumber = __LINE__; \
		UnitTrace::out("%s(%d): %s\n", _UnitData.pSourceFilePath, _UnitData.sourceLineNumber, _UnitData.pMemberName  ); \
		printf("%s(%d): %s\n", _UnitData.pSourceFilePath, _UnitData.sourceLineNumber, _UnitData.pMemberName  ); \
		/*this->teardown();*/ \
		return; \
	} \
	else\
	{\
	}\
}

// Here is the magic
//
// 1) it creates a class, <TestName>_Test
//    it is derived from Test class
// 2) it defines the constructor and calls the base constructor with the name
//    it registers the name of the test to a single class (TestRegistry)
// 3) It also overloads the Test::run() method with the body of the macro

#define TEST_END }

#define TEST( TestName, GroupName, TestFlagEnable ) \
class TestName##_##GroupName##_Test : public Test \
{ \
	public: \
		TestName##_##GroupName##_Test() noexcept : \
		Test( STRING_ME(TestName##_##GroupName##_Test)) \
		{ \
		}; \
		TestName##_##GroupName##_Test & operator = (const TestName##_##GroupName##_Test &) = delete;\
		TestName##_##GroupName##_Test(const TestName##_##GroupName##_Test &) = delete;\
	\
	void run(UnitData &_UnitData, UnitStats &_UnitStats) override;\
} TestName##_##GroupName##_instance; \
\
void TestName##_##GroupName##_Test::run( UnitData &_UnitData, UnitStats &_UnitStats ) \
{ \
	if(!TestFlagEnable) \
	{ \
		_UnitStats.testCount--; \
		_UnitStats.testPass--; \
	} \
	else 

#define TEST_WITH_TEARDOWN( TestName, GroupName, TestFlagEnable ) \
class TestName##_##GroupName##_Test : public Test \
{ \
	public: \
		TestName##_##GroupName##_Test() noexcept : \
		Test( STRING_ME(TestName##_##GroupName##_Test)) \
		{ \
		}; \
		TestName##_##GroupName##_Test & operator = ( const TestName##_##GroupName##_Test & ) = delete;\
		TestName##_##GroupName##_Test( const TestName##_##GroupName##_Test & ) = delete;\
	\
	void run(UnitData &_UnitData, UnitStats &_UnitStats) override;\
	void teardown() override;\
} TestName##_##GroupName##_instance; \
\
void TestName##_##GroupName##_Test::run( UnitData &_UnitData, UnitStats &_UnitStats ) \
{ \
	if (!TestFlagEnable) \
	{ \
		_UnitStats.testCount--; \
		_UnitStats.testPass--; \
	} \
	else


#define TEST_TEARDOWN( TestName, GroupName ) \
void TestName##_##GroupName##_Test::teardown() 


#endif

// ---  End of File ---------------
