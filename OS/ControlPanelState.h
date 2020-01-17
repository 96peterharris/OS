#pragma once
#include "GUIState.h"
#include "Button.h"
#include "Timebar.h"
#include "VMState.h"
#include "RAMState.h"
#include "DiscState.h"
#include "HelpState.h"
#include "CommunicationState.h"
#include "ProcessesManagementState.h"
#include "ProcessSchedulingState.h"

extern bool isMousePressed;

class ControlPanelState :
	public GUIState
{
private:
	//variables
	TimeBar timebar = TimeBar(1000.f, 50.f, &font, "Control panel");
	sf::Texture texture;
	sf::Font font;
	sf::RectangleShape background;
	std::map<std::string, Button*> buttons;
	sf::RectangleShape menu1;
	sf::RectangleShape menu2;
	sf::Texture menutexture;

	//Functions
	void initBackground();
	void initFonts();
	void initButtons();

public:
	ControlPanelState(sf::RenderWindow *window, std::stack<GUIState*> *states, sf::Event *event);
	virtual ~ControlPanelState();

	//Functions
	void updateKeybinds(const float& dt);
	void update(const float& dt);
	void updateButtons();
	void updateTimebar();
	void renderButtons(sf::RenderTarget* target = nullptr);
	void render(sf::RenderTarget* target = nullptr);
};
