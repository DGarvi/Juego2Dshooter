#include "SFML-2.6.0\include\SFML\Graphics.hpp"
#include "SFML-2.6.0\include\SFML\Audio.hpp"
#include <vector>
#include <iostream>
#include <chrono>
#include <ctime>
#include <windows.h>
#include "SFML-2.6.0\include\SFML\System\Time.hpp"

#include "Enemy.h"
#include "Hero.h"
#include "Missile.h"
#include "EnemyMissile.h"
#include "Powerup.h"
#include "Enemy2.h"

//SOUND BUFFERS
sf::Music bgMusic;
sf::SoundBuffer fireBuffer;
sf::SoundBuffer fireBuffer2;
sf::SoundBuffer hitBuffer;
sf::SoundBuffer hit2Buffer;

sf::Sound fireSound	(fireBuffer);
sf::Sound fireSound2 (fireBuffer2);
sf::Sound hitSound	(hitBuffer);
sf::Sound hit2Sound (hit2Buffer);

//WINDOW RENDER PARAMETERS
sf::Vector2f     viewSize	(1024, 768);
sf::VideoMode    vm			(viewSize.x, viewSize.y);
sf::RenderWindow window		(vm, "Guille vs gitanos", sf::Style::Default);

//SPRITE AND TEXTURE
sf::Texture skyTexture;
sf::Sprite  skySprite;

sf::Texture bgTexture;
sf::Sprite  bgSprite;

Hero hero;

std::vector<Enemy*>        enemies;
std::vector<Enemy2*>       enemies2;
std::vector<Missile*>      missiles;
std::vector<EnemyMissile*> enemyMissiles;
std::vector<Powerup*>      powerups;

float currentTime;
float prevTime              = 0.0f;
float currentTime2;
float prevTime2				= 0.0f;
float currentTime3;
float prevTime3				= 0.0f;
float currentTimeBullet;
float prevTimeBullet		= 0.0f;

float cadence				= 0.20f;
float bulletSpeed			= 2000.0f;

float heroSpeed				= 600.0f;
bool  playerMovingUp		= false;
bool  playerMovingDown		= false;
bool  playerMovingLeft		= false;
bool  playerMovingRight		= false;
bool  heroPowered			= false;

sf::Vector2f playerPosition;
bool playerMoving = false;

sf::Vector2i mousePos;

float maxSpeed				= 200.0f;
float spawnRate				= 3.0f;
float minSpawnRate			= 0.8f;
float enemyBulletSpeed		= 270.0f;
float minFirerate			= 0.7f;

//float powerupSpawnRate		= 10.0f;

int   score					= 0;
int	  currentScore;
bool  gameover				= true;
bool  notFirstTime			= false;

float timerBuff = 10.0f;
float elapsedTime;

//Text
sf::Font headingFont;
sf::Text headingText;

sf::Font scoreFont;
sf::Text scoreText;

sf::Text timerText;

sf::Text tutorialText;
sf::Text gameoverText;

void shoot					(sf::Vector2f playerPos	, sf::Vector2i mousePos);
bool checkCollisionBullet	(sf::Sprite sprite1		, sf::Sprite sprite2);
bool checkCollisionHero		(sf::Sprite sprite1		, sf::Sprite sprite2);
void enemyShoot				(sf::Vector2f playerPos	, sf::Vector2f enemyPos);
void reset();


