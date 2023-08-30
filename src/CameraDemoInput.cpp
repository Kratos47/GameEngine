//----------------------------------------------------------------------------
// Copyright 2019, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "CameraDemoInput.h"
#include "InputMan.h"
#include "Keyboard.h"
#include "CameraMan.h"

void CameraDemoInput()
{
	Keyboard *key = InputMan::GetKeyboard();


	if (key->GetKeyState(Keyboard::KEY_SPACE))
	{
		Camera* pCamera = CameraMan::GetCurrent(Camera::Type::PERSPECTIVE_3D);

		if (pCamera->getName() == Camera::Name::CAMERA_0)
		{
			CameraMan::SetCurrent(Camera::Name::CAMERA_1, Camera::Type::PERSPECTIVE_3D);
		}
		else
		{
			CameraMan::SetCurrent(Camera::Name::CAMERA_0, Camera::Type::PERSPECTIVE_3D);
		}
	}

	if (key->GetKeyState(Keyboard::KEY_0))
	{
		CameraMan::SetCurrent(Camera::Name::CAMERA_0, Camera::Type::PERSPECTIVE_3D);
	}

	if (key->GetKeyState(Keyboard::KEY_1))
	{
		CameraMan::SetCurrent(Camera::Name::CAMERA_1, Camera::Type::PERSPECTIVE_3D);
	}

	if (key->GetKeyState(Keyboard::KEY_2))
	{
		CameraMan::SetCurrent(Camera::Name::CAMERA_2, Camera::Type::PERSPECTIVE_3D);
	}

	if (key->GetKeyState(Keyboard::KEY_3))
	{
		CameraMan::SetCurrent(Camera::Name::CAMERA_3, Camera::Type::PERSPECTIVE_3D);
	}

	//if (key->GetKeyState(Keyboard::KEY_W))
	//{
	//	Camera *pCam = CameraMan::Find(Camera::Name::CAMERA_0);
	//	
	//	Vect pos;
	//	Vect tar;
	//	Vect up;
	//	Vect upNorm;
	//	Vect forwardNorm;
	//	Vect rightNorm;

	//	pCam->GetHelper(up,tar, pos, upNorm, forwardNorm, rightNorm);

	//	// OK, now 3 points... pos, tar, up
	//	//     now 3 normals... upNorm, forwardNorm, rightNorm

	//	// Let's Roll

	//	float delta = 0.04f;
	//	up += delta * upNorm;
	//	pos += delta * upNorm;
	//	tar += delta * upNorm;

	//	pCam->SetHelper(up, tar, pos);

	//}

	//if (key->GetKeyState(Keyboard::KEY_S))
	//{
	//	Camera *pCam = CameraMan::Find(Camera::Name::CAMERA_0);

	//	Vect pos;
	//	Vect tar;
	//	Vect up;
	//	Vect upNorm;
	//	Vect forwardNorm;
	//	Vect rightNorm;

	//	pCam->GetHelper(up, tar, pos, upNorm, forwardNorm, rightNorm);

	//	// OK, now 3 points... pos, tar, up
	//	//     now 3 normals... upNorm, forwardNorm, rightNorm

	//	// Let's Roll

	//	float delta = -0.04f;
	//	up += delta * upNorm;
	//	pos += delta * upNorm;
	//	tar += delta * upNorm;

	//	pCam->SetHelper(up, tar, pos);

	//}

	//if (key->GetKeyState(Keyboard::KEY_D))
	//{
	//	Camera *pCam = CameraMan::Find(Camera::Name::CAMERA_0);

	//	Vect pos;
	//	Vect tar;
	//	Vect up;
	//	Vect upNorm;
	//	Vect forwardNorm;
	//	Vect rightNorm;

	//	pCam->GetHelper(up, tar, pos, upNorm, forwardNorm, rightNorm);

	//	// OK, now 3 points... pos, tar, up
	//	//     now 3 normals... upNorm, forwardNorm, rightNorm

	//	// Let's Roll

	//	float delta = 0.04f;
	//	up += delta * rightNorm;
	//	pos += delta * rightNorm;
	//	tar += delta * rightNorm;

	//	pCam->SetHelper(up, tar, pos);

	//}

	//if (key->GetKeyState(Keyboard::KEY_A))
	//{
	//	Camera *pCam = CameraMan::Find(Camera::Name::CAMERA_0);

	//	Vect pos;
	//	Vect tar;
	//	Vect up;
	//	Vect upNorm;
	//	Vect forwardNorm;
	//	Vect rightNorm;

	//	pCam->GetHelper(up, tar, pos, upNorm, forwardNorm, rightNorm);

	//	// OK, now 3 points... pos, tar, up
	//	//     now 3 normals... upNorm, forwardNorm, rightNorm

	//	// Let's Roll

	//	float delta = -0.04f;
	//	up += delta * rightNorm;
	//	pos += delta * rightNorm;
	//	tar += delta * rightNorm;

	//	pCam->SetHelper(up, tar, pos);

	//}

	
	//if (key->GetKeyState(Keyboard::KEY_ARROW_DOWN))
	//{

	//	//Orbit_Up()

	//	Camera* pCam = CameraMan::Find(Camera::Name::CAMERA_0);


	//	Vect pos, tar, up;

	//	Vect upNorm, forwardNorm, rightNorm;

	//	pCam->GetHelper(up, tar, pos, upNorm, forwardNorm, rightNorm);

	//	Matrix Trans(Matrix::Trans, tar);
	//	Matrix NegTrans(Matrix::Trans, -tar);
	//	Matrix Rot;
	//	Rot.set(rightNorm, -0.01f);

	//	Matrix M = NegTrans * Rot * Trans;

	//	up = up * M;
	//	pos = pos * M;
	//	tar = tar * M;

	//	pCam->SetHelper(up, tar, pos);
	//}

	//if (key->GetKeyState(Keyboard::KEY_ARROW_UP))
	//{
	//	//void CameraNode::Orbit_Down()

	//	Camera* pCam = CameraMan::Find(Camera::Name::CAMERA_0);

	//	Vect pos, tar, up;

	//	Vect upNorm, forwardNorm, rightNorm;

	//	pCam->GetHelper(up, tar, pos, upNorm, forwardNorm, rightNorm);

	//	Matrix Trans(Matrix::Trans, tar);
	//	Matrix NegTrans(Matrix::Trans, -tar);
	//	Matrix Rot;
	//	Rot.set(rightNorm, 0.01f);

	//	Matrix M = NegTrans * Rot * Trans;

	//	up = up * M;
	//	pos = pos * M;
	//	tar = tar * M;

	//	pCam->SetHelper(up, tar, pos);
	//}

	//if (key->GetKeyState(Keyboard::KEY_ARROW_RIGHT))
	//{
	//	//void CameraNode::Orbit_Right()

	//	Camera* pCam = CameraMan::Find(Camera::Name::CAMERA_0);

	//	Vect pos, tar, up;

	//	Vect upNorm, forwardNorm, rightNorm;

	//	pCam->GetHelper(up, tar, pos, upNorm, forwardNorm, rightNorm);

	//	Matrix Trans(Matrix::Trans, tar);
	//	Matrix NegTrans(Matrix::Trans, -tar);
	//	Matrix Rot;
	//	Rot.set(upNorm, +0.01f);

	//	Matrix M = NegTrans * Rot * Trans;

	//	up = up * M;
	//	pos = pos * M;
	//	tar = tar * M;

	//	pCam->SetHelper(up, tar, pos);
	//}

	//if (key->GetKeyState(Keyboard::KEY_ARROW_LEFT))
	//{
	//	//void CameraNode::Orbit_Left()

	//	Camera* pCam = CameraMan::Find(Camera::Name::CAMERA_0);

	//	Vect pos, tar, up;

	//	Vect upNorm, forwardNorm, rightNorm;

	//	pCam->GetHelper(up, tar, pos, upNorm, forwardNorm, rightNorm);


	//	Matrix Trans(Matrix::Trans, tar);
	//	Matrix NegTrans(Matrix::Trans, -tar);
	//	Matrix Rot;
	//	Rot.set(upNorm, -0.01f);

	//	Matrix M = NegTrans * Rot * Trans;

	//	up = up * M;
	//	pos = pos * M;
	//	tar = tar * M;

	//	pCam->SetHelper(up, tar, pos);
	//}

}

//--- End of File -------------------------------------------------------------
