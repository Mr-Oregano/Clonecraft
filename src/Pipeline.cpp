
#include "Pipeline.h"

#include <iostream>
#include <fstream>
#include <vector>

Pipeline::Pipeline(std::initializer_list<Shader> shaders)
{
	m_Handle = glCreateProgram();

	std::vector<GLuint> shaderHandles;
	for (const auto &shader : shaders)
	{
		GLuint shaderHandle = glCreateShader(shader.type);
		const char *source = shader.source.c_str();
		glShaderSource(shaderHandle, 1, &source, nullptr);
		glCompileShader(shaderHandle); // TODO: Log compilation status
		glAttachShader(m_Handle, shaderHandle);
		shaderHandles.push_back(shaderHandle);
	}

	glLinkProgram(m_Handle); // TODO: Log link status

	// Attached shaders are no longer needed after linking
	for (GLuint shaderHandle : shaderHandles)
	{
		glDetachShader(m_Handle, shaderHandle);
		glDeleteShader(shaderHandle);
	}	
}

Pipeline::~Pipeline()
{
	glDeleteProgram(m_Handle);
}

// Uniforms

void Pipeline::UniformMat4(const char *name, const glm::mat4 &mat)
{
	glUniformMatrix4fv(UniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat));
}

GLint Pipeline::UniformLocation(const char *name)
{
	auto it = m_Uniforms.find(name);
	if (it == m_Uniforms.end())
	{
		GLint loc = glGetUniformLocation(m_Handle, name);
		if (loc == -1)
		{
			std::cout << "Warn: Failed to locate uniform \"" << name << "\"" << std::endl;
			return -1;
		}

		m_Uniforms[name] = loc;
		return loc;
	}
	return it->second;
}

static std::string FileToString(const char *filePath)
{
	std::ifstream ifs(filePath);
	// TODO: error checking

	return std::string(std::istreambuf_iterator<char>{ifs}, std::istreambuf_iterator<char>{});
}

std::shared_ptr<Pipeline> Pipeline::CreateFromFiles(
	const char *vertFilePath, const char *fragFilePath)
{
	return std::make_shared<Pipeline>(std::initializer_list<Shader>{
		{ShaderType::VERTEX, FileToString(vertFilePath)},
		{ShaderType::FRAGMENT, FileToString(fragFilePath)}
	});
}