#include "app.h"

int main(void)
{
	Application::Config config{
		.fps = 60,
		.monitor = 1,
		.screenHeight = 450,
		.screenWidth = 800,
		.windowTitle = "Raylib App",
		.models = {
			{ "../assets/models/plane.obj", "../assets/textures/plane_diffuse.png" }
		},
	};

	Application app(config);
	app.run();

	return 0;
}
