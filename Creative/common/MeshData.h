#pragma once

#ifndef MESH_H
#define MESH_H

#include <glad/glad.h> // holds all OpenGL type declarations

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../Shader.h"

#include "../Object.h"
#include "../pba/core/PbaViewer.h"

#include <memory>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
using namespace std;
using namespace pba;


struct Vertex {
	// position
	glm::vec3 Position;
	// normal
	glm::vec3 Normal;
	// texCoords
	glm::vec2 TexCoords;
	// tangent
	glm::vec3 Tangent;
	// bitangent
	glm::vec3 Bitangent;
};

struct Texture_Mesh {
	unsigned int id;
	string type;
	string path;
};

class MeshData : public Object {
public:
	/*  Mesh Data  */
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture_Mesh> textures;
	unsigned int VAO;

	/*  Functions  */
	// constructor
	MeshData(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture_Mesh> textures)
	{
		this->vertices = vertices;
		this->indices = indices;
		this->textures = textures;

		// now that we have all the required data, set the vertex buffers and its attribute pointers.
		setupMesh();

		m_modelMatrix = glm::mat4(1.0f);
	}

	// render the mesh
	void Draw()
	{
		PbaViewer* _viewer = PbaViewer::Instance();

		Renderer renderer;
		Camera* _camera = _viewer->GetMainCamera();

		if (m_Texture)
		{
			m_Texture->Bind();
		}

		{
			glm::mat4 projection = glm::perspective(glm::radians(_camera->Zoom),
				(float)PbaViewer::Instance()->GetScreenWidth() / (float)PbaViewer::Instance()->GetScreenHeight()
				, 0.1f, 100.0f);
			m_Shader->SetUniformMat4f("projection", projection);

			glm::mat4 view = _camera->GetViewMatrix();
			m_Shader->SetUniformMat4f("view", view);

			// calculate the model matrix for each object and pass it to shader before drawing
			m_modelMatrix = glm::translate(m_modelMatrix, glm::vec3(1.0f, 0.3f, 0.5f));
// 			float angle = 20.0f * 9;
// 			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			m_Shader->SetUniformMat4f("model", m_modelMatrix);

			m_Shader->Bind();
			renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
		}
	}

	void updateTranMat(glm::mat4& trans)
	{
		m_modelMatrix = trans;
	}

private:

	glm::mat4 m_modelMatrix;

	/*  Functions    */
	// initializes all the buffer objects/arrays
	void setupMesh()
	{
		m_VAO = std::make_unique<VertexArray>();

		m_VertexBuffer = std::make_unique<VertexBuffer>(&vertices[0], vertices.size() * sizeof(Vertex));
		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(3);
		layout.Push<float>(2);
		layout.Push<float>(3);
		layout.Push<float>(3);
		m_VAO->AddBuffer(*m_VertexBuffer, layout);

		m_IndexBuffer = std::make_unique<IndexBuffer>(&indices[0], indices.size());

		m_Shader = std::make_unique<Shader>("res/shaders/Basic3D.shader");
		m_Shader->Bind();

		if (!textures.empty())
		{
			m_Texture = std::make_unique<Texture>(textures[0].path);
			m_Shader->SetUniform1i("texture1", 0);
		}
	}
};

typedef std::shared_ptr<MeshData> Mesh;
#endif