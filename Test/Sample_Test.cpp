//-----------------------------------------------------------------------------
// Copyright Ed Keenan 2019
//----------------------------------------------------------------------------- 

//---------------------------------------------------------------------------
// HEADER FILES:
//---------------------------------------------------------------------------
#include "Framework.h"
#include "_UnitTestConfiguration.h"


//---------------------------------------------------------------------------
// Test Files:
//---------------------------------------------------------------------------

#if Test_Enable
TEST(sample, sample_group, Test_Enable)
{
	int x;
	x = 5;

	CHECK(x == 5);

} TEST_END
#endif


// ---  End of File ---------------
