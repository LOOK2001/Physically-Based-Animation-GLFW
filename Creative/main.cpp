#include "pba/core/PbaViewer.h"

#include "Object.h"
#include "tests/TestTexture2D.h"
#include "pba/things/BouncingBalls.h"

#include <iostream>

int main(int argc, char* argv[])
{
	PbaViewer* viewer = PbaViewer::Instance();

	std::vector<std::string> vec;
	for (int i = 0; i < argc; i++) {
		vec.push_back(argv[i]);
	}
	viewer->Init(vec);

	Camera* mainCamera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));
	viewer->SetMainCamera(mainCamera);

	Renderer renderer;

	pba::PbaThing currentTest = pba::TextureThing();

	viewer->AddThing(currentTest);

	viewer->MainLoop();

	return 0;
}