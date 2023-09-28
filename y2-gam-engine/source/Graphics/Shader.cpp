/******************************************************************************/
/*!
\par        Image Engine
\file       Shader.cpp

\author     Xavier Choa (k.choa@digipen.edu)
\date       Sep 5, 2023

\brief      Implementation file for the Shader class.

			Contains the declarations for the Shader class which is responsible
			for managing and setting up shaders for rendering. The class provides
			functions for using shaders, setting uniform variables, and other
			shader-related operations.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
			or disclosure of this file or its contents without the prior
			written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "Graphics/Shader.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include "Logging/LoggingSystem.hpp"
#include "Logging/backward.hpp"
#include <vector>


/*  _________________________________________________________________________ */
/*! Shader

@param vertFile
The path to the vertex shader file.

@param fragFile
The path to the fragment shader file.

This constructor initializes the Shader object by creating a shader from the
provided vertex and fragment shader files.

*/
Shader::Shader(std::string const& vertFile, std::string const& fragFile)
	:pgmHdl{} {
	CreateShaderFromFile(vertFile, fragFile);
}

/*  _________________________________________________________________________ */
/*! ~Shader

Destructor for the Shader class. It deletes the shader program from OpenGL.

*/
Shader::~Shader() {
	glDeleteProgram(pgmHdl);
}

/*  _________________________________________________________________________ */
/*! CreateShaderFromString

@param vertSrc
The source code for the vertex shader as a string.

@param fragSrc
The source code for the fragment shader as a string.

This function creates a shader program from the provided vertex and fragment
shader source strings.

*/
void Shader::CreateShaderFromString(std::string const& vertSrc, std::string const& fragSrc) {
	// Create an empty vertex shader handle
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

	// Send the vertex shader source code to GL
	// Note that std::string's .c_str is NULL character terminated.
	const GLchar* source = vertSrc.c_str();
	glShaderSource(vertexShader, 1, &source, 0);

	// Compile the vertex shader
	glCompileShader(vertexShader);


	//Checks to see if compliation succeeded or failed
	GLint isCompiled = 0;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

		// Failed to compile, delete shader
		glDeleteShader(vertexShader);

		std::string str(infoLog.data());
		LoggingSystem::GetInstance().Log(LogLevel::ERROR_LEVEL, "Vertex Shader compilation failure! " + str, __FUNCTION__);
		return;
	}

	// Create an empty fragment shader handle
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	// Send the fragment shader source code to GL
	// Note that std::string's .c_str is NULL character terminated.
	source = (const GLchar*)fragSrc.c_str();
	glShaderSource(fragmentShader, 1, &source, 0);

	// Compile the fragment shader
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

		// Failed to compile, delete both shaders to prevent leak
		glDeleteShader(fragmentShader);
		glDeleteShader(vertexShader);

		std::string str(infoLog.data());
		LoggingSystem::GetInstance().Log(LogLevel::ERROR_LEVEL, "Fragment Shader compilation failure! " + str, __FUNCTION__);
		return;
	}

	// Vertex and fragment shaders are successfully compiled.
	// Now time to link them together into a program.
	// Get a program object.
	pgmHdl = glCreateProgram();
	if (pgmHdl == 0) {
		LoggingSystem::GetInstance().Log(LogLevel::ERROR_LEVEL, "Unable to create program handle!", __FUNCTION__);
	}

	// Attach our shaders to our program
	glAttachShader(pgmHdl, vertexShader);
	glAttachShader(pgmHdl, fragmentShader);

	// Link our program
	glLinkProgram(pgmHdl);

	// Check if program is linked successully
	GLint isLinked = 0;
	glGetProgramiv(pgmHdl, GL_LINK_STATUS, (int*)&isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(pgmHdl, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(pgmHdl, maxLength, &maxLength, &infoLog[0]);

		// Link failed, delete program and both shaders
		glDeleteProgram(pgmHdl);
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		std::string str(infoLog.data());
		LoggingSystem::GetInstance().Log(LogLevel::ERROR_LEVEL, "Shader link failure! " + str, __FUNCTION__);
		return;
	}

	// Always detach shaders after a successful link.
	glDetachShader(pgmHdl, vertexShader);
	glDetachShader(pgmHdl, fragmentShader);

	LoggingSystem::GetInstance().Log(LogLevel::INFO_LEVEL, "Succesfully Compiled and Linked!", __FUNCTION__);
}

