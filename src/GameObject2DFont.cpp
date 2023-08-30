#include "GameObject2DFont.h"
#include "MathEngine.h"
#include "GraphicsObjectFont.h"
#include "GraphicsObject.h"
#include "GlyphMan.h"
#include "GameTime.h"

	GameObject2DFont::GameObject2DFont(GraphicsObjectFont* graphicsObject, std::string str, float x, const float y)
	: GameObject(graphicsObject)
	{
		assert(graphicsObject != 0);

		this->angle = 0.0f;
		this->posX = x;
		this->posY = y;
		this->scaleX = 1.0f;
		this->scaleY = 1.0f;

		this->pMessege = str;

		this->UpdateMessege(str);
	}

	GameObject2DFont::~GameObject2DFont()
	{

	}


	void GameObject2DFont::Draw()
	{
		GraphicsObjectFont* pGrp = (GraphicsObjectFont*)this->GetGraphicsObject();

		float xTmp = this->posX;
		float yTmp = this->posY;

		float xEnd = this->posX;

		float scale = 1.0f;

		for (unsigned int i = 0; i < pMessege.length(); i++)
		{
			int key = pMessege[i];

			Glyph* pGlyph = GlyphMan::Find(key);
			assert(pGlyph != nullptr);

			xTmp = xEnd + pGlyph->pSubRect.width* scale / 2;

			pGrp->Swap(pGlyph, xTmp* scale, yTmp * scale);
			pGrp->Render();

			xEnd = pGlyph->pSubRect.width * scale / 2.0f + xTmp;
		}

		// Goal: update the world matrix
		Matrix Scale(Matrix::Scale::XYZ, this->scaleX * scale, this->scaleY * scale, 1.0f);
		Matrix Rot(Matrix::Rot1::Z, this->angle);
		Matrix Trans(Matrix::Trans::XYZ, this->posX, this->posY, 0.0f);

		*this->poWorld = Scale * Rot * Trans;

		// push to graphics object
		this->poGraphicsObject->SetWorld(*this->poWorld);
	}

	void GameObject2DFont::Update(Time currentTime)
	{
		UNUSED_VAR(currentTime);
		// Goal: update the world matrix
		Matrix Scale(Matrix::Scale::XYZ, this->scaleX , this->scaleY , 1.0f);
		Matrix Rot(Matrix::Rot1::Z, this->angle);
		Matrix Trans(Matrix::Trans::XYZ, this->posX, this->posY, 0.0f);

		*this->poWorld = Scale * Rot * Trans;

		// push to graphics object
		this->poGraphicsObject->SetWorld(*this->poWorld);
		
	}

	void GameObject2DFont::UpdateMessege(std::string str)
	{
		this->pMessege = str;
	}

