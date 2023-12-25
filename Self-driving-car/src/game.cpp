#include "game.h"

constexpr unsigned int str2int(const char* str, int h = 0)
{
	return !str[h] ? 5381 : (str2int(str, h + 1) * 33) ^ str[h];
}

Game::Game(sf::RenderWindow& inWindow)
	:
	window(inWindow)
{
	//create neural network structure from inputs
	aiSizeVector.push_back(9);
	std::cout << "input the neural network's size (enter a number to add that number of nodes in the highlighted layer, enter \"done\" to start the simulation):\n";
	std::cout << "9, _, 3: ";
	std::string command;
	std::cin >> command;
	while (command != "done")
	{
		char* pEnd = NULL;
		int num = strtod(command.c_str(), &pEnd);

		if (num <= 0)
		{
			std::cout << "enter a valid value please: ";
		}
		else
		{
			aiSizeVector.push_back(num);
			for (int i = 0; i < aiSizeVector.size(); i++)
			{
				std::cout << aiSizeVector[i] << ", ";
			}
			std::cout << "_, 3: ";
		}

		std::cin >> command;
	}
	std::cout << "\n";
	aiSizeVector.push_back(3);

	//create cars with the desired ai size
	for (int i = 0; i < nCars; i++) 
	{
		cars.push_back(Car(aiSizeVector));
	}

	//create window
	settings.antialiasingLevel = 5;
	window.create(sf::VideoMode(windowDim.x, windowDim.y), "Game", sf::Style::Default, settings);
	window.setView(sf::View(sf::Vector2f(640, 360), windowDim));

	//start thread that takes console inputs
	std::thread thread(&Game::takeConsoleInputs, this);
	thread.detach();
}

void Game::frame()
{
	//take inputs
	uint64_t second = getTime() / 1000;
	if ((getTime() - second * 1000) / 1000.f >= (float)currentInputPs / maxInputPs)
	{
		input();
		currentInputPs++;
	}

	//update
	if ((getTime() - second * 1000) / 1000.f >= (float)currentUpdatePs / maxUpdatePs)
	{
		if (!isPaused)
		{
			update();
			currentUpdatePs++;
		}
	}

	//draw
	if ((getTime() - second * 1000) / 1000.f >= (float)currentDrawPs / maxDrawPs)
	{
		draw();
		currentDrawPs++;
	}

	//output and reset ps stats
	if (second != currentSecond)
	{	
		if (outputPsStat)
			std::cout << "\tframes/sec: " << currentDrawPs << ", updates/sec: " << currentUpdatePs << ", inputs/sec: " << currentInputPs << "\n";

		currentSecond = second;
		currentInputPs = 0;
		currentUpdatePs = 0;
		currentDrawPs = 0;
	}
}

void Game::takeConsoleInputs()
{
	auto help = []()
	{
		std::cout << "hi dude, here is a list of the commands you can enter through this console, any string that does not coincide with any of this commands will be ignored:\n";
		std::cout << "  -  help\t\t\t\tmakes this message pop up\n";
		std::cout << "  -  psstats\t\t\t\ttoggles the output of \'per second\'-related stats\n";
		
		std::cout << "\nWARNING: some commands may output continuous information through the console, if you want to enter another command, just type it and press enter, it does not matter if the command gets split up\n";
		
		std::cout << "\nthere are also some commands which you can execute by pressing some keys while the main window has focus, here is a list of them:\n";
		std::cout << "  -  left click + move mouse\t\tmove the camera\n";
		std::cout << "  -  left click + e + move mouse\tcreate wall, release mouse to stop building\n";
		std::cout << "  -  backspace\t\t\t\tdelete the last wall created\n";
		std::cout << "  -  mouse wheel\t\t\tzoom/un-zoom\n";
		std::cout << "  -  alt + enter\t\t\ttoggle fullscreen\n";		
		std::cout << "  -  p\t\t\t\t\tpause/un-pause the simulation\n\n";
	};

	help();

	while (true)
	{	
		std::string command;
		std::cin >> command;
		std::cout << "\n";

		switch (str2int(command.data()))
		{
		case str2int("help"):
			help();
			break;
		case str2int("psstats"):
			outputPsStat = !outputPsStat;
			break;
		default:
			break;
		}
	}
}

