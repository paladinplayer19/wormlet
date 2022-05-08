#pragma once
#include <SFML/Graphics.hpp>

float VectorMagnitude(sf::Vector2f a);
float VectorMagnitude(sf::Vector3f a);
sf::Vector2f VectorNormalise(sf::Vector2f a);
sf::Vector3f VectorNormalise(sf::Vector3f a);
float VectorDot(sf::Vector2f a, sf::Vector2f b);
float VectorDot(sf::Vector3f a, sf::Vector3f b);
sf::Vector3f VectorCross(sf::Vector3f a, sf::Vector3f b);
sf::Vector2f CollisionDepth(sf::FloatRect a, sf::FloatRect b);