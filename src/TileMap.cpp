#include "TileMap.hpp"

#include <SFML/Graphics.hpp>
#include <tmxlite/Map.hpp>
#include <tmxlite/TileLayer.hpp>
#include <tmxlite/Layer.hpp>
#include <string>
#include <iostream>


TileMap::TileMap() :
	mTempSprite(),
	mMap()
{

}

void TileMap::load(std::string filename, sf::Texture& tileset)
{
	mMap.load(filename);
	mTempSprite.setTexture(tileset);
}

void TileMap::drawLayer(sf::RenderTarget& target, std::size_t id)
{
	const auto& layers = mMap.getLayers();
	const auto& layer = *dynamic_cast<const tmx::TileLayer*>(layers[id].get());

	if(layer.getType() != tmx::Layer::Type::Tile) return;

	sf::Vector2u tileSize;
	tileSize.x = mMap.getTileSize().x;
	tileSize.y = mMap.getTileSize().y;

	std::size_t width = mMap.getBounds().width / tileSize.x;
	std::size_t height = mMap.getBounds().height / tileSize.y;

	const auto& layerIDs = layer.getTiles();

	for(std::size_t y = 0; y < height; y++)
	{
		for(std::size_t x = 0; x < width; x++)
		{
			std::size_t i = x + y * width;
			if(i < layerIDs.size() && i >= 0)
			{
				std::uint32_t id = layerIDs[i].ID - 1;
				if(id == 0) continue;


				int tilesetWidth = mTempSprite.getTexture()->getSize().x / tileSize.x;

				int rectX = id % tilesetWidth;
				int rectY = id / tilesetWidth;

				mTempSprite.setTextureRect(sf::IntRect (rectX * tileSize.x, rectY * tileSize.y, tileSize.x, tileSize.y));
				mTempSprite.setPosition(x * tileSize.x, y * tileSize.y);

				target.draw(mTempSprite);
			}

		}
	}
}

