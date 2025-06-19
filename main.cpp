#include "app.h"

#include "drone.h"
#include "plane.h"
#include "spaceship.h"

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

	app.addObject(Plane::create(rl::Model::fromFile(PLANE_CONFIG_PATH)));
	app.addObject(Drone::create(rl::Model::fromFile(DRONE_CONFIG_PATH)));
	app.addObject(Spaceship::create(rl::Model::fromFile(SPACESHIP_CONFIG_PATH)));

	app.run();

	return 0;
}
