#include "CreateFileInput.h"

void CreateFileInput::initBackground()
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

	this->menutexture.loadFromFile("Resources/Images/menu.png");
	this->menu1.setSize(sf::Vector2f(256.f, 349.f));
	this->menu1.setTexture(&menutexture);
	this->menu1.setPosition(sf::Vector2f(120.f, 200.f));
	this->menu2.setSize(sf::Vector2f(256.f, 349.f));
	this->menu2.setTexture(&menutexture);
	this->menu2.setPosition(sf::Vector2f(370.f, 200.f));
}

void CreateFileInput::initFonts()
{
	if (!this->font.loadFromFile("Fonts/AppleGaramond.ttf"))
	{
		throw("ERROR:could not load font");
	}
}

void CreateFileInput::initButtons()
{
	this->buttons["HELP"] = new Button(900, 0, 29, 50, &this->font, "", "Resources/Images/help.png", "Resources/Images/helpMoused.png", "Resources/Images/helpClicked.png");
	this->buttons["EXIT"] = new Button(950, 0, 44, 50, &this->font, "", "Resources/Images/quit.png", "Resources/Images/quitHover.png", "Resources/Images/quitHover.png");
	this->buttons["STEP"] = new Button(457, 700, 85, 85, &this->font, "", "Resources/Images/step.png", "Resources/Images/stepHover.png", "Resources/Images/stepClicked.png");

	this->buttons["LOGO"] = new Button(0, 300, 127, 151, &this->font, "", "Resources/Images/logo.png", "Resources/Images/logoHover.png", "Resources/Images/logoClicked.png");

	this->buttons["PROGRAMS"] = new Button(130, 250, 244, 44, &this->font, "Programs");
	this->buttons["FILE_MANAGER"] = new Button(130, 350, 244, 44, &this->font, "File manager");
	this->buttons["CONTROL_PANEL"] = new Button(130, 450, 244, 44, &this->font, "Control panel");

	this->buttons["CREATE_FILE"] = new Button(380, 230, 244, 44, &this->font, "Create file");
	this->buttons["DELETE_FILE"] = new Button(380, 280, 244, 44, &this->font, "Delete file");
	this->buttons["RENAME_FILE"] = new Button(380, 330, 244, 44, &this->font, "Rename file");
	this->buttons["WRITE_TO_FILE"] = new Button(380, 380, 244, 44, &this->font, "Write to file");
	this->buttons["OVERWRITE_FILE"] = new Button(380, 430, 244, 44, &this->font, "Overwrite file");
	this->buttons["READ"] = new Button(380, 480, 244, 44, &this->font, "Read from file");
}

CreateFileInput::CreateFileInput(sf::RenderWindow *window, std::stack<GUIState*> *states, sf::Event *event)
	:GUIState(window, states, event)
{

	this->initFonts();
	this->initButtons();
	this->initBackground();

}

CreateFileInput::~CreateFileInput()
{
	auto it = this->buttons.begin();
	for (it = this->buttons.begin(); it != this->buttons.end(); ++it) {
		delete it->second;
	}
}

void CreateFileInput::updateKeybinds(const float& dt)
{
	this->checkForQuit();
}

void CreateFileInput::update(const float& dt)
{
	if (event->type == sf::Event::MouseButtonReleased) {
		isMousePressed = false;
	}

	this->updateKeybinds(dt);
	this->updateMousePositions();
	this->updateTimebar();
	this->updateButtons();

	if (input.keycounter < 1) {
		this->updateInput();
	}
	else {
		System::FS.createFile(input.vec.at(0));
		this->states->pop();
	}
}

void CreateFileInput::updateButtons()
{
	//updating buttons and does all funcionality
	for (auto &it : this->buttons) {
		it.second->update(this->mousePosView);
	}

	if (this->buttons["LOGO"]->isPressed() && isMousePressed == false)
	{
		isMousePressed = true;
		this->states->pop();
	}

	if (this->buttons["PROGRAMS"]->isPressed() && isMousePressed == false)
	{
		isMousePressed = true;
		this->states->pop();
	}

	if (this->buttons["CONTROL_PANEL"]->isPressed() && isMousePressed == false)
	{
		isMousePressed = true;
		this->states->pop();
	}

	if (this->buttons["FILE_MANAGER"]->isPressed() && isMousePressed == false)
	{
		isMousePressed = true;
		this->states->pop();
	}

	if (this->buttons["CREATE_FILE"]->isPressed() && isMousePressed == false)
	{
		isMousePressed = true;
		this->states->pop();
	}

	if (this->buttons["DELETE_FILE"]->isPressed() && isMousePressed == false)
	{	
		isMousePressed = true;
		this->states->pop();
	}

	if (this->buttons["RENAME_FILE"]->isPressed() && isMousePressed == false)
	{
		isMousePressed = true;
		this->states->pop();
	}

	if (this->buttons["WRITE_TO_FILE"]->isPressed() && isMousePressed == false)
	{
		isMousePressed = true;
		this->states->pop();
	}

	if (this->buttons["OVERWRITE_FILE"]->isPressed() && isMousePressed == false)
	{
		isMousePressed = true;
		this->states->pop();
	}

	if (this->buttons["HELP"]->isPressed() && isMousePressed == false)
	{
		isMousePressed = true;
		this->states->push(new HelpState(this->window, this->states, this->event));
	}

	if (this->buttons["STEP"]->isPressed() && isMousePressed == false)
	{
		isMousePressed = true;
		System::CPU.nextStep();
	}

	//Quiting shell
	if (this->buttons["EXIT"]->isPressed() && isMousePressed == false)
	{
		this->window->close();
	}
}

void CreateFileInput::updateTimebar()
{
	this->timebar.getActualTime();
}

void CreateFileInput::updateInput()
{
	this->input.update();
}

void CreateFileInput::renderButtons(sf::RenderTarget * target)
{
	for (auto &it : this->buttons) {
		it.second->render(target);
	}
}

void CreateFileInput::render(sf::RenderTarget* target)
{
	if (!target) {
		target = this->window;
	}

	target->draw(this->background);
	target->draw(this->menu1);
	target->draw(this->menu2);
	this->timebar.render(target);
	this->renderButtons(target);
	this->input.render(target);

	//Only for tests, remove later
	/*sf::Text mouseText;
	mouseText.setPosition(this->mousePosView.x, this->mousePosView.y - 50);
	mouseText.setFont(this->font);
	mouseText.setCharacterSize(16);
	std::stringstream ss;
	ss << this->mousePosView.x << " " << this->mousePosView.y;
	mouseText.setString(ss.str());

	target->draw(mouseText);*/
}