void init() {

	//TEXTURE LOAD
	
	bgTexture.loadFromFile		("Assets/graphics/white.png");
	bgSprite.setTexture			(bgTexture);

	scoreFont.loadFromFile		("Assets/fonts/Arial.ttf");

	//SCORE TEXT LOAD
	scoreText.setFont			(scoreFont);
	scoreText.setString			("Puntuación: 0");
	scoreText.setCharacterSize	(45);
	scoreText.setFillColor		(sf::Color::White);

	sf::FloatRect scoreBounds = scoreText.getLocalBounds();

	scoreText.setOrigin			(scoreBounds.width / 2, scoreBounds.height / 2);
	scoreText.setPosition		(sf::Vector2f(viewSize.x * 0.5f, viewSize.y * 0.10f));


	//TUTORIAL TEXT LOAD
	tutorialText.setFont		(scoreFont);
	tutorialText.setString		(" Coge botellas de licor café para ser invulnerable a las balas,\n\n correr más y disparar más rápido\n\n APUNTA con el ratón y pulsa click izquierdo para DISPARAR\n\n Pulsa W A S D para MOVERTE\n\n\n\n Haz click para empezar ");
	tutorialText.setCharacterSize(32);
	tutorialText.setFillColor	(sf::Color::Black);

	sf::FloatRect tutorialBounds = tutorialText.getLocalBounds();
	tutorialText.setOrigin		(tutorialBounds.width / 2, tutorialBounds.height / 2);
	tutorialText.setPosition	(sf::Vector2f(viewSize.x * 0.5f, viewSize.y * 0.30f));

	//GAMEOVER TEXT LOAD
	gameoverText.setFont(scoreFont);
	gameoverText.setString("Te ha matao un gitano xd\n\n\n\n			Score: 0" + std::to_string(score));
	gameoverText.setCharacterSize(50);
	gameoverText.setFillColor(sf::Color::White);

	sf::FloatRect gameoverBounds = gameoverText.getLocalBounds();
	gameoverText.setOrigin(gameoverBounds.width / 2, gameoverBounds.height / 2);
	gameoverText.setPosition(sf::Vector2f(viewSize.x * 0.5f, viewSize.y * 0.5f));

	//BACKGROUND MUSIC
	bgMusic.openFromFile("Assets/audio/bgMusic.ogg");
	bgMusic.play();

	//HIT SOUND LOAD
	hitBuffer.loadFromFile("Assets/audio/hit.ogg");
	hit2Buffer.loadFromFile("Assets/audio/hit2.ogg");
	fireBuffer.loadFromFile("Assets/audio/fire.ogg");
	fireBuffer2.loadFromFile("Assets/audio/fire.ogg");

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
		if (100 + score * 4.0f < maxSpeed){
			
			enemy->enemySpeed += score * 4.0f;
			std::cout << enemy->enemySpeed << std::endl;
		
		}
		else {
			
			enemy->enemySpeed = maxSpeed;
			std::cout << enemy->enemySpeed << std::endl;
		
		}

		if (1.5f - score / 20.0f > minFirerate) {

			enemy->enemyFireRate -= score / 20.0f;
			std::cout << enemy->enemyFireRate << std::endl;

		}
		else {

			enemy->enemyFireRate = minFirerate;
			std::cout << enemy->enemyFireRate << std::endl;

		}

	enemies.push_back(enemy);
	
	}

}

void spawnPowerup() {

	int randLoc = rand() % 4;
	sf::Vector2f powerupPos;

	//DIFFERENT ENEMY SPAWN POINTS
	if (powerups.size() < 1) {

		switch (randLoc) {

		case 0: powerupPos = sf::Vector2f(viewSize.x * 0.70f, viewSize.y * 0.70f);
			break;

		case 1: powerupPos = sf::Vector2f(viewSize.x * 0.70f, viewSize.y * 0.30f);
			break;

		case 2: powerupPos = sf::Vector2f(viewSize.x * 0.30f, viewSize.y * 0.30f);
			break;

		case 3: powerupPos = sf::Vector2f(viewSize.x * 0.30f, viewSize.y * 0.70f);
			break;

		default: printf("Incorrect y Value \n");
			return;

		}

		Powerup* powerup = new Powerup();
		powerup->init("Assets/graphics/powerup.png", powerupPos);

		powerups.push_back(powerup);

	}

}

void spawnEnemy2() {

	int randLoc = rand() % 4;
	sf::Vector2f enemy2Pos;

	//DIFFERENT ENEMY SPAWN POINTS
	if (enemies2.size() < 1) {

		switch (randLoc) {

		case 0: enemy2Pos = sf::Vector2f(viewSize.x * 0, viewSize.y);
			break;

		case 1: enemy2Pos = sf::Vector2f(viewSize.x , viewSize.y * 0);
			break;

		case 2: enemy2Pos = sf::Vector2f(viewSize.x * 0, viewSize.y * 0);
			break;

		case 3: enemy2Pos = sf::Vector2f(viewSize.x, viewSize.y);
			break;

		default: printf("Incorrect y Value \n");
			return;

		}

		Enemy2* enemy2 = new Enemy2();
		enemy2->init("Assets/graphics/enemy2.png", enemy2Pos);

		enemies2.push_back(enemy2);

	}

}


