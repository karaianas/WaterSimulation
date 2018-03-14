#pragma once
#include "Core.h"
#include "Obj.h"
#include "Particle.h"
#include "HashTable.h"
#include "Cube.h"

#define NUM 2000
#define BNUM 0

using namespace std;

class ParticleSystem {
public:

	//Particle particles[NUM];
	vector<Particle*> particles;
	HashTable ht;

	ParticleSystem();
	int tap();

	void update(float dt);
	void draw(GLuint program, glm::mat4 P, glm::mat4 V);
	void drawObj(Obj* obj, GLuint program, glm::mat4 P, glm::mat4 V);
	void drawBox(Cube* cube, GLuint program, glm::mat4 P, glm::mat4 V);
	void initBuffers();

	void setNeighbors();

	//vector<Particle*> particles;

private:
	GLuint VAO, VBO_pos, VBO_nor;
	GLuint uMVP, uModel, uColor;

	vector<glm::vec3> positions;
	vector<glm::vec3> normals;

	float radius, hradius, rho_rest;
	int gridSize;
	int particleCount;
};