#ifndef GRAPH_OBJ_SIMPLE_COLOR_H
#define GRAPH_OBJ_SIMPLE_COLOR_H

#include "GraphicsObject.h"

class GraphObj_SimpleColor : public GraphicsObject
{
public:
	GraphObj_SimpleColor(Model *model, ShaderObject *shader, const Vect &colorVec);
	GraphObj_SimpleColor() = delete;
	~GraphObj_SimpleColor();
	GraphObj_SimpleColor(const GraphObj_SimpleColor&) = delete;
	GraphObj_SimpleColor& operator = (const GraphObj_SimpleColor&) = delete;

	// Rendermaterial contract
	virtual void SetState() override;
	virtual void SetDataGPU() override;
	virtual void Draw() override;
	virtual void RestoreState() override;

	// Uniform Data
	Vect *pColorVec;
};
#endif // !GRAPH_OBJ_SIMPLE_COLOR_H

