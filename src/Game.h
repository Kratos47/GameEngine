//----------------------------------------------------------------------------
// Copyright Ed Keenan 2019
//----------------------------------------------------------------------------

#ifndef GAME_H
#define GAME_H

#include <sb7.h>
#include "Engine.h"
#include "GameTime.h"
#include "Timer.h"
class Game : public Engine
{
public:
	const static int SCREEN_WIDTH = 1800;
	const static int SCREEN_HEIGHT = 900;

	// constructor
	Game(const char* windowName, const int Width, const int Height);
	~Game();

private:
	virtual void Initialize() override;
	virtual void LoadContent() override;
	virtual void Update(float currentTime) override;
	virtual void Draw() override;
	virtual void UnLoadContent() override;
	virtual void ClearBufferFunc() override;

	// No need for this...
	// virtual void onResize(int w, int h) override;

public:
	//// Nice and clean
	Timer intervalTimer;
	Timer globalTimer;
};

#endif

// ---  End of File ---------------
