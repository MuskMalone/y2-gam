#pragma once

#include <glad/glad.h>
#include <string>
#include <Math/Mat44.hpp>
#include <Math/Vec3.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Graphics/GLSLShader.hpp>
//class Mat44;
//class Vec3;

class Shader {
public:
	Shader(std::string const& vertFile, std::string const& fragFile);
	~Shader();

	void CreateShaderFromString(std::string const& vertSrc, std::string const& fragSrc);
	void CreateShaderFromFile(std::string const& vertFile, std::string const& fragFile);

	void Use() const;
	void Unuse() const;


	void SetUniform(std::string const& name, GLboolean val);
	void SetUniform(std::string const& name, GLint val);
	void SetUniform(std::string const& name, GLfloat val);
	void SetUniform(std::string const& name, GLfloat x, GLfloat y);
	void SetUniform(std::string const& name, GLfloat x, GLfloat y, GLfloat z);
	void SetUniform(std::string const& name, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
	void SetUniform(std::string const& name, glm::vec2 const& val);
	void SetUniform(std::string const& name, glm::vec3 const& val);
	void SetUniform(std::string const& name, glm::vec4 const& val);
	void SetUniform(std::string const& name, glm::mat3 const& val);
	void SetUniform(std::string const& name, glm::mat4 const& val);
	void SetUniform(std::string const& name, int* val, unsigned int count);
private:
	unsigned int pgmHdl;
};