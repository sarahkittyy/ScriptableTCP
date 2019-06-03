#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include <cstring>

#include "imgui/imgui-SFML.h"
#include "imgui/imgui.h"

#include "State/Error.hpp"
#include "State/MainMenu.hpp"
#include "State/State.hpp"

class Client : public State
{

public:
	/**
	 * @brief Construct a new Client state
	 * 
	 * @param window See State()
	 * @param main_state See State()
	 */
	Client(sf::RenderWindow& window,
		   std::unique_ptr<State>& main_state);

	/**
	 * @brief Checks for attempts to send the data,
	 * and if so, then waits for a response
	 * 
	 */
	void updateFrame();

	/**
	 * @brief Draw all SFML data.
	 * 
	 */
	void drawSFML();

	/**
	 * @brief Draw all ImGui gui.
	 * 
	 */
	void drawImGui();

private:
	char mIp[20]		 = "";
	int mPort			 = 0;
	char mToSend[150]	= "";
	bool mWasSendPressed = false;

	std::string mLastReceived = "";
};