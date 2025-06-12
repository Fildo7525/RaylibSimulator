#include "app.h"

#include "plane.h"

int main(void)
{
	rl::Application::Config config{
		.fps = 60,
		.monitor = 1,
		.screenHeight = 450,
		.screenWidth = 800,
		.windowTitle = "Raylib App",
		.camera = nullptr,
	};

	rl::Application app(config);

	app.addObject(Plane::create(rl::Model::fromFile("../resources/plane.json")));

	app.run();

	return 0;
}
