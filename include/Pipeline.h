#pragma once

#include "Gfx.h"
#include "Math.h"

#include <unordered_map>
#include <initializer_list>
#include <memory>

enum ShaderType
{
	VERTEX = GL_VERTEX_SHADER,
	FRAGMENT = GL_FRAGMENT_SHADER
};

struct Shader
{
	ShaderType type;
	std::string source;
};

class Pipeline
{
public:
	Pipeline(std::initializer_list<Shader> shaders);
	~Pipeline();

	inline void Bind() const { glUseProgram(m_Handle); }
	inline GLuint GetHandle() { return m_Handle; }

	// Uniforms
	void UniformMat4(const char *name, const glm::mat4 &mat);

private:
	GLint UniformLocation(const char *name);

private:
	GLuint m_Handle;
	std::unordered_map<const char *, GLint> m_Uniforms;

public:
	static std::shared_ptr<Pipeline> CreateFromFiles(
		const char *vertFilePath, const char *fragFilePath);
};