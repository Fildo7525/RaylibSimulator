#include "app.h"
#include "loader.h"

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

	rl::Object plane(rl::Model{ PLANE_MODEL_PATH, PLANE_TEXTURE_PATH });
	app.addObject(plane);

	app.run();

	return 0;
}
