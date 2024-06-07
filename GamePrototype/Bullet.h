#pragma once
#include "Vector2f.h"

class Bullet
{
public:
	Bullet(Point2f pos, Vector2f directionvelocity);

	void Draw();
	void Update(float elapsedSec);

	Circlef GetHitbox();
	bool Expired();

private:
	Circlef m_Circle;
	Vector2f m_DirectionVelocity;
	float m_Time;
};

