#include "LoadingState.h"

void LoadingState::initBackground()
{
	this->background.setSize(
		sf::Vector2f
		(
			static_cast<float>(this->window->getSize().x),
			static_cast<float>(this->window->getSize().y)
		)
	);

	this->texture.loadFromFile("Resources/images/startScreen.png");
	this->background.setTexture(&texture);
	this->soundBuffer.loadFromFile("Resources/Sounds/intro.wav");
	this->sound.setBuffer(soundBuffer);
	sound.play();
}

LoadingState::LoadingState(sf::RenderWindow *window, std::stack<GUIState*> *states, sf::Event *event)
	:GUIState(window, states, event)
{
	this->initBackground();
}

LoadingState::~LoadingState()
{
	
}

void LoadingState::updateKeybinds(const float& dt)
{
	this->checkForQuit();
}

void LoadingState::update(const float& dt)
{
	if (event->type == sf::Event::MouseButtonReleased) {
		isMousePressed = false;
	}
	this->updateKeybinds(dt);
	this->updateMousePositions();
	if (time < 3) {
		time = time + dt;
	}
	else {
		this->states->push(new InitState(this->window, this->states, this->event));
	}
}


void LoadingState::render(sf::RenderTarget* target)
{
	if (!target) {
		target = this->window;
	}

	target->draw(this->background);
	//Only for tests, remove later
	/*
	sf::Text mouseText;
	mouseText.setPosition(this->mousePosView.x, this->mousePosView.y - 50);
	mouseText.setFont(this->font);
	mouseText.setCharacterSize(16);
	std::stringstream ss;
	ss << this->mousePosView.x << " " << this->mousePosView.y;
	mouseText.setString(ss.str());&

	target->draw(mouseText); */
}
