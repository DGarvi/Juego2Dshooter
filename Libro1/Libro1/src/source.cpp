#include "SFML-2.6.0\include\SFML\Graphics.hpp"


sf::Vector2f viewSize(1024, 768);
sf::VideoMode vm(viewSize.x, viewSize.y);
sf::RenderWindow window(vm, "SFML Game", sf::Style::Default);

sf::Texture skyTexture;
sf::Sprite skySprite;
sf::Texture bgTexture;
sf::Sprite bgSprite;
sf::Texture heroTexture;
sf::Sprite heroSprite;

sf::Vector2f playerPosition;
bool playerMovingRight = false;
bool playerMovingLeft = false; 
bool playerMovingUp = false;
bool playerMovingDown = false;
float speed = 500.0f;

void init() {

	skyTexture.loadFromFile("Assets/graphics/sky.png");
	skySprite.setTexture(skyTexture);
	bgTexture.loadFromFile("Assets/graphics/bg.png");
	bgSprite.setTexture(bgTexture);
	heroTexture.loadFromFile("Assets/graphics/hero.png");
	heroSprite.setTexture(heroTexture);
	heroSprite.setPosition(sf::Vector2f(viewSize.x / 2, viewSize.y / 2));
	heroSprite.setOrigin(heroTexture.getSize().x / 2, heroTexture.getSize().y / 2);

}

void draw() {

	window.draw(skySprite);
	window.draw(bgSprite);
	window.draw(heroSprite);
}

void updateInput() {

	sf::Event event;

	while (window.pollEvent(event)) {

		

		if (event.type == sf::Event::KeyPressed) {

			if (event.key.code == sf::Keyboard::Up) {

				playerMovingUp = true;
			}

		}


		if (event.type == sf::Event::KeyReleased) {

			if (event.key.code == sf::Keyboard::Up) {

				playerMovingUp = false;
			}
		}


		if (event.type == sf::Event::KeyPressed) {

			if (event.key.code == sf::Keyboard::Down) {

				playerMovingDown = true;
			}

		}


		if (event.type == sf::Event::KeyReleased) {

			if (event.key.code == sf::Keyboard::Down) {

				playerMovingDown = false;
			}
		}
		

		if (event.type == sf::Event::KeyPressed) {

			if (event.key.code == sf::Keyboard::Left) {

				playerMovingLeft = true;
			}

		}


		if (event.type == sf::Event::KeyReleased) {

			if (event.key.code == sf::Keyboard::Left) {

				playerMovingLeft = false;
			}
		}

		if (event.type == sf::Event::KeyPressed) {
		
			if (event.key.code == sf::Keyboard::Right) {
				
				playerMovingRight = true;
			}
		
		}
	

		if (event.type == sf::Event::KeyReleased) {

			if (event.key.code == sf::Keyboard::Right) {
				
				playerMovingRight = false;
			}
		}
		
		

		if (event.key.code == sf::Keyboard::Escape || event.type == sf::Event::Closed)
			window.close();

	}

}

void update(float dt) {

	if (playerMovingRight) {
		heroSprite.move(speed * dt, 0);

	}

	if (playerMovingLeft) {
		heroSprite.move(-speed * dt, 0);
		}

	if (playerMovingUp) {
		heroSprite.move(0, -speed * dt);
	}

	if (playerMovingDown) {
		heroSprite.move(0, speed * dt);

	}
}

int main() {
	sf::Clock clock;
	//Inicializar objetos
	init();

	//Abrir window
	while (window.isOpen()) {


		//Keyboard events
		updateInput();
		//Update game
		sf::Time dt = clock.restart();
		update(dt.asSeconds());
		//Update game objects
		window.clear(sf::Color::Red);
		
		draw();
		
		//Render game objects
		window.display();

	}


	return 0;
}