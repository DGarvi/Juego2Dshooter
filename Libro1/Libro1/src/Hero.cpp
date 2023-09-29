#include "Hero.h"

Hero::Hero() {}

Hero::~Hero() {}

void Hero::init(std::string textureName, int frameCount, float animDuration, sf::Vector2f position) {

	m_position = position;

	m_frameCount = frameCount;
	m_animDuration = animDuration;

	//Load texture
	m_texture.loadFromFile(textureName.c_str());

	m_spriteSize = sf::Vector2i(92, 126);

	//Create sprite and attack texture
	m_sprite.setTexture(m_texture);
	m_sprite.setTextureRect(sf::IntRect(0, 0, m_spriteSize.x, m_spriteSize.y));
	m_sprite.setPosition(m_position);
	m_sprite.setOrigin(m_spriteSize.x / 2, m_spriteSize.y / 2);

}

void Hero::update(float dt) {

	// Animate Sprite
	m_elapsedTime += dt;

	int animFrame = static_cast<int> ((m_elapsedTime / m_animDuration) * m_frameCount) % m_frameCount;

	m_sprite.setTextureRect(sf::IntRect(animFrame * m_spriteSize.x, 0, m_spriteSize.x, m_spriteSize.y));
	
	// Update Position
	
	m_sprite.setPosition(m_position);

	if (m_position.y >= 768) {

		m_position.y = 768;
	

	}
	if (m_position.x >= 1024) {

		m_position.x = 1024;

	}
	if (m_position.x <= 0) {

		m_position.x = 0;

	}
	if (m_position.y <= 0) {

		m_position.y = 0;

	}

}

void Hero::move(float velocityx, float velocityy) {


		m_position.x = m_position.x + velocityx ;
		m_position.y = m_position.y + velocityy ;
	
	
}

sf::Sprite Hero::getSprite() {

	return m_sprite;
}