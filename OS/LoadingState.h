#pragma once
#include "GUIState.h"
#include "Button.h"
#include "Timebar.h"
#include "InitState.h"

extern bool isMouseP1ressed;

class LoadingState :
	public GUIState
{
private:
	//variables
	sf::Texture texture;
	sf::RectangleShape background;
	float time = 0;
	sf::SoundBuffer soundBuffer;
	sf::Sound sound;

	//Functions
	void initBackground();

public:
	LoadingState(sf::RenderWindow *window, std::stack<GUIState*> *states, sf::Event *event);
	virtual ~LoadingState();

	//Functions
	void updateKeybinds(const float& dt);
	void update(const float& dt);
	void render(sf::RenderTarget* target = nullptr);
};

