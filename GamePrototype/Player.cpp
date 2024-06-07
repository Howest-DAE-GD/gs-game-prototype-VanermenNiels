#include "pch.h"
#include "Player.h"
#include <iostream>
#include "Bullet.h"

using namespace utils;

Player::Player(float viewportWidth, float viewportHeight):
    m_WindowWidth{ viewportWidth },
    m_WindowHeight{viewportHeight}
{
    m_TonguePoints.push_back(Point2f{ 0,0 });
    m_BulletHitboxes.push_back(Circlef{});
    m_TonguePoints.resize(20);
}

Player::~Player()
{
    for (int currentBullet{}; currentBullet < m_Bullets.size(); ++currentBullet)
    {
        delete m_Bullets[currentBullet];
        m_Bullets[currentBullet] = nullptr;
    }
    m_Bullets.erase(std::remove_if(m_Bullets.begin(), m_Bullets.end(), [](Bullet* bullet) { return bullet == nullptr; }), m_Bullets.end());
}

void Player::Draw()
{
    for (int currentBullet{}; currentBullet < m_Bullets.size(); ++currentBullet)
    {
        m_Bullets[currentBullet]->Draw();
    }

    glPushMatrix();
    {
        glTranslatef(m_Pos.x, m_Pos.y, 0);
        glRotatef(m_Angle - 90.f, 0, 0, 1.f);

        SetColor(Color4f{ 1.f, 0.5f, 0.6f, 1.f });
        FillRect(m_Tongue);

        SetColor(Color4f{ 0.5f, 0.5f, 1.f, 1.f });
        FillEllipse(Point2f{ 0.f, 0.f }, m_Radius, m_Radius);
    }
    glPopMatrix();

    Rectf healthBar{};
    healthBar.width = m_Health * 10.f;
    healthBar.height = 30.f;
    healthBar.left = 0.f;
    healthBar.bottom = m_WindowHeight - healthBar.height;

    SetColor(Color4f{ 0.f, 1.f, 0.f, 1.f });
    FillRect(healthBar);
    SetColor(Color4f{ 1.f, 1.f, 1.f, 1.f });
    healthBar.width = m_MaxHealth * 10.f;
    DrawRect(healthBar, 4.f);


    Font = Texture("Bullets: " + std::to_string(m_BulletsAvailable), "DIN-Light.otf", 50, Color4f{1.f, 1.f, 1.f, 1.f});
    Font.Draw(Point2f{ m_WindowWidth - Font.GetWidth() - 1, m_WindowHeight - 51.f});
    Font = Texture("Wave: " + std::to_string(m_CurrentWave), "DIN-Light.otf", 50, Color4f{ 1.f, 1.f, 1.f, 1.f });
    Font.Draw(Point2f{ 0, 1 });
}

