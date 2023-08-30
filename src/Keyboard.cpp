//----------------------------------------------------------------------------
// Copyright Ed Keenan 2019
//----------------------------------------------------------------------------

#include <assert.h>

#include "Game.h"
#include "Keyboard.h"

extern Game *pGame;

// Use this to read keyboard
bool Keyboard::GetKeyState( Keyboard::Key key )
{
	bool value;

	
	if( pGame->GetKey(key) == GLFW_PRESS )
	{
		value = true;
	}
	else
	{
		value = false;
	}

	return value;
}

// --- End of File --------------------------------------------------
