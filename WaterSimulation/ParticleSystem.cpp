#include "ParticleSystem.h"

using namespace std;

ParticleSystem::ParticleSystem()
{
	particleCount = 0;

	//radius = 0.01f;
	//hradius = 0.01f;
	//rho_rest = 3000;
	radius = 0.01f;
	hradius = 0.1f;//0.08
	float mass = 0.3f;// pow(hradius, 3) * rho_rest;
	rho_rest = mass / pow(hradius, 3);
	float k = 1.0f;
	int d = 3;

	gridSize = 1000;
	float spacing = hradius;
	ht = HashTable(gridSize, spacing);

	//float tem = sqrt(100);
	//int count = 0;
	//for (int i = 0; i < tem; i++)
	//{
	//	for (int j = 0; j < tem; j++)
	//	{
	//		Particle* p = new Particle();
	//		p->id = count;
	//		p->setConstants(d, hradius, rho_rest, mass, k);
	//		p->position = glm::vec3(float(i) / tem - 0.4f, 0.0f, float(j) / tem - 0.4f);
	//		particles.push_back(p);
	//		count++;
	//	}
	//}

	//for (int i = 0; i < tem; i++)
	//{
	//	for (int j = 0; j < tem; j++)
	//	{
	//		Particle* p = new Particle();
	//		p->id = count;
	//		p->setConstants(d, hradius, rho_rest, mass, k);
	//		p->position = glm::vec3(float(i) / tem - 0.43f, 0.2f, float(j) / tem - 0.43f);
	//		particles.push_back(p);
	//		count++;
	//	}
	//}

	for (int i = 0; i < NUM; i++)
	{
		Particle* p = new Particle();
		p->id = i;
		p->setConstants(d, hradius, rho_rest, mass, k);

		float dx = float(rand() % 1000) / 1000.0f - 0.5f;
		float dy = float(rand() % 1000) / 1000.0f - 0.5f;
		float dz = float(rand() % 1000) / 1000.0f - 0.5f;
		p->position = 1.0f * glm::vec3(dx, dy, dz);
		p->velocity = 1.0f * glm::vec3(dx, dy, dz);
		particles.push_back(p);
		//ht.addToCell(&particles[particleCount]);

	}

	//setNeighbors();

}

