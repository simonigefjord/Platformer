#ifndef TILEMAP_HPP
#define TILEMAP_HPP

#include <SFML/Graphics.hpp>
#include <tmxlite/Map.hpp>
#include <tmxlite/TileLayer.hpp>
#include <tmxlite/Layer.hpp>
#include <string>

class TileMap
{
public:
	TileMap();

	void load(std::string filename, sf::Texture& tileset);
	void drawLayer(sf::RenderTarget& target, std::size_t id);

private:
	sf::Sprite mTempSprite;
	tmx::Map mMap;
};

#endif // TILEMAP_HPP

