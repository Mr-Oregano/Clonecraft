
#include "Window.h"
#include "Gfx.h"
#include "Math.h"
#include "Pipeline.h"

#include <stb_image.h>
#include <iostream>
#include <fstream>

int WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE,
	_In_ LPSTR pCmdLine,
	_In_ int nCmdShow)
{

	Window window({ 1280, 720, "Minecraft" });

	glm::mat4 modelMat = glm::translate(glm::mat4(1.f), {0.f, 0.f, -2.5f});
	glm::mat4 viewProjMat = glm::perspective(glm::radians(70.f), 16.f / 9.f, 0.1f, 100.f);

	GLuint vao = 0;
	glCreateVertexArrays(1, &vao);

	GLuint vbo, ibo = 0;
	glCreateBuffers(1, &vbo);
	glCreateBuffers(1, &ibo);

	float data[] = {
		-0.5f, -0.5f,  0.5f,  0.f, 0.f, 
		 0.5f, -0.5f,  0.5f,  1.f, 0.f,
		 0.5f,  0.5f,  0.5f,  1.f, 1.f,
		-0.5f,  0.5f,  0.5f,  0.f, 1.f,
		
		 0.5f, -0.5f,  0.5f,  0.f, 0.f,
		 0.5f, -0.5f, -0.5f,  1.f, 0.f,
		 0.5f,  0.5f, -0.5f,  1.f, 1.f,
		 0.5f,  0.5f,  0.5f,  0.f, 1.f,

		 0.5f, -0.5f, -0.5f,  0.f, 0.f,
		-0.5f, -0.5f, -0.5f,  1.f, 0.f,
		-0.5f,  0.5f, -0.5f,  1.f, 1.f,
		 0.5f,  0.5f, -0.5f,  0.f, 1.f,

		-0.5f, -0.5f, -0.5f,  0.f, 0.f,
		-0.5f, -0.5f,  0.5f,  1.f, 0.f,
		-0.5f,  0.5f,  0.5f,  1.f, 1.f,
		-0.5f,  0.5f, -0.5f,  0.f, 1.f,

		-0.5f,  0.5f,  0.5f,  0.f, 0.f,
		 0.5f,  0.5f,  0.5f,  1.f, 0.f,
		 0.5f,  0.5f, -0.5f,  1.f, 1.f,
		-0.5f,  0.5f, -0.5f,  0.f, 1.f,

		-0.5f, -0.5f, -0.5f,  0.f, 0.f,
		 0.5f, -0.5f, -0.5f,  1.f, 0.f,
		 0.5f, -0.5f,  0.5f,  1.f, 1.f,
		-0.5f, -0.5f,  0.5f,  0.f, 1.f,
	};

	unsigned char indices[] = {
		0 , 1 , 2 , 2 , 3 , 0 ,    // front
		4 , 5 , 6 , 6 , 7 , 4 ,    // right
		8 , 9 , 10, 10, 11, 8 ,    // back
		12, 13, 14, 14, 15, 12,    // left
		16, 17, 18, 18, 19, 16,    // up
		20, 21, 22, 22, 23, 20,    // down
	};

	glNamedBufferStorage(vbo, sizeof(data), data, GL_DYNAMIC_STORAGE_BIT);
	glNamedBufferStorage(ibo, sizeof(indices), indices, GL_DYNAMIC_STORAGE_BIT);

	glVertexArrayVertexBuffer(vao, 0, vbo, 0, sizeof(float) * 5);
	
	glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribFormat(vao, 1, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float));
	
	glVertexArrayAttribBinding(vao, 0, 0);
	glVertexArrayAttribBinding(vao, 1, 0);
	
	glEnableVertexArrayAttrib(vao, 0);
	glEnableVertexArrayAttrib(vao, 1);

	glVertexArrayElementBuffer(vao, ibo);
	glBindVertexArray(vao);

	GLuint texture = 0;
	glCreateTextures(GL_TEXTURE_2D, 1, &texture);
	glTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTextureParameteri(texture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	int width = 0;
	int height = 0;
	int channels = 0;
	unsigned char *texData = stbi_load("res/textures/dirt.png", &width, &height, &channels, 0);

	glTextureStorage2D(texture, 1, GL_RGBA8, width, height);
	glTextureSubImage2D(texture, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, texData);

	stbi_image_free(texData);

	glBindTextureUnit(0, texture);

	auto pipeline = Pipeline::CreateFromFiles("res/shaders/default-vert.glsl", "res/shaders/default-frag.glsl");

	pipeline->Bind();
	pipeline->UniformMat4("u_ViewProj", viewProjMat);

	glClearColor(0.16f, 0.7f, 0.83f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	window.Show();
	while (window.PollEvents())
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		modelMat = glm::rotate(modelMat, glm::radians(1.f), glm::normalize(glm::vec3(1,1,1)));
		pipeline->UniformMat4("u_Model", modelMat);

		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, nullptr);
		window.SwapBuffers();
	}

	return 0;
}