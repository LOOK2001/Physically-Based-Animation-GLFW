#include "PbaThing.h"
#include <iostream>

using namespace std;

using namespace pba;

pba::PbaThingyDingy::PbaThingyDingy(const std::string& nam /*= "PbaThingyDingyNoName"*/):
	visible(true),
	dt(1.0 / 24.0)/*(1.0/240.0)*/,
	name(nam),
	animate(true)
{}

void pba::PbaThingyDingy::Keyboard(int key, int scancode, int action, int mods)
{
	if (action != GLFW_PRESS)
		return;

	switch (key)
	{
		case GLFW_KEY_SPACE:
			animate = !animate;
			if (!animate)
			{
				std::cout << "STOP\n";
			}
			else
			{
				std::cout << "START\n";
			}
		break;
		case GLFW_KEY_T:
			if (mods & GLFW_MOD_CAPS_LOCK) {
				dt *= 1.1; std::cout << "time step " << dt << std::endl;
			}	
			else {
				dt /= 1.1; std::cout << "time step " << dt << std::endl;
			}	
		break;
		default:
		break;
	}
}

void PbaThingyDingy::Idle() { if (animate) { solve(); } }

void PbaThingyDingy::Usage()
{
	std::cout << "=== PbaThing ===\n";
	std::cout << "SPACEBAR     start/stop animation\n";
	std::cout << "t/T          reduce/increase animation time step\n";
}
