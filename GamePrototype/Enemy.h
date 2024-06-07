#pragma once
#include <vector>
#include "Vector2f.h"
class Enemy
{
public:
	enum class Type
	{
		normal,
		medium,
		big,
		Boss
	};
	Enemy(Point2f pos);

	void Draw() const;
	void Update(float elapsedSec, Point2f playerPos);

	bool TongueHitFunctionality(std::vector<Point2f> collisionPoints, bool stickingTongueOut, Vector2f playervelocity, Vector2f tongueVelocity, float elapsedSec);
	bool CheckBulletHit(Circlef bulletHitbox, float damage);

	bool GetDead();
	Circlef GetHitbox();
	Type GetEnemyType();
	float GetDamage();
	float GetEatReturn();

protected:
	Circlef m_Hitbox;
	Vector2f m_Velocity;
	Color4f m_Color;
	Type m_Type;

	float m_Damage;
	float m_Health;
	bool  m_Grabbed;
};

