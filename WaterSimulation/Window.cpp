#include "Window.h"

#include "ParticleSystem.h"
#include "Obj.h"
#include "Window.h"

const char* window_title = "Water Simulation";
int Window::width;
int Window::height;

#define VERTEX_SHADER_PATH ".//Shaders//shader.vert"
#define FRAGMENT_SHADER_PATH ".//Shaders//shader.frag"
GLint shaderProgram;

float dtheta = 3.14159f / 60;
float theta = 0.0f;
float radius = 5.0f;
float cylheight = 0.0f;

glm::vec3 cam_pos(radius * sin(theta), cylheight, radius * cos(theta));
//glm::vec3 cam_look_at(0.0f, cylheight, 0.0f);
glm::vec3 cam_look_at(0.0f, 0.0f, 0.0f);
glm::vec3 cam_up(0.0f, 1.0f, 0.0f);
glm::mat4 Window::P;
glm::mat4 Window::V;

bool wireframe = false;
bool isLeft = false;
bool isRight = false;
bool isPlay = false;

float totalTime = 0.0f;
glm::vec2 prev_pos;
glm::vec2 prev_pos2;

ParticleSystem ps;
Obj* obj;
Cube* box;

float boxScale = 1.0f;

using namespace std;

void Window::initialize_objects()
{
	shaderProgram = LoadShaders(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);
	obj = new Obj(".//Models//particle_low2.obj");
	obj->setColor(glm::vec3(0.3f, 0.7f, 1.0f));
	box = new Cube();
	box->setColor(glm::vec3(0.5f, 0.7f, 0.9f));
	//ps.update(0.1f);

}

void Window::clean_up()
{
	//delete M;
	glDeleteProgram(shaderProgram);
}

GLFWwindow* Window::create_window(int width, int height)
{
	// Initialize GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return NULL;
	}

	// 4x antialiasing
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__
	// Ensure that minimum OpenGL version is 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Enable forward compatibility and allow a modern OpenGL context
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create the GLFW window
	GLFWwindow* window = glfwCreateWindow(width, height, window_title, NULL, NULL);

	// Check if the window could not be created
	if (!window)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		fprintf(stderr, "Either GLFW is not installed or your graphics card does not support modern OpenGL.\n");
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window
	glfwMakeContextCurrent(window);

	// Set swap interval to 1
	glfwSwapInterval(1);

	// Get the width and height of the framebuffer to properly resize the window
	glfwGetFramebufferSize(window, &width, &height);
	// Call the resize callback to make sure things get drawn immediately
	Window::resize_callback(window, width, height);

	return window;
}

void Window::resize_callback(GLFWwindow* window, int width, int height)
{
#ifdef __APPLE__
	glfwGetFramebufferSize(window, &width, &height); // In case your Mac has a retina display
#endif
	Window::width = width;
	Window::height = height;
	// Set the viewport size. This is the only matrix that OpenGL maintains for us in modern OpenGL!
	glViewport(0, 0, width, height);

	if (height > 0)
	{
		P = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);
		V = glm::lookAt(cam_pos, cam_look_at, cam_up);
	}
}

void Window::idle_callback()
{
	if (isPlay)
	{
		ps.update(0.01f);//0.0001f
	}
	
}

void Window::display_callback(GLFWwindow* window)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	ps.drawObj(obj, shaderProgram, P, V);
	ps.drawBox(box, shaderProgram, P, V);

	glfwPollEvents();
	glfwSwapBuffers(window);
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{

	if (action == GLFW_PRESS)
	{
		if (key == GLFW_KEY_ESCAPE)
		{
			glfwSetWindowShouldClose(window, GL_TRUE);
		}

		if (key == GLFW_KEY_SPACE)
		{
			if (isPlay)
				isPlay = false;
			else
				isPlay = true;
		}

		if (key == GLFW_KEY_UP)
		{
			if (boxScale < 2.0f)
			{
				//cout << boxScale << endl;
				boxScale += 0.1f;
				box->update(glm::scale(glm::mat4(1.0f), glm::vec3(1.0f) * boxScale));
				ps.boxSizeUpdate(boxScale);
			}
		}

		if (key == GLFW_KEY_DOWN)
		{
			if (boxScale > 1.1f)
			{
				boxScale -= 0.1f;
				box->update(glm::scale(glm::mat4(1.0f), glm::vec3(1.0f) * boxScale));
				ps.boxSizeUpdate(boxScale);
			}
		}

		if (key == GLFW_KEY_P)
		{
			ps.tap();
		}

		if (key == GLFW_KEY_A)
		{
			theta -= dtheta;
			cam_pos = glm::vec3(radius * sin(theta), cylheight, radius * cos(theta));
			cam_look_at[1] = cylheight;
			V = glm::lookAt(cam_pos, cam_look_at, cam_up);
		}

		if (key == GLFW_KEY_D)
		{
			theta += dtheta;
			cam_pos = glm::vec3(radius * sin(theta), cylheight, radius * cos(theta));
			cam_look_at[1] = cylheight;
			V = glm::lookAt(cam_pos, cam_look_at, cam_up);
		}

		if (key == GLFW_KEY_W)
		{
			cylheight += 0.2f;
			cam_pos[1] = cylheight;
			cam_look_at[1] = cylheight;
			V = glm::lookAt(cam_pos, cam_look_at, cam_up);
		}

		if (key == GLFW_KEY_S)
		{
			cylheight -= 0.2f;
			cam_pos[1] = cylheight;
			cam_look_at[1] = cylheight;
			V = glm::lookAt(cam_pos, cam_look_at, cam_up);
		}

		// Wireframe mode
		if (key == GLFW_KEY_1)
		{
			if (wireframe)
				wireframe = false;
			else
				wireframe = true;
		}

	}
}

