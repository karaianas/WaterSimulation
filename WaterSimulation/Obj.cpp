#include "Obj.h"

using namespace std;

Obj::Obj(const char* path)
{
	worldM = glm::mat4(1.0f);
	parser(path);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &VBO2);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Obj::~Obj()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &VBO2);
	glDeleteBuffers(1, &EBO);
}

void Obj::draw(GLuint program, glm::mat4 VP)
{
	glUseProgram(program);

	//worldM = M;
	//glm::mat4 MVP = Window::P * Window::V * worldM;
	glm::mat4 MVP = VP * worldM;

	uMVP = glGetUniformLocation(program, "MVP");
	uModel = glGetUniformLocation(program, "model");
	uColor = glGetUniformLocation(program, "color");

	// Send information
	glUniformMatrix4fv(uMVP, 1, GL_FALSE, &MVP[0][0]);
	glUniformMatrix4fv(uModel, 1, GL_FALSE, &worldM[0][0]);
	glUniform3f(uColor, color[0], color[1], color[2]);

	glBindVertexArray(VAO);

	glPointSize(1.0f);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

void Obj::setColor(glm::vec3 _color)
{
	color = _color;
}

void Obj::parser(const char * path)
{
	// Populate the face indices, vertices, and normals vectors with the OBJ Object data
	FILE* fp;     // file pointer
	float x, y, z;  // vertex coordinates
	float r, g, b;  // vertex color
	int c1, c2;    // characters read from file
	GLuint f0, f1, f2, f0_, f1_, f2_;

	fp = fopen(path, "rb");  // make the file name configurable so you can load other files
	if (fp == NULL) { std::cerr << "error loading file" << std::endl; exit(-1); }  // just in case the file can't be found or is corrupt

	c1 = fgetc(fp);
	c2 = fgetc(fp);

	while (c1 != EOF)
	{
		// if it's a vertex coord
		if ((c1 == 'v') && (c2 == ' '))
		{
			fscanf(fp, "%f %f %f %f %f %f", &x, &y, &z, &r, &g, &b);
			float scalef = 0.05f;//0.01 actual radius
			vertices.push_back(glm::vec3(x * scalef, y * scalef, z * scalef));
			//std::cout << vertices[0].x << " " << vertices[0].y << " " << vertices[0].z << std::endl;
		}
		// if it's a vertex normal
		else if ((c1 == 'v') && (c2 == 'n'))
		{
			fscanf(fp, " %f %f %f", &x, &y, &z);
			normals.push_back(glm::vec3(x, y, z));
			//std::cout << normals[0].x << " " << normals[0].y << " " << normals[0].z << std::endl;
		}
		// if it's a face
		else if ((c1 == 'f') && (c2 == ' '))
		{
			fscanf(fp, "%u//%u %u//%u %u//%u ", &f0, &f0_, &f1, &f1_, &f2, &f2_);
			indices.push_back(f0 - 1);
			indices.push_back(f1 - 1);
			indices.push_back(f2 - 1);
			//cout << f0 - 1 << " " << f1 - 1 << " " << f2 - 1 << endl;
		}
		else if (c1 == '#')
		{
			while (c1 != '\n' && c1 != EOF)
				c1 = fgetc(fp);
		}

		c1 = fgetc(fp);
		c2 = fgetc(fp);
	}

	fclose(fp);   // make sure you don't forget to close the file when done

	std::cout << "DONE parsing" << std::endl;

	if (normals.size() < vertices.size())
		normals = vertices;
}
