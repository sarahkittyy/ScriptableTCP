#pragma once

#include <SFML/Graphics.hpp>

#include <memory>

#include "imgui/imgui-SFML.h"
#include "imgui/imgui.h"

#include "State/Client.hpp"
#include "State/HostInfo.hpp"
#include "State/State.hpp"

/**
 * @brief The initial state.
 * Prompts the user if they want to connect or host.
 * 
 */
class MainMenu : public State
{
public:
	/**
	 * @brief Construct a new Main Menu object
	 * 
	 * @param window See State()
	 * @param main_state See State()
	 */
	MainMenu(sf::RenderWindow& window,
			 std::unique_ptr<State>& main_state);

	/**
	 * @brief Checks for button press states.
	 * 
	 */
	void updateFrame();

	/**
	 * @brief Nothing to render for SFML.
	 * 
	 */
	void drawSFML();

	/**
	 * @brief Draws the two choice buttons, connect/host.
	 * 
	 */
	void drawImGui();

private:
	bool wasConnectPressed = false;
	bool wasHostPressed	= false;
	bool wasQuitPressed	= false;
};