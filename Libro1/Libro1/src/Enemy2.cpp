#include "Enemy2.h"

Enemy2::Enemy2() {}

Enemy2::~Enemy2() {};

void Enemy2::init(std::string textureName, sf::Vector2f position) {

	m_position = position;
	enemy2Speed = 300.0f;

	m_texture.loadFromFile(textureName.c_str());
	m_sprite.setTexture(m_texture);
	m_sprite.setPosition(m_position);
	m_sprite.setOrigin(m_texture.getSize().x / 2, m_texture.getSize().y / 2);

}

void Enemy2::update(float heroPosx, float heroPosy, float enemySpeed, float dt) {
	
	if (heroPosx > m_sprite.getPosition().x) {
		m_sprite.move(enemySpeed * dt, 0);
		m_sprite.setScale(-1.0f, 1.0);
	}
	else {
		m_sprite.move(-enemySpeed * dt, 0);
		m_sprite.setScale(1.0f, 1.0);
	}
	if (heroPosy > m_sprite.getPosition().y) {
		m_sprite.move(0, enemySpeed * dt);
	}
	else {
		m_sprite.move(0, -enemySpeed * dt);
	}
}

sf::Sprite Enemy2::getSprite() {

	return m_sprite;

}