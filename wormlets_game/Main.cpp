#include <SFML/Graphics.hpp>
#include <math.h>

int main()
{

	sf::RenderWindow window(sf::VideoMode(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height), "Grenade Battle");
	window.setKeyRepeatEnabled(false);

	// Load player Texture
	sf::Texture playerTexture;
	playerTexture.loadFromFile("Assets/Graphics/player_1.png");

	// Load floor texture
	sf::Texture floorTexture;
	floorTexture.loadFromFile("Assets/Graphics/tile.png");
	floorTexture.setRepeated(true);

	// Set player Sprite
	sf::Sprite playerSpr;
	playerSpr.setTexture(playerTexture);
	playerSpr.setOrigin(playerTexture.getSize().x / 2, playerTexture.getSize().y / 2);
	
    // Set floor sprite
	sf::Sprite floorSpr;
	floorSpr.setTexture(floorTexture);
	floorSpr.setTextureRect(sf::IntRect(0, 0, window.getSize().x, floorTexture.getSize().y));
	sf::FloatRect floorAABB = sf::FloatRect();
    sf::Vector2f floorSize = sf::Vector2f();

	
	//Player Movement
	sf::FloatRect playerAABB = sf::FloatRect();
	sf::Vector2f playerSize = sf::Vector2f();
	sf::Vector2f playerVel = sf::Vector2f(0.0f, 0.0f);
	sf::Vector2f playerAcc = sf::Vector2f(0.0f, 0.0f);
	sf::Vector2f currentPos = sf::Vector2f(0.0f, 0.0f);
	sf::Vector2f prevPos = sf::Vector2f(0.0f, 0.0f);
	sf::Vector2f jumpHeight = sf::Vector2f(0.0f, 300.0f);
	sf::Vector2f floorPos = sf::Vector2f(0, window.getSize().y / 2);
	sf::Vector2f playerPos = sf::Vector2f(window.getSize().x / 2, floorPos.y - floorAABB.height);
	sf::Vector2f distance = sf::Vector2f(0.0f, 0.0f);
	float gravity = 10.0f;
	float accRate_x = 10000.0f;
	float accRate_y = 11550.0f;
	float drag_x = 0.009f;
	float drag_y = 0.009f;
	bool canJump = false;
	bool isJumping = true;
	bool isTouching = false;

	
	//Game Clock
		sf::Clock clock;
	while (window.isOpen())
	{
		sf::Time frameTime = clock.restart();
		float deltaTime = frameTime.asSeconds();

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Escape) {
					window.close();
				}
			}
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Space) {
					isJumping = true;
					if (canJump) 
					{
					currentPos = playerPos;
					playerAcc.y = -accRate_y;
					}
				}
			}
		}

		playerAcc.x = 0;
		
		sf::Vector2f deltaVel;
		sf::Vector2f deltaPos;
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			playerAcc.x = -accRate_x;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			playerAcc.x = accRate_x;
		}
		prevPos = playerPos;

		playerAABB = playerSpr.getGlobalBounds();
		floorAABB = floorSpr.getGlobalBounds();

		playerSize.x = playerAABB.width;
		playerSize.y = playerAABB.height;
		floorSize.x = floorAABB.width;
		floorSize.y = floorAABB.height;

		sf::RectangleShape player(playerSize);
		sf::RectangleShape floor(floorSize);

		player.setPosition(sf::Vector2f(playerAABB.left, playerAABB.top));
		floor.setPosition(sf::Vector2f(floorAABB.left, floorAABB.top));

		sf::Vector2f playerWH = sf::Vector2f(playerAABB.width, playerAABB.height);
		sf::Vector2f floorWH = sf::Vector2f(floorAABB.width, floorAABB.height);

		sf::Vector2f playerCentre = sf::Vector2f(playerAABB.left, playerAABB.top);
		sf::Vector2f floorCentre = sf::Vector2f(floorAABB.left, floorAABB.top);
		playerCentre += 0.5f * playerWH;
		floorCentre += 0.5f * floorWH;

		isTouching = playerAABB.intersects(floorAABB);

		distance = floorCentre - playerCentre;

		sf::Vector2f minDistance = sf::Vector2f(0.0f, 0.0f);
		sf::Vector2f depth = sf::Vector2f(0.0f, 0.0f);

		minDistance.x = 0.5f * playerWH.x + 0.5f * floorWH.x;
		minDistance.y = 0.5f * playerWH.y + 0.5f * floorWH.y;

		if (distance.x < 0) {

			minDistance.x = -minDistance.x;
		}
		if (distance.y < 0) {

			minDistance.y = -minDistance.y;

		}

		depth = minDistance - distance;
		
		sf::Vector2f absDepth = sf::Vector2f(abs(depth.x), abs(depth.y));

		if (isTouching) {
			
			playerPos = prevPos;

			if (absDepth.x < absDepth.y) {


				playerPos.x -= depth.x;
				playerVel.x = 0;
			}
			else
			{
				playerPos.y -= depth.y;
				playerVel.y = 0;
			}

			

			canJump = true;
			isJumping = false;
		}

		/*if ()
		{
			playerAcc.y = 0.0f;
			playerVel.y = 0.0f;
			playerPos.y = groundPos.y;
			canJump = true;
			isJumping = false;
		}*/


		if (isJumping)
		{
			canJump = false;
					
			// If player reaches new displaced position then apply gravity
			
				playerAcc.y += gravity;
			// Y
			
			deltaVel.y = playerAcc.y * deltaTime;
			playerVel.y = playerVel.y + deltaVel.y;

			playerVel.y = playerVel.y - playerVel.y * drag_y;

			deltaPos.y = playerVel.y * deltaTime;
			playerPos.y = playerPos.y + deltaPos.y;
			

			
			
		}
		

		// X
		deltaVel.x = playerAcc.x * deltaTime;
		playerVel.x = playerVel.x + deltaVel.x;

		playerVel.x = playerVel.x - playerVel.x * drag_x;
			
		deltaPos.x = playerVel.x * deltaTime;
		playerPos.x = playerPos.x + deltaPos.x;

		
		
		
		
		
	    
		
		
		

		floorSpr.setPosition(floorPos);
		playerSpr.setPosition(playerPos);

		window.clear();
		
		window.draw(floorSpr);
		window.draw(playerSpr);
		
		

		window.display();

	}


	return 0;
}