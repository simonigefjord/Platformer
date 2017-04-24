#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "ResourceHolder.hpp"
#include "ResourceIdentifiers.hpp"
#include "AnimatedSprite.hpp"
#include "Animation.hpp"
#include "TileMap.hpp"

#include <SFML/Graphics.hpp>
#include <string>

class Application
{
public:
	Application(sf::VideoMode& videoMode, std::string title);
	void run();

private:
	void centerRenderWindow();
	void processInput();
	void load();
	void update(sf::Time deltaTime);
	void updateStatistics(sf::Time elapsedTime);
	void draw(sf::RenderTarget& target);
	void drawStatistics(sf::RenderTarget& target);
	sf::Vector2f centerText(sf::Text& text);

private:
	static const sf::Vector2u Scale;
	static const sf::Time TimePerFrame;
	static const sf::Color ClearColor;
	static const bool Debug;
	sf::RenderWindow mRenderWindow;
	sf::RenderTexture mRenderTexture;
	bool mVSyncEnabled;
	TextureHolder mTextures;
	FontHolder mFonts;
	sf::Text mStatisticsText;
	sf::Time mStatisticsUpdateTime;
	unsigned int mStatisticsNumFrames;
	sf::Text mPausedText;
	bool mPaused;

	AnimatedSprite mSprite;
	Animation mWalk, mJump;
	Animation* mCurrentAnimation;
	TileMap mMap;
};

#endif // APPLICATION_HPP
