#pragma once

#include "../Object.h"
#include "../pba/core/DynamicalState.h"


using namespace pba;

// Represents a single particle and its state
struct Particle {
	Vector Position;
	Color Color;
	GLfloat Life;

	Particle() : Position(0.0f), Color(1.0f), Life(0.0f) {}
};

class ParticleGenerator : public Object
{
public:
	ParticleGenerator(unsigned int num, DynamicalState& pq);
	// Update all particles
	virtual void OnUpdate(float deltaTime);
	// Render all particles
	virtual void Display();
	
	// Initializes buffer and vertex attributes
	void Init();
	// Returns the first Particles index that's currently unused
	int firstUnusedParticle();
	// Respawns particle
	void respawnParticle(Particle& particle, DynamicalState& pq);

private:
	// State
	std::vector<Particle> particles;
	unsigned int amount;
	// Render state

	DynamicalState& PQ;
};