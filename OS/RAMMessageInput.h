#pragma once
#include "GUIState.h"
#include "Button.h"
#include "Timebar.h"
#include "InputField.h"
#include "HelpState.h"

extern bool isMousePressed;


class RAMMessageInput :
	public GUIState
{
private:
	//variables
	TimeBar timebar = TimeBar(1000.f, 50.f, &font, "Type message");
	sf::Texture texture;
	sf::Font font;
	sf::RectangleShape background;
	std::map<std::string, Button*> buttons;
	InputField input = InputField(&font, event);
	sf::RectangleShape menu1;
	sf::RectangleShape menu2;
	sf::RectangleShape menu3;
	sf::Texture menutexture;

	//Functions
	void initBackground();
	void initFonts();
	void initButtons();

public:
	RAMMessageInput(sf::RenderWindow *window, std::stack<GUIState*> *states, sf::Event *event);
	virtual ~RAMMessageInput();

	//Functions
	void updateKeybinds(const float& dt);
	void update(const float& dt);
	void updateButtons();
	void updateTimebar();
	void updateInput();
	void renderButtons(sf::RenderTarget* target = nullptr);
	void render(sf::RenderTarget* target = nullptr);
};