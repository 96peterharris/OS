#include "GUIState.h"

GUIState::GUIState(sf::RenderWindow *window, std::stack<GUIState*> *states, sf::Event *event)
{
	this->window = window;
	this->window->setKeyRepeatEnabled(false);
	this->states = states;
	this->event = event;
	this->quit = false;
}


GUIState::~GUIState()
{
}

const bool & GUIState::getQuit() const
{
	return this->quit;
}

void GUIState::checkForQuit()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
	{
		this->quit = true;
	}
}

void GUIState::updateMousePositions()
{
	this->mousePosScreen = sf::Mouse::getPosition();
	this->mousePosWindow = sf::Mouse::getPosition(*this->window);
	this->mousePosView = this->window->mapPixelToCoords(sf::Mouse::getPosition(*this->window));

}
