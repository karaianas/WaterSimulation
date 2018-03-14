#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

uniform mat4 MVP;
uniform mat4 model;
uniform vec3 color;
//uniform vec2 light;

//out vec3 P;
out vec3 N;
out vec3 C;
//out vec2 lights;

void main()
{
    gl_Position = MVP * vec4(position.x, position.y, position.z, 1.0);
	//P = vec3(model * vec4(position, 1.0f));
	N = mat3(transpose(inverse(model))) * normal;
	C = color;
    //sampleExtraOutput = color;
	//lights = light;
}
