#include "Graphics/Shader.hpp"
#include <Graphics/GLSLShader.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include "Logging/LoggingSystem.hpp"
#include "Logging/backward.hpp"

Shader::Shader(std::string const& vertFile, std::string const& fragFile)
	:pgmHdl{} {
	CreateShaderFromFile(vertFile, fragFile);
}

Shader::~Shader() {
	glDeleteProgram(pgmHdl);
}

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

		/*std::cout << "Vertex Shader compilation failure!\n";
		std::cout << infoLog.data() << std::endl;*/
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

		//std::cout << "Fragment Shader compilation failure!\n";
		//std::cout << infoLog.data() << std::endl;
		std::string str(infoLog.data());
		LoggingSystem::GetInstance().Log(LogLevel::ERROR_LEVEL, "Fragment Shader compilation failure! " + str, __FUNCTION__);
		return;
	}

	// Vertex and fragment shaders are successfully compiled.
	// Now time to link them together into a program.
	// Get a program object.
	pgmHdl = glCreateProgram();
	if (pgmHdl == 0) {
		//std::cout << "ERROR: Unable to create program handle!\n";
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

		//std::cout << "Shader link failure!\n";
		//std::cout << infoLog.data() << std::endl;
		std::string str(infoLog.data());
		LoggingSystem::GetInstance().Log(LogLevel::ERROR_LEVEL, "Shader link failure! " + str, __FUNCTION__);
		return;
	}

	// Always detach shaders after a successful link.
	glDetachShader(pgmHdl, vertexShader);
	glDetachShader(pgmHdl, fragmentShader);

	//std::cout << "Successfully Compiled and Linked!\n";
	LoggingSystem::GetInstance().Log(LogLevel::INFO_LEVEL, "Succesfully Compiled and Linked!", __FUNCTION__);
}

void Shader::CreateShaderFromFile(std::string const& vertFile, std::string const& fragFile) {
	std::ifstream inVertFile{ vertFile };
	if (!inVertFile) {
		//std::cout << "ERROR: Cannot open vertex file: " << vertFile << std::endl;
		LoggingSystem::GetInstance().Log(LogLevel::ERROR_LEVEL, "Cannot open vertex file: " + vertFile, __FUNCTION__);
	}

	std::stringstream vertSrc;
	vertSrc << inVertFile.rdbuf();
	inVertFile.close();

	std::ifstream inFragFile{ fragFile };
	if (!inFragFile) {
		//std::cout << "ERROR: Cannot open fragment file: " << fragFile << std::endl;
		LoggingSystem::GetInstance().Log(LogLevel::ERROR_LEVEL, "Cannot open fragment file: " + fragFile, __FUNCTION__);
	}

	std::stringstream fragSrc;
	fragSrc << inFragFile.rdbuf();
	inFragFile.close();

	CreateShaderFromString(vertSrc.str(), fragSrc.str());
}

void Shader::Use() const {
	glUseProgram(pgmHdl);
}

void Shader::Unuse() const {
	glUseProgram(0);
}

void Shader::SetUniform(std::string const& name, GLboolean val) {
	GLint loc { glGetUniformLocation(pgmHdl, name.c_str()) };
	if (loc >= 0) {
		glUniform1i(loc, val);
	}
	else {
		//std::cout << "Uniform variable " << name << " doesn't exist" << std::endl;
		LoggingSystem::GetInstance().Log(LogLevel::ERROR_LEVEL, "Uniform variable " + name + " doesn't exist", __FUNCTION__);
	}
}

void Shader::SetUniform(std::string const& name, GLint val) {
	GLint loc{ glGetUniformLocation(pgmHdl, name.c_str()) };
	if (loc >= 0) {
		glUniform1i(loc, val);
	}
	else {
		//std::cout << "Uniform variable " << name << " doesn't exist" << std::endl;
		LoggingSystem::GetInstance().Log(LogLevel::ERROR_LEVEL, "Uniform variable " + name + " doesn't exist", __FUNCTION__);
	}
}

void Shader::SetUniform(std::string const& name, GLfloat val) {
	GLint loc{ glGetUniformLocation(pgmHdl, name.c_str()) };
	if (loc >= 0) {
		glUniform1f(loc, val);
	}
	else {
		//std::cout << "Uniform variable " << name << " doesn't exist" << std::endl;
		LoggingSystem::GetInstance().Log(LogLevel::ERROR_LEVEL, "Uniform variable " + name + " doesn't exist", __FUNCTION__);
	}
}

void Shader::SetUniform(std::string const& name, GLfloat x, GLfloat y) {
	GLint loc{ glGetUniformLocation(pgmHdl, name.c_str()) };
	if (loc >= 0) {
		glUniform2f(loc, x, y);
	}
	else {
		//std::cout << "Uniform variable " << name << " doesn't exist" << std::endl;
		LoggingSystem::GetInstance().Log(LogLevel::ERROR_LEVEL, "Uniform variable " + name + " doesn't exist", __FUNCTION__);
	}
}

