#include "SFML-2.6.0\include\SFML\Graphics.hpp"
#include "SFML-2.6.0\include\SFML\Audio.hpp"
#include <vector>
#include <iostream>

#include "Enemy.h"
#include "Hero.h"
#include "Missile.h"


//sf::Music bgMusic;
sf::SoundBuffer fireBuffer;
sf::SoundBuffer hitBuffer;

sf::Sound fireSound(fireBuffer);
sf::Sound hitSound(hitBuffer);

sf::Vector2f viewSize(1024, 768);
sf::VideoMode vm(viewSize.x, viewSize.y);
sf::RenderWindow window(vm, "SFML Game", sf::Style::Default);


sf::Texture skyTexture;
sf::Sprite skySprite;

sf::Texture bgTexture;
sf::Sprite bgSprite;

Hero hero;

std::vector<Enemy*> enemies;
std::vector<Missile*> missiles;

float currentTime;
float prevTime = 0.0f;
float currentTimeBullet;
float prevTimeBullet = 0.0f;

float cadence = 0.20f;
int ammo = 10;
float bulletSpeed = 2000.0f;

float heroSpeed = 400.0f;
bool playerMovingUp = false;
bool playerMovingDown = false;
bool playerMovingLeft = false;
bool playerMovingRight = false;

int enemySpeed = 100;
int maxSpeed = 450;
float spawnRate = 1.50f;
float minSpawnRate = 0.80f;

int score = 0;
bool gameover = true;

//Text
sf::Font headingFont;
sf::Text headingText;

sf::Font scoreFont;
sf::Text scoreText;

sf::Text ammoText;

sf::Text tutorialText;

sf::Vector2f playerPosition;
bool playerMoving = false;

void shoot(sf::Vector2f playerPos, sf::Vector2i mousePos);
bool checkCollisionBullet(sf::Sprite sprite1, sf::Sprite sprite2);
bool checkCollisionHero(sf::Sprite sprite1, sf::Sprite sprite2);

void reset();

sf::Vector2i mousePos;

void init() {

	skyTexture.loadFromFile("Assets/graphics/sky.png");
	skySprite.setTexture(skyTexture);
	
	bgTexture.loadFromFile("Assets/graphics/bg.png");
	bgSprite.setTexture(bgTexture);

	headingFont.loadFromFile("Assets/fonts/SnackerComic.ttf");

	headingText.setFont(headingFont);
	headingText.setString("Chica Bazooka");
	headingText.setCharacterSize(84);
	headingText.setFillColor(sf::Color::White);

	sf::FloatRect headingBounds = headingText.getLocalBounds();
	headingText.setOrigin(headingBounds.width / 2, headingBounds.height / 2);
	headingText.setPosition(sf::Vector2f(viewSize.x * 0.5f, viewSize.y * 0.10f));

	scoreFont.loadFromFile("Assets/fonts/Arial.ttf");

	scoreText.setFont(scoreFont);
	scoreText.setString("Puntuación: 0");
	scoreText.setCharacterSize(45);
	scoreText.setFillColor(sf::Color::White);

	sf::FloatRect scoreBounds = scoreText.getLocalBounds();
	scoreText.setOrigin(scoreBounds.width / 2, scoreBounds.height / 2);
	scoreText.setPosition(sf::Vector2f(viewSize.x * 0.5f, viewSize.y * 0.20f));

	ammoText.setFont(scoreFont);
	ammoText.setString("Puntuación: 0");
	ammoText.setCharacterSize(45);
	ammoText.setFillColor(sf::Color::White);

	sf::FloatRect ammoBounds = ammoText.getLocalBounds();
	ammoText.setOrigin(ammoBounds.width / 2, ammoBounds.height / 2);
	ammoText.setPosition(sf::Vector2f(200.0f, viewSize.y * 0.85f));

	tutorialText.setFont(scoreFont);
	tutorialText.setString("Pulsa Click Izquierdo para DISPARAR y apunta con el Ratón\n\n Pulsa W A S D para MOVERTE\n\n\n Haz click para empezar");
	tutorialText.setCharacterSize(32);
	tutorialText.setFillColor(sf::Color::White);

	sf::FloatRect tutorialBounds = tutorialText.getLocalBounds();
	tutorialText.setOrigin(tutorialBounds.width / 2, tutorialBounds.height / 2);
	tutorialText.setPosition(sf::Vector2f(viewSize.x * 0.5f, viewSize.y * 0.45f));

	//Audio
	//bgMusic.openFromFile("Assets/audio/bgMusic.ogg");
	//bgMusic.play();

	hitBuffer.loadFromFile("Assets/audio/hit.ogg");
	fireBuffer.loadFromFile("Assets/audio/fire.ogg");

	hero.init("Assets/graphics/heroAnim.png", 4, 1.0f, sf::Vector2f(viewSize.x * 0.25f, viewSize.y * 0.5f));

	srand((int)time(0));
}

