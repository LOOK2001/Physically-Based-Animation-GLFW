#include "PbaActor.h"

PbaActor::PbaActor(pba::DynamicalState& pq)
	:PQ(pq)
{

}

PbaActor::~PbaActor()
{

}

void PbaActor::OnUpdate()
{
	glm::mat4 trans = glm::mat4(1.0f);
	glm::vec3 pos;
	for (size_t i = 0; i < PQ->nb(); i++)
	{
		pos = glm::vec3(PQ->pos(i).X(), PQ->pos(i).Y(), PQ->pos(i).Z());
		trans = glm::translate(trans, pos);

		if (i >= m_Models.size())
			break;

		m_Models[i]->updateTranMat(trans);
	}
}

void PbaActor::Display()
{
	for (auto model : m_Models)
	{
		model->Draw();
	}
}

void PbaActor::addModel(Model* _m)
{
	m_Models.push_back(_m);
}

void PbaActor::addModel(std::vector<Model*>& _m)
{
	for (auto m : _m)
	{
		m_Models.push_back(m);
	}
}
