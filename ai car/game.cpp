#include "game.h"

constexpr unsigned int str2int(const char* str, int h = 0)
{
	return !str[h] ? 5381 : (str2int(str, h + 1) * 33) ^ str[h];
}

Game::Game(sf::RenderWindow& inWindow)
	:
	window(inWindow)
{
	settings.antialiasingLevel = 5;

	window.create(sf::VideoMode(windowDim.x, windowDim.y), "Game", sf::Style::Default, settings);
	window.setView(sf::View(sf::Vector2f(640, 360), windowDim));

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
		update();
		currentUpdatePs++;
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
		{
			std::cout << "frames/sec: " << currentDrawPs << ", updates/sec: " << currentUpdatePs << ", inputs/sec: " << currentInputPs << "\n";
		}
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
		std::cout << "  -  help\tmakes this message pop up\n";
		std::cout << "  -  psstats\ttoggles the output of \'per second\'-related stats\n";
		
		std::cout << "\nWARNING: some commands may output continuous information through the console, if you want to enter another command, just type it and press enter, it does not matter if the command gets split up\n";
		
		std::cout << "\nthere are also some commands which you can execute by pressing some keys while the main window has focus, here is a list of them:\n";
		std::cout << "  -  left click + move mouse\t\tmove the camera\n";
		std::cout << "  -  left click + e + move mouse\tcreate wall, release mouse to stop building\n";
		std::cout << "  -  mouse wheel\tzoom/un-zoom\n";
		std::cout << "  -  alt + enter\ttoggle fullscreen\n";		
		std::cout << "  -  p\t\t\tpause/un-pause the simulation\n\n";
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

		//handle view movement and walls creation
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			if (window.hasFocus() && !isBuilding && !sf::Keyboard::isKeyPressed(sf::Keyboard::E))
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

			if (window.hasFocus() && !isMoving)
			{
				if (!isBuilding)
				{
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
					{
						Wall wall;
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
					walls[walls.size() - 1].isEditing = false;
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
	}

	lastMousePos = mousePos;
}

void Game::update()
{	
	//handle cars
	for (int car = 0; car < nCars; car++)
	{
		if (!isPaused && !cars[car].crashed)
		{
			cars[car].moveCar();
			cars[car].collide(walls);
			cars[car].resetSensors();	
			cars[car].updateSensors(walls);
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
	for (int i = 0; i < nCars; i++)
	{
		cars[i].drawCar(window);
	}

	window.display();
}

/*
bool Game::collision(sf::RectangleShape car, sf::VertexArray wall)
{
	sf::Vector2f rect1Point[4];
	sf::Vector2f rect2Point[4];
	float rect1Proj[4];
	float rect2Proj[4];
	/*
	for (int i = 0; i < 4; i++)
	{
		rect1Point[i] = vertexRect(rect1, i);
		rect2Point[i] = vertexRect(rect2, i);
	}

	float angles[4] = {
		360 - rect1.getRotation(),
		360 - rect1.getRotation() + 90,
		360 - rect2.getRotation(),
		360 - rect2.getRotation() + 90
	};

	for (int a = 0; a < 4; a++)
	{
		for (int b = 0; b < 4; b++)
		{
			float ipot = sqrt(std::pow(rect1Point[b].x, 2) + std::pow(rect1Point[b].y, 2));
			float ang = acos(rect1Point[b].x / ipot) * 180 / 3.1415;
			float angg = angles[a] + ang;
			if (rect1Point[b].y <= 0)
			{
				angg = angles[a] - ang;
			}
			rect1Proj[b] = cos(angg * 3.1415 / 180) * ipot;
		}
		for (int b = 0; b < 4; b++)
		{
			float ipot = sqrt(std::pow(rect2Point[b].x, 2) + std::pow(rect2Point[b].y, 2));
			float ang = acos(rect2Point[b].x / ipot) * 180 / 3.1415;
			float angg = angles[a] + ang;
			if (rect2Point[b].y <= 0)
			{
				angg = angles[a] - ang;
			}
			rect2Proj[b] = cos(angg * 3.1415 / 180) * ipot;
		}

		float max1, min1, max2, min2;

		max1 = std::max(std::max(std::max(rect1Proj[3], rect1Proj[2]), rect1Proj[1]), rect1Proj[0]);
		min1 = std::min(std::min(std::min(rect1Proj[3], rect1Proj[2]), rect1Proj[1]), rect1Proj[0]);
		max2 = std::max(std::max(std::max(rect2Proj[3], rect2Proj[2]), rect2Proj[1]), rect2Proj[0]);
		min2 = std::min(std::min(std::min(rect2Proj[3], rect2Proj[2]), rect2Proj[1]), rect2Proj[0]);

		if (!((min1 > min2 && min1 < max2) || (max1 > min2 && max1 < max2) || (min2 > min1 && min2 < max1) || (max2 > min1 && max2 < max1)))
		{
			return false;
		}
	}
	return true;
}

//n: 0 = top-left; 1 = top-right; 2 = bot-right; 3 = bot-left
sf::Vector2f Game::vertexRect(sf::RectangleShape rect, int n)
{
	float incVertices[4];

	float ipot = sqrt(std::pow(rect.getSize().x, 2) + std::pow(rect.getSize().y, 2));
	float diagInc = acos(rect.getSize().x / ipot) * 180 / 3.1415;

	incVertices[0] = 360 - rect.getRotation() + diagInc;
	incVertices[1] = 360 - rect.getRotation() - diagInc;
	incVertices[2] = 360 - rect.getRotation() + 180 + diagInc;
	incVertices[3] = 360 - rect.getRotation() + 180 - diagInc;

	return (rect.getPosition() + sf::Vector2f(cos(incVertices[n] * 3.1415 / 180) * ipot / 2, -sin(incVertices[n] * 3.1415 / 180) * ipot / 2));
}
*/

uint64_t Game::getTime()
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}