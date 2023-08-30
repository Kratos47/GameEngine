#include <sb7.h>
#include "MathEngine.h"
#include "CameraMan.h"
#include "CameraNode.h"
#include "Camera.h"
#include "Model.h"
#include "Shader.h"
#include "GraphObj_SimpleColor.h"

GraphObj_SimpleColor::GraphObj_SimpleColor(Model * model, ShaderObject * shader, const Vect & colorVec)
	: GraphicsObject(model, shader)
{
	// Is there a model and shader?
	assert(model != 0);
	assert(shader != 0);

	this->pColorVec = new Vect(colorVec);
}

GraphObj_SimpleColor::~GraphObj_SimpleColor()
{
	delete this->pColorVec;
}

void GraphObj_SimpleColor::SetState()
{
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
}

void GraphObj_SimpleColor::SetDataGPU()
{
	// Set shader as active
	this->pShaderObj->SetActive();
	// Set the VAO
	glBindVertexArray(this->GetModel()->vao);

	// Get active camera
	Camera *pCamera = CameraMan::GetCurrent(Camera::Type::PERSPECTIVE_3D);


	Matrix world = this->GetWorld();
	Matrix view = pCamera->getViewMatrix();
	Matrix proj = pCamera->getProjMatrix();

	glUniformMatrix4fv(this->pShaderObj->GetLocation("proj_matrix"), 1, GL_FALSE, (float *)&proj);
	// Send world matrix to GPU
	glUniformMatrix4fv(this->pShaderObj->GetLocation("world_matrix"), 1, GL_FALSE, (float *)&world);
	// Send view matrix to GPU
	glUniformMatrix4fv(this->pShaderObj->GetLocation("view_matrix"), 1, GL_FALSE, (float *)&view);

	// Send color vector to GPU
	glUniform4fv(this->pShaderObj->GetLocation("color_vector"), 1, (float *)this->pColorVec);
}

void GraphObj_SimpleColor::Draw()
{
	// Starting point of the IBO
	glDrawElements(GL_TRIANGLES, this->GetModel()->numTris * 3, GL_UNSIGNED_INT, 0);
}

void GraphObj_SimpleColor::RestoreState()
{
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
}

//void GraphObj_SimpleColor::DeleteGPUData()
//{
//	// Delete vertex array object
//	this->pModel->DeleteVAO();
//	// Delete program object
//	this->pShader->DeleteProgram();
//	// Delete vertex buffer object
//	this->pModel->DeleteVBO();
//}
