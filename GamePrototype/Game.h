#pragma once
#include "BaseGame.h"
#include <iostream>
#include <vector>
#include "Texture.h"
class Player;
class Enemy;

class Game : public BaseGame
{
public:
	explicit Game( const Window& window );
	Game(const Game& other) = delete;
	Game& operator=(const Game& other) = delete;
	Game( Game&& other) = delete;
	Game& operator=(Game&& other) = delete;
	// http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rh-override
	~Game();

	void Update( float elapsedSec ) override;
	void Draw( ) const override;

	// Event handling
	void ProcessKeyDownEvent( const SDL_KeyboardEvent& e ) override;
	void ProcessKeyUpEvent( const SDL_KeyboardEvent& e ) override;
	void ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e ) override;
	void ProcessMouseDownEvent( const SDL_MouseButtonEvent& e ) override;
	void ProcessMouseUpEvent( const SDL_MouseButtonEvent& e ) override;

private:

	// FUNCTIONS
	void Initialize();
	void Cleanup( );
	void ClearBackground( ) const;

	void InitializeEnemies();
	void Reset();

	Player* m_Player;
	std::vector<Enemy*> m_Enemies;
	Enemy* m_Test;

	int m_AmountOfEnemies{5};

	Texture* m_Font{ new Texture("Game Over! (Press 'R' to restart)", "DIN-Light.otf", 100, Color4f{1.f, 1.f, 1.f, 1.f}) };
};