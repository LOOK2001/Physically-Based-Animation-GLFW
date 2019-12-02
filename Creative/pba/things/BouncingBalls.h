#pragma once

#include "../core/Vector.h"
#include "../core/Color.h"
#include "../core/PbaThing.h"
#include "../core/DynamicalState.h"
#include "../base/ExplicitDynamics.h"
//#include "RK4.h"
#include "../base/ForceLibrary.h"
#include "../base/CollisionSurface.h"
#include "../base/CollisionHandler.h"
#include "../base/ParticleEmitter.h"
#include "../base/PbaUtils.h"

#include "../../game/particle_generator.h"
#include "../../game/PbaActor.h"

#include "../../common/Model.h"

#include <cstdlib>

#include <iostream>

using namespace std;
namespace pba {
	class BouncingBallsThing : public PbaThingyDingy
	{
	public:
		BouncingBallsThing(const std::string nam = "BouncingBallsThing") :
			PbaThingyDingy(name), emit(false), box (pba::makeCollisionSurface()),
			emitter(ParticleEmitter(Vector(0, 0, 0), Vector(0, 0, 0), 0.25, 1.0))
		{
			state = CreateDynamicalState(name + "DynamicalData");
			state->add(2);
			force = CreateSimpleGravityForce(pba::Vector(0.0, -0.1, 0.0));
			GISolver solvera = CreateAdvancePosition(state, collisions);
			GISolver solverb = CreateAdvanceVelocity(state, force);
			solver = CreateLeapFrogSolver(solvera, solverb);
			std::cout << name << "constructed\n";
		}
		~BouncingBallsThing() {}

		void Init(const std::vector<std::string>& args) 
		{
			CollisionSurface cube = pba::GenerateCollisionCube(10.0);
			AddCollisionSurface(cube);

			for (size_t i = 0; i < state->nb(); i++)
			{
				Vector v = Vector(drand48(), drand48(), drand48());
				state->set_vel(i, v);
			}
			particle = new ParticleGenerator(state->nb(), state);
			particle->Init();

			PbaActor* pPbaActor= new PbaActor(state);
			m_PbaActors.push_back(pPbaActor);

			Model* Pokemon = new Model("res/models/Oddish.obj");

			pPbaActor->addModel(Pokemon);
			//pPbaActor->addModel(Pokemon);
		}

		// Callback function
		void Display()
		{
			//particle->Display();

			for (auto actor : m_PbaActors)
			{
				actor->Display();
			}
		}

		void Keyboard(int key, int scancode, int action, int mods)
		{
			PbaThingyDingy::Keyboard(key, scancode, action, mods);
		}

		void solve()
		{
			if (emit)
			{
				int nbincrease = 10;
				state->add(nbincrease);
				Vector P, V;
				Color C;
				std::cout << "Emit Points" << state->nb() << std::endl;
				for (size_t i = state->nb() - nbincrease; i < state->nb(); i++)
				{
					emitter.emit(P, V, C);
					state->set_pos(i, P);
					state->set_vel(i, V);
					state->set_ci(i, C);
				}
			}
			solver->solve(dt);

			//particle->OnUpdate(0.0f);
		}

		void OnUpdate(float deltaTime)
		{
			for (auto actor : m_PbaActors)
			{
				actor->OnUpdate();
			}
		}

		void Reset()
		{
			// Distribute particles with random positions
			Vector P, V;
			Color C;
			for (size_t i = 0; i < state->nb(); i++) 
			{
				emitter.emit(P, V, C);
				state->set_pos(i, P);
				state->set_vel(i, V);
				state->set_ci(i, C);
			}
		}

		void Usage()
		{
			PbaThingyDingy::Usage();
			cout << "=== " << name << " ===\n";
			cout << "v            toggle visibility of collision surface\n";
			cout << "w            toggle wireframe/solid display of collision surface\n";
			cout << "g/G          reduce/increase gravitational constant\n";
			cout << "e            toggle particle emission on/off\n";
//			cout << "k            toggle collision trace tree on/off\n";
			cout << "c/C          reduce/increase coefficient of restitution\n";
			cout << "l            use leap frog solver\n";
			cout << "f            use forward euler solver\n";
// 			cout << "b            use backward euler solver\n";
// 			cout << "6            use sixth order sover\n";
			cout << "m            create another smaller cube inside the scene\n";
		};

		void AddCollisionSurface(pba::CollisionSurface& s, pba::CollisionSurface& s2)
		{
			std::cout << "Add CollisionSurface\n";
			box = s;
			boxS = s2;
			collisions.set_collision_surface(box);
		}

		void AddCollisionSurface(pba::CollisionSurface& s)
		{
			std::cout << "Add CollisionSurface\n";
			box = s;
			collisions.set_collision_surface(box);
		}

	private:
		bool emit;

		pba::DynamicalState state;
		pba::Force force;
		pba::GISolver solver;

		pba::CollisionSurface box;
		pba::CollisionSurface boxS;
		pba::ElasticCollisionHandler collisions;
		pba::ParticleEmitter emitter;

		ParticleGenerator* particle;

		std::vector<PbaActor*> m_PbaActors;
	};

	pba::PbaThing BouncingBalls() { return PbaThing(new BouncingBallsThing()); }
}