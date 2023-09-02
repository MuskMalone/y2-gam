#pragma once

#include <glad/glad.h>
#include <string>
#include <Math/Mat44.hpp>
#include <Math/Vec3.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Graphics/GLSLShader.hpp>
//class Mat44;
//class Vec3;

class Shader
{
public:
	Shader(std::string const& vertexPath, std::string const& fragmentPath);

	void Activate();

	void SetUniform(GLchar const* name, GLboolean val) { mShader.SetUniform(name, val); }
	void SetUniform(GLchar const* name, GLint val) { mShader.SetUniform(name, val); }
	void SetUniform(GLchar const* name, GLfloat val) { mShader.SetUniform(name, val); }
	void SetUniform(GLchar const* name, GLfloat x, GLfloat y) { mShader.SetUniform(name, x, y); }
	void SetUniform(GLchar const* name, GLfloat x, GLfloat y, GLfloat z) { mShader.SetUniform(name, x, y, z); }
	void SetUniform(GLchar const* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w) { mShader.SetUniform(name, x, y, z, w); }
	void SetUniform(GLchar const* name, glm::vec2 const& val) { mShader.SetUniform(name, val); }
	void SetUniform(GLchar const* name, glm::vec3 const& val) { mShader.SetUniform(name, val); }
	void SetUniform(GLchar const* name, glm::vec4 const& val) { mShader.SetUniform(name, val); }
	void SetUniform(GLchar const* name, glm::mat3 const& val) { mShader.SetUniform(name, val); }
	void SetUniform(GLchar const* name, glm::mat4 const& val) { mShader.SetUniform(name, val); }


private:
	GLuint mId;
	GLSLShader mShader;
};
