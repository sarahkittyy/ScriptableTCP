#pragma once

#include <SFML/Graphics.hpp>

#include <memory>
#include <string>

#include "imgui/imgui-SFML.h"
#include "imgui/imgui.h"

#include "State/MainMenu.hpp"
#include "State/State.hpp"

/**
 * @brief A state that other states can switch to in case of an error.
 * 
 */
class Error : public State
{
public:
	/**
	 * @brief Construct a new Error object
	 * 
	 * @param window See State()
	 * @param main_state See State()
	 * @param error_msg The error message to display.
	 */
	Error(sf::RenderWindow& window,
		  std::unique_ptr<State>& main_state,
		  std::string error_msg);

	/**
	 * @brief Returns to the main menu if the button was pressed.
	 * 
	 */
	void updateFrame();

	/**
	 * @brief Does nothing.
	 * 
	 */
	void drawSFML();

	/**
	 * @brief Renders the error message, and a button to return to the main menu.
	 * 
	 */
	void drawImGui();

private:
	/**
	 * @brief The error message to display.
	 * 
	 */
	std::string mErrorMessage;

	bool wasMenuPressed = false;
};