void Player::CalculateAngle()
{
    float dx = m_MousePos.x - m_Pos.x;
    float dy = m_MousePos.y - m_Pos.y;

    float angleRad{ std::atan2(dy, dx) };

    if (!m_LockedIn) m_Angle = angleRad * (180.f / M_PI);
}
void Player::Update(float elapsedSec)
{
    CalculateAngle();
    m_DirectionVector = Vector2f{};
    if (m_Left)  m_DirectionVector.x = -1;
    if (m_Right) m_DirectionVector.x =  1;
    if (m_Up)    m_DirectionVector.y =  1;
    if (m_Down)  m_DirectionVector.y = -1;

    if ((m_Left || m_Right) && (m_Up || m_Down))
    {
        m_DirectionVector.x /= 1.5f;
        m_DirectionVector.y /= 1.5f;
    }

    if (m_Pos.x - m_Radius > 0.f && m_Pos.x + m_Radius < m_WindowWidth) m_Pos.x += m_DirectionVector.x * m_Speed * elapsedSec;
    else
    {
        if (m_DirectionVector.x > 0.f && m_Pos.x - m_Radius <= 0.f)            m_Pos.x += m_DirectionVector.x * m_Speed * elapsedSec;
        if (m_DirectionVector.x < 0.f && m_Pos.x + m_Radius >= m_WindowHeight) m_Pos.x += m_DirectionVector.x * m_Speed * elapsedSec;
    }
    if (m_Pos.y - m_Radius > 0.f && m_Pos.y + m_Radius < m_WindowHeight) m_Pos.y += m_DirectionVector.y * m_Speed * elapsedSec;
    else
    {
        if (m_DirectionVector.y > 0.f && m_Pos.y - m_Radius <= 0.f)            m_Pos.y += m_DirectionVector.y * m_Speed * elapsedSec;
        if (m_DirectionVector.y < 0.f && m_Pos.y + m_Radius >= m_WindowHeight) m_Pos.y += m_DirectionVector.y * m_Speed * elapsedSec;
    }

    if (m_Growing) 
    {
        m_Tongue.height += 500.f * elapsedSec;
        if (m_Tongue.height >= 250.f)
        {
          m_Growing = false;
          m_Shrinking = true;
        }
    }
    if (m_Shrinking)
    {
        m_Tongue.height -= 300 * elapsedSec;
        if (m_Tongue.height <= 35.f)
        {
          m_IsClicked = false;
          m_LockedIn = false;

            m_Tongue.height = 35.f;
        }
    }

    if (m_Invulnarable)
    {
        m_InvulnarableTimer += elapsedSec * 10.f;
        if (m_InvulnarableTimer >= 30.f)
        {
            m_InvulnarableTimer = 0.f;
            m_Invulnarable = false;
        }
    }

    m_GetDamagedTimer += elapsedSec * 10.f;
    if (m_GetDamagedTimer >= 50.f)
    {
        m_GetDamagedTimer = 0.f;
        m_Health -= m_HealthDepletionAmount;
    }
    BulletUpdate(elapsedSec);
}

void Player::MouseMovement(const SDL_MouseMotionEvent& e)
{
    if (!m_LockedIn) 
    {
        m_MousePos.x = float(e.x);
        m_MousePos.y = float(e.y);
    }
}

void Player::KeyDown(const SDL_KeyboardEvent& e)
{
    switch (e.keysym.sym)
    {
    case SDLK_a:
        m_Left = true;
        break;
    case SDLK_d:
        m_Right = true;
        break;
    case SDLK_w:
        m_Up = true;
        break;
    case SDLK_s:
        m_Down = true;
        break;
    case SDLK_e:
        if (!m_IsClicked)
        {
            m_Growing = true;
            m_IsClicked = true;
            m_LockedIn = true;
        }
    }
}

void Player::KeyUp(const SDL_KeyboardEvent& e)
{
    switch (e.keysym.sym)
    {
    case SDLK_a:
        m_Left = false;
        break;
    case SDLK_d:
        m_Right = false;
        break;
    case SDLK_w:
        m_Up = false;
        break;
    case SDLK_s:
        m_Down = false;
        break;
    }
}

void Player::MouseDownEvent(const SDL_MouseButtonEvent& e)
{
    if (e.button == SDL_BUTTON_LEFT && !m_LockedIn &&(m_BulletsAvailable > 0))
    {
        float xPos{ m_MousePos.x - m_Pos.x };
        float yPos{ m_MousePos.y - m_Pos.y };

        //Calculate length of location vector
        float mag{ CalcMagnitude(m_MousePos) };

        //Calculate normalised vector
        float xDir{ xPos / mag };
        float yDir{ yPos / mag };

        m_Bullets.push_back(new Bullet(m_Pos, Vector2f{ xDir , yDir }));

        --m_BulletsAvailable;
    }
}

