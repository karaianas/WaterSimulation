#pragma once
#include "Core.h"
#include "Obj.h"
#include "Particle.h"
#include "HashTable.h"
#include "Cube.h"

using namespace std;

class ParticleSystem {
public:

	//Particle particles[NUM];
	vector<Particle*> particles;
	HashTable ht;

	ParticleSystem();
	~ParticleSystem();

	int tap();
	void initialize(bool mode);
	void boxSizeUpdate(float size_);
	void viscosityUpdate(bool mode);
	void stiffUpdate(bool mode);
	void colorModeUpdate();

	void update(float dt);
	void draw(GLuint program, glm::mat4 P, glm::mat4 V);
	void drawObj(Obj* obj, GLuint program, glm::mat4 P, glm::mat4 V);
	void drawBox(Cube* cube, GLuint program, glm::mat4 P, glm::mat4 V);
	void initBuffers();

	void setNeighbors();

	void splashDemo();
	void collisionDemo();
	void gravityDemo(glm::mat4 R);

	//vector<Particle*> particles;

private:
	GLuint VAO, VBO_pos, VBO_nor;
	GLuint uMVP, uModel, uColor;

	vector<glm::vec3> positions;
	vector<glm::vec3> normals;

	float radius, smoothLen, rho_rest, mass, k, d, viscosity;
	float boxSize, boxElasticity;
	int gridSize;
	int NUM;

	bool colorMode;
	bool isColl;
};