void spawnEnemy() {

	int randLoc = rand() % 4;

	sf::Vector2f enemyPos;

	if (enemies.size() < 10){

	switch (randLoc) {

	case 0: enemyPos = sf::Vector2f(viewSize.x * 0.75f, viewSize.y);
		
		break;
	
	case 1: enemyPos = sf::Vector2f(viewSize.x, viewSize.y * 0.60f);
		
		break;

	case 2: enemyPos = sf::Vector2f(viewSize.x, viewSize.y * 0.40f);
		
		break;
	
	case 3: enemyPos = sf::Vector2f(viewSize.x * 0.40f, viewSize.y);

		break;

	default: printf("Incorrect y Value \n");
		return;

	}


	Enemy* enemy = new Enemy();
	enemy->init("Assets/graphics/enemy.png", enemyPos);

	enemies.push_back(enemy);
	
	}

}


void draw() {

	window.draw(skySprite);
	window.draw(bgSprite);
	
	window.draw(hero.getSprite());

	for (Enemy* enemy : enemies) {
		window.draw(enemy->getSprite());
	}

	for (Missile* missile : missiles) {
		window.draw(missile->getSprite());
	}

	if (gameover) {
		window.draw(headingText);
		window.draw(tutorialText);
	}
	else {
		window.draw(scoreText);
		window.draw(ammoText);
	}

}

void updateInput(float dtb) {

	sf::Event event;
	
	currentTimeBullet += dtb;

	//while pending events
	while (window.pollEvent(event)) {
	
		
		
		
		if (event.type == sf::Event::KeyPressed) {
			if (event.key.code == sf::Keyboard::W) {
				playerMovingUp = true;
			}
			if (event.key.code == sf::Keyboard::A) {
				playerMovingLeft = true;
			}
			if (event.key.code == sf::Keyboard::D) {
				playerMovingRight = true;
			}
			if (event.key.code == sf::Keyboard::S) {
				playerMovingDown = true;
			}
		}
		
		if (event.type == sf::Event::KeyReleased) {
			if (event.key.code == sf::Keyboard::W) {
				playerMovingUp = false;
			}
			if (event.key.code == sf::Keyboard::A) {
				playerMovingLeft = false;
			}
			if (event.key.code == sf::Keyboard::D) {
				playerMovingRight = false;
			}
			if (event.key.code == sf::Keyboard::S) {
				playerMovingDown = false;
			}

		}

		

			if (event.type == sf::Event::MouseButtonPressed) {
				if (event.key.code == sf::Mouse::Left) {
					
					if (gameover) {

						gameover = false;
						reset();

					}
					else {
						if (currentTimeBullet >= prevTimeBullet + cadence) {
							if(ammo > 0){
								
								mousePos = sf::Mouse::getPosition(window);

								shoot(hero.getSprite().getPosition(), mousePos);
								ammo--;
								
								std::string finalAmmo = "Munición: " + std::to_string(ammo);
								ammoText.setString(finalAmmo);
								sf::FloatRect ammoBounds = ammoText.getLocalBounds();
								ammoText.setOrigin(ammoBounds.width / 2, ammoBounds.height / 2);
								ammoText.setPosition(sf::Vector2f(200.0f, viewSize.y * 0.85f));
								
								prevTimeBullet = currentTimeBullet;
							}
						}

					}
				}

			}

			if (event.type == sf::Event::Closed)
				window.close();

			if (event.key.code == sf::Keyboard::Escape)
				gameover = true;

	}

}

void update(float dt) {
	hero.update(dt);
	currentTime += dt;


	if (playerMovingDown) {
		hero.move(0, heroSpeed * dt);
	}
	if (playerMovingUp) {
		hero.move(0, -heroSpeed * dt);
	}
	if (playerMovingLeft) {
		hero.move(-heroSpeed * dt, 0);
	}
	if (playerMovingRight) {
		hero.move(heroSpeed * dt, 0);
	}


	if (currentTime >= prevTime + spawnRate) {
		
		
		spawnEnemy();
		std::cout << enemySpeed << std::endl;
		prevTime = currentTime;
		enemySpeed = enemySpeed + 20;
		spawnRate = spawnRate - 0.10f;

		if (enemySpeed > maxSpeed) {
			enemySpeed = maxSpeed;
		}

		if (spawnRate < minSpawnRate) {
			spawnRate = minSpawnRate;
		}

	}

	for (int i = 0; i < enemies.size(); i++) {

		Enemy* enemy = enemies[i];

		enemy->update(hero.getSprite().getPosition().x, hero.getSprite().getPosition().y, enemySpeed, dt);


	}

	for (int i = 0; i < missiles.size(); i++) {

		Missile* missile = missiles[i];

		missile->update(dt);

		if (missile->getSprite().getPosition().x > viewSize.x) {

			missiles.erase(missiles.begin() + i);
			delete(missile);

		}
	}

	for (int i = 0; i < enemies.size(); i++) {

		Enemy* enemy = enemies[i];


		if (checkCollisionHero(hero.getSprite(), enemy->getSprite())) {

			gameover = true;

		}

	}


	 for (int j = 0; j < enemies.size(); j++) {
		 for (int i = 0; i < missiles.size(); i++) {
			
			Missile* missile = missiles[i];
			Enemy* enemy = enemies[j];

			if (checkCollisionBullet(missile->getSprite(), enemy->getSprite())) {

				hitSound.play();
				score++;
				ammo++;

				std::string finalScore = "Puntuación: " + std::to_string(score);
				scoreText.setString(finalScore);
				sf::FloatRect scoreBounds = scoreText.getLocalBounds();
				scoreText.setOrigin(scoreBounds.width / 2, scoreBounds.height / 2);
				scoreText.setPosition(sf::Vector2f(viewSize.x * 0.5f, viewSize.y * 0.10f));

				std::string finalAmmo = "Munición: " + std::to_string(ammo);
				ammoText.setString(finalAmmo);
				sf::FloatRect ammoBounds = ammoText.getLocalBounds();
				ammoText.setOrigin(ammoBounds.width / 2, ammoBounds.height / 2);
				ammoText.setPosition(sf::Vector2f(200.0f, viewSize.y * 0.85f));

				missiles.erase(missiles.begin() + i);
				enemies.erase(enemies.begin() + j);


				delete(missile);
				delete(enemy);


			}
		}
	}


}

