#pragma once
#include<SFML/Graphics.hpp>
#include<SFML/Window.hpp>
#include <iostream>
#include <sstream>
#include <string>
#include <chrono>
#include <ctime>

extern bool isMousePressed;

class TimeBar
{
private:
	sf::RectangleShape shape;
	sf::Font* font;
	sf::Text text1;
	sf::Text text2;
	std::string actualTime;
public:
	TimeBar(float width, float height, sf::Font * font, std::string text);
	virtual ~TimeBar();

	//functions
	void getActualTime();
	void render(sf::RenderTarget* target);
};

