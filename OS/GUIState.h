#pragma once

#include<vector>
#include"Headers.h"
#include<SFML/Graphics.hpp>
#include<SFML/Window.hpp>
#include<SFML/Audio.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stack>
#include <vector>
#include <map>
#include <exception>

extern bool isMousePressed;

class GUIState
{
	protected:
	std::stack<GUIState*> *states; //To nie jest kopia states z shell tylko wskaznik na wlasnie tamtn stos zeby móc mieæ do nie go dostêp
	sf::RenderWindow *window;
	sf::Event *event;
	std::vector<sf::Texture> textures;
	bool quit;


	sf::Vector2i mousePosScreen;
	sf::Vector2i mousePosWindow;
	sf::Vector2f mousePosView;

public:
	
	GUIState(sf::RenderWindow *window, std::stack<GUIState*> *states, sf::Event *event);
	virtual ~GUIState();

	const bool& getQuit() const;

	virtual void checkForQuit();

	virtual void updateMousePositions();
	virtual void updateKeybinds(const float& dt) = 0;
	virtual void update(const float& dt) = 0;
	virtual void render(sf::RenderTarget* target = nullptr) = 0;
};

