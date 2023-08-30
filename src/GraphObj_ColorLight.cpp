#include <assert.h>
#include "CameraMan.h"
#include "CameraNode.h"
#include "Camera.h"
#include "Model.h"
#include "Shader.h"
#include "GraphObj_ColorLight.h"

GraphObj_ColorLight::GraphObj_ColorLight(Model * model, ShaderObject * shader, const Vect & lightPos, const Vect & lightColor)
	: GraphicsObject(model, shader)
{
	// Is there a model and shader?
	assert(model != 0);
	assert(shader != 0);

	this->poLightPos = new Vect(lightPos);
	this->poLightColor = new Vect(lightColor);
}

GraphObj_ColorLight::~GraphObj_ColorLight()
{
	delete this->poLightPos;
	delete this->poLightColor;
}

void GraphObj_ColorLight::SetState()
{

}

void GraphObj_ColorLight::SetDataGPU()
{
	// Set shader as active
	this->pShaderObj->SetActive();
	// Set the VAO
	glBindVertexArray(this->GetModel()->vao);

	// Get active camera
	Camera *pCamera = CameraMan::GetCurrent(Camera::Type::PERSPECTIVE_3D);

	// Get world, view, and projection matrices
	Matrix world = this->GetWorld();
	Matrix view = pCamera->getViewMatrix();
	Matrix proj = pCamera->getProjMatrix();

	// Send projection matrix to GPU
	glUniformMatrix4fv(this->pShaderObj->GetLocation("proj_matrix"), 1, GL_FALSE, (float *)&proj);
	// Send view matrix to GPU
	glUniformMatrix4fv(this->pShaderObj->GetLocation("view_matrix"), 1, GL_FALSE, (float *)&view);
	// Send world matrix to GPU
	glUniformMatrix4fv(this->pShaderObj->GetLocation("world_matrix"), 1, GL_FALSE, (float *)&world);

	// Send light position vector to GPU
	glUniform3fv(this->pShaderObj->GetLocation("vLightPos"), 1, (float *)this->poLightPos);
	// Send light color vector to GPU
	glUniform4fv(this->pShaderObj->GetLocation("vColor"), 1, (float *)this->poLightColor);
}

void GraphObj_ColorLight::Draw()
{
	// Starting point of the IBO
	glDrawElements(GL_TRIANGLES, this->GetModel()->numTris * 3, GL_UNSIGNED_INT, 0);
}

void GraphObj_ColorLight::RestoreState()
{
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
}