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
		.camera{ 0 },
	};

	rl::Application app(config);

	app.addObject(Plane::create(rl::Model{ PLANE_MODEL_PATH, PLANE_TEXTURE_PATH }));

	app.run();

	return 0;
}
