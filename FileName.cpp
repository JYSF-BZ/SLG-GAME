#include<SFML/Graphics.hpp>
#include <type_traits>
#include"Application.h"
#include<ctime>

int main() {
	srand(time(0));
	Application app;
	while (app.IsRunning()) {
		app.ProcessEvents();
		app.Update();
		app.Render();
	}
	return 0;
}