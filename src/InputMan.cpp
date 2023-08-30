//----------------------------------------------------------------------------
// Copyright Ed Keenan 2019
//----------------------------------------------------------------------------

#include <assert.h>
#include "InputMan.h"
//#include "CameraMan.h"
//#include "CameraNode.h"

InputMan *InputMan::poInputMan = nullptr;

void InputMan::Create( )
{
	InputMan::privCreate( );
}

void InputMan::Destroy()
{
	InputMan *pMan = InputMan::privInstance();
	assert(pMan);

	delete pMan->poInputMan;
}

InputMan::InputMan( )
{
	this->poKeyboard = new Keyboard(  );
	assert(this->poKeyboard);
}

void InputMan::privCreate(   )
{
	InputMan::poInputMan = new InputMan(  );
	assert(poInputMan);
}

InputMan::~InputMan()
{
	delete this->poKeyboard;
}

Keyboard *InputMan::GetKeyboard()
{
	InputMan *pMan = InputMan::privInstance();
	return pMan->poKeyboard;
}

Mouse *InputMan::GetMouse()
{
	InputMan *pMan = InputMan::privInstance();
	return pMan->poMouse;
}

InputMan *InputMan::privInstance()
{
	assert( poInputMan );
	return poInputMan;
}

//void InputMan::Update()
//{
//	// Check for Camera movement updates
//	InputMan::CamInputUpdate();
//}

//void InputMan::CamInputUpdate()
//{
//	InputMan *pInstance = InputMan::privInstance();
//	
//	Keyboard *pKeyboard = InputMan::GetKeyboard();
//
//	CameraNode *pCamNode = CameraMan::GetCurrent();
//
//	bool StateCurr = pKeyboard->GetKeyState(Keyboard::Key::KEY_SPACE);
//	
//	// If Space was pressed 
//	
//	if (StateCurr == true && pInstance->PrevState == false)
//	{
//		// next camera
//		CameraManager::CycleCamera();
//	}
//
//	// Store current state in previous state
//	pInstance->PrevState = StateCurr;
//
//	// tilt camera up
//	if (pKeyboard->GetKeyState(Keyboard::Key::KEY_ARROW_UP) == true)
//	{
//		pCamNode->Orbit_Up();
//	}
//	// tilt camera down
//	if (pKeyboard->GetKeyState(Keyboard::Key::KEY_ARROW_DOWN) == true)
//	{
//		pCamNode->Orbit_Down();
//	}
//	// tilt camera right
//	if (pKeyboard->GetKeyState(Keyboard::Key::KEY_ARROW_RIGHT) == true)
//	{
//		pCamNode->Orbit_Right();
//	}
//	// tilt camera left
//	if (pKeyboard->GetKeyState(Keyboard::Key::KEY_ARROW_LEFT) == true)
//	{
//		pCamNode->Orbit_Left();
//	}
//	// Move camera Forward
//	if (pKeyboard->GetKeyState(Keyboard::Key::KEY_W) == true)
//	{
//		pCamNode->MoveForward();
//	}
//	// Move camera back
//	if (pKeyboard->GetKeyState(Keyboard::Key::KEY_S) == true)
//	{
//		pCamNode->MoveBack();
//	}
//	// Move camera right
//	if (pKeyboard->GetKeyState(Keyboard::Key::KEY_D) == true)
//	{
//		pCamNode->MoveRight();
//	}
//	// Move camera left
//	if (pKeyboard->GetKeyState(Keyboard::Key::KEY_A) == true)
//	{
//		pCamNode->MoveLeft();
//	}
//}

// --- End of File --------------------------------------------------
