#pragma once
#include "SFML-2.6.0\include\SFML\Graphics.hpp"
#include "EnemyMissile.h"

class Enemy
{
public:
	Enemy();
	~Enemy();
	bool enemyShootAllowed;
	float enemyTimer;

	void init(std::string textureName, sf::Vector2f position);
	void update(float heroPosx, float heroPosy, float enemySpeed, float dt);
	sf::Sprite getSprite();
	void enemyShoot(sf::Vector2f playerPos, sf::Vector2f enemyPos, float enemyBulletSpeed, std::vector<EnemyMissile*> enemyMissiles);

private:
	sf::Texture m_texture;
	sf::Sprite m_sprite;
	sf::Vector2f m_position;

};

