//----------------------------------------------------------------------------
// Copyright Ed Keenan 2019
//----------------------------------------------------------------------------

#ifndef SHADER_OBJECT_H
#define SHADER_OBJECT_H

#include <sb7.h>
#include "ShaderLink.h"

class ShaderObject : public ShaderLink
{
public:
	enum Name
	{
		NULL_SHADER,
		COLOR_LIGHT,
		TEXTURE_SIMPLE,
		TEXTURE_POINT_LIGHT,
		SIMPLE_COLOR,
		COLOR_SINGLE,
		SPRITE,
		COLOR_SKIN,
		LIGHT_SKIN,
		REAL_COLOR_LIGHT,
		COMPUTE_MIXER,
		COMPUTE_WORLD
	};

	enum class Type
	{
		VERTEX,
		COMPUTE,
	};

public:
	ShaderObject(ShaderObject::Name shaderName, const char * const pShaderBaseFileName, ShaderObject::Type type);

	ShaderObject() = delete;
	ShaderObject(const ShaderObject &) = delete;
	ShaderObject &operator = (ShaderObject &) = delete;
	virtual ~ShaderObject() = default;  // Keenan(20)

	void Dispatch(unsigned int groupsize_x, unsigned int groupsize_y, unsigned int groupsize_z);

	void SetActive() const;
	GLint GetLocation(const char * const pUniformName) const;   // Keenan(20)

private:
	bool privCreateShader(GLuint &programObject, const char * const pShaderBaseFileName, ShaderObject::Type shaderType);
	bool privLoadNCompile(GLuint &shaderObject, const char * const pShaderFileName, GLenum shader_type);

public:
	Name     name;
	Type	 type; 
	GLuint   programObject;

};

#endif

// --- End of File --------------------------------------------------
