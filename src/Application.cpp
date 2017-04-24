#include "Application.hpp"
#include "Animation.hpp"
#include "TileMap.hpp"

#include <SFML/Graphics.hpp>
#include <string>
#include <tmxlite/Map.hpp>
#include <tmxlite/TileLayer.hpp>
#include <iostream>

const sf::Vector2u Application::Scale = sf::Vector2u(3, 3);
const sf::Time Application::TimePerFrame = sf::seconds(1.f / 60.f);
const sf::Color Application::ClearColor = sf::Color(93, 152, 141);
const bool Application::Debug = true;

Application::Application(sf::VideoMode& videoMode, std::string title) :
	mRenderWindow(videoMode, title, sf::Style::Close),
	mRenderTexture(),
	mVSyncEnabled(true),
	mTextures(),
	mFonts(),
	mStatisticsText(),
	mStatisticsUpdateTime(),
	mStatisticsNumFrames(0),
	mPausedText(),
	mPaused(false),
	mSprite(sf::seconds(0.2), true, false),
	mWalk(),
	mJump(),
	mCurrentAnimation(),
	mMap()
{
	mRenderWindow.setVerticalSyncEnabled(mVSyncEnabled);
	mRenderWindow.setMouseCursorVisible(false);
	mRenderTexture.create(videoMode.width / Scale.x, videoMode.height / Scale.y);
	centerRenderWindow();
	load();
}

void Application::load()
{
	mTextures.load(Textures::Characters, "assets/textures/characters.png");
	mTextures.load(Textures::Sheet, "assets/textures/sheet.png");
	mTextures.load(Textures::Swoosh, "assets/textures/swoosh.png");

	mFonts.load(Fonts::UbuntuR, "assets/fonts/Ubuntu-R.ttf");
	mFonts.load(Fonts::UbuntuB, "assets/fonts/Ubuntu-B.ttf");

	mStatisticsText.setFont(mFonts.get(Fonts::UbuntuB));
	mStatisticsText.setCharacterSize(14);
	mStatisticsText.setPosition(5.f, 5.f);
	mStatisticsText.setOutlineColor(sf::Color::Black);
	mStatisticsText.setOutlineThickness(1.f);

	mPausedText.setString("Game paused...");
	mPausedText.setFont(mFonts.get(Fonts::UbuntuR));
	mPausedText.setCharacterSize(40);
	mPausedText.setOutlineColor(sf::Color::Black);
	mPausedText.setOutlineThickness(1.f);

	mPausedText.setPosition(centerText(mPausedText));

	mWalk.setSpriteSheet(mTextures.get(Textures::Characters));
	mWalk.addFrame(sf::IntRect(0 * 32, 0, 32, 32));
	mWalk.addFrame(sf::IntRect(1 * 32, 0, 32, 32));
	mWalk.addFrame(sf::IntRect(2 * 32, 0, 32, 32));
	mWalk.addFrame(sf::IntRect(3 * 32, 0, 32, 32));

	mJump.setSpriteSheet(mTextures.get(Textures::Characters));
	mJump.addFrame(sf::IntRect(0 * 32, 0, 32, 32));
	mJump.addFrame(sf::IntRect(1 * 32, 0, 32, 32));
	mJump.addFrame(sf::IntRect(2 * 32, 0, 32, 32));
	mJump.addFrame(sf::IntRect(3 * 32, 0, 32, 32));

	mCurrentAnimation = &mWalk;

	mSprite.setPosition(sf::Vector2f(16 * 4, 16 * 8));
	mSprite.setOrigin(16.f, 16.f);

	mMap.load("assets/tmx/map.tmx", mTextures.get(Textures::Sheet));
}

void Application::update(sf::Time deltaTime)
{
	mSprite.play(*mCurrentAnimation);
	mSprite.update(deltaTime);
	// mSprite.move(sf::Vector2f(1.f, 0.f) * deltaTime.asSeconds() * 50.f);
}

void Application::draw(sf::RenderTarget& target)
{
	mMap.drawLayer(target, 0);
	mMap.drawLayer(target, 1);
	mMap.drawLayer(target, 2);
	mMap.drawLayer(target, 3);
	target.draw(mSprite);
}

void Application::centerRenderWindow()
{
	sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
	sf::Vector2u desktopSize(desktopMode.width, desktopMode.height);
	sf::Vector2u windowSize(mRenderWindow.getSize());

	sf::Vector2i centeredPosition;
	centeredPosition.x = (desktopSize.x - windowSize.x) / 2;
	centeredPosition.y = (desktopSize.y - windowSize.y) / 2;

	mRenderWindow.setPosition(centeredPosition);
}

void Application::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;

	while(mRenderWindow.isOpen())
	{
		sf::Time elapsedTime = clock.restart();
		timeSinceLastUpdate += elapsedTime;

		while(timeSinceLastUpdate > TimePerFrame)
		{
			timeSinceLastUpdate -= TimePerFrame;

			processInput();

			if(mPaused) continue;

			update(TimePerFrame);
		}

		updateStatistics(elapsedTime);

		mRenderTexture.clear(ClearColor);
		draw(mRenderTexture);
		mRenderTexture.display();

		mRenderWindow.clear();

		sf::Sprite renderTextureSprite(mRenderTexture.getTexture());
		renderTextureSprite.setScale(sf::Vector2f(Scale));

		mRenderWindow.draw(renderTextureSprite);

		drawStatistics(mRenderWindow);
		mRenderWindow.display();
	}
}

void Application::processInput()
{
	sf::Event event;

	while(mRenderWindow.pollEvent(event))
	{
		switch(event.type)
		{
		case sf::Event::Closed:
			mRenderWindow.close();
			break;
		case sf::Event::LostFocus:
			mPaused = true;
			break;
		case sf::Event::GainedFocus:
			mPaused = false;
			break;
		default: break;
		}
	}
}

void Application::updateStatistics(sf::Time elapsedTime)
{
	mStatisticsUpdateTime += elapsedTime;
	mStatisticsNumFrames++;

	if(mStatisticsUpdateTime >= sf::seconds(1.f))
	{
		std::string statisticsString = "Frames/Second: " + std::to_string(mStatisticsNumFrames);
		statisticsString += "\nTime/Update: " + std::to_string(mStatisticsUpdateTime.asMicroseconds() / mStatisticsNumFrames) + " us";

		if(mVSyncEnabled) statisticsString += "\nVSync enabled";
		else statisticsString += "\nVSync disabled";

		mStatisticsText.setString(statisticsString);

		mStatisticsUpdateTime -= sf::seconds(1.f);
		mStatisticsNumFrames = 0;
	}
}

void Application::drawStatistics(sf::RenderTarget& target)
{
	if(!Debug) return;

	target.draw(mStatisticsText);

	if(mPaused) target.draw(mPausedText);
}

sf::Vector2f Application::centerText(sf::Text& text)
{
	sf::FloatRect textRect = text.getLocalBounds();
	int windowWidth = mRenderWindow.getSize().x;
	int windowHeight = mRenderWindow.getSize().y;
	text.setOrigin(textRect.left + textRect.width / 2.f, textRect.top  + textRect.height / 2.f);

	return sf::Vector2f(windowWidth / 2.f,windowHeight / 2.f);
}