/*  _________________________________________________________________________ */
/*! CreateShaderFromFile

@param vertFile
The path to the vertex shader file.

@param fragFile
The path to the fragment shader file.

This function reads the source code from the provided vertex and fragment shader
files and creates a shader program.

*/
void Shader::CreateShaderFromFile(std::string const& vertFile, std::string const& fragFile) {
	std::ifstream inVertFile{ vertFile };
	if (!inVertFile) {
		LoggingSystem::GetInstance().Log(LogLevel::ERROR_LEVEL, "Cannot open vertex file: " + vertFile, __FUNCTION__);
	}

	std::stringstream vertSrc;
	vertSrc << inVertFile.rdbuf();
	inVertFile.close();

	std::ifstream inFragFile{ fragFile };
	if (!inFragFile) {
		LoggingSystem::GetInstance().Log(LogLevel::ERROR_LEVEL, "Cannot open fragment file: " + fragFile, __FUNCTION__);
	}

	std::stringstream fragSrc;
	fragSrc << inFragFile.rdbuf();
	inFragFile.close();

	CreateShaderFromString(vertSrc.str(), fragSrc.str());
}

/*  _________________________________________________________________________ */
/*! Use

This function sets the current shader program to the one represented by this
Shader object.

*/
void Shader::Use() const {
	glUseProgram(pgmHdl);
}

/*  _________________________________________________________________________ */
/*! Unuse

This function unsets the current shader program, effectively disabling any shaders.

*/
void Shader::Unuse() const {
	glUseProgram(0);
}

/*  _________________________________________________________________________ */
/*! SetUniform

@param name
The name of the uniform variable in the shader.

@param val
The boolean value to set the uniform variable to.

This function sets the specified uniform variable in the shader program to the
provided boolean value.

*/
void Shader::SetUniform(std::string const& name, GLboolean val) {
	GLint loc { glGetUniformLocation(pgmHdl, name.c_str()) };
	if (loc >= 0) {
		glUniform1i(loc, val);
	}
	else {
		LoggingSystem::GetInstance().Log(LogLevel::ERROR_LEVEL, "Uniform variable " + name + " doesn't exist", __FUNCTION__);
	}
}

/*  _________________________________________________________________________ */
/*! SetUniform

@param name
The name of the uniform variable in the shader.

@param val
The integer value to set the uniform variable to.

This function sets the specified uniform variable in the shader program to the
provided integer value.

*/
void Shader::SetUniform(std::string const& name, GLint val) {
	GLint loc{ glGetUniformLocation(pgmHdl, name.c_str()) };
	if (loc >= 0) {
		glUniform1i(loc, val);
	}
	else {
		LoggingSystem::GetInstance().Log(LogLevel::ERROR_LEVEL, "Uniform variable " + name + " doesn't exist", __FUNCTION__);
	}
}


/*  _________________________________________________________________________ */
/*! SetUniform

@param name
The name of the uniform variable in the shader.

@param val
The float value to set the uniform variable to.

This function sets the specified uniform variable in the shader program to the
provided float value.

*/
void Shader::SetUniform(std::string const& name, GLfloat val) {
	GLint loc{ glGetUniformLocation(pgmHdl, name.c_str()) };
	if (loc >= 0) {
		glUniform1f(loc, val);
	}
	else {
		LoggingSystem::GetInstance().Log(LogLevel::ERROR_LEVEL, "Uniform variable " + name + " doesn't exist", __FUNCTION__);
	}
}

/*  _________________________________________________________________________ */
/*! SetUniform

@param name
The name of the uniform variable in the shader.

@param x, y
The x and y float values to set the uniform variable to.

This function sets the specified uniform variable in the shader program to the
provided 2D float values.

*/
void Shader::SetUniform(std::string const& name, GLfloat x, GLfloat y) {
	GLint loc{ glGetUniformLocation(pgmHdl, name.c_str()) };
	if (loc >= 0) {
		glUniform2f(loc, x, y);
	}
	else {
		LoggingSystem::GetInstance().Log(LogLevel::ERROR_LEVEL, "Uniform variable " + name + " doesn't exist", __FUNCTION__);
	}
}

/*  _________________________________________________________________________ */
/*! SetUniform

@param name
The name of the uniform variable in the shader.

@param x, y, z
The x, y, and z float values to set the uniform variable to.

This function sets the specified uniform variable in the shader program to the
provided 3D float values.

*/
void Shader::SetUniform(std::string const& name, GLfloat x, GLfloat y, GLfloat z) {
	GLint loc{ glGetUniformLocation(pgmHdl, name.c_str()) };
	if (loc >= 0) {
		glUniform3f(loc, x, y, z);
	}
	else {
		LoggingSystem::GetInstance().Log(LogLevel::ERROR_LEVEL, "Uniform variable " + name + " doesn't exist", __FUNCTION__);
	}
}

