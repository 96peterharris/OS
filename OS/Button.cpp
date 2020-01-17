#include "Button.h"

Button::Button(float x, float y, float width, float height, sf::Font * font, std::string text,
	std::string tnormal, std::string thover, std::string tactive)
{
	this->shape.setPosition(sf::Vector2f(x, y));
	this->shape.setSize(sf::Vector2f(width, height));
	this->buttonState = BTN_IDLE;
	
	this->font = font;
	this->text.setFont(*this->font);
	this->text.setString(text);
	this->text.setFillColor(sf::Color::White);
	this->text.setOutlineColor(sf::Color::Black);
	this->text.setOutlineThickness(5);
	this->text.setCharacterSize(20);
	this->text.setPosition(
		this->shape.getPosition().x + (this->shape.getGlobalBounds().width / 2.f) - this->text.getGlobalBounds().width / 2.f +22,
		this->shape.getPosition().y + (this->shape.getGlobalBounds().height / 2.f) - this->text.getGlobalBounds().height / 2.f - 5
	);

	this->idleTexture.loadFromFile(tnormal);
	this->hoverTexture.loadFromFile(thover);
	this->activeTexture.loadFromFile(tactive);
	this->shape.setTexture(&idleTexture);
}

Button::Button(float x, float y, float width, float height, sf::Font * font, std::string text)
{
	this->shape.setPosition(sf::Vector2f(x, y));
	this->shape.setSize(sf::Vector2f(width, height));
	this->buttonState = BTN_IDLE;

	this->font = font;
	this->text.setFont(*this->font);
	this->text.setString(text);
	this->text.setFillColor(sf::Color::White);
	this->text.setOutlineColor(sf::Color::Black);
	this->text.setOutlineThickness(2);
	this->text.setCharacterSize(20);
	this->text.setPosition(
		this->shape.getPosition().x + (this->shape.getGlobalBounds().width / 2.f) - this->text.getGlobalBounds().width / 2.f + 22,
		this->shape.getPosition().y + (this->shape.getGlobalBounds().height / 2.f) - this->text.getGlobalBounds().height / 2.f - 5
	);

	this->idleTexture.loadFromFile("Resources/Images/ButtonNormal.png");
	this->hoverTexture.loadFromFile("Resources/Images/ButtonHover.png");
	this->activeTexture.loadFromFile("Resources/Images/ButtonActive.png");
	this->shape.setTexture(&idleTexture);
}

Button::~Button()
{

}

//Accesors

const bool Button::isPressed() const
{
	if (this->buttonState == BTN_ACTIVE)
		return true;

	return false;
}


//Functions

void Button::update(const sf::Vector2f mousePos)
{
	//Idle
	this->buttonState = BTN_IDLE;
	this->text.setFillColor(sf::Color::White);

	//Hover
	if (this->shape.getGlobalBounds().contains(mousePos)) 
	{
		this->buttonState = BTN_HOVER;
		this->text.setFillColor(sf::Color(179.f, 179.f, 179.f));
		//Pressed
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			this->buttonState = BTN_ACTIVE;
			this->text.setFillColor(sf::Color(233.f, 175.f, 175.f));
		}
	}

	switch (this->buttonState)
	{
	case BTN_IDLE:
		this->shape.setTexture(&this->idleTexture);
		break;

	case BTN_HOVER:
		this->shape.setTexture(&this->hoverTexture);	
		break;

	case BTN_ACTIVE:
		this->shape.setTexture(&this->activeTexture);
		break;

	default:
		this->shape.setFillColor(sf::Color::Red);
		break;
	}
}

void Button::render(sf::RenderTarget * target)
{
	target->draw(this->shape);
	target->draw(this->text);
}