void Shader::SetUniform(std::string const& name, GLfloat x, GLfloat y, GLfloat z) {
	GLint loc{ glGetUniformLocation(pgmHdl, name.c_str()) };
	if (loc >= 0) {
		glUniform3f(loc, x, y, z);
	}
	else {
		//std::cout << "Uniform variable " << name << " doesn't exist" << std::endl;
		LoggingSystem::GetInstance().Log(LogLevel::ERROR_LEVEL, "Uniform variable " + name + " doesn't exist", __FUNCTION__);
	}
}

void Shader::SetUniform(std::string const& name, GLfloat x, GLfloat y, GLfloat z, GLfloat w) {
	GLint loc{ glGetUniformLocation(pgmHdl, name.c_str()) };
	if (loc >= 0) {
		glUniform4f(loc, x, y, z, w);
	}
	else {
		//std::cout << "Uniform variable " << name << " doesn't exist" << std::endl;
		LoggingSystem::GetInstance().Log(LogLevel::ERROR_LEVEL, "Uniform variable " + name + " doesn't exist", __FUNCTION__);
	}
}

void Shader::SetUniform(std::string const& name, glm::vec2 const& val) {
	GLint loc{ glGetUniformLocation(pgmHdl, name.c_str()) };
	if (loc >= 0) {
		glUniform2f(loc, val.x, val.y);
	}
	else {
		//std::cout << "Uniform variable " << name << " doesn't exist" << std::endl;
		LoggingSystem::GetInstance().Log(LogLevel::ERROR_LEVEL, "Uniform variable " + name + " doesn't exist", __FUNCTION__);
	}
}

void Shader::SetUniform(std::string const& name, glm::vec3 const& val) {
	GLint loc{ glGetUniformLocation(pgmHdl, name.c_str()) };
	if (loc >= 0) {
		glUniform3f(loc, val.x, val.y, val.z);
	}
	else {
		//std::cout << "Uniform variable " << name << " doesn't exist" << std::endl;
		LoggingSystem::GetInstance().Log(LogLevel::ERROR_LEVEL, "Uniform variable " + name + " doesn't exist", __FUNCTION__);
	}
}

void Shader::SetUniform(std::string const& name, glm::vec4 const& val) {
	GLint loc{ glGetUniformLocation(pgmHdl, name.c_str()) };
	if (loc >= 0) {
		glUniform4f(loc, val.x, val.y, val.z, val.w);
	}
	else {
		//std::cout << "Uniform variable " << name << " doesn't exist" << std::endl;
		LoggingSystem::GetInstance().Log(LogLevel::ERROR_LEVEL, "Uniform variable " + name + " doesn't exist", __FUNCTION__);
	}
}

void Shader::SetUniform(std::string const& name, glm::mat3 const& val) {
	GLint loc{ glGetUniformLocation(pgmHdl, name.c_str()) };
	if (loc >= 0) {
		glUniformMatrix3fv(loc, 1, GL_FALSE, &val[0][0]);
	}
	else {
		//std::cout << "Uniform variable " << name << " doesn't exist" << std::endl;
		LoggingSystem::GetInstance().Log(LogLevel::ERROR_LEVEL, "Uniform variable " + name + " doesn't exist", __FUNCTION__);
	}
}

void Shader::SetUniform(std::string const& name, glm::mat4 const& val) {
	GLint loc{ glGetUniformLocation(pgmHdl , name.c_str()) };
	if (loc >= 0) {
		glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(val));
	}
	else {
		//std::cout << "Uniform variable " << name << " doesn't exist" << std::endl;
		LoggingSystem::GetInstance().Log(LogLevel::ERROR_LEVEL, "Uniform variable " + name + " doesn't exist", __FUNCTION__);
	}
}

void Shader::SetUniform(std::string const& name, int* val, unsigned int count) {
	GLint loc{ glGetUniformLocation(pgmHdl , name.c_str()) };
	if (loc >= 0) {
		glUniform1iv(loc, count, val);
	}
	else {
		//std::cout << "Uniform variable " << name << " doesn't exist" << std::endl;
		LoggingSystem::GetInstance().Log(LogLevel::ERROR_LEVEL, "Uniform variable " + name + " doesn't exist", __FUNCTION__);
	}
}

//Shader::Shader(std::string const& vertexPath, std::string const& fragmentPath)
//{
//	std::vector<std::pair<GLenum, std::string>> shdrfiles{
//		std::make_pair(GL_VERTEX_SHADER, vertexPath),
//			std::make_pair(GL_FRAGMENT_SHADER, fragmentPath)
//	};
//	GLSLShader shdrpgm;
//	shdrpgm.CompileLinkValidate(shdrfiles);
//	if (GL_FALSE == shdrpgm.IsLinked()) {
//		std::cout << "Unable to compile/link/validate shader programs\n";
//		std::cout << shdrpgm.GetLog() << "\n";
//		std::exit(EXIT_FAILURE);
//	}
//	mShader = shdrpgm;
//	mId = shdrpgm.GetHandle();
//}
//
//void Shader::Activate()
//{
//	glUseProgram(mId);
//}