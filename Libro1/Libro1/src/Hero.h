#pragma once
#include "SFML-2.6.0\include\SFML\Graphics.hpp"

class Hero
{
public:
	Hero();
	~Hero();

	void init(std::string textureName, int frameCount, float animDuration, sf::Vector2f position);
	void update(float dt);
	void move(float velocityx, float velocityy);
	sf::Sprite getSprite();

private:
	sf::Texture m_texture;
	sf::Sprite m_sprite;
	sf::Vector2f m_position;

	float m_velocityx;
	float m_velocityy;

	int m_frameCount;
	float m_animDuration;
	float m_elapsedTime;
	sf::Vector2i m_spriteSize;

};

