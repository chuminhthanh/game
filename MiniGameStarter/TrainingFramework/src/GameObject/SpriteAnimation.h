#pragma once
#include "BaseObject.h"
class SpriteAnimation : public BaseObject
{
protected:
	Vector2			m_Vec2DPos;
	GLint			m_iHeight;
	GLint			m_iWidth;
	GLint			m_numFrames;
	GLint			m_currentFrame;
	GLfloat			m_frameTime;
	GLfloat			m_currentTime;
	GLint			m_numActions;
	GLint			m_currentAction;//index from 0 to m_numActions-1

public:
	SpriteAnimation(std::shared_ptr<Model> model, std::shared_ptr<Shader> shader, std::shared_ptr<Texture> texture, GLint numFrames, GLint numActions, GLint currentAction, GLfloat frameTime);
	~SpriteAnimation();

	void		Init() override;
	void		Draw() final;
	void		Update(GLfloat deltatime) override;

	void		Set2DPosition(GLint x, GLint y);
	void		Set2DPosition(Vector2 v);
	void		SetRotation(Vector3 rotation);

	Vector2		Get2DPosition();
	void		SetSize(GLint width, GLint height);
};