void Window::mouse_callback(GLFWwindow* window, int button, int action, int mods)
{

	if (button == GLFW_MOUSE_BUTTON_LEFT)
	{
		if (action == GLFW_PRESS)
		{
			double xpos;
			double ypos;
			glfwGetCursorPos(window, &xpos, &ypos);
			prev_pos = glm::vec2(xpos, ypos);
			isLeft = true;
		}
		else if (action == GLFW_RELEASE)
		{
			isLeft = false;
		}
	}

	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		double xpos;
		double ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		prev_pos2 = glm::vec2(xpos, ypos);
		isRight = true;
	}
	else
		isRight = false;

}

glm::vec3 Window::trackball(glm::vec2 point)
{
	glm::vec3 mapped;
	float d;
	mapped.x = (2.0f * point.x - width) / width;
	mapped.y = (height - 2.0f * point.y) / height;
	mapped.z = 0.0f;

	d = glm::length(mapped);
	d = (d < 1.0f) ? d : 1.0f;
	mapped.z = sqrtf(1.001f - d * d);
	mapped = glm::normalize(mapped);

	return mapped;
}

void Window::cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (isLeft)
	{

		float wx = (2.0f * xpos - width) / width;
		float wy = (height - 2.0f * ypos) / height;

		float wx_ = (2.0f * prev_pos.x - width) / width;
		float wy_ = (height - 2.0f * prev_pos.y) / height;

		glm::mat4 T = glm::translate(glm::mat4(1.0f), glm::vec3(wx - wx_, wy - wy_, 0.0f));
		glm::mat4 PV = P * V;

		// sth

		prev_pos = glm::vec2(xpos, ypos);
	}
	if (isRight)
	{
		//// Calculate angle, axis, and rotation mtx
		//glm::vec3 prev_pos_ball = trackball(prev_pos2);
		//glm::vec3 curr_pos_ball = trackball(glm::vec2(xpos, ypos));

		//float angle = glm::degrees(glm::acos(std::min(1.0f, glm::dot(curr_pos_ball, prev_pos_ball))));
		//glm::vec3 axis = glm::normalize(glm::cross(prev_pos_ball, curr_pos_ball));
		//glm::mat4 R = glm::rotate(glm::mat4(1.0f), angle * 0.01f, axis);

		//// Point rotation
		//glm::mat4 PV = P * V;
		//glm::vec4 p = glm::vec4(cp->getPos(), 1.0f);
		//cp->setPos(R * p);
		//cloth->updateWind(cp->getPos());
		//parachute->updateWind(cp->getPos());

		//// Model rotation
		////M->rotate(R);

		//// Camera rotation
		////cam_pos = glm::vec3(glm::rotate(glm::mat4(1.0f), -angle * 0.01f, axis) * glm::vec4(cam_pos, 1.0f));
		////cam_up = glm::vec3(glm::rotate(glm::mat4(1.0f), -angle * 0.01f, axis) * glm::vec4(cam_up, 1.0f));
		////V = glm::lookAt(cam_pos, cam_look_at, cam_up);

		//prev_pos2 = glm::vec2(xpos, ypos);
	}
}

void Window::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	float scalefactor = (yoffset > 0) ? 1.1f : 1 / 1.1f;
	//cam_pos = glm::vec3(glm::scale(glm::mat4(1.0f), glm::vec3(scalefactor)) * glm::vec4(cam_pos, 1.0f));
	if (isRight)
	{
		//glm::vec4 p = glm::vec4(cp->getPos(), 1.0f);
		//cp->setPos(scalefactor * p);
		//cloth->updateWind(cp->getPos());
		//parachute->updateWind(cp->getPos());
	}
	else
	{
		radius *= scalefactor;
		cam_pos = glm::vec3(radius * sin(theta), cylheight, radius * cos(theta));
		V = glm::lookAt(cam_pos, cam_look_at, cam_up);
	}
}
