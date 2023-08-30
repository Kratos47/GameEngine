//----------------------------------------------------------------------------
// Copyright 2019, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include <assert.h>

#include "Model.h"
#include "GraphicsObject_WireframeConstantColor.h"
#include "ShaderObject.h"
#include "Camera.h"

#include "CameraMan.h"
#include "CameraNode.h"
#include "Camera.h"

extern bool boundingSphereOn;

GraphicsObject_WireframeConstantColor::GraphicsObject_WireframeConstantColor(Model *model, ShaderObject *pShaderObj, Vect &wireColor)
: GraphicsObject(model, pShaderObj)
{
	assert(model);
	assert(pShaderObj);
	this->pWireColor = new Vect(wireColor);
}

GraphicsObject_WireframeConstantColor::~GraphicsObject_WireframeConstantColor()
{
	delete this->pWireColor;
}

void GraphicsObject_WireframeConstantColor::SetState()
{	
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);
}

void GraphicsObject_WireframeConstantColor::SetDataGPU()
{	     
	// Draw Me: -----------------------------------------

	// set the vao
	glBindVertexArray(this->GetModel()->vao);
	
		// Set state
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDisable(GL_CULL_FACE);

	// use this shader
	this->pShaderObj->SetActive();

	// Get active camera
	Camera* pCamera = CameraMan::GetCurrent(Camera::Type::PERSPECTIVE_3D);


	Matrix world = this->GetWorld();
	Matrix view = pCamera->getViewMatrix();
	Matrix proj = pCamera->getProjMatrix();

	glUniformMatrix4fv(this->pShaderObj->GetLocation("proj_matrix"), 1, GL_FALSE, (float *)&proj);
	glUniformMatrix4fv(this->pShaderObj->GetLocation("view_matrix"), 1, GL_FALSE, (float *)&view);
	glUniformMatrix4fv(this->pShaderObj->GetLocation("world_matrix"), 1, GL_FALSE, (float *)&world);

	glUniform4fv(this->pShaderObj->GetLocation("wire_color"), 1, (float *)this->pWireColor);
}

void GraphicsObject_WireframeConstantColor::Draw( )
{	    
	if (boundingSphereOn == true)
	{
		//The starting point of the IBO
		glDrawElements(GL_TRIANGLES, 3 * this->GetModel()->numTris, GL_UNSIGNED_INT, 0);
	}
}

void GraphicsObject_WireframeConstantColor::RestoreState()
{	     
	// Restore state
	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

//--- End of File -------------------------------------------------------------
