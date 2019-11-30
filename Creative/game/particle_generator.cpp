#include "particle_generator.h"
#include "../common.h"
#include "../Renderer.h"
#include "../pba/core/PbaViewer.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <iostream>

ParticleGenerator::ParticleGenerator(unsigned int num, DynamicalState& pq)
	:PQ(pq), amount(num)
{

}

void ParticleGenerator::OnUpdate(float deltaTime)
{
	size_t size = (amount > PQ->nb()) ? PQ->nb() : amount;
	// Update all particles
	for (size_t i = 0; i < size; i++)
	{
		Particle& p = this->particles[i];
		p.Position = PQ->pos(i);
	}
}

void ParticleGenerator::Display()
{
	PbaViewer* _viewer = PbaViewer::Instance();

	Renderer renderer;
	Camera* _camera = _viewer->GetMainCamera();

	m_Texture->Bind();

	{
		glm::mat4 projection = glm::perspective(glm::radians(_camera->Zoom),
			(float)PbaViewer::Instance()->GetScreenWidth() / (float)PbaViewer::Instance()->GetScreenHeight()
			, 0.1f, 100.0f);
		m_Shader->SetUniformMat4f("projection", projection);

		glm::mat4 view = _camera->GetViewMatrix();
		m_Shader->SetUniformMat4f("view", view);

		size_t size = (amount > PQ->nb()) ? PQ->nb() : amount;
		for (size_t i = 0; i < size; i++)
		{
			Particle& p = this->particles[i];
		
			// calculate the model matrix for each object and pass it to shader before drawing
			glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
			model = glm::translate(model, glm::vec3(p.Position.X(), p.Position.Y(), p.Position.Z()) );
			float angle = 20.0f * 9;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.0f, 0.0f));
			m_Shader->SetUniformMat4f("model", model);

			m_Shader->Bind();
			renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
		}
	}
}

void ParticleGenerator::Init()
{
	const float vertices[] = {
			-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
			0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
			0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
			0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
			-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

			-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
			0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
			0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
			0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
			-0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
			-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

			-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
			-0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
			-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
			-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

			0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
			0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
			0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
			0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
			0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
			0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

			-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
			0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
			0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
			0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
			-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

			-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
			0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
			0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
			0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
			-0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
			-0.5f, 0.5f, -0.5f, 0.0f, 1.0f
	};
	size_t size = sizeof(vertices);

	unsigned int indices[] = {
		0, 1, 2,
		3, 4, 5,
		6, 7, 8,
		9, 10, 11,
		12, 13, 14,
		15, 16, 17,
		18, 19, 20,
		21, 22, 23,
		24, 25, 26,
		27, 28, 29,
		30, 31, 32,
		33, 34, 35,
		36
	};

	m_VAO = std::make_unique<VertexArray>();

	m_VertexBuffer = std::make_unique<VertexBuffer>(vertices, size);
	VertexBufferLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(2);
	m_VAO->AddBuffer(*m_VertexBuffer, layout);

	m_IndexBuffer = std::make_unique<IndexBuffer>(indices, size / sizeof(float));

	m_Shader = std::make_unique<Shader>("res/shaders/Basic3D.shader");
	m_Shader->Bind();

	m_Texture = std::make_unique<Texture>("res/textures/ChernoLogo.png");
	m_Shader->SetUniform1i("texture1", 0);

	// Create amount default particle instances
	size_t size_particles = (amount > PQ->nb()) ? PQ->nb() : amount;
	for (size_t i = 0; i < size_particles; i++)
		particles.push_back(Particle());
}
