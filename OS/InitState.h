#pragma once
#include "GUIState.h"
#include "Button.h"
#include "Timebar.h"
#include "MainMenuState.h"

extern bool isMouseP1ressed;

class InitState :
	public GUIState
{
private:
	//variables
	TimeBar timebar = TimeBar(1000.f, 50.f, &font, "");
	sf::Texture texture;
	sf::Font font;
	sf::RectangleShape background;
	std::map<std::string, Button*> buttons;

	//Functions
	void initBackground();
	void initFonts();
	void initButtons();

public:
	InitState(sf::RenderWindow *window, std::stack<GUIState*> *states, sf::Event *event);
	virtual ~InitState();

	//Functions
	void updateKeybinds(const float& dt);
	void update(const float& dt);
	void updateButtons();
	void updateTimebar();
	void renderButtons(sf::RenderTarget* target = nullptr);
	void render(sf::RenderTarget* target = nullptr);
};

