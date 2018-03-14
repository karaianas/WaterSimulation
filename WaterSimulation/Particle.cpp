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

void Particle::adjust(float dt)
{

	float size = 0.5f - 0.05f;
	float threshold = 0.0001f;//0.0001f
	float elasticity = 0.3f;
	int counter = 0;

	if (position.y < -size)
	{
		glm::vec3 normal(0.0f, 1.0f, 0.0f);
		float t = (glm::dot(normal, position) + size) / glm::dot(normal, velocity);
		glm::vec3 hit = position - t * velocity;
		position.y = 2.0f * hit.y - position.y;

		glm::vec3 force_ = glm::dot(velocity, normal) * normal * -mass * (1 + elasticity) / dt;
		glm::vec3 accel = (1.0f / mass) * force_;
		velocity += accel * dt;
		position += velocity * dt;
	}
	else if (position.y > size)
	{
		glm::vec3 normal(0.0f, -1.0f, 0.0f);
		float t = (glm::dot(normal, position) + size) / glm::dot(normal, velocity);
		glm::vec3 hit = position - t * velocity;
		position.y = 2.0f * hit.y - position.y;

		glm::vec3 force_ = glm::dot(velocity, normal) * normal * -mass * (1 + elasticity) / dt;
		glm::vec3 accel = (1.0f / mass) * force_;
		velocity += accel * dt;
		position += velocity * dt;
	}

	if (position.x < -size)
	{
		glm::vec3 normal(1.0f, 0.0f, 0.0f);
		float t = (glm::dot(normal, position) + size) / glm::dot(normal, velocity);
		glm::vec3 hit = position - t * velocity;
		position.x = 2.0f * hit.x - position.x;
		glm::vec3 force_ = glm::dot(velocity, normal) * normal * -mass * (1 + elasticity) / dt;
		glm::vec3 accel = (1.0f / mass) * force_;
		velocity += accel * dt;
		position += velocity * dt;
	}
	else if (position.x > size)
	{
		glm::vec3 normal(-1.0f, 0.0f, 0.0f);
		float t = (glm::dot(normal, position) + size) / glm::dot(normal, velocity);
		glm::vec3 hit = position - t * velocity;
		position.x = 2.0f * hit.x - position.x;
		glm::vec3 force_ = glm::dot(velocity, normal) * normal * -mass * (1 + elasticity) / dt;
		glm::vec3 accel = (1.0f / mass) * force_;
		velocity += accel * dt;
		position += velocity * dt;
	}

	if (position.z < -size)
	{
		glm::vec3 normal(0.0f, 0.0f, 1.0f);
		float t = (glm::dot(normal, position) + size) / glm::dot(normal, velocity);
		glm::vec3 hit = position - t * velocity;
		position.z = 2.0f * hit.z - position.z;
		glm::vec3 force_ = glm::dot(velocity, normal) * normal * -mass * (1 + elasticity) / dt;
		glm::vec3 accel = (1.0f / mass) * force_;
		velocity += accel * dt;
		position += velocity * dt;
	}
	else if (position.z > size)
	{
		glm::vec3 normal(0.0f, 0.0f, -1.0f);
		float t = (glm::dot(normal, position) + size) / glm::dot(normal, velocity);
		glm::vec3 hit = position - t * velocity;
		position.z = 2.0f * hit.z - position.z;
		glm::vec3 force_ = glm::dot(velocity, normal) * normal * -mass * (1 + elasticity) / dt;
		glm::vec3 accel = (1.0f / mass) * force_;
		velocity += accel * dt;
		position += velocity * dt;
	}

}

void Particle::update(float dt)
{
	// (1) Compute acceleration
	glm::vec3 accel = (1.0f / mass) * force;

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
		//if (pow(rho, 2) < 0.00001f || pow(neighbors[i]->rho, 2) < 0.00001f)
		//	return;
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
		if (neighbors[i]->rho < 0.00001f)
			return;
		force_ += neighbors[i]->mass / neighbors[i]->rho * (velocity - neighbors[i]->velocity) \
			* glm::dot(x_ij , dW_ij(neighbors[i]->position)) / float(glm::dot(x_ij, x_ij) * 0.01 * pow(h, 2));
	}
	float viscos = 0.0000001f;// 0.000001f; //0.0000001f;//0.000001f;
	force_ *= 2 * mass * viscos;
	applyForce(force_);
}

void Particle::F_gravity()
{
	glm::vec3 g(0, -9.8, 0);
	//glm::mat4 R = glm::rotate(glm::mat4(1.0f), 2.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	//g = glm::vec3(R * glm::vec4(g, 1.0f));
	glm::vec3 force_ = g * mass;
	applyForce(force_);
}

float Particle::W_ij(glm::vec3 p_)
{
	float q = glm::length(p_ - position) / h;
	
	return 1.0f / pow(h, d) * f(q);
}

glm::vec3 Particle::dW_ij(glm::vec3 p_)
{
	float norm = glm::length(p_ - position);
	float q = norm / h;
	
	//if (q < 0.00001f)
	//	return glm::vec3(0.0f);

	float dfdq = df(q);
	float C = 1.0f / pow(h, d + 1) * dfdq / norm;

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
