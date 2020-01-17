#pragma once
#include "GUIState.h"
#include "Button.h"
#include "Timebar.h"
#include "ProgramsState.h"
#include "FileManagerState.h"
#include "ControlPanelState.h"

extern bool isMousePressed;

class MainMenuState :
	public GUIState
{
private:
	//variables
	TimeBar timebar = TimeBar(1000.f, 50.f, &font, "Main menu");
	sf::Texture texture;
	sf::Font font;
	sf::RectangleShape background;
	std::map<std::string, Button*> buttons;
	sf::RectangleShape menu1;
	sf::Texture menutexture;

	//Functions
	void initBackground();
	void initFonts();
	void initButtons();

public:
	MainMenuState(sf::RenderWindow *window, std::stack<GUIState*> *states, sf::Event *event);
	virtual ~MainMenuState();

	//Functions
	void updateKeybinds(const float& dt);
	void update(const float& dt);
	void updateButtons();
	void updateTimebar();
	void renderButtons(sf::RenderTarget* target = nullptr);
	void render(sf::RenderTarget* target = nullptr);
};

