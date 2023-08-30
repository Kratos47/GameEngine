#ifndef FONT_OBJECT_2D_H
#define FONT_OBJECT_2D_H

#include "MathEngine.h"
#include "GameObject.h"

class GraphicsObjectFont;

class GameObject2DFont : public GameObject
{
public:

	GameObject2DFont(GraphicsObjectFont* graphicsObject, std::string str, float x, const float y);

	// Big four
	GameObject2DFont() = delete;
	GameObject2DFont(const GameObject2DFont&) = delete;
	GameObject2DFont& operator=(GameObject2DFont&) = delete;
	virtual ~GameObject2DFont() override;


	virtual void Draw() override;
	virtual void Update(Time currentTime) override;

	void UpdateMessege(std::string str);

public:
	float scaleX;
	float scaleY;
	float posX;
	float posY;
	float angle;

	std::string pMessege;
};

#endif
