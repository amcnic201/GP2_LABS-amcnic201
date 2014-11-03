#include "Shader.h"

//Load it from memory buffer
GLuint loadShaderFromMemory(const char * pMem, SHADER_TYPE shaderType)
{
	GLuint program = glCreateShader(shaderType);
	glShaderSource(program, 1, &pMem, NULL);
	glCompileShader(program);
	return program;
}

//Load Shader from file
GLuint loadShaderFromFile(const std::string& filename, SHADER_TYPE shaderType)

{
	std::string fileContents;
	std::ifstream file; //Used to open the file
	file.open(filename.c_str(), std::ios::in);
	if (!file)
	{
		return 0;

	}
	
	//calculate file size
	if (file.good()) //If file is good then we seek to end (file.seekg) then this enables us to get the lenght of file

	{
		file.seekg(0, std::ios::end);
		unsigned long len = file.tellg();
		file.seekg(std::ios::beg);

		if (len == 0)
		{
			std::cout << "File has no contents" << std::endl;
			return 0;

		}
		//We then resize the string which contains the contents of the file
		//so that it matches the size of (fileContents_resize)
		//then read and close the file
		fileContents.resize(len);
		file.read(&fileContents[0], len);
		file.close();
		GLuint program = loadShaderFromMemory(fileContents.c_str(), shaderType);
		return program;

	}

	return 0;

}

bool checkForCompilerErrors(GLuint shaderProgram)
{
	GLint isCompiled = 0;
	glGetShaderiv(shaderProgram, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(shaderProgram, GL_INFO_LOG_LENGTH, &maxLength);

		//The maxLength includes the NULL character
		std::string infoLog;
		infoLog.resize(maxLength);
		glGetShaderInfoLog(shaderProgram, maxLength, &maxLength, &infoLog[0]);

		std::cout << "Shader not compiled" << infoLog << std::endl;

		//We dont need the shader anymore
		glDeleteShader(shaderProgram);
		return true;
	}
	
	return false;
}