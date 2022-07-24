#include "GSPlay.h"

#include "Shader.h"
#include "Texture.h"
#include "Model.h"
#include "Camera.h"
#include "Font.h"
#include "Sprite2D.h"
#include "Sprite3D.h"
#include "Text.h"
#include "GameButton.h"
#include "SpriteAnimation.h"
#include "CMath.h"
#include "../Utils.h"

GSPlay::GSPlay()
{
}


GSPlay::~GSPlay()
{
}


void GSPlay::Init()
{
	auto model = ResourceManagers::GetInstance()->GetModel("Sprite2D.nfg");
	auto texture = ResourceManagers::GetInstance()->GetTexture("backgroundRepeat.tga");

	// background
	auto shader = ResourceManagers::GetInstance()->GetShader("TextureShader");
	m_background = std::make_shared<Sprite2D>(model, shader, texture);
	m_backgroundPosition = Vector2(Globals::screenWidth/2, Globals::screenHeight/2);
	m_background->Set2DPosition(m_backgroundPosition.x, m_backgroundPosition.y);
	m_background->SetRotation(Vector3(M_PI, 0, 0));
	m_background->SetSize(Globals::screenWidth, Globals::screenHeight);
	//secondary background
	m_secondaryBackground = std::make_shared<Sprite2D>(model, shader, texture);
	m_secondaryBackground->Set2DPosition(m_backgroundPosition.x + Globals::screenWidth, m_backgroundPosition.y);
	m_secondaryBackground->SetRotation(Vector3(M_PI, 0, 0));
	m_secondaryBackground->SetSize(Globals::screenWidth, Globals::screenHeight);

	// button close
	texture = ResourceManagers::GetInstance()->GetTexture("btn_close.tga");
	std::shared_ptr<GameButton>  button = std::make_shared<GameButton>(model, shader, texture);
	button->Set2DPosition(Globals::screenWidth - 50, 50);
	button->SetSize(50, 50);
	button->SetOnClick([this]() {
			GameStateMachine::GetInstance()->PopState();
		});
	m_listButton.push_back(button);

	// score
	shader = ResourceManagers::GetInstance()->GetShader("TextShader");
	std::shared_ptr<Font> font = ResourceManagers::GetInstance()->GetFont("Brightly Crush Shine.otf");
	m_score = std::make_shared< Text>(shader, font, "score: 10", TextColor::RED, 1.0);
	m_score->Set2DPosition(Vector2(5, 25));

	shader = ResourceManagers::GetInstance()->GetShader("Animation");
	texture = ResourceManagers::GetInstance()->GetTexture("Actor1_2.tga");
	std::shared_ptr<SpriteAnimation> playerIdleAnimation = std::make_shared<SpriteAnimation>(model, shader, texture, 9, 6, 3, 0.1f);
	
	m_player2DPosition = Vector2(100, Globals::screenHeight - 135);
	playerIdleAnimation->Set2DPosition(m_player2DPosition);
	playerIdleAnimation->SetSize(30, 40);
	playerIdleAnimation->SetRotation(Vector3(0, M_PI, 0));
	m_playerCharacter = playerIdleAnimation;
	m_listAnimation.push_back(playerIdleAnimation);
	m_playerState = IDLE;
	m_KeyPress = 0;
	srand(time(NULL));
}

void GSPlay::Exit()
{
}

void GSPlay::Pause()
{
}

void GSPlay::Resume()
{
}


void GSPlay::HandleEvents()
{
}

void GSPlay::HandleKeyEvents(int key, bool bIsPressed)
{
	if (bIsPressed)
	{
		switch (key)
		{
		case KEY_MOVE_LEFT:
			m_KeyPress |= 1;
			break;
		case KEY_MOVE_BACKWORD:
			m_KeyPress |= 1<<1;
			break;
		case KEY_MOVE_RIGHT:
			m_KeyPress |= 1<<2;
			break;
		case KEY_MOVE_FORWORD:
			m_KeyPress |= 1<<3;
			break;
		case KEY_SPACE:
			m_KeyPress |= 1 << 4;
		default:
			break;
		}
	}
	else
	{
		switch (key)
		{
		case KEY_MOVE_LEFT:
			m_KeyPress ^= 1;
			break;
		case KEY_MOVE_BACKWORD:
			m_KeyPress ^= 1 << 1;
			break;
		case KEY_MOVE_RIGHT:
			m_KeyPress ^= 1 << 2;
			break;
		case KEY_MOVE_FORWORD:
			m_KeyPress ^= 1 << 3;
			break;
		case KEY_SPACE:
			m_KeyPress ^= 1 << 4;
		default:
			break;
		}
	}
}

