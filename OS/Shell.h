#pragma once
#include"LoadingState.h"

extern bool isMousePressed;

class Shell
{
private:
	//Variables
	sf::RenderWindow *window;
	sf::Event sfevent;
	sf::Image image;

	sf::Clock dtClock;
	float dt;

	std::stack<GUIState*> states;

	//Initialization
	void initWindow();
	void initStates();

public:
	Shell();
	virtual ~Shell();

	//Functions
	void updateDt();
	void updateSFMLEvents();
	void update();

	//render
	void render();

	//core
	void run();	
};

