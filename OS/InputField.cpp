#include "InputField.h"

InputField::InputField(sf::Font * font, sf::Event * event)
{
	this->t1.loadFromFile("Resources/Images/textBackground.png");
	this->t2.loadFromFile("Resources/Images/textForeground.png");

	this->shape1.setPosition(sf::Vector2f(200, 600));
	this->shape1.setSize(sf::Vector2f(578, 95));
	this->shape1.setTexture(&t1);

	this->shape2.setPosition(sf::Vector2f(230, 630));
	this->shape2.setSize(sf::Vector2f(518, 36));
	this->shape2.setTexture(&t2);

	this->event = event;
	this->font = font;
	this->text.setFont(*this->font);
	this->text.setFillColor(sf::Color::Black);
	this->text.setCharacterSize(20);
	this->text.setPosition(230, 630);

	this->mainText.setString("Type here your text or number:");
	this->mainText.setFont(*this->font);
	this->mainText.setFillColor(sf::Color::Black);
	this->mainText.setCharacterSize(20);
	this->mainText.setPosition(230, 600);
}

InputField::~InputField()
{

}

/*void InputField::update() 
{
	if (event->type == sf::Event::TextEntered) {
		time += clock.getElapsedTime().asSeconds();

		//domyœlnie x = 0.5, isFirstCharacter = true

		if (isFirstCharacter == true) {
			
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace) && input.size() != 0) {
					input.pop_back();
					clock.restart();
					time = 0;
				}
				else if (event->text.unicode < 128 && event->text.unicode > 10) {
					input += event->text.unicode;
					clock.restart();
					time = 0;
				}
			
			isFirstCharacter = false;
			x = 0.5;
			//po pierwszej, zmieniamy, ¿e zosta³a wpisana pierwsza, ale do nastêpnej czekamy 0.5s
		}
		else if (time > x) {
			
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace) && input.size() != 0) {
					input.pop_back();
					clock.restart();
					time = 0;
				}
				else if (event->text.unicode < 128 && event->text.unicode > 10) {
					input += event->text.unicode;
					clock.restart();
					time = 0;
				}
			
			x = 0.2;
			//po nastêpnej, zmieniamy, ¿e zosta³a wpisana nastêpna i do ka¿dej nast. ju¿ czekamy 0.2s
		}

		// Tutaj reszta kodu z backspace itd.
	}
	else {
		isFirstCharacter = true;
		x = 0.5;
	}
		text.setString(input);
}*/

void InputField::update()
{
	if (event->type == sf::Event::TextEntered)
	{
		time += clock.getElapsedTime().asSeconds();
		
		if (time > x) {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace) && input.size() != 0) {
				input.pop_back();
				clock.restart();
				time = 0;
			}
			else if (event->text.unicode < 128 && event->text.unicode > 10) {
				if (input.size() < 64) {
					input += event->text.unicode;
				}
				clock.restart();
				time = 0;
			}

			if (event->type == sf::Event::KeyPressed) {
				x = 0.2f;
			}
			else {
				x = 0.4f;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
				//std::cout << input;
				input.pop_back();
				vec.push_back(input);
				keycounter++;
				input = "";
			}
		}
		
	}

	text.setString(input);
}


void InputField::render(sf::RenderTarget *target)
{
	target->draw(this->shape1);
	target->draw(this->shape2);
	target->draw(this->text);
	target->draw(this->mainText);
}
