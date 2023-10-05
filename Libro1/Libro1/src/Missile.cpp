#include "Missile.h"

Missile::~Missile() {}
Missile::Missile() {}

void Missile::init(std::string textureName, sf::Vector2f position, float speedx, float speedy) {

	m_speedx		= speedx;
	m_speedy		= speedy;
	m_position		= position;
	m_texture.loadFromFile		(textureName.c_str());
	m_sprite.setTexture			(m_texture);
	m_sprite.setPosition		(m_position);
	m_sprite.setOrigin			(m_texture.getSize().x / 2, m_texture.getSize().y / 2);

}

void Missile::update(float dt) {
	
	m_sprite.move(m_speedx * dt, m_speedy * dt);

}

sf::Sprite Missile::getSprite() {

	return m_sprite;

}
