#pragma once
#include "SFML-2.6.0\include\SFML\Graphics.hpp"

class Missile {

public:

	Missile();
	~Missile();

	void init(std::string textureName, sf::Vector2f position, float speedx, float speedy);
	void update(float dt);
	sf::Sprite getSprite();

private:

	sf::Texture		m_texture;
	sf::Sprite		m_sprite;
	sf::Vector2f	m_position;
	float			m_speedx;
	float			m_speedy;
};

