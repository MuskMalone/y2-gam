/******************************************************************************/
/*!
\par        Image Engine
\file       Shader.hpp

\author     Xavier Choa (k.choa@digipen.edu)
\date       Sep 5, 2023

\brief      Header file for the Shader class.

			Contains the declarations for the Shader class which is responsible
			for managing and setting up shaders for rendering. The class provides
			functions for using shaders, setting uniform variables, and other
			shader-related operations.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
			or disclosure of this file or its contents without the prior
			written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once

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