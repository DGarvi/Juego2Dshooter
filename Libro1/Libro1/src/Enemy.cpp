#include "Enemy.h"

Enemy::Enemy() {}

Enemy::~Enemy() {};

void Enemy::init(std::string textureName, sf::Vector2f position) {

	m_position = position;
	enemyShootAllowed = true;
	enemyTimer = 0.0f;
	

	m_texture.loadFromFile(textureName.c_str());

	m_sprite.setTexture(m_texture);
	m_sprite.setPosition(m_position);
	m_sprite.setOrigin(m_texture.getSize().x / 2, m_texture.getSize().y / 2);

}

void Enemy::update(float heroPosx, float heroPosy, float enemySpeed, float dt) {

	
	if (heroPosx > m_sprite.getPosition().x) {
		m_sprite.move(enemySpeed * dt, 0);
	}
	else {
		m_sprite.move(-enemySpeed * dt, 0);
	}
	if (heroPosy > m_sprite.getPosition().y) {
		m_sprite.move(0, enemySpeed / 1.34f * dt);
	}
	else {
		m_sprite.move(0, -enemySpeed / 1.34f * dt);
	}

}

sf::Sprite Enemy::getSprite() {

	return m_sprite;
}

