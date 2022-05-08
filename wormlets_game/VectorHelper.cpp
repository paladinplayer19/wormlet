#include "VectorHelper.h"
#include <math.h>

float VectorMagnitude(sf::Vector2f a)
{
	float mag = sqrt(a.x * a.x + a.y * a.y);

	return mag;
}

float VectorMagnitude(sf::Vector3f a)
{
	float mag = sqrt(a.x * a.x + a.y * a.y + a.z * a.z);

	return mag;
}

sf::Vector2f VectorNormalise(sf::Vector2f a)
{
	return a / VectorMagnitude(a);
}

sf::Vector3f VectorNormalise(sf::Vector3f a)
{
	return a / VectorMagnitude(a);
}

float VectorDot(sf::Vector2f a, sf::Vector2f b)
{

	return a.x * b.x + a.y * b.y;
}

float VectorDot(sf::Vector3f a, sf::Vector3f b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

sf::Vector3f VectorCross(sf::Vector3f a, sf::Vector3f b)
{
	sf::Vector3f c;

	c.x = a.y * b.z - a.z * b.y;
	c.y = a.z * b.x - a.x * b.z;
	c.z = a.x * b.y - a.y * b.x;
		
	return c;
}

sf::Vector2f CollisionDepth(sf::FloatRect a, sf::FloatRect b)
{
	sf::Vector2f aWidthHeight = sf::Vector2f(a.width, a.height);
	sf::Vector2f bWidthHeight = sf::Vector2f(b.width, b.height);

	sf::Vector2f aCentre = sf::Vector2f(a.left, a.top) + 0.5f * aWidthHeight;
	sf::Vector2f bCentre = sf::Vector2f(b.left, b.top) + 0.5f * bWidthHeight;

	sf::Vector2f distance = aCentre - bCentre;
	sf::Vector2f minDistance = 0.5f * aWidthHeight + 0.5f * bWidthHeight;

	if (distance.x < 0)
	{
		minDistance.x = -minDistance.x;
	}
	if (distance.y < 0)
	{
		minDistance.y = -minDistance.y;
	}

	sf::Vector2f depth = minDistance - distance;

	return depth;
}
