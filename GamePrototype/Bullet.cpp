#include "pch.h"
#include "Bullet.h"
#include "utils.h"
#include <iostream>

using namespace utils;

Bullet::Bullet(Point2f pos, Vector2f directionvelocity):
	m_Circle{ pos, 8.f},
	m_DirectionVelocity{directionvelocity},
	m_Time{}
{
}

void Bullet::Draw()
{
	SetColor(Color4f{ 1.f, 1.f, 1.f, 1.f });
	FillEllipse(m_Circle.center, m_Circle.radius, m_Circle.radius);
}

void Bullet::Update(float elapsedSec)
{
	m_Circle.center.x += ( 400.f * m_DirectionVelocity.x ) * elapsedSec;
	m_Circle.center.y += ( 400.f * m_DirectionVelocity.y ) * elapsedSec;

	m_Time += elapsedSec;
}

Circlef Bullet::GetHitbox()
{
	return m_Circle;
}

bool Bullet::Expired()
{
	return m_Time >= 2.f;
}