void GSPlay::HandleTouchEvents(int x, int y, bool bIsPressed)
{
	for (auto button : m_listButton)
	{
		if(button->HandleTouchEvents(x, y, bIsPressed))
		{
			break;
		}
	}
}

void GSPlay::HandleMouseMoveEvents(int x, int y)
{
}

void GSPlay::Update(float deltaTime)
{
	/*if (m_KeyPress & (1<<2)) 
	{
		m_player2DPosition += Vector2(m_playerSpeed * deltaTime, 0);
	}
	if (m_KeyPress & 1)
	{
		m_player2DPosition -= Vector2(m_playerSpeed * deltaTime, 0);
	}*/
	UpdateObstacles(deltaTime);
	UpdateCharacter(deltaTime);
	UpdateBackground(deltaTime);
	for (auto it : m_listButton)
	{
		it->Update(deltaTime);
	}
	for (auto it : m_listAnimation)
	{
		it->Update(deltaTime);
	}
}
void GSPlay::UpdateCharacter(float deltaTime)
{
	if (m_KeyPress & (1 << 4))
	{
		if (m_playerState == JUMP)
		{
			m_jumpTime += deltaTime;
			m_JumpV0 = min(m_maxJumpV0, m_minJumpV0 + m_jumpTime * (m_maxJumpV0 - m_minJumpV0));
			if (m_JumpV0 <= GRAVITY * 40 * m_jumpTime * m_jumpTime)
			{
				m_playerState = FALL;
			}
			else
			{
				GLfloat height = m_JumpV0 * m_jumpTime - GRAVITY * 40 * m_jumpTime * m_jumpTime;
				m_player2DPosition.y = Globals::screenHeight - 135 - height;
			}
		}
		else if (m_playerState == IDLE) {
			m_jumpTime = 0;
			m_playerState = JUMP;
		}
	}
	else if (m_playerState == JUMP) {
		m_playerState = FALL;
	}
	if (m_playerState == FALL)
	{
		m_jumpTime += deltaTime;
		GLfloat height = m_JumpV0 * m_jumpTime - GRAVITY * 40 * m_jumpTime * m_jumpTime;
		if (height <= 0)
		{
			height = 0;
			m_playerState = IDLE;
		}
		m_player2DPosition.y = Globals::screenHeight - 135 - height;
	}
	m_player2DPosition.x = Utils::Clamp(m_player2DPosition.x, m_playerXMinPos, m_playerXMaxPos);
	m_playerCharacter->Set2DPosition(m_player2DPosition);
}

void GSPlay::UpdateBackground(float deltaTime)
{
	const float BACKGROUND_SPEED = 20;
	m_backgroundPosition.x -= BACKGROUND_SPEED * deltaTime;
	if (m_backgroundPosition.x <= (-0.5f*Globals::screenWidth))
	{
		m_backgroundPosition.x += Globals::screenWidth;
	}
	m_background->Set2DPosition(m_backgroundPosition.x, m_backgroundPosition.y);
	m_secondaryBackground->Set2DPosition(m_backgroundPosition.x + Globals::screenWidth, m_backgroundPosition.y);
}

void GSPlay::UpdateObstacles(float deltaTime)
{
	static float spawnTime = 0.0f;
	spawnTime -= deltaTime;
	for (auto it : m_listObstacle)
	{
		auto current2DPosition = Vector2(it->GetPosition().x, it->GetPosition().y);
		current2DPosition.x -= deltaTime * 100;
		it->Set2DPosition(current2DPosition.x, current2DPosition.y);
	}
	if (!m_listObstacle.empty() && m_listObstacle.front()->GetPosition().x < -50)
	{
		m_listObstacle.pop_front();
	}
	if (spawnTime <= 0)
	{
		auto model = ResourceManagers::GetInstance()->GetModel("Sprite2D.nfg");
		auto texture = ResourceManagers::GetInstance()->GetTexture("catus.tga");

		// background
		auto shader = ResourceManagers::GetInstance()->GetShader("TextureShader");
		auto catus = std::make_shared<Sprite2D>(model, shader, texture);
		auto height = Utils::RandomInRange(40, 70);
		catus->SetSize(40, height);
		catus->Set2DPosition(Globals::screenWidth+50, Globals::screenHeight - 115 - height/2);
		catus->SetRotation(Vector3(M_PI, 0, 0));
		m_listObstacle.push_back(catus);
		spawnTime = Utils::RandomInRange(0.8f, 2.2f);
	}
}

void GSPlay::Draw()
{
	m_background->Draw();
	m_secondaryBackground->Draw();
	m_score->Draw();
	for (auto it : m_listButton)
	{
		it->Draw();
	}

	for (auto it : m_listObstacle)
	{
		it->Draw();
	}

	for (auto it : m_listAnimation)
	{
		it->Draw();
	}
}