#pragma once

#include "../pba/core/DynamicalState.h"
#include "../pba/base/CollisionSurface.h"

#include "../common/Model.h"

//manager of model and physical animation
class PbaActor
{
public:
	PbaActor(pba::DynamicalState& pq);
	~PbaActor();

	void OnUpdate();

	void Display();

	void addModel(Model* _m);
	void addModel(std::vector<Model*>& _m);

private:
	pba::DynamicalState& PQ;
	CollisionSurface surf;

	std::vector<Model*> m_Models;
};
