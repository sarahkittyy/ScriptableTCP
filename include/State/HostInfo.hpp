#pragma once

#include <SFML/Graphics.hpp>
#include "imgui/imgui-SFML.h"
#include "imgui/imgui.h"

#include <iostream>
#include <memory>

#include "State/Host.hpp"
#include "State/MainMenu.hpp"
#include "State/State.hpp"

/**
 * @brief Gathers which port to connect to.
 * 
 */
class HostInfo : public State
{
public:
	/**
	 * @brief Construct a new Host Info object
	 * 
	 * @param window See State()
	 * @param main_state See State()
	 */
	HostInfo(sf::RenderWindow& window,
			 std::unique_ptr<State>& main_state);

	/**
	 * @brief Just waits until the host() button is pressed,
	 * before transferring control to the Host State.
	 * 
	 */
	void updateFrame();

	/**
	 * @brief Nothing to draw.
	 * 
	 */
	void drawSFML();

	/**
	 * @brief Renders the prompt for the port.
	 * 
	 */
	void drawImGui();

private:
	int port			= 5050;
	bool wasHostPressed = false;
	bool wasBackPressed = false;
};