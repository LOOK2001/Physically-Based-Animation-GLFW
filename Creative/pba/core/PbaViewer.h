#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <string>

#include "../../Camera.h"
#include "../../Renderer.h"
#include "../../Object.h"
#include "PbaThing.h"


// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class PbaViewer
{
public:
	static PbaViewer* Instance()
	{
		if (pPbaViewer == nullptr)
			pPbaViewer = new PbaViewer();

		return pPbaViewer;
	}

	~PbaViewer() {};

	//! Initialization, including GLUT initialization.
	int Init(const std::vector<std::string>& args);
	//! render loop
	void MainLoop();

	//! Cascading callback for initiating a display event
	static void Display();
	//! Cascading callback for an idle  event 
	static void Idle();
	//! Cascading callback for reseting parameters
	static void Reset();
	//! Cascading callback to home parameters
	static void Home();

	void SetMainCamera(Camera* _camera) { pMainCamera = _camera; };
	Camera* GetMainCamera() { return pMainCamera; };

	GLFWwindow* getWindow() { return window; }

	const unsigned int GetScreenWidth() { return SCR_WIDTH; }
	const unsigned int GetScreenHeight() { return SCR_HEIGHT; }

	void AddThing(pba::PbaThing& t);

	//! Cascading callback for usage information
	void Usage();

private:
	// process all input: query GLFW whether relevant keys are pressed/released
	// this frame and react accordingly
	static void processInput(GLFWwindow* window, double deltaTime);

	// glfw: whenever the window size changed (by OS or user resize) 
	// this callback function executes
	static void framebuffer_size_callback(GLFWwindow* window, int width, int heigh);

	// glfw: whenever the mouse moves, this callback is called
	static void mouse_callback(GLFWwindow* window, double xpos, double ypos);

	// glfw: whenever the mouse scroll wheel scrolls, this callback is called
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

	// glfw: whenever the mouse presses, this callback is called
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int modes);

	// glfw: whenever the key presses, this callback is called
	static void Keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

private:
	//static PbaViewer* pMainCamera;
	static PbaViewer* pPbaViewer;
	static Camera* pMainCamera;

	static GLFWwindow* window;

	// settings
	static const unsigned int SCR_WIDTH;
	static const unsigned int SCR_HEIGHT;

	// camera
	static float lastX;
	static float lastY;
	static bool firstMouse;

	// timing
	float deltaTime = 0.0f;	// time between current frame and last frame
	float lastFrame = 0.0f;

	Renderer* renderer;

	// These are the objects that do the important processing. 
	static std::vector<pba::PbaThing> things;

	PbaViewer();
};