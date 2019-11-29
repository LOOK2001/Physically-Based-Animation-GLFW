#include <iostream>

#include "PbaViewer.h"


Camera* PbaViewer::pMainCamera(nullptr);
GLFWwindow* PbaViewer::window(nullptr);
const unsigned int PbaViewer::SCR_WIDTH = 1280;
const unsigned int PbaViewer::SCR_HEIGHT = 720;
float PbaViewer::lastX = SCR_WIDTH / 2.0f;
float PbaViewer::lastY = SCR_HEIGHT / 2.0f;
bool PbaViewer::firstMouse = true;
std::vector<pba::PbaThing> PbaViewer::things;


void PbaViewer::processInput(GLFWwindow* window, double deltaTime)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		pMainCamera->ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		pMainCamera->ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		pMainCamera->ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		pMainCamera->ProcessKeyboard(RIGHT, deltaTime);
}

void PbaViewer::framebuffer_size_callback(GLFWwindow* window, int width, int heigh)
{
	glViewport(0, 0, width, heigh);
}

void PbaViewer::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	pMainCamera->ProcessMouseMovement(xoffset, yoffset);
}

void PbaViewer::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	pMainCamera->ProcessMouseScroll(yoffset);
}

void PbaViewer::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (action != GLFW_PRESS)
		return;

	switch (button)
	{
	case GLFW_MOUSE_BUTTON_LEFT:
		std::cout << "Mouse left button clicked!" << std::endl;
		break;
	case GLFW_MOUSE_BUTTON_MIDDLE:
		std::cout << "Mouse middle button clicked!" << std::endl;
		break;
	case GLFW_MOUSE_BUTTON_RIGHT:
		std::cout << "Mouse right button clicked!" << std::endl;
		break;
	default:
		return;
	}

	for (auto thing : things)
	{
		thing->Mouse(button, action, mods);
	}
}

void PbaViewer::Keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action != GLFW_PRESS)
		return;

	switch (key)
	{
	case GLFW_KEY_R:
		Reset();
		break;
	case GLFW_KEY_H:
		Home();
	default:
		break;
	}

	for (auto thing : things)
	{
		thing->Keyboard(key, scancode, action, mods);
	}
}

int PbaViewer::Init(const std::vector<std::string>& args)
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

	// glfw window creation
	// --------------------
	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Physically Based Animation", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetKeyCallback(window, Keyboard_callback);

	// tell GLFW to capture the mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	glEnable(GL_DEPTH_TEST);
}

void PbaViewer::MainLoop()
{
	Usage();

	while (!glfwWindowShouldClose(window))
	{
		// pre-frame time logic
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		processInput(window, deltaTime);

		// render
		renderer->Clear();

		Idle();

		for (size_t i = 0; i < things.size(); i++)
		{
			things[i]->OnUpdate(0.0f);
		}

		Display();

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
}

void PbaViewer::Display()
{
	for (size_t i = 0; i < things.size(); i++)
	{
		if (things[i]->isVisible()) { things[i]->Display(); }
	}
}

void PbaViewer::Idle()
{
	for (size_t i = 0; i < things.size(); i++)
	{
		things[i]->Idle();
	}
}

void PbaViewer::Reset()
{
	std::cout << "Reset\n";
	for (size_t i = 0; i < things.size(); i++)
	{
		things[i]->Reset();
	}
}

void PbaViewer::Home()
{
	std::cout << "Home\n";
	pMainCamera->Reset();
	for (size_t i = 0; i < things.size(); i++)
	{
		things[i]->Home();
	}
}

void PbaViewer::AddThing(pba::PbaThing& t)
{
	things.push_back(t);
	std::cout << t->Name() << "added to viewer.\n";
}

void PbaViewer::Usage()
{
	using namespace std;

	cout << "PBA Viewer   usage:\n";
	cout << "f/F          reduce/increase the camera FOV\n";
	cout << "+/=          move camera farther from the view point\n";
	cout << "-/_          move camera closer to the view point\n";
	cout << "SHIFT+mouse  move camera perpendicular to the view direction\n";
	cout << "r            reset sim parameters\n";
	cout << "h            home display parameters\n";

	for (size_t i = 0; i < things.size(); i++)
	{
		things[i]->Usage();
	}
}

PbaViewer* PbaViewer::pPbaViewer = nullptr;

PbaViewer::PbaViewer()
{
	renderer = new Renderer();

	std::cout << "PbaViewer Loaded\n";
}

