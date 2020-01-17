#pragma once
#include "GUIState.h"
#include "Button.h"
#include "Timebar.h"
#include "HelpState.h"
#include "PCBInput.h"
#include "PCBSemInput.h"

extern bool isMousePressed;


class ProcessesManagementState :
	public GUIState
{
private:
	//variables
	TimeBar timebar = TimeBar(1000.f, 50.f, &font, "Processes Management");
	sf::Texture texture;
	sf::Font font;
	sf::RectangleShape background;
	std::map<std::string, Button*> buttons;
	sf::RectangleShape menu1;
	sf::RectangleShape menu2;
	sf::RectangleShape menu3;
	sf::Texture menutexture;

	//Functions
	void initBackground();
	void initFonts();
	void initButtons();

public:
	ProcessesManagementState(sf::RenderWindow *window, std::stack<GUIState*> *states, sf::Event *event);
	virtual ~ProcessesManagementState();

	//Functions
	void updateKeybinds(const float& dt);
	void update(const float& dt);
	void updateButtons();
	void updateTimebar();
	void renderButtons(sf::RenderTarget* target = nullptr);
	void render(sf::RenderTarget* target = nullptr);
};