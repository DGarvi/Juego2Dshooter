#include "SFML-2.6.0\include\SFML\Graphics.hpp"
#include "SFML-2.6.0\include\SFML\Audio.hpp"
#include <vector>
#include <iostream>

#include "Enemy.h"
#include "Hero.h"
#include "Missile.h"
#include "EnemyMissile.h"

//SOUND BUFFERS
//sf::Music bgMusic;
sf::SoundBuffer fireBuffer;
sf::SoundBuffer hitBuffer;

sf::Sound fireSound	(fireBuffer);
sf::Sound hitSound	(hitBuffer);

//WINDOW RENDER PARAMETERS
sf::Vector2f     viewSize	(1024, 768);
sf::VideoMode    vm			(viewSize.x, viewSize.y);
sf::RenderWindow window		(vm, "SFML Game", sf::Style::Default);

//SPRITE AND TEXTURE
sf::Texture skyTexture;
sf::Sprite  skySprite;

sf::Texture bgTexture;
sf::Sprite  bgSprite;

Hero hero;

std::vector<Enemy*>        enemies;
std::vector<Missile*>      missiles;
std::vector<EnemyMissile*> enemyMissiles;

float currentTime;
float prevTime              = 0.0f;
float currentTimeBullet;
float prevTimeBullet		= 0.0f;

float cadence				= 0.20f;
int   ammo					= 10;
float bulletSpeed			= 2000.0f;

float heroSpeed				= 400.0f;
bool  playerMovingUp		= false;
bool  playerMovingDown		= false;
bool  playerMovingLeft		= false;
bool  playerMovingRight		= false;

float maxSpeed				= 300.0f;
float spawnRate				= 3.0f;
float minSpawnRate			= 1.0f;
float enemyBulletSpeed		= 500.0f;
bool  enemyShootAllowed		= false;

int   score					= 0;
bool  gameover				= true;

//Text
sf::Font headingFont;
sf::Text headingText;

sf::Font scoreFont;
sf::Text scoreText;

sf::Text ammoText;

sf::Text tutorialText;

sf::Vector2f playerPosition;
bool playerMoving = false;

sf::Vector2i mousePos;

void shoot					(sf::Vector2f playerPos	, sf::Vector2i mousePos);
bool checkCollisionBullet	(sf::Sprite sprite1		, sf::Sprite sprite2);
bool checkCollisionHero		(sf::Sprite sprite1		, sf::Sprite sprite2);
void enemyShoot				(sf::Vector2f playerPos	, sf::Vector2f enemyPos);
void reset();


void init() {

	//TEXTURE LOAD
	skyTexture.loadFromFile		("Assets/graphics/sky.png");
	skySprite.setTexture		(skyTexture);
	
	bgTexture.loadFromFile		("Assets/graphics/bg.png");
	bgSprite.setTexture			(bgTexture);

	scoreFont.loadFromFile		("Assets/fonts/Arial.ttf");

	//SCORE TEXT LOAD
	scoreText.setFont			(scoreFont);
	scoreText.setString			("Puntuación: 0");
	scoreText.setCharacterSize	(45);
	scoreText.setFillColor		(sf::Color::White);

	sf::FloatRect scoreBounds = scoreText.getLocalBounds();

	scoreText.setOrigin			(scoreBounds.width / 2, scoreBounds.height / 2);
	scoreText.setPosition		(sf::Vector2f(viewSize.x * 0.5f, viewSize.y * 0.20f));

	//AMMO TEXT LOAD
	ammoText.setFont			(scoreFont);
	ammoText.setString			("Puntuación: 0");
	ammoText.setCharacterSize	(45);
	ammoText.setFillColor		(sf::Color::White);

	sf::FloatRect ammoBounds =	ammoText.getLocalBounds();
	ammoText.setOrigin			(ammoBounds.width / 2, ammoBounds.height / 2);
	ammoText.setPosition		(sf::Vector2f(200.0f, viewSize.y * 0.85f));

	//TUTORIAL TEXT LOAD
	tutorialText.setFont		(scoreFont);
	tutorialText.setString		("Pulsa Click Izquierdo para DISPARAR y apunta con el Ratón\n\n Pulsa W A S D para MOVERTE\n\n\n Haz click para empezar");
	tutorialText.setCharacterSize(32);
	tutorialText.setFillColor	(sf::Color::Magenta);

	sf::FloatRect tutorialBounds = tutorialText.getLocalBounds();
	tutorialText.setOrigin		(tutorialBounds.width / 2, tutorialBounds.height / 2);
	tutorialText.setPosition	(sf::Vector2f(viewSize.x * 0.5f, viewSize.y * 0.45f));

	//BACKGROUND MUSIC
	//bgMusic.openFromFile("Assets/audio/bgMusic.ogg");
	//bgMusic.play();

	//HIT SOUND LOAD
	hitBuffer.loadFromFile("Assets/audio/hit.ogg");
	fireBuffer.loadFromFile("Assets/audio/fire.ogg");

	//HERO LOAD
	hero.init("Assets/graphics/heroAnim.png", 4, 1.0f, sf::Vector2f(viewSize.x * 0.5f, viewSize.y * 0.5f));

	//TIMER LOAD
	srand((int)time(0));
}

