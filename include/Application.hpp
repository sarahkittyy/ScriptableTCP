#pragma once

#include <SFML/Graphics.hpp>
#include "imgui/imgui-SFML.h"
#include "imgui/imgui.h"

#include <memory>
#include <vector>

#include "State/MainMenu.hpp"

/**
 * @brief Stores the main program loop as well as all program data.
 * 
 */
class Application
{
public:
	/**
	 * @brief Initialize variables.
	 * 
	 */
	Application();

	/**
	 * @brief Main program loop.
	 * 
	 * @return int The exit code of the program.
	 */
	int run();

private:
	/**
	 * @brief The program's window.
	 * 
	 */
	sf::RenderWindow mWindow;

	/**
	 * @brief The current state of the program.
	 * 
	 */
	std::unique_ptr<State> mCurrentState;
};