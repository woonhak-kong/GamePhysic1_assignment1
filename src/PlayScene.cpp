#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"
#include <sstream>
#include <iomanip>

// required for IMGUI
#include "imgui.h"
#include "imgui_sdl.h"
#include "Renderer.h"
#include "Util.h"

PlayScene::PlayScene() :
	m_startingX(50.f),
	m_startingY(600.f),
	m_launchElevationAngle(45.f),
	m_launchSpeed(100.f),
	m_accelerationGravity({ 0.f, 9.8f }),
	m_orientation({ 0.5,0.5 }),
	m_isStart(false),
	m_playTime(0)

{
	PlayScene::start();

	m_pBall->getTransform()->position.x = m_startingX;
	m_pBall->getTransform()->position.y = m_startingY;
	m_velocity.x = m_orientation.x * m_launchSpeed;
	m_velocity.y = m_orientation.y * m_launchSpeed;
}

PlayScene::~PlayScene()
= default;

void PlayScene::draw()
{
	TextureManager::Instance().drawBySize("bg", 0, 0, 1280, 720);
	TextureManager::Instance().drawBySize("bg2", 0, 600, 1280, 120);


	SDL_SetRenderDrawColor(Renderer::Instance().getRenderer(), 0, 255, 0, 255);
	float lineLength = 500;
	SDL_RenderDrawLineF(Renderer::Instance().getRenderer(), m_startingX, m_startingY, m_startingX + m_orientation.x * lineLength, m_startingY + m_orientation.y * lineLength);


	SDL_SetRenderDrawColor(Renderer::Instance().getRenderer(), 0, 0, 255, 255);
	SDL_RenderDrawLineF(Renderer::Instance().getRenderer(), m_startingX - 1500, m_startingY, m_startingX + 1500, m_startingY);



	SDL_SetRenderDrawColor(Renderer::Instance().getRenderer(), 255, 0, 0, 255);
	float beforeX = m_startingX;
	float beforeY = m_startingY;
	float x = 0;
	float y = 0;


	// drawing trajectory
	for(float t = 0.2; t < 40; t = t + 0.2)
	{
		x = m_startingX + m_velocity.x * t + (0.5f * m_accelerationGravity.x * (t * t));
		y = m_startingY + m_velocity.y * t + (0.5f * m_accelerationGravity.y * (t * t));

		SDL_RenderDrawLineF(Renderer::Instance().getRenderer(), beforeX, beforeY, x, y);

		beforeX = x;
		beforeY = y;
	}



	// V(f) = V(i) + at
	// a is always 0;
	// so, 0 + bT + cT^2 = 0;
	// T(b+cT) = 0;
	// T = 0 or -(b/c)
	float a = 0;
	float b = m_velocity.y;
	float c = 0.5f * m_accelerationGravity.y;

	float time1 = 0;
	float time2 = -1*(b/c);


	SDL_FRect point1 = { (m_startingX + m_velocity.x * time1 + (0.5f * m_accelerationGravity.x * (time1 * time1))) - 10,m_startingY - 10,20.f,20.f };
	SDL_FRect point2 = { (m_startingX + m_velocity.x * time2 + (0.5f * m_accelerationGravity.x * (time2 * time2))) - 10,m_startingY - 10,20.f,20.f };
	SDL_SetRenderDrawColor(Renderer::Instance().getRenderer(), 0, 255, 255, 255);
	SDL_RenderFillRectF(Renderer::Instance().getRenderer(), &point1);
	SDL_RenderFillRectF(Renderer::Instance().getRenderer(), &point2);


	drawDisplayList();
	SDL_SetRenderDrawColor(Renderer::Instance().getRenderer(), 0, 0, 0, 255);



}

void PlayScene::update()
{

	if(m_isStart)
	{
		float t = Game::Instance().getDeltaTime()*10;
		m_playTime += t;
		//std::cout << t << std::endl;


		// kinemetic equation
		// Displacement = D(i) + vt + 1/2at^2
		m_pBall->getTransform()->position.x = m_startingX + m_velocity.x * m_playTime
			+ (0.5f * m_accelerationGravity.x * (m_playTime * m_playTime));

		m_pBall->getTransform()->position.y = m_startingY + m_velocity.y * m_playTime
		+ ( 0.5f * m_accelerationGravity.y *(m_playTime * m_playTime)) ;


	}
	std::stringstream string;
	string << std::fixed<< std::setprecision(2) << "Posision : (" << m_pBall->getTransform()->position.x << ", "
		<< m_pBall->getTransform()->position.y << ")";
	m_positionLabel->setText(string.str());

	string.str("");
	string << std::fixed << std::setprecision(2) << "Luanch Speed : " << m_launchSpeed ;
	m_speedLabel->setText(string.str());

	// calculating velocity
	float velocityX = m_isStart ? m_velocity.x : 0;
	float velocityY = m_isStart ? m_velocity.y + m_accelerationGravity.y * m_playTime : 0;
	string.str("");
	string << std::fixed << std::setprecision(2) << "Velocity : (" << velocityX << ", " << velocityY << ")";
	m_velocityLabel->setText(string.str());


	string.str("");
	string << std::fixed << std::setprecision(2) << "Acceleration : " << "(" << m_accelerationGravity.x << ", " << m_accelerationGravity.y << ")";
	m_accelerationLabel->setText(string.str());

	string.str("");
	string << std::fixed << std::setprecision(2) << "Time : " << m_playTime;
	m_timeLabel->setText(string.str());

	string.str("");
	string << std::fixed << std::setprecision(2) << "Angle : " << m_launchElevationAngle;
	m_angleLabel->setText(string.str());


	updateDisplayList();
}