void spawnEnemy() {

	int randLoc =	rand() % 8;
	sf::Vector2f	enemyPos;

	//DIFFERENT ENEMY SPAWN POINTS
	if (enemies.size() < 10){

		switch (randLoc) {

			case 0: enemyPos = sf::Vector2f(viewSize.x * 0.50f, viewSize.y);		
			break;
	
			case 1: enemyPos = sf::Vector2f(viewSize.x * 0.50f, 0);		
			break;

			case 2: enemyPos = sf::Vector2f(viewSize.x, viewSize.y * 0.50f);		
			break;
	
			case 3: enemyPos = sf::Vector2f(0, viewSize.y * 0.50f);
			break;

			case 4: enemyPos = sf::Vector2f(viewSize.x, viewSize.y);
			break;

			case 5: enemyPos = sf::Vector2f(0, viewSize.y);
			break;
			
			case 6: enemyPos = sf::Vector2f(viewSize.x, 0);
			break;
			
			case 7: enemyPos = sf::Vector2f(0, 0);
			break;

			default: printf("Incorrect y Value \n");
			return;

		}

	Enemy* enemy = new Enemy();
	enemy->init("Assets/graphics/enemy.png", enemyPos);

		//INCREASE SPEED IN FUTURE ENEMIES PER SCORE
		if (100 + score * 5.0f < maxSpeed){
			
			enemy->enemySpeed += score * 5.0f;
			std::cout << enemy->enemySpeed << std::endl;
		
		}
		else {
			
			enemy->enemySpeed = maxSpeed;
			std::cout << enemy->enemySpeed << std::endl;
		
		}

	enemies.push_back(enemy);
	
	}

}


void draw() {

	//DRAW 
	window.draw	(skySprite);
	window.draw	(bgSprite);
	window.draw	(hero.getSprite());

	for (Enemy* enemy : enemies) {
		window.draw(enemy->getSprite());
	}

	for (Missile* missile : missiles) {
		window.draw(missile->getSprite());
	}

	for (EnemyMissile* missile : enemyMissiles) {
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

	//WHILE PENDING EVENTS
	while (window.pollEvent(event)) {
	
		//MOVE HERO	
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
		//STOP HERO
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

		//START GAME/SHOOT WHEN CLICK
		if (event.type == sf::Event::MouseButtonPressed) {
			
			if (event.key.code == sf::Mouse::Left) {
					
				if (gameover) {

					gameover = false;
					reset();

				}
				else {
				
					if (currentTimeBullet >= prevTimeBullet + cadence) {
						
						if(ammo > 0){
								
							mousePos =	sf::Mouse::getPosition(window);
							shoot		(hero.getSprite().getPosition(), mousePos);
							ammo--;
								
							std::string finalAmmo =			"Munición: " + std::to_string(ammo);
							ammoText.setString				(finalAmmo);
							sf::FloatRect ammoBounds =		ammoText.getLocalBounds();
							ammoText.setOrigin				(ammoBounds.width / 2	, ammoBounds.height / 2);
							ammoText.setPosition			(sf::Vector2f(200.0f	, viewSize.y * 0.85f));
								
							prevTimeBullet = currentTimeBullet;
							
						}
					}
				}
			}
		}
		//CLOSE WINDOW ON TOP RIGHT X
		if (event.type == sf::Event::Closed){
		
			window.close();
		
		}
		//EXIT CURRENT GAME
		if (event.key.code == sf::Keyboard::Escape){
			
			gameover = true;
		
		}

	}

}

void update(float dt) {
	
	hero.update(dt, viewSize, playerMovingRight, playerMovingLeft);
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
		
		prevTime = currentTime;
		spawnRate = spawnRate - 0.10f;
		
		if (spawnRate < minSpawnRate) {
			spawnRate = minSpawnRate;
		}

	}
	
	for (int i = 0; i < enemies.size(); i++) {
		
		Enemy* enemy = enemies[i];

		sf::Vector2f heroPos	=			sf::Vector2f(hero.getSprite().getPosition().x	, hero.getSprite().getPosition().y);
		sf::Vector2f enemyPos	=			sf::Vector2f(enemy->getSprite().getPosition().x	, enemy->getSprite().getPosition().y);	
		enemy->update						(hero.getSprite().getPosition().x, hero.getSprite().getPosition().y, enemy->enemySpeed, dt);
		enemy->enemyTimer		+= dt;
		
		if (enemy->enemyTimer >= enemy->enemyPrevTime + enemy->enemyFireRate) {
				
			enemy->enemyShootAllowed = true;
			enemy->enemyPrevTime	 = enemy->enemyTimer;
			
		}
					
		if (enemy->enemyShootAllowed) {

			enemyShoot(heroPos, enemyPos);			
			enemy->enemyShootAllowed = false;
			
		}				
	}

	for (int i = 0; i < enemyMissiles.size(); i++) {

		EnemyMissile* missile = enemyMissiles[i];

		missile->update(dt);

		if (missile->getSprite().getPosition().x > viewSize.x) {

			enemyMissiles.erase(enemyMissiles.begin() + i);
			delete(missile);

		}
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

	for (int i = 0; i < enemyMissiles.size(); i++) {

		EnemyMissile* missile = enemyMissiles[i];
		
		if (checkCollisionHero(hero.getSprite(), missile->getSprite())) {

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

				std::string finalScore				= "Puntuación: " + std::to_string(score);
				scoreText.setString(finalScore);
				sf::FloatRect scoreBounds			= scoreText.getLocalBounds();
				scoreText.setOrigin					(scoreBounds.width / 2	, scoreBounds.height / 2);
				scoreText.setPosition(sf::Vector2f	(viewSize.x * 0.5f		, viewSize.y * 0.10f));

				std::string finalAmmo				= "Munición: "	 + std::to_string(ammo);
				ammoText.setString(finalAmmo);
				sf::FloatRect ammoBounds			= ammoText.getLocalBounds();
				ammoText.setOrigin					(ammoBounds.width / 2	, ammoBounds.height / 2);
				ammoText.setPosition(sf::Vector2f	(200.0f					, viewSize.y * 0.85f));

				missiles.erase	(missiles.begin() + i);
				enemies.erase	(enemies.begin()  + j);

				delete(missile);
				delete(enemy);

			}
		}
	}
}

