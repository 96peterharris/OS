#pragma once
#include "GUIState.h"
#include "Button.h"
#include "Timebar.h"
#include "InputField.h"
#include "HelpState.h"

extern bool isMousePressed;


class PCBSegmentsInput :
	public GUIState
{
private:
	//variables
	TimeBar timebar = TimeBar(1000.f, 50.f, &font, "Type ");
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
	PCBSegmentsInput(sf::RenderWindow *window, std::stack<GUIState*> *states, sf::Event *event);
	virtual ~PCBSegmentsInput();

	//Functions
	void updateKeybinds(const float& dt);
	void update(const float& dt);
	void updateButtons();
	void updateTimebar();
	void updateInput();
	void renderButtons(sf::RenderTarget* target = nullptr);
	void render(sf::RenderTarget* target = nullptr);
};