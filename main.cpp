#include "app.h"

int main(void)
{
	Application::Config config{
		.fps = 60,
		.screenHeight = 450,
		.screenWidth = 800,
		.windowTitle = "Raylib App",
	};

	Application app(config);
	app.run();

	return 0;
}
