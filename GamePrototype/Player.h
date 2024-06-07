#pragma once
#include "Vector2f.h"
#include <vector>
#include "utils.h"
#include "Texture.h"
class Bullet;
class Player final
{
public:
	Player(float viewportWidth, float viewportHeight);
	~Player();

	void Draw();
	void Update(float elapsedSec);

	void CalculateAngle();

	void MouseMovement(const SDL_MouseMotionEvent& e);
	void KeyDown(const SDL_KeyboardEvent& e);
	void KeyUp(const SDL_KeyboardEvent& e);
	void MouseDownEvent(const SDL_MouseButtonEvent& e);
	void DeleteBullet(int index);

	Rectf GetTongueRectf();
	Circlef GetHitbox();
	bool GetTongueOut();
	float GetTongueVelocity();
	Vector2f GetPlayerVelocity();

	void IncrementCurrentWave();
	void AddDamageOverTime();

	void CollisionFunctionality(Circlef enemyHitbox, float damage);

	std::vector<Circlef> GetBulletHitboxes();
	std::vector<Point2f> GetTonguePoints();
	bool CheckTongueHitFinished(bool tongueHit, float eatHealth);
	bool GetDead();

private:
	float m_PlayerRadius{};
	Point2f m_Pos{100.f, 100.f};

	Rectf m_Tongue{-10.f, 0.f, 20.f, 35.f};
	Rectf m_Hitbox{};
	Vector2f m_DirectionVector{};
	float m_CharacterSpeed{};
	Point2f m_MousePos{};

	int m_BulletsAvailable{};

	std::vector<Point2f> m_TonguePoints{};
	std::vector<Bullet*> m_Bullets{};
	std::vector<Circlef> m_BulletHitboxes{};

	bool m_Left{};
	bool m_Right{};
	bool m_Up{};
	bool m_Down{};

	bool m_Invulnarable{};
	float m_InvulnarableTimer{};

	float m_Angle{};
	float m_Radius{30.f};

	float m_Speed{ 300.f };
	float m_Health{ 50.f };
	float m_MaxHealth{ m_Health };

	bool m_Growing{};
	bool m_Shrinking{};
	bool m_IsClicked{};
	bool m_LockedIn{};

	float m_WindowWidth;
	float m_WindowHeight;

	float m_GetDamagedTimer{};
	float m_HealthDepletionAmount{1.f};

	Rectf rect{ 200, 200, 100, 100 };

	Texture Font{Texture("")};
	int m_CurrentWave{};

	float CalcMagnitude(const Point2f& otherPoint);
	void BulletUpdate(float elapsedSec);
};