void Game::input()
{			
	//update mouse position
	sf::Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition());
	sf::Vector2f mouseP = window.mapPixelToCoords(sf::Mouse::getPosition(window));

	if (window.hasFocus())
	{
		//handle fullscreen
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt) && sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
		{
			if (canFullscreen)
			{
				sf::View view(window.getView());

				if (isFullscreen)
				{
					window.create(sf::VideoMode(windowDim.x, windowDim.y), "Game", sf::Style::Default, settings);
				}
				else
				{
					window.create(sf::VideoMode::getDesktopMode(), "Game", sf::Style::Fullscreen, settings);
				}
				window.setView(view);
				isFullscreen = !isFullscreen;
			}
			canFullscreen = false;
		}
		else
		{
			canFullscreen = true;
		}

		//handle camera movement and walls creation
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			//move camera movement
			if (!isBuilding && !sf::Keyboard::isKeyPressed(sf::Keyboard::E))
			{
				sf::View view(window.getView());
				float x = window.getView().getSize().x / window.getSize().x;
				float y = window.getView().getSize().y / window.getSize().y;
				view.move((lastMousePos - mousePos).x * x, (lastMousePos - mousePos).y * y);
				window.setView(view);
				isMoving = true;
			}
			else
			{
				isMoving = false;
			}

			//handle walls creation
			if (!isMoving)
			{
				if (!isBuilding)
				{
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
					{
						sf::VertexArray wall;
						wall.resize(2);
						wall.setPrimitiveType(sf::Lines);
						wall[0].position = mouseP;
						wall[1].position = mouseP;
						wall[0].color = sf::Color::Black;
						wall[1].color = sf::Color::Black;

						walls.push_back(wall);
						isBuilding = true;
					}
				}
				else
				{
					walls[walls.size() - 1][1].position = mouseP;
				}
			}
		}
		else if (isBuilding)
		{
			isBuilding = false;
		}
		
		//handle pause
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
		{
			if (canPause)
			{
				isPaused = !isPaused;
				canPause = false;
			}
		}
		else
		{
			canPause = true;
		}

		//handle walls destruction
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Backspace))
		{
			if (canDeleteWall)
			{
				if (walls.size() > 0)
				{
					walls.pop_back();
				}
				canDeleteWall = false;
			}
		}
		else
		{
			canDeleteWall = true;
		}
	}

	lastMousePos = mousePos;
}

void Game::update()
{	
	bool areAllCrushed = true;

	//handle cars
	for (auto& car : cars)
	{
		if (!car.crashed)
		{
			areAllCrushed = false;
			car.updateCar(walls);
		}
	}

	//revive and breed
	if (areAllCrushed)
	{	
		//find two best cars
		int first = 0;
		int second = 1;
		for (int i = 1; i < cars.size(); i++)
		{
			if (cars[i].fitness > cars[first].fitness)
			{
				second = first;
				first = i;
			}
			else if (cars[i].fitness > cars[second].fitness)
			{
				second = i;
			}
		}

		Ai firstAi = cars[first].ai;
		Ai secondAi = cars[second].ai;

		//breed and revive
		for (int i = 0; i < cars.size(); i++)
		{
			cars[i].ai = Ai(firstAi, secondAi);
			cars[i].revive();
		}
	}
}

void Game::draw()
{
	window.clear(sf::Color(130, 130, 130));

	//draw walls
	for (int a = 0; a < walls.size(); a++)
	{
		window.draw(walls[a]);
	}
	//draw cars
	for (auto& car : cars)
	{
		car.drawCar(window);
	}

	window.display();
}

uint64_t Game::getTime()
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}