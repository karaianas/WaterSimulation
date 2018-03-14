#include "ParticleSystem.h"

using namespace std;

Particle::Particle()
{
	flag = false;
	position = glm::vec3(0.0f);
	velocity = glm::vec3(0.0f);
	force = glm::vec3(0.0f);

	pressure = 0.0f;
	rho = 0.0f;
}

void Particle::setConstants(int d_, float h_, float rho_rest_, float mass_, float k_)
{
	d = d_;
	h = h_;
	rho_rest = rho_rest_;
	mass = mass_;
	k = k_;

	// Set commonly used constants
	gravity = glm::vec3(0, -9.8f, 0.0f) * mass;
	massInv = 1.0f / mass;
	h3Inv = 1.0f / pow(h, d);
	h4Inv = 1.0f / pow(h, d + 1);
	h2_01 = 0.01f * pow(h, 2);
};

void Particle::adjust(float dt)
{
	for (int i = 0; i < 3; i++)
	{
		glm::vec3 normal(0.0f);

		if (position[i] < -size)
		{
			normal[i] = 1.0f;

			// Find hit position
			float t = (glm::dot(normal, position) + size) / glm::dot(normal, velocity);
			glm::vec3 hit = position - t * velocity;

			// Adjust position
			position[i] = 2.0f * hit[i] - position[i];

			// Apply impulse
			glm::vec3 force_ = glm::dot(velocity, normal) * normal * -massElas / dt;
			glm::vec3 accel = massInv * force_;
			velocity += accel * dt;
			position += velocity * dt;
		}
		else if (position[i] > size)
		{
			normal[i] = -1.0f;

			// Find hit position
			float t = (glm::dot(normal, position) + size) / glm::dot(normal, velocity);
			glm::vec3 hit = position - t * velocity;

			// Adjust position
			position[i] = 2.0f * hit[i] - position[i];

			// Apply impulse
			glm::vec3 force_ = glm::dot(velocity, normal) * normal * -massElas / dt;
			glm::vec3 accel = massInv * force_;
			velocity += accel * dt;
			position += velocity * dt;
		}
	}
}

void Particle::update(float dt)
{
	// (1) Compute acceleration
	glm::vec3 accel = massInv * force;

	// (2) New velocity and new positon
	velocity += accel * dt;
	position += velocity * dt;

	// (*) Collision with the box
	adjust(dt);

	// (3) zero out the force
	force = glm::vec3(0.0f);
}

void Particle::calcDensity()
{
	float temp = 0.0f;
	for (int i = 0; i < neighbors.size(); i++)
		temp += neighbors[i]->mass * W_ij(neighbors[i]->position);
	rho = temp;
}

void Particle::calcPressure()
{
	pressure = k * (pow(rho / rho_rest, 7) - 1.0f);
}

void Particle::F_pressure()
{
	glm::vec3 force_(0.0f);
	for (int i = 0; i < neighbors.size(); i++)
	{
		if (pow(rho, 2) < 0.00001f || pow(neighbors[i]->rho, 2) < 0.00001f)
			return;

		force_ += neighbors[i]->mass * (pressure / pow(rho, 2) + \
			neighbors[i]->pressure / pow(neighbors[i]->rho, 2)) * dW_ij(neighbors[i]->position);
	}
	force_ *= -mass;
	applyForce(force_);
}

void Particle::F_viscosity()
{
	glm::vec3 force_(0.0f);
	for (int i = 0; i < neighbors.size(); i++)
	{
		glm::vec3 x_ij = position - neighbors[i]->position;
		float nrho = neighbors[i]->rho;
		if (nrho < 0.00001f)
			return;
		float denom = float(glm::dot(x_ij, x_ij) * h2_01);
		//if (denom < 0.00001f)
		//	return;
		force_ += neighbors[i]->mass / nrho * (velocity - neighbors[i]->velocity) \
			* glm::dot(x_ij , dW_ij(neighbors[i]->position)) / denom;
	}
	float viscos = 0.0000005f;//0.000001f; //0.0000005f; //0.0000001f;
	force_ *= 2 * mass * viscos;
	applyForce(force_);
}

void Particle::F_gravity()
{
	//glm::vec3 g(0, -9.8, 0);
	//glm::mat4 R = glm::rotate(glm::mat4(1.0f), 2.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	//g = glm::vec3(R * glm::vec4(g, 1.0f));
	//glm::vec3 force_ = g * mass;
	applyForce(gravity);
}

float Particle::W_ij(glm::vec3 p_)
{
	float q = glm::length(p_ - position) / h;
	
	return h3Inv * f(q);
}

glm::vec3 Particle::dW_ij(glm::vec3 p_)
{
	float norm = glm::length(p_ - position);
	float q = norm / h;
	
	if (norm < 0.00001f)
		return glm::vec3(0.0f);

	float dfdq = df(q);
	float C = h4Inv * dfdq / norm;

	return C * (position - p_);
}

float Particle::f(float q)
{
	//if (q <= 0.00001f)
	//	return 0.0f;

	float scaleFactor = 3.0f / (2.0f * PI);
	float result = 0.0f;

	if (q >= 0.0f && q < 1.0f)
		result = 2.0f / 3.0f - pow(q, 2) + 0.5f * pow(q, 3);
	else if (q >= 1.0f && q < 2.0f)
		result = 1.0f / 6.0f * pow(2.0f - q, 3);
	else
		result = 0.0f;

	return scaleFactor * result;
}

float Particle::df(float q)
{
	float scaleFactor = 3.0f / (2.0f * PI);
	float result = 0.0f;

	if (q >= 0.0f && q < 1.0f)
		result = -2 * q + 1.5f * pow(q, 2);
	else if (q >= 1.0f && q < 2.0f)
		result = -0.5f * pow(2 - q, 2);
	else
		result = 0.0f;

	return scaleFactor * result;
}