void Player::DeleteBullet(int index)
{
    delete m_Bullets[index];
    m_Bullets[index] = nullptr;
    m_Bullets.erase(std::remove_if(m_Bullets.begin(), m_Bullets.end(), [](Bullet* enemy) { return enemy == nullptr; }), m_Bullets.end());

    m_BulletHitboxes[index] = Circlef{};
    m_BulletHitboxes.erase(m_BulletHitboxes.begin() + index);
}

Rectf Player::GetTongueRectf()
{
    return Rectf();
}

Circlef Player::GetHitbox()
{
    return Circlef{m_Pos, m_Radius};
}

bool Player::GetTongueOut()
{
    return m_LockedIn;
}

float Player::GetTongueVelocity()
{
    return 500.f;
}

Vector2f Player::GetPlayerVelocity()
{
    if (m_Left || m_Right || m_Down || m_Up) return Vector2f{ m_Speed * m_DirectionVector.x, m_Speed * m_DirectionVector.y };
    else return Vector2f{};
}

void Player::IncrementCurrentWave()
{
    ++m_CurrentWave;
}

void Player::AddDamageOverTime()
{
    ++m_HealthDepletionAmount;
}

void Player::CollisionFunctionality(Circlef enemyHitbox, float damage)
{
    if (IsOverlapping(enemyHitbox, GetHitbox()) && !m_Invulnarable)
    {
        m_Health -= damage;
        m_Invulnarable = true;
    }
}

std::vector<Circlef> Player::GetBulletHitboxes()
{
    if (m_Bullets.size() > 0) return m_BulletHitboxes;
    else return std::vector<Circlef>{};
}

std::vector<Point2f> Player::GetTonguePoints()
{
    //Calculate location vector
    float xPos{m_MousePos.x - m_Pos.x};
    float yPos{m_MousePos.y - m_Pos.y};

    //Calculate length of location vector
    float mag{ CalcMagnitude(m_MousePos) };

    //Calculate normalised vector
    float xDir{xPos / mag};
    float yDir{yPos / mag};

    //Calculate x coordinate by multiplying length of tongue by the direction of the tongue and adding it to the xPos of the player
    for (int i{}; i < m_Tongue.width; ++i)
    {
        if (i >= m_Tongue.width / 2) m_TonguePoints[i] = Point2f{ (m_Pos.x - i) + xDir * m_Tongue.height, m_Pos.y + yDir * m_Tongue.height };
        else m_TonguePoints[i] = Point2f{ (m_Pos.x + i) + xDir * m_Tongue.height, m_Pos.y + yDir * m_Tongue.height };
    }

    return m_TonguePoints;
}

bool Player::CheckTongueHitFinished(bool tongueHit, float eatHealth)
{
    if (tongueHit && m_LockedIn)
    {
        m_Shrinking = true;
        m_Growing   = false;
    }
    
    if (tongueHit&& m_Tongue.height < 40.f)
    {
        m_Health += eatHealth;
        ++m_BulletsAvailable;
        if (m_Health > m_MaxHealth) m_Health = m_MaxHealth;
        return true;
    }
    return false;
}

bool Player::GetDead()
{
    return m_Health <= 0.f;
}

float Player::CalcMagnitude(const Point2f& otherPoint)
{
    float xPos{ otherPoint.x - m_Pos.x };
    float yPos = { otherPoint.y - m_Pos.y };

    return float(sqrt((xPos * xPos) + (yPos * yPos)));
}

void Player::BulletUpdate(float elapsedSec)
{
    m_BulletHitboxes.resize(m_Bullets.size());
    for (int currentBullet{}; currentBullet < m_Bullets.size(); ++currentBullet)
    {
        m_Bullets[currentBullet]->Update(elapsedSec);

        if (m_BulletHitboxes.size() == 0) m_BulletHitboxes.push_back(m_Bullets[currentBullet]->GetHitbox());
        else m_BulletHitboxes[currentBullet] = m_Bullets[currentBullet]->GetHitbox();

        if (m_Bullets[currentBullet]->Expired()) DeleteBullet(currentBullet);
    }
}
