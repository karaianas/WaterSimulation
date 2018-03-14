#include "Cube.h"

using namespace std;

Cube::Cube()
{
	worldM = glm::mat4(1.0f);// glm::scale(glm::mat4(1.0f), glm::vec3(1.0f) * 0.25f);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO_pos);
	glGenBuffers(1, &VBO_nor);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_pos);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0); 

	glBindBuffer(GL_ARRAY_BUFFER, VBO_nor);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 3 * sizeof(GLfloat), (GLvoid*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Cube::~Cube()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO_pos);
	glDeleteBuffers(1, &VBO_nor);
	glDeleteBuffers(1, &EBO);
}

void Cube::draw(GLuint program, glm::mat4 VP)
{
	glUseProgram(program);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	worldM[3][3] = 4.0f;
	glm::mat4 MVP = VP * worldM;

	uMVP = glGetUniformLocation(program, "MVP");
	uModel = glGetUniformLocation(program, "model");
	uColor = glGetUniformLocation(program, "color");

	// Send information
	glUniformMatrix4fv(uMVP, 1, GL_FALSE, &MVP[0][0]);
	glUniformMatrix4fv(uModel, 1, GL_FALSE, &worldM[0][0]);
	glUniform3f(uColor, color[0], color[1], color[2]);
	glBindVertexArray(VAO);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glLineWidth(2.0f);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glLineWidth(0.5f);

	glBindVertexArray(0);
	glDisable(GL_CULL_FACE);
}

void Cube::setColor(glm::vec3 color_)
{
	color = color_;
}

void Cube::update(glm::mat4 S)
{
	worldM = S;// worldM * S;
}