/*  _________________________________________________________________________ */
/*! SetUniform

@param name
The name of the uniform variable in the shader.

@param x, y, z, w
The x, y, z, and w float values to set the uniform variable to.

This function sets the specified uniform variable in the shader program to the
provided 4D float values.

*/
void Shader::SetUniform(std::string const& name, GLfloat x, GLfloat y, GLfloat z, GLfloat w) {
	GLint loc{ glGetUniformLocation(pgmHdl, name.c_str()) };
	if (loc >= 0) {
		glUniform4f(loc, x, y, z, w);
	}
	else {
		LoggingSystem::GetInstance().Log(LogLevel::ERROR_LEVEL, "Uniform variable " + name + " doesn't exist", __FUNCTION__);
	}
}

/*  _________________________________________________________________________ */
/*! SetUniform

@param name
The name of the uniform variable in the shader.

@param val
The 2D vector value to set the uniform variable to.

This function sets the specified uniform variable in the shader program to the
provided 2D vector value.

*/
void Shader::SetUniform(std::string const& name, glm::vec2 const& val) {
	GLint loc{ glGetUniformLocation(pgmHdl, name.c_str()) };
	if (loc >= 0) {
		glUniform2f(loc, val.x, val.y);
	}
	else {
		LoggingSystem::GetInstance().Log(LogLevel::ERROR_LEVEL, "Uniform variable " + name + " doesn't exist", __FUNCTION__);
	}
}

/*  _________________________________________________________________________ */
/*! SetUniform

@param name
The name of the uniform variable in the shader.

@param val
The 3D vector value to set the uniform variable to.

This function sets the specified uniform variable in the shader program to the
provided 3D vector value.

*/
void Shader::SetUniform(std::string const& name, glm::vec3 const& val) {
	GLint loc{ glGetUniformLocation(pgmHdl, name.c_str()) };
	if (loc >= 0) {
		glUniform3f(loc, val.x, val.y, val.z);
	}
	else {
		LoggingSystem::GetInstance().Log(LogLevel::ERROR_LEVEL, "Uniform variable " + name + " doesn't exist", __FUNCTION__);
	}
}

/*  _________________________________________________________________________ */
/*! SetUniform

@param name
The name of the uniform variable in the shader.

@param val
The 4D vector value to set the uniform variable to.

This function sets the specified uniform variable in the shader program to the
provided 4D vector value.

*/
void Shader::SetUniform(std::string const& name, glm::vec4 const& val) {
	GLint loc{ glGetUniformLocation(pgmHdl, name.c_str()) };
	if (loc >= 0) {
		glUniform4f(loc, val.x, val.y, val.z, val.w);
	}
	else {
		LoggingSystem::GetInstance().Log(LogLevel::ERROR_LEVEL, "Uniform variable " + name + " doesn't exist", __FUNCTION__);
	}
}

/*  _________________________________________________________________________ */
/*! SetUniform

@param name
The name of the uniform variable in the shader.

@param val
The 3x3 matrix value to set the uniform variable to.

This function sets the specified uniform variable in the shader program to the
provided 3x3 matrix value.

*/
void Shader::SetUniform(std::string const& name, glm::mat3 const& val) {
	GLint loc{ glGetUniformLocation(pgmHdl, name.c_str()) };
	if (loc >= 0) {
		glUniformMatrix3fv(loc, 1, GL_FALSE, &val[0][0]);
	}
	else {
		LoggingSystem::GetInstance().Log(LogLevel::ERROR_LEVEL, "Uniform variable " + name + " doesn't exist", __FUNCTION__);
	}
}

/*  _________________________________________________________________________ */
/*! SetUniform

@param name
The name of the uniform variable in the shader.

@param val
The 4x4 matrix value to set the uniform variable to.

This function sets the specified uniform variable in the shader program to the
provided 4x4 matrix value.

*/
void Shader::SetUniform(std::string const& name, glm::mat4 const& val) {
	GLint loc{ glGetUniformLocation(pgmHdl , name.c_str()) };
	if (loc >= 0) {
		glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(val));
	}
	else {
		LoggingSystem::GetInstance().Log(LogLevel::ERROR_LEVEL, "Uniform variable " + name + " doesn't exist", __FUNCTION__);
	}
}

/*  _________________________________________________________________________ */
/*! SetUniform

@param name
The name of the uniform variable in the shader.

@param val
The pointer to an array of integer values.

@param count
The number of integer values in the array.

This function sets the specified uniform variable in the shader program to the
provided array of integer values.

*/
void Shader::SetUniform(std::string const& name, int* val, unsigned int count) {
	GLint loc{ glGetUniformLocation(pgmHdl , name.c_str()) };
	if (loc >= 0) {
		glUniform1iv(loc, count, val);
	}
	else {
		LoggingSystem::GetInstance().Log(LogLevel::ERROR_LEVEL, "Uniform variable " + name + " doesn't exist", __FUNCTION__);
	}
}