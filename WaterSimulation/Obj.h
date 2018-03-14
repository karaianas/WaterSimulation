#pragma once
#include "Core.h"

using namespace std;

class Obj {
public:
	Obj(const char* path);
	~Obj();

	void draw(GLuint program, glm::mat4 VP);
	void setColor(glm::vec3 _color);
	void setWorldM(glm::mat4 M) { worldM = M; };

	void parser(const char* path);

private:
	glm::mat4 worldM;
	vector<glm::vec3> vertices;
	vector<glm::vec3> normals;
	vector<GLuint> indices;

	GLuint VBO, VAO, VBO2, EBO;
	GLuint uMVP, uModel, uColor;
	glm::vec3 color;
};