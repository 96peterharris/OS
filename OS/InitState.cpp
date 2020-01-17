#include "InitState.h"

void InitState::initBackground()
{
	this->background.setSize(
		sf::Vector2f
		(
			static_cast<float>(this->window->getSize().x),
			static_cast<float>(this->window->getSize().y)
		)
	);

	this->texture.loadFromFile("Resources/images/MMBackground.png");
	this->background.setTexture(&texture);
}

void InitState::initFonts()
{
	if (!this->font.loadFromFile("Fonts/AppleGaramond.ttf"))
	{
		throw("ERROR:could not load font");
	}
}

void InitState::initButtons()
{
	this->buttons["HELP"] = new Button(900, 0, 29, 50, &this->font, "", "Resources/Images/help.png", "Resources/Images/helpMoused.png", "Resources/Images/helpClicked.png");
	this->buttons["EXIT"] = new Button(950, 0, 44, 50, &this->font, "", "Resources/Images/quit.png", "Resources/Images/quitHover.png", "Resources/Images/quitHover.png");
	this->buttons["STEP"] = new Button(457, 700, 85, 85, &this->font, "", "Resources/Images/step.png", "Resources/Images/stepHover.png", "Resources/Images/stepClicked.png");

	this->buttons["LOGO"] = new Button(0, 300, 127, 151, &this->font, "","Resources/Images/logo.png", "Resources/Images/logoHover.png", "Resources/Images/logoClicked.png");
	
}

InitState::InitState(sf::RenderWindow *window, std::stack<GUIState*> *states, sf::Event *event)
	:GUIState(window, states, event)
{
	this->initFonts();
	this->initButtons();
	this->initBackground();
}

InitState::~InitState()
{
	auto it = this->buttons.begin();
	for (it = this->buttons.begin(); it != this->buttons.end(); ++it) {
		delete it->second;
	}
}


void InitState::updateKeybinds(const float& dt)
{
	this->checkForQuit();
}

void InitState::update(const float& dt)
{
	if (event->type == sf::Event::MouseButtonReleased) {
		isMousePressed = false;
	}
	this->updateKeybinds(dt);
	this->updateMousePositions();
	this->updateTimebar();
	this->updateButtons();

}

void InitState::updateButtons()
{
	//updating buttons and does all funcionality
	for (auto &it : this->buttons) {
		it.second->update(this->mousePosView);
	}

	if (this->buttons["HELP"]->isPressed() && isMousePressed == false)
	{
		isMousePressed = true;
		this->states->push(new HelpState(this->window, this->states, this->event));
	}

	if (this->buttons["LOGO"]->isPressed() && isMousePressed == false)
	{
		isMousePressed = true;
		this->states->push(new MainMenuState(this->window, this->states, this->event));
	}

	if (this->buttons["STEP"]->isPressed() && isMousePressed == false)
	{
		isMousePressed = true;
		System::CPU.nextStep();
	}

	//Quiting shell
	if (this->buttons["EXIT"]->isPressed() && isMousePressed == false)
	{
		isMousePressed = true;
		this->window->close();
	}

}

void InitState::updateTimebar()
{
	this->timebar.getActualTime();
}

void InitState::renderButtons(sf::RenderTarget * target)
{
	for (auto &it : this->buttons) {
		it.second->render(target);
	}
}

void InitState::render(sf::RenderTarget* target)
{
	if (!target) {
		target = this->window;
	}

	target->draw(this->background);
	this->timebar.render(target);
	this->renderButtons(target);

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