void ParticleSystem::update(float dt)
{
	// (1) Find neighbors
	for (int i = 0; i < NUM; i++)
		particles[i]->flag = false;

	for (int i = 0; i < NUM; i++)
	{
		particles[i]->neighbors.clear();
		for (int j = 0; j < NUM + BNUM; j++)
		{
			if (particles[i]->id != particles[j]->id)
			{
				float len = glm::length(particles[i]->position - particles[j]->position);
				if (len <= 2.0f * hradius)
				{
					if (i == 327)
						particles[j]->flag = true;
					particles[i]->neighbors.push_back(particles[j]);
				}
			}
		}
	}

	// (2) Compute density and pressure
	for (int i = 0; i < NUM; i++)
	{
		particles[i]->calcDensity();
		particles[i]->calcPressure();
	}

	// (3) Compute forces
	for (int i = 0; i < NUM; i++)
	{
		particles[i]->F_pressure();
		particles[i]->F_viscosity();
		particles[i]->F_gravity();
	}

	// (4) Update particles and the hash table
	for (int i = 0; i < NUM; i++)
	{
		particles[i]->update(dt);
	}
}
/*
void ParticleSystem::update(float dt)
{
	// (1) Find neighbors
	for (int i = 0; i < NUM; i++)
	{
		particles[i].neighbors.clear();
		for (int j = 0; j < NUM; j++)
		{
			if (particles[i].id != particles[j].id)
			{
				float len = glm::length(particles[i].position - particles[j].position);
				if (len <= 2.0f * hradius)
				{
					//cout << i << ": " << j << endl;
					particles[i].neighbors.push_back(&particles[j]);
				}
			}
		}
		//if(particles[i].neighbors.size() > 1)
		//	cout << i << ":" << particles[i].neighbors.size() << endl;
		//ht.getNeighbors(&particles[i]);

	}

	// (2) Compute density and pressure
	for (int i = 0; i < NUM; i++)
	{
		particles[i].calcDensity();
		particles[i].calcPressure();
		//if(particles[i].neighbors.size() > 0)
		//{
		//	//cout << particles[i].neighbors.size() << endl;
		//	//cout << i <<":" << particles[i].rho << endl;
		//	//cout << i << ":" << particles[i].pressure << endl;
		//	//cout << particles[i].rho << " / " << rho_rest << " = " << particles[i].rho / rho_rest << endl;
		//}
	}

	// (3) Compute forces
	for (int i = 0; i < NUM; i++)
	{
		particles[i].F_pressure();
		//particles[i].F_viscosity();
		particles[i].F_gravity();
	}

	// (4) Update particles and the hash table
	for (int i = 0; i < NUM; i++)
	{
		//int prev = ht.getCell(particles[i].position);
		particles[i].update(dt);
		//int post = ht.getCell(particles[i].position);

		//if (prev != post)
		//{
		//	ht.deleteFromCell(prev, &particles[i]);
		//	ht.addToCell2(post, &particles[i]);
		//}
	}
}
*/
void ParticleSystem::draw(GLuint program, glm::mat4 P, glm::mat4 V)
{
	glUseProgram(program);

	initBuffers();

	glm::mat4 worldM = glm::mat4(1.0f);
	glm::mat4 MVP = P * V *worldM;

	uMVP = glGetUniformLocation(program, "MVP");
	uModel = glGetUniformLocation(program, "model");
	uColor = glGetUniformLocation(program, "color");

	// Now send these values to the shader program
	glUniformMatrix4fv(uMVP, 1, GL_FALSE, &MVP[0][0]);
	glUniformMatrix4fv(uModel, 1, GL_FALSE, &worldM[0][0]);
	//glUniform3f(uColor, 0.4f * 0.8f, 0.7f * 0.8f, 1.0f * 0.8f);
	glUniform3f(uColor, 1.0f, 0.0f, 0.0f);

	glBindVertexArray(VAO);
	glPointSize(radius * 200.0f);
	glDrawArrays(GL_POINTS, 0, NUM);
	glBindVertexArray(0);
}

void ParticleSystem::drawObj(Obj* obj, GLuint program, glm::mat4 P, glm::mat4 V)
{
	initBuffers();
	int count = 0;
	for (int i = 0; i < NUM + BNUM; i++)
	{
		glm::vec3 pos = particles[i]->position;
		glm::mat4 M(1.0f);
		M[3] = glm::vec4(pos, 1.0f);
		obj->setColor(glm::vec3(0.2f, 0.4f, 1.0f));
		
		//if (count == 327)
		//{
		//	obj->setColor(glm::vec3(1.0f, 0.0f, 0.0f));
		//}
		//else
		//{
		//	if(particles[i]->flag)
		//		obj->setColor(glm::vec3(0.0f, 1.0f, 0.0f));
		//	else
		//		obj->setColor(glm::vec3(0.0f, 1.0f, 1.0f) * float(particles[i]->id) / float(NUM));
		//}
		//
		obj->setWorldM(M);
		obj->draw(program, P * V);
		count++;
	}
}

void ParticleSystem::drawBox(Cube * cube, GLuint program, glm::mat4 P, glm::mat4 V)
{
	cube->draw(program, P * V);
}

void ParticleSystem::initBuffers()
{
	positions.clear();
	for (int i = 0; i < NUM; i++)
		positions.push_back(particles[i]->position);

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO_pos);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO_pos);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_pos);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * positions.size(), positions.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);

	//glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * tnormals.size(), tnormals.data(), GL_STATIC_DRAW);

	//glEnableVertexAttribArray(1);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	
}

void ParticleSystem::setNeighbors()
{
	//for(int i = 0; i < NUM; i++)
	//{
	//	ht.getNeighbors(&particles[i]);
	//}
}