void PlayScene::clean()
{
	removeAllChildren();
}

void PlayScene::handleEvents()
{
	EventManager::Instance().update();



	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		TheGame::Instance().quit();
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_1))
	{
		TheGame::Instance().changeSceneState(START_SCENE);
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_2))
	{
		TheGame::Instance().changeSceneState(END_SCENE);
	}

	//if (EventManager::Instance().isKeyDown(SDL_SCANCODE_SPACE))
	//{
	//	reset();
	//	m_isStart = true;
	//}
}

void PlayScene::start()
{
	// Set GUI Title
	m_guiTitle = "Play Scene";

	TextureManager::Instance().load("../Assets/textures/bg1.png", "bg");
	TextureManager::Instance().load("../Assets/textures/bg2.png", "bg2");

	const SDL_Color blue = { 0, 255, 255, 255 };
	float yCoor = 30.f;
	m_positionLabel = new Label("", "Consolas", 20, blue, glm::vec2(900.0f, yCoor += 20));
	m_positionLabel->setParent(this);
	addChild(m_positionLabel);
	m_speedLabel = new Label("", "Consolas", 20, blue, glm::vec2(900.0f, yCoor += 20));
	m_speedLabel->setParent(this);
	addChild(m_speedLabel);
	m_velocityLabel = new Label("", "Consolas", 20, blue, glm::vec2(900.0f, yCoor += 20));
	m_velocityLabel->setParent(this);
	addChild(m_velocityLabel);
	m_accelerationLabel = new Label("", "Consolas", 20, blue, glm::vec2(900.0f, yCoor += 20));
	m_accelerationLabel->setParent(this);
	addChild(m_accelerationLabel);
	m_timeLabel = new Label("", "Consolas", 20, blue, glm::vec2(900.0f, yCoor += 20));
	m_timeLabel->setParent(this);
	addChild(m_timeLabel);
	m_angleLabel = new Label("", "Consolas", 20, blue, glm::vec2(900.0f, yCoor += 20));
	m_angleLabel->setParent(this);
	addChild(m_angleLabel);




	// Plane Sprite
	m_pBall= new Ball();
	addChild(m_pBall);



	m_pBall->getTransform()->position.x = m_startingX;
	m_pBall->getTransform()->position.y = m_startingY;
	m_orientation.x = glm::cos(m_launchElevationAngle * Util::Deg2Rad);
	m_orientation.y = -glm::sin(m_launchElevationAngle * Util::Deg2Rad);
	m_velocity.x = m_orientation.x * m_launchSpeed;
	m_velocity.y = m_orientation.y * m_launchSpeed;
	ImGuiWindowFrame::Instance().setGUIFunction(std::bind(&PlayScene::GUI_Function, this));
}

void PlayScene::GUI_Function()
{
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();

	ImGui::Begin("Setting values", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoMove);


	if (ImGui::Button("Activate"))
	{
		reset();
		m_isStart = true;
	}
	//if(ImGui::SliderFloat("Starting X", &m_startingX, 0.f, 1280.f))
	//{
	//	//m_startingX = startingX;
	//	m_pBall->getTransform()->position.x = m_startingX;
	//}
	if(ImGui::SliderFloat("GroundPosition", &m_startingX, 0.f, 1280.f))
	{
		//m_startingY = startingY;
		m_pBall->getTransform()->position.x = m_startingX;
	}
	if(ImGui::SliderFloat("Launch Speed", &m_launchSpeed, 10.f, 200.f))
	{
		//m_launchSpeed = launchSpeed;
		m_velocity.x = m_orientation.x * m_launchSpeed;
		m_velocity.y = m_orientation.y * m_launchSpeed;
	}
	if(ImGui::SliderFloat("Angle Y", &m_launchElevationAngle, -90.f, 90.f))
	{
		// for calculating launch velocity
		m_orientation.x = glm::cos(m_launchElevationAngle * Util::Deg2Rad);
		m_orientation.y = -glm::sin(m_launchElevationAngle * Util::Deg2Rad);
		m_velocity.x = m_orientation.x * m_launchSpeed;
		m_velocity.y = m_orientation.y * m_launchSpeed;
		std::cout << m_orientation.x << ", " << m_orientation.y << std::endl;
	}
	if(ImGui::SliderFloat("Gravity", &m_accelerationGravity.y, 1.f, 20.f))
	{
		//m_accelerationGravity.y = accelationGravity;
		std::cout << "acceleration - (" << m_accelerationGravity.x << ", " << m_accelerationGravity.y << ")" << std::endl;
	}

	if(ImGui::Button("Reset"))
	{
		reset();
	}


	//static float float3[3] = { 0.0f, 1.0f, 1.5f };
	//if(ImGui::SliderFloat3("My Slider", float3, 0.0f, 2.0f))
	//{
	//	std::cout << float3[0] << std::endl;
	//	std::cout << float3[1] << std::endl;
	//	std::cout << float3[2] << std::endl;
	//	std::cout << "---------------------------\n";
	//}

	ImGui::End();
}

void PlayScene::reset()
{
	m_isStart = false;
	m_playTime = 0;
	m_pBall->getTransform()->position.x = m_startingX;
	m_pBall->getTransform()->position.y = m_startingY;
}
