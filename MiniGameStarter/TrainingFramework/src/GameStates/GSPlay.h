#pragma once
#include "GameStateBase.h"

class Sprite2D;
class Sprite3D;
class Text;
class GameButton;
class SpriteAnimation;

class GSPlay :
	public GameStateBase
{
public:
	GSPlay();
	~GSPlay();

	void	Init() override;
	void	Exit() override;

	void	Pause() override;
	void	Resume() override;

	void	HandleEvents() override;
	void	HandleKeyEvents(int key, bool bIsPressed) override;
	void	HandleTouchEvents(int x, int y, bool bIsPressed) override;
	void	HandleMouseMoveEvents(int x, int y) override;
	void	Update(float deltaTime) override;
	void	Draw() override;
	void	UpdateObstacles(float deltaTime);
	void	UpdateBackground(float deltaTime);
	void	UpdateCharacter(float deltaTime);
	int m_KeyPress = 0;

private:
	std::shared_ptr<Sprite2D>	m_background;
	std::shared_ptr<Sprite2D>	m_secondaryBackground;
	std::list< std::shared_ptr<Sprite2D>> m_listObstacle;
	std::shared_ptr<Text>		m_score;
	std::list<std::shared_ptr<GameButton>>	m_listButton;
	std::list<std::shared_ptr<SpriteAnimation>>	m_listAnimation;

	//character
	enum PlayerState {
		IDLE = 0,
		JUMP = 1,
		FALL = 2,
		DEATH = 3
	};
	std::shared_ptr<SpriteAnimation> m_playerCharacter;
	Vector2 m_backgroundPosition;
	float m_playerSpeed = 100;
	float m_JumpV0;
	float m_minJumpV0 = 300;
	float m_maxJumpV0 = 500;
	float m_playerXMinPos = 50;
	float m_playerXMaxPos = 200;
	float m_jumpTime = 0;
	
	PlayerState m_playerState;
	Vector2 m_player2DPosition;
};