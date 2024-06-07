#include "pch.h"
#include "Enemy.h"
#include "utils.h"
#include <iostream>

using namespace utils;

Enemy::Enemy(Point2f pos):
	m_Damage{5.f},
	m_Color{},
	m_Velocity{150.f, 150.f},
	m_Hitbox{pos, 30.f},
	m_Health{ 5.f },
	m_Grabbed{},
	m_Type{ Type::normal}
{
	m_Velocity.x = rand() % 50 + 70;
	m_Velocity.y = m_Velocity.x;
}

void Enemy::Draw() const
{
	SetColor(Color4f{ 0.f, 1.f, 0.f, 1.f });
	FillEllipse(m_Hitbox.center, m_Hitbox.radius, m_Hitbox.radius);

	Rectf HealthRect{};
	HealthRect.width = m_Health * 10.f;
	HealthRect.height = 10.f;
	HealthRect.left = m_Hitbox.center.x - HealthRect.width / 2.f;
	HealthRect.bottom = m_Hitbox.center.y + m_Hitbox.radius + 5.f;
	
	Color4f healthBarColor{};
	if (m_Health <= 5.f) healthBarColor = Color4f{ 0.f, 1.f, 0.f, 1.f };
	else if (m_Health <= 10.f) healthBarColor = Color4f{ 1.f, 1.f, 0.f, 1.f };
	else if (m_Health <= 15.f) healthBarColor = Color4f{ 1.f, 0.f, 0.f, 1.f };

	SetColor(healthBarColor);
	FillRect(HealthRect);
	SetColor(Color4f{ 1.f, 1.f, 1.f, 1.f });
	DrawRect(HealthRect, 2.f);
}

void Enemy::Update(float elapsedSec, Point2f playerPos)
{
	Vector2f velocity{ m_Velocity };
	if ((playerPos.x > m_Hitbox.center.x && playerPos.y > m_Hitbox.center.y) || (playerPos.x < m_Hitbox.center.x && playerPos.y < m_Hitbox.center.y))
	{
		velocity.x /= 2;
		velocity.y /= 2;
	}
	if (!m_Grabbed)
	{
		if (playerPos.x > m_Hitbox.center.x) m_Hitbox.center.x += m_Velocity.x * elapsedSec;
		if (playerPos.y > m_Hitbox.center.y) m_Hitbox.center.y += m_Velocity.y * elapsedSec;
		if (playerPos.x < m_Hitbox.center.x) m_Hitbox.center.x -= m_Velocity.x * elapsedSec;
		if (playerPos.y < m_Hitbox.center.y) m_Hitbox.center.y -= m_Velocity.y * elapsedSec;

	}
}

bool Enemy::TongueHitFunctionality(std::vector<Point2f> collisionPoints, bool stickingTongueOut, Vector2f tongueVelocity, Vector2f playervelocity, float elapsedSec)
{
	int collidingTonguePoint {};
	for (int currentPoint{}; currentPoint < collisionPoints.size(); ++currentPoint)
	{
		if (IsPointInCircle(collisionPoints[currentPoint], m_Hitbox) && m_Health <= 5.f && stickingTongueOut)
		{
			collidingTonguePoint = currentPoint;
			m_Grabbed = true;
			break;
		}
	}

	if (m_Grabbed && m_Health <= 5.f && stickingTongueOut)
	{
		if (collisionPoints[collidingTonguePoint].x > m_Hitbox.center.x) m_Hitbox.center.x += (playervelocity.x * elapsedSec) + (tongueVelocity.x * elapsedSec);
		if (collisionPoints[collidingTonguePoint].y > m_Hitbox.center.y) m_Hitbox.center.y += (playervelocity.y * elapsedSec) + (tongueVelocity.y * elapsedSec);
		if (collisionPoints[collidingTonguePoint].x < m_Hitbox.center.x) m_Hitbox.center.x -= (playervelocity.x * elapsedSec) + (tongueVelocity.x * elapsedSec);
		if (collisionPoints[collidingTonguePoint].y < m_Hitbox.center.y) m_Hitbox.center.y -= (playervelocity.x * elapsedSec) + (tongueVelocity.y * elapsedSec);

		m_Hitbox.center.x += (playervelocity.x * elapsedSec);
		m_Hitbox.center.y += (playervelocity.y * elapsedSec);

		m_Damage = 0.f;
		return true;
	}
	return false;
}

bool Enemy::CheckBulletHit(Circlef bulletHitbox, float damage)
{
	std::cout << bulletHitbox.center.x;
	if (IsOverlapping(m_Hitbox, bulletHitbox))
	{
		m_Health -= damage;
		std::cout << m_Health << std::endl;
		return true;
	}
	return false;
}

bool Enemy::GetDead()
{
	return m_Health <= 0.f;
}

Circlef Enemy::GetHitbox()
{
	return m_Hitbox;
}

Enemy::Type Enemy::GetEnemyType()
{
	return m_Type;
}

float Enemy::GetDamage()
{
	return m_Damage;
}

float Enemy::GetEatReturn()
{
	return m_Health;
}
