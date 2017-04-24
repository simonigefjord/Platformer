#include "AnimatedSprite.hpp"

#include <SFML/Graphics.hpp>

AnimatedSprite::AnimatedSprite(sf::Time frameTime, bool paused, bool looped) :
	mAnimation(NULL),
	mFrameTime(frameTime),
	mCurrentFrame(0),
	mPaused(paused),
	mLooped(looped),
	mTexture(NULL)
{

}

void AnimatedSprite::update(sf::Time deltaTime)
{
	if(!mPaused && mAnimation)
	{
		mCurrentTime += deltaTime;

		if (mCurrentTime >= mFrameTime)
		{
			mCurrentTime = sf::microseconds(mCurrentTime.asMicroseconds() % mFrameTime.asMicroseconds());

			if (mCurrentFrame + 1 < mAnimation->getSize())
				mCurrentFrame++;
			else
			{
				mCurrentFrame = 0;

				if (!mLooped)
				{
					mPaused = true;
				}

			}

			setFrame(mCurrentFrame, false);
		}
	}
}

void AnimatedSprite::setAnimation(const Animation& animation)
{
	mAnimation = &animation;
	mTexture = mAnimation->getSpriteSheet();
	mCurrentFrame = 0;
	setFrame(mCurrentFrame);
}

void AnimatedSprite::setFrameTime(sf::Time frameTime)
{
	mFrameTime = frameTime;
}

void AnimatedSprite::play()
{
	mPaused = false;
}

void AnimatedSprite::play(const Animation& animation)
{
	if(getAnimation() != &animation) setAnimation(animation);
	play();
}

void AnimatedSprite::pause()
{
	mPaused = true;
}

void AnimatedSprite::stop()
{
	mPaused = true;
	mCurrentFrame = 0;
	setFrame(mCurrentFrame);
}

void AnimatedSprite::setLooped(bool looped)
{
	mLooped = looped;
}

void AnimatedSprite::setColor(const sf::Color& color)
{
	mVerticies[0].color = color;
	mVerticies[1].color = color;
	mVerticies[2].color = color;
	mVerticies[3].color = color;
}

void AnimatedSprite::setFrame(std::size_t frame, bool reset)
{
	if(mAnimation)
	{
		sf::IntRect rect = mAnimation->getFrame(frame);

		mVerticies[0].position = sf::Vector2f(0.f, 0.f);
		mVerticies[1].position = sf::Vector2f(0.f, static_cast<float>(rect.height));
		mVerticies[2].position = sf::Vector2f(static_cast<float>(rect.width), static_cast<float>(rect.height));
		mVerticies[3].position = sf::Vector2f(static_cast<float>(rect.width), 0.f);

		float left = static_cast<float>(rect.left) + 0.0001f;
		float right = left + static_cast<float>(rect.width);
		float top = static_cast<float>(rect.top);
		float bottom = top + static_cast<float>(rect.height);

		mVerticies[0].texCoords = sf::Vector2f(left, top);
		mVerticies[1].texCoords = sf::Vector2f(left, bottom);
		mVerticies[2].texCoords = sf::Vector2f(right, bottom);
		mVerticies[3].texCoords = sf::Vector2f(right, top);
	}

	if (reset) mCurrentTime = sf::Time::Zero;
}

const Animation* AnimatedSprite::getAnimation() const
{
	return mAnimation;
}

sf::FloatRect AnimatedSprite::getLocalBounds() const
{
	sf::IntRect rect = mAnimation->getFrame(mCurrentFrame);

	float width = static_cast<float>(std::abs(rect.width));
	float height = static_cast<float>(std::abs(rect.height));

	return sf::FloatRect(0.f, 0.f, width, height);
}

sf::FloatRect AnimatedSprite::getGlobalBounds() const
{
	return getTransform().transformRect(getLocalBounds());
}

bool AnimatedSprite::isLooped() const
{
	return mLooped;
}

bool AnimatedSprite::isPlaying() const
{
	return !mPaused;
}

sf::Time AnimatedSprite::getFrameTime() const
{
	return mFrameTime;
}

void AnimatedSprite::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if(mAnimation && mTexture)
	{
		states.transform *= getTransform();
		states.texture = mTexture;
		target.draw(mVerticies, 4, sf::Quads, states);
	}
}
