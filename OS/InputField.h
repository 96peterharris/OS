#pragma once
#include<SFML/Graphics.hpp>
#include<SFML/Window.hpp>
#include <iostream>
#include <string>
#include <map>

class InputField
{
private:
	sf::RectangleShape shape1;
	sf::RectangleShape shape2;
	sf::Texture t1;
	sf::Texture t2;
	sf::Font* font;
	std::string input;
	sf::Text text;
	sf::Text mainText;
	sf::Event *event;
	sf::Clock clock;
	float time = 0;
	float x = 0.5; 
	

public:
	int keycounter = 0;
	std::vector<std::string> vec;

	InputField(sf::Font* font, sf::Event *event);
	~InputField();

	//functions
	void update();
	void render(sf::RenderTarget* target);
};