void draw() {

	//DRAW 
	window.draw	(bgSprite);
	window.draw	(hero.getSprite());
	
	for (Powerup* powerup : powerups) {
		window.draw(powerup->getSprite());
	}
	
	for (Enemy2* enemy2 : enemies2) {
		window.draw(enemy2->getSprite());
	}

	for (Enemy* enemy : enemies) {
		window.draw(enemy->getSprite());
	}
	
	for (Missile* missile : missiles) {
		window.draw(missile->getSprite());
	}

	for (EnemyMissile* missile : enemyMissiles) {
		window.draw(missile->getSprite());
	}

	if (gameover && !notFirstTime) {
		
		window.draw(headingText);
		window.draw(tutorialText);
	
	}
	else if(gameover && notFirstTime) {
		
		window.draw(gameoverText);
	
	}
	else {
		
		window.draw(scoreText);
		
		if (heroPowered) {

			window.draw(timerText);
		
		}
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
					notFirstTime = true;
					reset();

				}
				else {
				
					if (currentTimeBullet >= prevTimeBullet + cadence) {
								
						mousePos =	sf::Mouse::getPosition(window);
						shoot		(hero.getSprite().getPosition(), mousePos);
									
						prevTimeBullet = currentTimeBullet;
							
						
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
	//currentTime2 += dt;
	currentTime3 += dt;

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
		spawnRate = spawnRate - 0.5f;
		
		if (spawnRate < minSpawnRate) {
			spawnRate = minSpawnRate;
		}

	}

	if (score % 20 == 0 && score != 0) {

		spawnEnemy2();

	}
	
	if (score % 15 == 0 && score != 0 && !heroPowered) {

		spawnPowerup();
		

	}

	if (heroPowered) {

		hero.setSprite("Assets/graphics/heroglow.png");
		heroSpeed = 900.0f;
		cadence = 0.10f;
		bulletSpeed = 4000.0f;
		
		timerText.setFont(scoreFont);		
		std::string timerS = "Kills restantes buff: " + std::to_string(5 - (score - currentScore));
		timerText.setCharacterSize(32);
		timerText.setFillColor(sf::Color::White);
		timerText.setString(timerS);

		sf::FloatRect timerBounds = timerText.getLocalBounds();
		timerText.setOrigin(timerBounds.width / 2, timerBounds.height / 2);
		timerText.setPosition(sf::Vector2f(viewSize.x * 0.8, viewSize.y * 0.2f));


		if (score - currentScore == 5) {
			
			hero.setSprite("Assets/graphics/heroAnim.png");
			heroSpeed = 600.0f;
			cadence = 0.20f;
			bulletSpeed = 2000.0f;
			heroPowered = false;
		
		}
	

	}



	/*if (currentTime2 >= prevTime2 + powerupSpawnRate) {

		spawnPowerup();

		prevTime2 = currentTime2;

	}*/
	

	
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

	for (int i = 0; i < enemies2.size(); i++) {

		Enemy2* enemy2 = enemies2[i];

		sf::Vector2f heroPos = sf::Vector2f(hero.getSprite().getPosition().x, hero.getSprite().getPosition().y);
		sf::Vector2f enemy2Pos = sf::Vector2f(enemy2->getSprite().getPosition().x, enemy2->getSprite().getPosition().y);
		enemy2->update(hero.getSprite().getPosition().x, hero.getSprite().getPosition().y, enemy2->enemy2Speed, dt);

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

			hero.setSprite("Assets/graphics/heroAnim.png");
			bgTexture.loadFromFile("Assets/graphics/gameover.png");
			bgSprite.setTexture(bgTexture);
			gameoverText.setString("Te ha matao un gitano xd\n\n\n\n			Score: " + std::to_string(score));
			window.draw(gameoverText);
			heroSpeed = 600.0f;
			cadence = 0.20f;
			bulletSpeed = 2000.0f;
			heroPowered = false;
			gameover = true;
		}

	}

	for (int i = 0; i < enemies2.size(); i++) {

		Enemy2* enemy2 = enemies2[i];

		if (checkCollisionHero(hero.getSprite(), enemy2->getSprite())) {

			hero.setSprite("Assets/graphics/heroAnim.png");
			bgTexture.loadFromFile("Assets/graphics/gameover.png");
			bgSprite.setTexture(bgTexture);
			gameoverText.setString("Te ha matao un gitano xd\n\n		Aun encima enano\n\n			Score: " + std::to_string(score));
			window.draw(gameoverText);
			heroSpeed = 600.0f;
			cadence = 0.20f;
			bulletSpeed = 2000.0f;
			heroPowered = false;
			gameover = true;
		}

	}

	for (int i = 0; i < powerups.size(); i++) {

		Powerup* powerup = powerups[i];

		if (checkCollisionHero(hero.getSprite(), powerup->getSprite())) {

			currentScore = score;
			heroPowered = true;
		
			powerups.erase(powerups.begin() + i);
			delete(powerup);
			powerups.clear();
			prevTime3 = 0.0f;
			currentTime3 = 0.0f;

		}

	}

	for (int i = 0; i < enemyMissiles.size(); i++) {

		EnemyMissile* missile = enemyMissiles[i];
		
		if (checkCollisionHero(hero.getSprite(), missile->getSprite()) && !heroPowered) {

			
			bgTexture.loadFromFile("Assets/graphics/gameover.png");
			bgSprite.setTexture(bgTexture);
			gameoverText.setString("Te ha matao un gitano xd\n\n\n\n			Score: " + std::to_string(score));
			window.draw(gameoverText);
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

				std::string finalScore				= "Puntuación: " + std::to_string(score);
				scoreText.setString(finalScore);
				sf::FloatRect scoreBounds			= scoreText.getLocalBounds();
				scoreText.setOrigin					(scoreBounds.width / 2	, scoreBounds.height / 2);
				scoreText.setPosition(sf::Vector2f	(viewSize.x * 0.5f		, viewSize.y * 0.10f));

				missiles.erase	(missiles.begin() + i);
				enemies.erase	(enemies.begin()  + j);

				delete(missile);
				delete(enemy);

			}
		}
	}

	 for (int j = 0; j < enemies2.size(); j++) {

		 for (int i = 0; i < missiles.size(); i++) {

			 Missile* missile = missiles[i];
			 Enemy2* enemy2 = enemies2[j];

			 if (checkCollisionBullet(missile->getSprite(), enemy2->getSprite())) {

				 hit2Sound.play();
				 score++;

				 std::string finalScore = "Puntuación: " + std::to_string(score);
				 scoreText.setString(finalScore);
				 sf::FloatRect scoreBounds = scoreText.getLocalBounds();
				 scoreText.setOrigin(scoreBounds.width / 2, scoreBounds.height / 2);
				 scoreText.setPosition(sf::Vector2f(viewSize.x * 0.5f, viewSize.y * 0.10f));

				 missiles.erase(missiles.begin() + i);
				 enemies2.erase(enemies2.begin() + j);

				 delete(missile);
				 delete(enemy2);

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

	missile->init("Assets/graphics/missile.png", sf::Vector2f(hero.getSprite().getPosition().x, hero.getSprite().getPosition().y), missileSpeedx, missileSpeedy);
	
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

	missile->init("Assets/graphics/enemymissile.png", sf::Vector2f(enemyPos.x, enemyPos.y), missileSpeedx, missileSpeedy);

	enemyMissiles.push_back(missile);

	fireSound2.play();

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

void reset() {

		bgTexture.loadFromFile("Assets/graphics/bg.png");
		bgSprite.setTexture(bgTexture);

		score = 0;
		currentTime = 0.0f;
		prevTime = 0.0f;
		currentTimeBullet = 0.0f;
		prevTimeBullet = 0.0f;
		scoreText.setString("Puntuación: 0");
		spawnRate = 1.50f;
		enemyBulletSpeed = 500.0f;

		hero.~Hero();
		hero.init("Assets/graphics/heroAnim.png", 4, 1.0f, sf::Vector2f(viewSize.x * 0.5f, viewSize.y * 0.5f));

		for (Enemy* enemy : enemies) {
			delete (enemy);
		}

		for (Enemy2* enemy2 : enemies2) {
			delete (enemy2);
		}

		for (Missile* missile : missiles) {
			delete (missile);
		}

		for (EnemyMissile* missile : enemyMissiles) {
			delete (missile);
		}

		for (Powerup* powerup : powerups) {
			delete (powerup);
		}

		enemies.clear();
		enemies2.clear();
		missiles.clear();
		enemyMissiles.clear();
		powerups.clear();
	
}

int main() {

	sf::Clock clock;
	sf::Clock bulletClock;
	
	//INITIALIZE OBJECTS
	init();

	//HIDE CONSOLE
	HWND hWnd = GetConsoleWindow();
	ShowWindow(hWnd, SW_HIDE);

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


