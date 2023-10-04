#include "EnemyMissile.h"


EnemyMissile::EnemyMissile() {}

EnemyMissile::~EnemyMissile() {}

void EnemyMissile::init(std::string textureName, sf::Vector2f position, float speedx, float speedy) {

	m_speedx = speedx;
	m_speedy = speedy;
	m_position = position;


	m_texture.loadFromFile(textureName.c_str());


	m_sprite.setTexture(m_texture);
	m_sprite.setPosition(m_position);
	m_sprite.setOrigin(m_texture.getSize().x / 2, m_texture.getSize().y / 2);

}

void EnemyMissile::update(float dt) {


	m_sprite.move(m_speedx * dt, m_speedy * dt);


}

sf::Sprite EnemyMissile::getSprite() {

	return m_sprite;

}

/*void EnemyMissile::enemyShoot(sf::Vector2f playerPos, sf::Vector2f enemyPos) {

	float missileSpeedx;
	float missileSpeedy;

	float mag = sqrt(((playerPos.x) * (playerPos.x)) + ((playerPos.y) * (playerPos.y)));


	if (playerPos.x > enemyPos.x) {
		missileSpeedx = enemyBulletSpeed * -(((enemyPos.x - playerPos.x)) / mag);

	}
	else {
		missileSpeedx = enemyBulletSpeed * -(((enemyPos.x - playerPos.x)) / mag);

	}


	if (playerPos.y > enemyPos.y) {
		missileSpeedy = enemyBulletSpeed * -(((enemyPos.y - playerPos.y)) / mag);

	}
	else {
		missileSpeedy = enemyBulletSpeed * -(((enemyPos.y - playerPos.y)) / mag);

	}


	EnemyMissile* missile = new EnemyMissile();

	missile->init("Assets/graphics/rocket.png", sf::Vector2f(enemyPos.x, enemyPos.y),
		missileSpeedx, missileSpeedy);

	enemyMissiles.push_back(missile);

	fireSound.play();

}*/