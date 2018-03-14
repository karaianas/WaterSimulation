#pragma once
#include "Core.h"
//#include "HashTable.h"

#define PI 3.14159

using namespace std;

class Particle {
public:

	Particle();
	void setConstants(int d_, float h_, float rho_rest_, float mass_, float k_, float vis_);
	
	void setHitConstants(float size_, float elasticity_)
	{
		size = size_;
		elasticity = elasticity_;
		massElas = mass * (1 + elasticity);
	};

	void adjust(float dt);
	void update(float dt);
	void applyForce(glm::vec3 &f) { force += f; };

	// Force calculation
	void calcDensity();
	void calcPressure();
	void F_pressure();
	void F_viscosity();
	void F_gravity();

	// Helper functions
	float W_ij(glm::vec3 p_);
	glm::vec3 dW_ij(glm::vec3 p_);
	float f(float q);
	float df(float q);

//private:
	int id;

	float mass;
	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 force;

	float pressure;
	float rho;
	float rho_rest;
	float h;
	float k;
	int d;
	float viscosity;

	// Commonly used constants
	float threshold;
	glm::vec3 gravity;
	float massInv;
	float massElas;
	float h3Inv;
	float h4Inv;
	float h2_01;

	// Collision
	float size;
	float elasticity;

	bool flag;
	vector<Particle*> neighbors;
};