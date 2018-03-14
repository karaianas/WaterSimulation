#pragma once
#include "Core.h"
//#include "HashTable.h"

#define PI 3.14159

using namespace std;

class Particle {
public:

	Particle();
	void setConstants(int d_, float h_, float rho_rest_, float mass_, float k_)
	{
		d = d_;
		h = h_;
		rho_rest = rho_rest_;
		mass = mass_;
		k = k_;
	};

	void adjust(float dt);

	void update(float dt);
	void applyForce(glm::vec3 &f) { force += f; };

	void calcDensity();
	void calcPressure();
	void F_pressure();
	void F_viscosity();
	void F_gravity();

	float W_ij(glm::vec3 p_);
	glm::vec3 dW_ij(glm::vec3 p_);
	float f(float q);
	float df(float q);

	int id;

//private:
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

	bool flag;
	vector<Particle*> neighbors;
};