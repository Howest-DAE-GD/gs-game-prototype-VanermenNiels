#include "pch.h"
#include "Game.h"
#include "Player.h"
#include "Enemy.h"

Game::Game( const Window& window ) 
	:BaseGame{ window },
	 m_Player{},
	 m_Test{},
	m_Enemies{}
{
	Initialize();
}

Game::~Game( )
{
	Cleanup( );
}

void Game::Initialize( )
{
	m_Player = new Player(GetViewPort().width, GetViewPort().height);
	InitializeEnemies();
}

void Game::Cleanup( )
{
	delete m_Player;
	m_Player = nullptr;
	delete m_Font;
	for (int i{}; i < m_Enemies.size(); ++i)
	{
		delete m_Enemies[i];
		m_Enemies[i] = nullptr;
	}
}

void Game::Update(float elapsedSec)
{
	if (!m_Player->GetDead())
	{
		m_Player->Update(elapsedSec);
		for (int currentEnemy{}; currentEnemy < m_Enemies.size(); ++currentEnemy)
		{
			if (m_Enemies[currentEnemy] != nullptr)
			{
				for (int currentBullet{}; currentBullet < m_Player->GetBulletHitboxes().size(); ++currentBullet)
				{
					if (m_Enemies[currentEnemy]->CheckBulletHit(m_Player->GetBulletHitboxes()[currentBullet], 5.f)) m_Player->DeleteBullet(currentBullet);
				}

				m_Enemies[currentEnemy]->Update(elapsedSec, m_Player->GetHitbox().center);
				m_Player->CollisionFunctionality(m_Enemies[currentEnemy]->GetHitbox(), m_Enemies[currentEnemy]->GetDamage());

				if (m_Player->CheckTongueHitFinished(m_Enemies[currentEnemy]->TongueHitFunctionality(m_Player->GetTonguePoints(), m_Player->GetTongueOut(), Vector2f{ m_Player->GetTongueVelocity(), m_Player->GetTongueVelocity() },
					m_Player->GetPlayerVelocity(), elapsedSec), m_Enemies[currentEnemy]->GetEatReturn()) || m_Enemies[currentEnemy]->GetDead())
				{
					delete m_Enemies[currentEnemy];
					m_Enemies[currentEnemy] = nullptr;
				}
			}
		}
		bool spawnReady{ true };
		for (Enemy* currentEnemy : m_Enemies)
		{
			if (currentEnemy != nullptr) spawnReady = false;
		}
		if (spawnReady) InitializeEnemies();
	}
}

void Game::Draw( ) const
{
	ClearBackground();

	m_Player->Draw();

	for (int currentEnemy{}; currentEnemy < m_Enemies.size(); ++currentEnemy)
	{
		if (m_Enemies[currentEnemy] != nullptr)
		{
			m_Enemies[currentEnemy]->Draw();
		}
	}

	if (m_Player->GetDead()) m_Font->Draw(Point2f{ GetViewPort().width / 2 - m_Font->GetWidth() / 2, GetViewPort().height / 2 - m_Font->GetHeight() / 2 });
}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
	m_Player->KeyDown(e);

	if (m_Player->GetDead() && e.keysym.sym == SDLK_r) Reset();
}

void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{
	m_Player->KeyUp(e);
}

void Game::ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e )
{
	m_Player->MouseMovement(e);
}

void Game::ProcessMouseDownEvent( const SDL_MouseButtonEvent& e )
{
	m_Player->MouseDownEvent(e);
	
}

void Game::ProcessMouseUpEvent( const SDL_MouseButtonEvent& e )
{
	//std::cout << "MOUSEBUTTONUP event: ";
	//switch ( e.button )
	//{
	//case SDL_BUTTON_LEFT:
	//	std::cout << " left button " << std::endl;
	//	break;
	//case SDL_BUTTON_RIGHT:
	//	std::cout << " right button " << std::endl;
	//	break;
	//case SDL_BUTTON_MIDDLE:
	//	std::cout << " middle button " << std::endl;
	//	break;
	//}
}

void Game::ClearBackground( ) const
{
	glClearColor( 0.0f, 0.0f, 0.3f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}

void Game::InitializeEnemies()
{
	Point2f pos{};
	int randomNumber{};
	for (int currentEnemy{}; currentEnemy < m_AmountOfEnemies; ++currentEnemy)
	{
		randomNumber = (rand() % 4) + 1;
		switch (randomNumber)
		{
		case 1:
			pos.x = float(-(rand() % 51));
			pos.y = float(rand() / GetViewPort().height);
			break;
		case 2:
			pos.x = float(rand() % 51 + GetViewPort().width);
			pos.y = float(rand() % int(GetViewPort().height));
			break;
		case 3:
			pos.x = float(rand() % int(GetViewPort().width));
			pos.y = float(-(rand() % 51));
			break;
		case 4:
			pos.x = float(rand() % int(GetViewPort().width));
			pos.y = float(rand() % 51 + int(GetViewPort().height));
			break;
		}
		m_Enemies.push_back(new Enemy(pos));
	}

	m_Player->IncrementCurrentWave();
	m_Player->AddDamageOverTime();
	m_AmountOfEnemies += 5.f;
}

void Game::Reset()
{
	delete m_Player;
	m_Player = nullptr;

	for (int i{}; i < m_Enemies.size(); ++i)
	{
		delete m_Enemies[i];
		m_Enemies[i] = nullptr;
	}

	m_Player = new Player(GetViewPort().width, GetViewPort().height);
	InitializeEnemies();
}
