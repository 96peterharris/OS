#include "HelpState.h"

void HelpState::initBackground()
{
	this->background.setSize(
		sf::Vector2f
		(
			static_cast<float>(this->window->getSize().x),
			static_cast<float>(this->window->getSize().y)
		)
	);

	this->texture.loadFromFile("Resources/images/helpLAYOUT.png");
	this->background.setTexture(&texture);
}

void HelpState::initFonts()
{
	if (!this->font.loadFromFile("Fonts/AppleGaramond.ttf"))
	{
		throw("ERROR:could not load font");
	}
}

void HelpState::initButtons()
{
	this->buttons["HELP"] = new Button(900, 0, 29, 50, &this->font, "", "Resources/Images/help.png", "Resources/Images/helpMoused.png", "Resources/Images/helpClicked.png");
	this->buttons["EXIT"] = new Button(950, 0, 44, 50, &this->font, "", "Resources/Images/quit.png", "Resources/Images/quitHover.png", "Resources/Images/quitHover.png");
	this->buttons["BACK"] = new Button(480, 700, 41, 41, &this->font, "", "Resources/Images/back.png", "Resources/Images/backHover.png", "Resources/Images/backClicked.png");

}

HelpState::HelpState(sf::RenderWindow *window, std::stack<GUIState*> *states, sf::Event *event)
	:GUIState(window, states, event)
{

	this->initFonts();
	this->initButtons();
	this->initBackground();
}

HelpState::~HelpState()
{
	auto it = this->buttons.begin();
	for (it = this->buttons.begin(); it != this->buttons.end(); ++it) {
		delete it->second;
	}
}

void HelpState::updateKeybinds(const float& dt)
{
	this->checkForQuit();
}

void HelpState::update(const float& dt)
{
	if (event->type == sf::Event::MouseButtonReleased) {
		isMousePressed = false;
	}
	this->updateKeybinds(dt);
	this->updateMousePositions();
	this->updateTimebar();
	this->updateButtons();

}

void HelpState::updateButtons()
{
	//updating buttons and does all funcionality
	for (auto &it : this->buttons) {
		it.second->update(this->mousePosView);
	}

	if (this->buttons["BACK"]->isPressed() && isMousePressed == false)
	{
		isMousePressed = true;
		this->states->pop();
	}

	//Quiting shell
	if (this->buttons["EXIT"]->isPressed() && isMousePressed == false)
	{
		this->window->close();
	}
}

void HelpState::updateTimebar()
{
	this->timebar.getActualTime();
}

void HelpState::renderButtons(sf::RenderTarget * target)
{
	for (auto &it : this->buttons) {
		it.second->render(target);
	}
}

void HelpState::render(sf::RenderTarget* target)
{
	if (!target) {
		target = this->window;
	}

	target->draw(this->background);
	this->timebar.render(target);
	this->renderButtons(target);

	//Only for tests, remove later
/*//Only for tests, remove later
	sf::Text mouseText;
	mouseText.setPosition(this->mousePosView.x, this->mousePosView.y - 50);
	mouseText.setFont(this->font);
	mouseText.setCharacterSize(16);
	std::stringstream ss;
	ss << this->mousePosView.x << " " << this->mousePosView.y;
	mouseText.setString(ss.str());&

	target->draw(mouseText); */
}
