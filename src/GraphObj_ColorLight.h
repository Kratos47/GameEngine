#ifndef GRAPH_OBJ_COLOR_LIGHT_H
#define GRAPH_OBJ_COLOR_LIGHT_H


#include "GraphicsObject.h"

class GraphObj_ColorLight : public GraphicsObject
{
public:
	GraphObj_ColorLight(Model *model, ShaderObject *shader, const Vect &lightPos, const Vect &lightColor);
	GraphObj_ColorLight() = delete;
	~GraphObj_ColorLight();
	GraphObj_ColorLight(const GraphObj_ColorLight&) = delete;
	GraphObj_ColorLight& operator = (const GraphObj_ColorLight&) = delete;

	// Rendermaterial contract
	virtual void SetState() override;
	virtual void SetDataGPU() override;
	virtual void Draw() override;
	virtual void RestoreState() override;

	// Uniform Data
	Vect *poLightColor;
	Vect *poLightPos;
};

#endif // !GRAPH_OBJ_COLOR_LIGHT_H

