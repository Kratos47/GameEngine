#include "GraphicsObjectFont.h"
#include "CameraMan.h"
#include "GraphicsObject.h"
#include "Glyph.h"
#include "Model.h"

GraphicsObjectFont::GraphicsObjectFont(Model* pModel, ShaderObject* pShaderObj)
	:GraphicsObject(pModel, pShaderObj)
{
	this->pGlyph = 0;

	// Create the orig Matrix 
	// The initial sprite setup, after this point standard world will adjust
	this->poMatrix_orig = new Matrix();

	this->poMatrix_uv = new Matrix();
}

GraphicsObjectFont::~GraphicsObjectFont()
{
	delete this->poMatrix_uv;
	delete this->poMatrix_orig;
}

void GraphicsObjectFont::SetState()
{
	glActiveTexture(GL_TEXTURE0);

	// Bind the texture
	GLuint textureID = Texture::Find(this->pGlyph->pTexture->name);

	assert(textureID);

	glBindTexture(GL_TEXTURE_2D, textureID);

	// comment out one
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glFrontFace(GL_CW);

	// Blends for sprites
	glEnable(GL_BLEND);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
}

void GraphicsObjectFont::SetDataGPU()
{
	// Get camera
	Camera* pCam = CameraMan::GetCurrent(Camera::Type::ORTHOGRAPHIC_2D);

	// set the vao
	glBindVertexArray(this->GetModel()->vao);

	// use this shader
	this->pShaderObj->SetActive();

	// World matrix ----------------------------
	Matrix TransToOriginLowerLeft(Matrix::Trans::XYZ, -pCam->getScreenWidth() / 2.0f, -pCam->getScreenHeight() / 2.0f, 0.0f);
	Matrix tmpMatrix = *this->poMatrix_orig * this->GetWorld() * TransToOriginLowerLeft;

	glUniformMatrix4fv(this->pShaderObj->GetLocation("proj_matrix"), 1, GL_FALSE, (float*)&pCam->getProjMatrix());
	glUniformMatrix4fv(this->pShaderObj->GetLocation("view_matrix"), 1, GL_FALSE, (float*)&pCam->getViewMatrix());
	glUniformMatrix4fv(this->pShaderObj->GetLocation("world_matrix"), 1, GL_FALSE, (float*)&tmpMatrix);
	glUniformMatrix4fv(this->pShaderObj->GetLocation("uv_matrix"), 1, GL_FALSE, (float*)this->poMatrix_uv);
}

void GraphicsObjectFont::Draw()
{
	// draw
	glDrawElements(GL_TRIANGLES, 3 * this->GetModel()->numTris, GL_UNSIGNED_INT, 0);   //The starting point of the IBO
}

void GraphicsObjectFont::RestoreState()
{
	glDisable(GL_BLEND);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
}

void GraphicsObjectFont::Swap(Glyph* pGlyph, const float _x, const float _y)
{
	UNUSED_VAR(pGlyph);
	UNUSED_VAR(_x);
	UNUSED_VAR(_y);

	assert(pGlyph);
	this->pGlyph = pGlyph;

	Matrix tmp = Matrix(Matrix::Trans::XYZ, _x, _y, 0.0f);
	this->SetWorld(tmp);


	float w = pGlyph->pSubRect.width / pGlyph->pTexture->width;
	float h = pGlyph->pSubRect.height / pGlyph->pTexture->height;
	float u = pGlyph->pSubRect.x / pGlyph->pTexture->width;
	float v = pGlyph->pSubRect.y / pGlyph->pTexture->height;
	Matrix ScaleUV(Matrix::Scale::XYZ, w, h, 1.0f);
	Matrix TransUV(Matrix::Trans::XYZ, u, v, 0.0f);
	*this->poMatrix_uv = ScaleUV * TransUV;

	
	*this->poMatrix_orig = Matrix(Matrix::Scale::XYZ, pGlyph->pSubRect.width, pGlyph->pSubRect.height, 1.0f);
}
