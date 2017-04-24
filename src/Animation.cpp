#include "Animation.hpp"

Animation::Animation() :
	mSpriteSheet(NULL)
{
}

void Animation::addFrame(sf::IntRect rect)
{
	mFrames.push_back(rect);
}

void Animation::setSpriteSheet(const sf::Texture& spriteSheet)
{
	mSpriteSheet = &spriteSheet;
}

const sf::Texture* Animation::getSpriteSheet() const
{
	return mSpriteSheet;
}

std::size_t Animation::getSize() const
{
	return mFrames.size();
}

const sf::IntRect& Animation::getFrame(std::size_t n) const
{
	return mFrames[n];
}