void shoot(sf::Vector2f playerPos, sf::Vector2i mousePos) {
	
	float missileSpeedx;
	float missileSpeedy;
		
	float mag = sqrt (((mousePos.x - playerPos.x) * (mousePos.x - playerPos.x)) + ((mousePos.y - playerPos.y) * (mousePos.y - playerPos.y)));

	if (playerPos.x > mousePos.x) {
		missileSpeedx = bulletSpeed *  (((mousePos.x - playerPos.x)) / mag);

	}
	else {
		missileSpeedx = bulletSpeed *   (((mousePos.x - playerPos.x)) / mag);

	}
	
	
	if (playerPos.y > mousePos.y) {
		missileSpeedy = bulletSpeed *   (((mousePos.y - playerPos.y)) / mag);

	}
	else {
		missileSpeedy = bulletSpeed *    (((mousePos.y - playerPos.y)) / mag);

	}

	
	Missile* missile = new Missile();

	missile->init("Assets/graphics/rocket.png", sf::Vector2f(hero.getSprite().getPosition().x, hero.getSprite().getPosition().y),
		 missileSpeedx, missileSpeedy);
	
	missiles.push_back(missile);

	fireSound.play();

}

bool checkCollisionHero(sf::Sprite sprite1, sf::Sprite sprite2) {

	sf::FloatRect shape1 = sprite1.getGlobalBounds();
	sf::FloatRect shape2 = sprite2.getGlobalBounds();

	shape1.height = shape1.height - 24;
	shape1.width  = shape1.width  - 30;	
	shape1.top    = shape1.top    + 12;
	shape1.left   = shape1.left   + 15;
	
	shape2.height = shape2.height - 24;
	shape2.width  = shape2.width  - 30;
	shape2.top    = shape2.top    + 12;
	shape2.left   = shape2.left   + 15;
	
	
	if (shape1.intersects(shape2)) {

		return true;
	}
	else {

		return false;
	}

}

bool checkCollisionBullet(sf::Sprite sprite1, sf::Sprite sprite2) {

	sf::FloatRect shape1 = sprite1.getGlobalBounds();
	sf::FloatRect shape2 = sprite2.getGlobalBounds();

	if (shape1.intersects(shape2)) {

		return true;
	}
	else {

		return false;
	}

}

int main() {
	sf::Clock clock;
	sf::Clock bulletClock;
	//Inicializar objetos
	init();


	//Abrir window
	while (window.isOpen()) {


		//Keyboard events
		sf::Time dtb = bulletClock.restart();
		updateInput(dtb.asSeconds());

		//Update game
		sf::Time dt = clock.restart();
		
		if (!gameover) {
			update(dt.asSeconds());
		}
		//Update game objects
		window.clear(sf::Color::Red);
		
		draw();
		
		//Render game objects
		window.display();

	}

	return 0;
}


void reset() {

	score = 0;
	currentTime = 0.0f;
	prevTime = 0.0f;
	currentTimeBullet = 0.0f;
	prevTimeBullet = 0.0f;
	ammo = 10;
	enemySpeed = 50;
	scoreText.setString("Puntuación: 0");
	ammoText.setString("Munición: 3");

	hero.~Hero();
	hero.init("Assets/graphics/heroAnim.png", 4, 1.0f, sf::Vector2f(viewSize.x * 0.25f, viewSize.y * 0.5f));

	for (Enemy* enemy : enemies) {
		delete (enemy);
	}

	for (Missile* missile : missiles) {
		delete (missile);
	}

	enemies.clear();
	missiles.clear();

}