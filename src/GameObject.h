//----------------------------------------------------------------------------
// Copyright Ed Keenan 2019
//----------------------------------------------------------------------------

#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "MathEngine.h"
#include "ShaderObject.h"
#include "Model.h"
#include "GraphicsObject.h"
#include "PCSNode.h"

class Time;
class GameObject : public PCSNode
{
public:
	GameObject(GraphicsObject* graphicsObject);
	// Big four    // Keenan(20)
	GameObject() = delete;
	GameObject(const GameObject &) = delete;
	GameObject &operator=(GameObject &) = delete;
	virtual ~GameObject();


	virtual void Draw();
	virtual void Update(Time currentTime) = 0;

	// Set / Get:
	GraphicsObject *GetGraphicsObject();
	Matrix *GetWorld();

	void SetWorld(Matrix *pWorld);

	bool GetDrawEnable();
	void SetDrawEnable(bool);

public:
	Matrix *poWorld;
	void baseUpdateBoundingSphere();

public:
	Sphere *poSphere;
	GraphicsObject  *poGraphicsObject;
	bool mDrawEnable;
	char pad[3];
};

#endif

//--- End of File ------------