void shoot(sf::Vector2f playerPos, sf::Vector2i mousePos) {
	
	float missileSpeedx;
	float missileSpeedy;
	float mag		= sqrt (((mousePos.x - playerPos.x) * (mousePos.x - playerPos.x)) + ((mousePos.y - playerPos.y) * (mousePos.y - playerPos.y)));
	
	missileSpeedx	= bulletSpeed *   (((mousePos.x - playerPos.x)) / mag);
	missileSpeedy	= bulletSpeed *   (((mousePos.y - playerPos.y)) / mag);

	
	Missile* missile = new Missile();

	missile->init("Assets/graphics/rocket.png", sf::Vector2f(hero.getSprite().getPosition().x, hero.getSprite().getPosition().y), missileSpeedx, missileSpeedy);
	
	missiles.push_back(missile);

	fireSound.play();

}

void enemyShoot(sf::Vector2f playerPos, sf::Vector2f enemyPos) {

	float missileSpeedx;
	float missileSpeedy;

	float mag		= sqrt(((playerPos.x - enemyPos.x ) * (playerPos.x - enemyPos.x)) + ((playerPos.y - enemyPos.y) * (playerPos.y - enemyPos.y)));

	missileSpeedx	= enemyBulletSpeed * (((playerPos.x - enemyPos.x)) / mag);
	missileSpeedy	= enemyBulletSpeed * (((playerPos.y - enemyPos.y)) / mag);

	EnemyMissile* missile = new EnemyMissile();

	missile->init("Assets/graphics/rocket.png", sf::Vector2f(enemyPos.x, enemyPos.y), missileSpeedx, missileSpeedy);

	enemyMissiles.push_back(missile);

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
	
	//INITIALIZE OBJECTS
	init();

	//OPEN WINDOW
	while (window.isOpen()) {

		//KEYBOARD EVENTS
		sf::Time dtb = bulletClock.restart();
		updateInput(dtb.asSeconds());

		//UPDATE GAME
		sf::Time dt = clock.restart();
		
		if (!gameover) {
			
			update(dt.asSeconds());
		
		}
		//UPDATE GAME OBJECTS
		window.clear(sf::Color::Black);
		draw();
		
		//RENDER
		window.display();

	}

}


void reset() {

	score				= 0;
	currentTime			= 0.0f;
	prevTime			= 0.0f;
	currentTimeBullet	= 0.0f;
	prevTimeBullet		= 0.0f;
	ammo				= 10;
	scoreText.setString ("Puntuación: 0");
	ammoText.setString	("Munición: 10");
	spawnRate			= 1.50f;
	enemyBulletSpeed	= 500.0f;

	hero.~Hero();
	hero.init("Assets/graphics/heroAnim.png", 4, 1.0f, sf::Vector2f(viewSize.x * 0.5f, viewSize.y * 0.5f));

	for (Enemy* enemy : enemies) {
		delete (enemy);
	}

	for (Missile* missile : missiles) {
		delete (missile);
	}

	for (EnemyMissile* missile : enemyMissiles) {
		delete (missile);
	}

	enemies.clear();
	missiles.clear();
	enemyMissiles.clear();
}