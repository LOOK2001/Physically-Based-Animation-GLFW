#pragma once

#include <vector>
#include <functional>
#include <memory>

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Texture.h"


namespace pba {
	class Object
	{
	public:
		Object(){}
		virtual ~Object() {}

		virtual void OnUpdate(float deltaTime) {}
		virtual void Display() {}

	protected:
		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<Texture> m_Texture;

		glm::mat4 m_Proj, m_View;
	};

	typedef std::shared_ptr<Object> pObject;
}
