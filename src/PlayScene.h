#pragma once
#ifndef __PLAY_SCENE__
#define __PLAY_SCENE__

#include "Ball.h"
#include "Scene.h"
#include "Plane.h"
#include "Player.h"
#include "Button.h"
#include "Label.h"

class PlayScene : public Scene
{
public:
	PlayScene();
	~PlayScene();

	// Scene LifeCycle Functions
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;
	virtual void handleEvents() override;
	virtual void start() override;
private:
	// IMGUI Function
	void GUI_Function();
	std::string m_guiTitle;

	glm::vec2 m_mousePosition;

	Ball* m_pBall;


	float m_startingX;
	float m_startingY;
	float m_launchElevationAngle;
	float m_launchSpeed;
	float m_playTime;
	glm::vec2 m_accelerationGravity;
	glm::vec2 m_orientation;
	glm::vec2 m_velocity;

	bool m_isStart;


private:
	void reset();
};

#endif /* defined (__PLAY_SCENE__) */