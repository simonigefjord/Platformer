#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <vector>

class Animation
{
public:
	Animation();

	void addFrame(sf::IntRect rect);
	void setSpriteSheet(const sf::Texture& spriteSheet);

	const sf::Texture* getSpriteSheet() const;
	std::size_t getSize() const;
	const sf::IntRect& getFrame(std::size_t n) const;

private:
	std::vector<sf::IntRect> mFrames;
	const sf::Texture* mSpriteSheet;
};

#endif // ANIMATION_HPP
