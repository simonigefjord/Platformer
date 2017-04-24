#include "Application.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

int main()
{
	sf::VideoMode videoMode(256  * 3, 192 * 3);

	Application app(videoMode, "Game");
	app.run();

	return 0;
}
