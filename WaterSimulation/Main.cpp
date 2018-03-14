#include "Main.h"

GLFWwindow* window;

void error_callback(int error, const char* description)
{
	// Print error
	fputs(description, stderr);
}

void setup_callbacks()
{
	// Set the error callback
	glfwSetErrorCallback(error_callback);
	// Set the key callback
	glfwSetKeyCallback(window, Window::key_callback);
	// Set the window resize callback
	glfwSetFramebufferSizeCallback(window, Window::resize_callback);
	// Set the mouse callback
	glfwSetMouseButtonCallback(window, Window::mouse_callback);
	// Set the cursor callback
	glfwSetCursorPosCallback(window, Window::cursor_pos_callback);
	// Set the mouse scroll callback
	glfwSetScrollCallback(window, Window::scroll_callback);
}

void setup_glew()
{

#ifndef __APPLE__
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{

		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		glfwTerminate();
	}
	fprintf(stdout, "Current GLEW version: %s\n", glewGetString(GLEW_VERSION));
#endif
}

void setup_opengl_settings()
{
#ifndef __APPLE__
	// Setup GLEW. Don't do this on OSX systems.
	setup_glew();
#endif
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDisable(GL_CULL_FACE);
	glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
}

void print_versions()
{
	// Get info of GPU and supported OpenGL version
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("OpenGL version supported %s\n", glGetString(GL_VERSION));

	//If the shading language symbol is defined
#ifdef GL_SHADING_LANGUAGE_VERSION
	std::printf("Supported GLSL version is %s.\n", (char *)glGetString(GL_SHADING_LANGUAGE_VERSION));
#endif
}

int main(void)
{
	// Create the GLFW window
	window = Window::create_window(720, 720);
	// Print OpenGL and GLSL versions
	print_versions();

	setup_callbacks();
	setup_opengl_settings();
	Window::initialize_objects();

	// Loop while GLFW window should stay open
	while (!glfwWindowShouldClose(window))
	{
		Window::display_callback(window);
		Window::idle_callback();
	}

	Window::clean_up();

	glfwDestroyWindow(window);

	glfwTerminate();

	exit(EXIT_SUCCESS);
}