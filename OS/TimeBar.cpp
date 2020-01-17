#include "TimeBar.h"


TimeBar::TimeBar(float width, float height, sf::Font * font, std::string text)
{
	
	this->shape.setSize(sf::Vector2f(width, height));
	this->shape.setFillColor(sf::Color(162, 162, 162, 129));

	this->font = font;
	this->text1.setFont(*this->font);
	this->text1.setString(text);
	this->text1.setFillColor(sf::Color::White);
	this->text1.setCharacterSize(20);
	this->text1.setPosition(25, 20);
	this->text1.setOutlineColor(sf::Color::Black);
	this->text1.setOutlineThickness(2);

	this->text2.setFont(*this->font);
	this->text2.setString(this->actualTime);
	this->text2.setFillColor(sf::Color::White);
	this->text2.setCharacterSize(20);
	this->text2.setPosition(405, 20);
	this->text2.setOutlineColor(sf::Color::Black);
	this->text2.setOutlineThickness(2);
}

TimeBar::~TimeBar()
{

}

void TimeBar::getActualTime()
{
	auto end = std::chrono::system_clock::now();
	std::time_t end_time = std::chrono::system_clock::to_time_t(end);
	std::stringstream ss;
	ss << std::ctime(&end_time);
	actualTime = ss.str();
	this->text2.setString(actualTime);
}

void TimeBar::render(sf::RenderTarget * target)
{
	target->draw(this->shape);
	target->draw(this->text1);
	target->draw(this->text2);
}
