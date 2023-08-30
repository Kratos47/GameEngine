//----------------------------------------------------------------------------
// Copyright Ed Keenan 2019
//----------------------------------------------------------------------------

#ifndef INPUT_MAN_H
#define INPUT_MAN_H

#include "Keyboard.h"
#include "Mouse.h"

class InputMan
{
public:
	InputMan(const InputMan &) = delete;
	InputMan & operator = (const InputMan &) = delete;
	~InputMan();

	static void Create();
	static void Destroy();
	static Keyboard *GetKeyboard();
	static Mouse *GetMouse();

	// Update input states
	static void Update();

	static void CamInputUpdate();

private:
	InputMan();

	static InputMan *privInstance();
	static void privCreate();

	// Data
	static InputMan *poInputMan;
	Keyboard		*poKeyboard;
	Mouse			*poMouse;

	bool	 PrevState;
	
	char a;
	char b;
	char c;
};

#endif

// --- End of File --------------------------------------------------
