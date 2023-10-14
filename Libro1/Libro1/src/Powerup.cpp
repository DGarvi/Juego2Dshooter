#include "Powerup.h"

Powerup::Powerup() {}

Powerup::~Powerup() {};

void Powerup::init(std::string textureName, sf::Vector2f position) {

	m_position = position;
	m_texture.loadFromFile(textureName.c_str());
	m_sprite.setTexture(m_texture);
	m_sprite.setPosition(m_position);
	m_sprite.setOrigin(m_texture.getSize().x / 2, m_texture.getSize().y / 2);

}


sf::Sprite Powerup::getSprite() {

	return m_sprite;

}
