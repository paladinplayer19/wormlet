#include <SFML/Graphics.hpp>
#include <math.h>
#include <vector>
#include "VectorHelper.h"

int main()
{
	// Sets up window
	sf::RenderWindow window(sf::VideoMode(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height), "Grenade Battle");
	window.setKeyRepeatEnabled(false);

	// Load player Texture
	sf::Texture playerTexture;
	playerTexture.loadFromFile("Assets/Graphics/player_1.png");

	// Load floor texture
	sf::Texture floorTexture;
	floorTexture.loadFromFile("Assets/Graphics/tile.png");
	floorTexture.setRepeated(true);

	// Load popup texture
	sf::Texture popupTexture;
	popupTexture.loadFromFile("Assets/Graphics/tile.png");

	// Set player Sprite
	sf::Sprite playerSpr;
	playerSpr.setTexture(playerTexture);
	playerSpr.setOrigin(playerTexture.getSize().x / 2, playerTexture.getSize().y / 2);
	
    // Set floor sprite
	sf::Sprite floorSpr;
	floorSpr.setTexture(floorTexture);
	floorSpr.setTextureRect(sf::IntRect(window.getSize().x / 2, 0, window.getSize().x / 2, floorTexture.getSize().y));
	sf::FloatRect floorAABB = sf::FloatRect();
    sf::Vector2f floorSize = sf::Vector2f();

	// Setup popup 
	sf::Sprite popupSpr;
	popupSpr.setTexture(popupTexture);
	popupSpr.setScale(5,5);
	popupSpr.setOrigin(popupTexture.getSize().x / 2, popupTexture.getSize().y / 2);
	sf::Vector2f popupPos = sf::Vector2f(window.getSize().x / 2, window.getSize().y);
	float showTimePassed = 0.0f;
	const float duration = 1.0f;
	sf::Vector2f offScreenPos = sf::Vector2f(popupPos);
	sf::Vector2f onScreenPos = sf::Vector2f(window.getSize().x / 2, window.getSize().y / 2);
	sf::Vector2f change = sf::Vector2f();
	bool isPlaying = false;

	//window.getSize().x / 2, window.getSize().y + popupSpr.getGlobalBounds().height

	// Setup Pips
	sf::Sprite pipSpr;
	sf::Texture pipTexture;

	pipTexture.loadFromFile("Assets/Graphics/pip.png");
	pipSpr.setTexture(pipTexture);
	std::vector<sf::Sprite> pipSprites;
	int pipNumb = 10;
	sf::Vector2f pipPos = sf::Vector2f();

	//
	sf::Vector2f gravity = sf::Vector2f(0, 1000.0f);
	sf::Vector2f firingPos = sf::Vector2f();
	sf::Vector2f firingDir = sf::Vector2f(1.0f, 0.0f);
	float firingSpd = 750.0f;
	//
	for (int i = 0; i < pipNumb; ++i)
	{
		pipSprites.push_back(pipSpr);
	}

	// Grenade 
	sf::Texture grenadeTexture;
	grenadeTexture.loadFromFile("Assets/Graphics/bullet.png");
	sf::Sprite grenadeSpr;
	grenadeSpr.setTexture(grenadeTexture);
	sf::Vector2f grenadeVel(0.0f, 0.0f);

	//Player Movement
	sf::FloatRect playerAABB = sf::FloatRect();
	sf::Vector2f playerSize = sf::Vector2f();
	sf::Vector2f playerVel = sf::Vector2f(0.0f, 0.0f);
	sf::Vector2f playerAcc = sf::Vector2f(0.0f, 0.0f);
	sf::Vector2f currentPos = sf::Vector2f(0.0f, 0.0f);
	sf::Vector2f prevPos = sf::Vector2f(0.0f, 0.0f);
	sf::Vector2f prevVel = sf::Vector2f(0.0f, 0.0f);
	sf::Vector2f prevAcc = sf::Vector2f(0.0f, 0.0f);
	sf::Vector2f jumpHeight = sf::Vector2f(0.0f, 300.0f);
	sf::Vector2f floorPos = sf::Vector2f(0, window.getSize().y / 2);
	sf::Vector2f playerPos = sf::Vector2f(window.getSize().x / 2, floorPos.y - floorAABB.height);
	sf::Vector2f distance = sf::Vector2f(0.0f, 0.0f);
	
	float accRate_x = 10000.0f;
	float accRate_y = 11550.0f;
	float drag_x = 0.009f;
	float drag_y = 0.009f;
	bool canJump = true;
	bool isTouching = false;

	//Bounding Box Offset and scale
	sf::Vector2f playerCollisionScale = sf::Vector2f(1,1);
	sf::Vector2f playerCollisionOffset = sf::Vector2f(1,1);
	sf::Vector2f floorCollisionScale = sf::Vector2f(1,1);
	sf::Vector2f floorCollisionOffset = sf::Vector2f(1,1);

	//Game Clock
	sf::Clock clock;

	while (window.isOpen())
	{
		prevPos = playerPos;
		prevVel = playerVel;
		prevAcc = playerAcc;

		// Clock setup
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

			// Spacebar detection
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Space) {
					
					// If the player is allowed to jump then apply movement
					if (canJump) 
					{
					currentPos = playerPos;
					playerAcc.y = -accRate_y;
					}
				}
			}
		}


		// Applies drag
		playerAcc.x = 0;
		
		//Declare changed movement variables
		sf::Vector2f deltaVel;
		sf::Vector2f deltaPos;
		
		// If left arrow is pressed then player will move left
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			playerAcc.x = -accRate_x;
		}

		// If right arrow is pressed then player will move right
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			playerAcc.x = accRate_x;
		}

		// Keeps track of players previous position
		prevPos = playerPos;

		// Takes information of the sprites and store the values in a variable
		playerAABB = playerSpr.getGlobalBounds();
		floorAABB = floorSpr.getGlobalBounds();

		playerAABB.left += playerCollisionOffset.x;
		playerAABB.top += playerCollisionOffset.y;
		playerAABB.width *= playerCollisionScale.x;
		playerAABB.height *= playerCollisionScale.y;

		floorAABB.left += floorCollisionOffset.x;
		floorAABB.top += floorCollisionOffset.y;
		floorAABB.width *= floorCollisionScale.x;
		floorAABB.height *= floorCollisionScale.y;

		// Stores the size of the x and y cooardinates of both sprites
		playerSize.x = playerAABB.width;	
		playerSize.y = playerAABB.height;
		floorSize.x = floorAABB.width;
		floorSize.y = floorAABB.height;

		// Creates a rectangle with the sizes
		sf::RectangleShape player(playerSize);
		sf::RectangleShape floor(floorSize);

		// Sets positions of the rectangles
		player.setPosition(sf::Vector2f(playerAABB.left, playerAABB.top));
		floor.setPosition(sf::Vector2f(floorAABB.left, floorAABB.top));

		// Sets Width and height variables
		sf::Vector2f playerWH = sf::Vector2f(playerAABB.width, playerAABB.height);
		sf::Vector2f floorWH = sf::Vector2f(floorAABB.width, floorAABB.height);

		//Stores centre values of each sprite
		sf::Vector2f playerCentre = sf::Vector2f(playerAABB.left, playerAABB.top);
		sf::Vector2f floorCentre = sf::Vector2f(floorAABB.left, floorAABB.top);
		playerCentre += 0.5f * playerWH;
		floorCentre += 0.5f * floorWH;

		// Checks if player collides with floor and then stores a boolean value in a variable
		isTouching = playerAABB.intersects(floorAABB);

		// Calculates distance  between player and the floor
		distance = floorCentre - playerCentre;

		// Declare variables
		sf::Vector2f minDistance = sf::Vector2f(0.0f, 0.0f);
		sf::Vector2f depth = sf::Vector2f(0.0f, 0.0f);

		//Calculates the minimum distance between floor and player sprites
		minDistance.x = 0.5f * playerWH.x + 0.5f * floorWH.x;
		minDistance.y = 0.5f * playerWH.y + 0.5f * floorWH.y;

		// Checks to see if which side of the floor the player is closer
		if (distance.x < 0) {

			minDistance.x = -minDistance.x;
		}
		if (distance.y < 0) {

			minDistance.y = -minDistance.y;

		}

		// Calculates the depth between the 2 sprites
		depth = minDistance - distance;
		
		// Calculates the absolute value of the depth values
		sf::Vector2f absDepth = sf::Vector2f(abs(depth.x), abs(depth.y));

		// If the player collides with the floor then move player sprite outside floor sprites bounds
		if (isTouching) {
			
			

			// Compares the absolute depth to decide which side to move the player to after collision
			if (absDepth.x < absDepth.y) {


				playerPos.x -= depth.x;
				playerVel.x = 0;
			}
			else
			{
				playerPos.y -= depth.y;
				playerVel.y = 0;
			}

			
			// Allows player to jump if they are on the floor
			canJump = true;
			
		}

		// If the player is not grounded then the player cant jump
		if (playerVel.y != 0.0f) {
			canJump = false;
		}


		
					
			// Applies gravity each frame 
		     playerAcc.y = gravity.y; 
			


			//////////////////////////////// Practical Task 4 - Physics Alternatives //////////////////////////////////////////////////

			const int EXPLICIT_EULER = 1;
			const int IMPLICIT_EULER = 2;
			const int VELOCITY_VERLET = 3;

			// Change Calculation mode by using the above variable names

			const int MODE = VELOCITY_VERLET;

			switch (MODE)
			{
			case EXPLICIT_EULER:
				
				// Applies movement on the y axis (aka jumping)
				deltaVel.y = prevAcc.y * deltaTime;
				playerVel.y = playerVel.y + deltaVel.y;

				playerVel.y = playerVel.y - playerVel.y * drag_y;

				deltaPos.y = prevVel.y * deltaTime;
				playerPos.y = playerPos.y + deltaPos.y;

		       // Applies movement on the x axis (Moving left and right)
				deltaVel.x = prevAcc.x * deltaTime;
				playerVel.x = playerVel.x + deltaVel.x;

				playerVel.x = playerVel.x - playerVel.x * drag_x;

				deltaPos.x = prevVel.x * deltaTime;
				playerPos.x = playerPos.x + deltaPos.x;

				break;

			case IMPLICIT_EULER:

				// Applies movement on the y axis (aka jumping)
				deltaVel.y = playerAcc.y * deltaTime;
				playerVel.y = playerVel.y + deltaVel.y;

				playerVel.y = playerVel.y - playerVel.y * drag_y;

				deltaPos.y = playerVel.y * deltaTime;
				playerPos.y = playerPos.y + deltaPos.y;

				// Applies movement on the x axis (Moving left and right)
				deltaVel.x = playerAcc.x * deltaTime;
				playerVel.x = playerVel.x + deltaVel.x;

				playerVel.x = playerVel.x - playerVel.x * drag_x;

				deltaPos.x = playerVel.x * deltaTime;
				playerPos.x = playerPos.x + deltaPos.x;

				break;

			
			case VELOCITY_VERLET:
			{
				sf::Vector2f firstHalfVel = prevVel + prevAcc * (deltaTime * 0.5f);
				// Applies movement on the y axis (aka jumping)
				deltaPos.y = firstHalfVel.y * deltaTime;
				playerPos.y = playerPos.y + deltaPos.y;

				deltaVel.y = playerAcc.y * (deltaTime * 0.5f);
				playerVel.y = firstHalfVel.y + deltaVel.y;

				playerVel.y = playerVel.y - playerVel.y * drag_y;


				// Applies movement on the x axis (Moving left and right)
				deltaPos.x = firstHalfVel.x * deltaTime;
				playerPos.x = playerPos.x + deltaPos.x;

				deltaVel.x = playerAcc.x * (deltaTime * 0.5f);
				playerVel.x = firstHalfVel.x + deltaVel.x;

				playerVel.x = playerVel.x - playerVel.x * drag_x;

			}
				break;

			default:

				break;
			}


		
		


		

		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
		{
			 isPlaying = true;
		}

		if (isPlaying) {
			showTimePassed += deltaTime;

			change = onScreenPos - offScreenPos;

			if (showTimePassed >= duration)
			{
				popupPos = offScreenPos + change;
			}
			else
			{

				sf::Vector2f k1 = change / duration * duration;


				sf::Vector2f k2 = sf::Vector2f(0.0f, 0.0f);


				sf::Vector2f k3 = offScreenPos;

				popupPos = k1 * showTimePassed * showTimePassed + k2 * showTimePassed + k3;
			}
		}


		// Sets positions for both player and floor
		floorSpr.setPosition(floorPos);
		playerSpr.setPosition(playerPos);
		popupSpr.setPosition(popupPos);

		// Practical Task 3 - Gravity Prediction

		sf::Vector2f mousPos = sf::Vector2f(sf::Mouse::getPosition(window));
		firingDir = mousPos - playerPos;
		firingDir = VectorNormalise(firingDir);
		
		sf::Vector2f firingVel = firingDir * firingSpd;
		firingPos = playerPos;
		
		
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::F))
		{
			grenadeSpr.setPosition(firingPos);
			grenadeVel = firingVel;
		}
		
		float pipTime = 0;
		for (int i = 0; i < pipSprites.size(); ++i)
		{
			
			pipTime += 0.1f;
			pipPos = (gravity * (pipTime * pipTime) / 2.0f + firingVel * pipTime + firingPos);
			pipSprites[i].setPosition(pipPos);
		}
		
		grenadeVel +=  gravity * frameTime.asSeconds();
		grenadeSpr.setPosition(grenadeSpr.getPosition() + grenadeVel * frameTime.asSeconds());

		/// Practical Task 5 - Reflection //////////////////////////////
		
		sf::FloatRect grenadeAABB = grenadeSpr.getGlobalBounds();

		if (floorAABB.intersects(grenadeAABB))
		{
			sf::Vector2f depth = CollisionDepth(grenadeAABB, floorAABB);
			sf::Vector2f absDepth = sf::Vector2f(abs(depth.x), abs(depth.y));
			sf::Vector2f normal;

			if (absDepth.x < absDepth.y)
			{
				sf::Vector2f grenadePos = grenadeSpr.getPosition();
				grenadePos.x += depth.x;
				grenadeSpr.setPosition(grenadePos);

				if (depth.x > 0)
				{
					normal = sf::Vector2f(-1, 0);
				}
				else
				{
					normal = sf::Vector2f(1, 0);
				}
			} else
			{
				sf::Vector2f grenadePos = grenadeSpr.getPosition();
				grenadePos.y += depth.y;
				grenadeSpr.setPosition(grenadePos);

				if (depth.y > 0)
				{
					normal = sf::Vector2f(0, -1);
				}
				else
				{
					normal = sf::Vector2f(0, 1);
				}
			}

			grenadeVel = grenadeVel - 2.0f * normal * (VectorDot(grenadeVel, normal));

		}

		// Clears
		window.clear();
		
		// Draws
		window.draw(floorSpr);
		window.draw(playerSpr);
		window.draw(popupSpr);
		window.draw(grenadeSpr);
		

		for (int i = 0; i < pipSprites.size(); ++i) {

			window.draw(pipSprites[i]);
		}
		
		
		// Displays
		window.display();

	}


	return 0;
}