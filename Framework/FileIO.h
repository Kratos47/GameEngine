//-----------------------------------------------------------------------------
// Copyright Ed Keenan 2019
//----------------------------------------------------------------------------- 
// FILE_IO  
//    v.2.1.0
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

#ifndef FILE_IO_H
#define FILE_IO_H

class FileIO
{
public:

	static void Open(const char * const pFirstName, const char * const pLastName) noexcept
	{
		assert(pFirstName);
		assert(pLastName);
		FileIO::privGetInstance()->privOpen(pFirstName, pLastName);
	}

	static void Close() noexcept
	{
		FileIO::privGetInstance()->privClose();
	}

	static FILE *GetHandle() noexcept
	{
		return FileIO::privGetInstance()->privGetHandle();
	}

private:

	void privOpen(const char * const pFirstName, const char * const pLastName) noexcept
	{
		const int length = 256;

		char pFileName[length] = { 0 };
		assert(pFileName);

		errno_t fileError(0);

		// wash the name to 0
		memset(&pFileName[0], 0, length);
		assert(strlen(pFirstName) < length / 4);
		assert(strlen(pLastName) < length / 4);

		strcat_s(&pFileName[0], length, pFirstName);
		strcat_s(&pFileName[0], length, pLastName);
		strcat_s(&pFileName[0], length, "_");

#ifdef _DEBUG
		const char *debug_string = "Debug.txt";
		strcat_s(&pFileName[0], length, debug_string);
		const char *mode = "DEBUG";
#endif

#ifdef NDEBUG
		const char *debug_string = "Release.txt";
		strcat_s(pFileName, length, debug_string);
		const char *mode = "RELEASE";
#endif

		fileError = fopen_s(&pFileHandle, &pFileName[0], "wt");
		assert(pFileHandle);

		fprintf(this->pFileHandle, "Program Assignment \n");
		fprintf(this->pFileHandle, "Name: %s%s\n", pFirstName, pLastName);
		fprintf(this->pFileHandle, "Mode: %s\n", mode);
		fprintf(this->pFileHandle, "\n");
		fprintf(this->pFileHandle, "-------------------------------------------------\n");
		fprintf(this->pFileHandle, "\n");

	}

	FILE *privGetHandle() noexcept
	{
		assert(pFileHandle);
		return this->pFileHandle;
	}
	
	static FileIO *privGetInstance() noexcept
	{
		static FileIO instance;
		return &instance;
	}

	void privClose() noexcept
	{
		errno_t fileError(0);
		assert(pFileHandle);

		fileError = fflush(this->pFileHandle);
		assert(!fileError);

		fileError = fclose(this->pFileHandle);
		this->pFileHandle = nullptr;
		assert(!fileError);
	}


	// ------------------------------------------
	// big six  
	// ------------------------------------------

	constexpr FileIO() noexcept
		:pFileHandle(nullptr)
	{

	};

	~FileIO()
	{
		if (nullptr != this->pFileHandle)
		{
			this->privClose();
		}
	};

	FileIO(FileIO &&) = delete;
	FileIO(const FileIO &) = delete;
	FileIO & operator=(const FileIO &) = delete;
	FileIO & operator=(FileIO &&) = delete;

	// ------------------------------------------
	// data
	// ------------------------------------------

	FILE *pFileHandle;
};

#endif 

// ---  End of File ---------------
