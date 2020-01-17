#include "Shell.h"


//Initializer functions
void Shell::initWindow()
{
	std::ifstream ifs("Config/Window.ini");
	sf::VideoMode window_bounds(800, 600);
	bool fullscreen = false;
	std::string title = "None";
	unsigned framerate_limit = 60;
	this->image.loadFromFile("Resources/Images/logo.png");

	if (ifs.is_open()) 
	{
		std::getline(ifs, title);
		ifs >> window_bounds.width >> window_bounds.height;
		ifs >> fullscreen;
		ifs >> framerate_limit;
	}

	ifs.close();

	this->window = new sf::RenderWindow(window_bounds, title, sf::Style::Titlebar | sf::Style::Close);
	this->window->setFramerateLimit(framerate_limit);
	this->window->setKeyRepeatEnabled(false);
	this->window->setIcon(image.getSize().x, image.getSize().y, image.getPixelsPtr());
}

void Shell::initStates()
{
	this->states.push(new LoadingState(this->window, &this->states, &this->sfevent));
}

Shell::Shell()
{
	PCB::createDummy();
	System::CPU.cpu_sch();

	this->initWindow();
	this->initStates();
}

Shell::~Shell()
{
	delete this->window;

	while (!this->states.empty()) 
	{
		delete this->states.top();
		this->states.pop();
	}
		
}

//Functions

void Shell::updateDt()
{
	this->dt = this->dtClock.restart().asSeconds();
}

void Shell::updateSFMLEvents()
{
	while (this->window->pollEvent(this->sfevent)) {
		if (this->sfevent.type == sf::Event::Closed)
			this->window->close();
	}

}

void Shell::update()
{
	this->updateSFMLEvents();
	if (!this->states.empty())
	{
		this->states.top()->update(this->dt);

		if (this->states.top()->getQuit()) 
		{
			delete this->states.top();
			this->states.pop();
		}
	}
	else {
		this->window->close();
	}
}



void Shell::render()
{
	this->window->clear();

	//render items
	if (!this->states.empty())
		this->states.top()->render();

	this->window->display();
}

void Shell::run()
{
	while (this->window->isOpen())
	{
		this->updateDt();
		this->update();
		this->render();
	}
}
