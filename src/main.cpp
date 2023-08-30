//----------------------------------------------------------------------------
// Copyright Ed Keenan 2019
//----------------------------------------------------------------------------

#include "Framework.h"
#include "Game.h"

Game *pGame = 0;
bool boundingSphereOn = false;

int CALLBACK WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	pGame = new Game("Use R,P,I,W for clip switch n to reset f to forward and b to rewind", 1800, 900);
	pGame->Run();
	delete pGame;

	return 0;
}

// ---  End of File ---------------
