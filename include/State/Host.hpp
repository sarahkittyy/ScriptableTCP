#pragma once

#include <cctype>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include "imgui/imgui-SFML.h"
#include "imgui/imgui.h"

#include "TextEditor.h"

#include "State/Error.hpp"
#include "State/MainMenu.hpp"
#include "State/State.hpp"

#include "Server.hpp"

#include "sol/sol.hpp"

/**
 * @brief Hosts the server, hosts the scripting box and
 * managing the server instance.
 * 
 */
class Host : public State
{
public:
	/**
	 * @brief Construct a new Host object
	 * 
	 * @param window See State()
	 * @param main_state See State()
	 * @param port The port to listen on.
	 */
	Host(sf::RenderWindow& window,
		 std::unique_ptr<State>& main_state,
		 unsigned short port);

	/**
	 * @brief Updates & parses server info.
	 * 
	 */
	void updateFrame();

	/**
	 * @brief Nothing...
	 * 
	 */
	void drawSFML();

	/**
	 * @brief Renders the GUI.
	 * 
	 */
	void drawImGui();

private:
	/**
	 * @brief The server itself.
	 * 
	 */
	Server mServer;

	/**
	 * @brief Evaluates the current lua code with the given args.
	 * 
	 * @param args The arguments passed by TCP.
	 * @return std::string The output of the script.
	 */
	std::string evaluateLua(std::vector<std::string> args);

	/**
	 * @brief Just calls the state to reset to error with the given message.
	 * 
	 */
	void error(std::string msg);

	/**
	 * @brief The coding window.
	 * 
	 */
	TextEditor mScriptBox;

	/**
	 * @brief The connection data string rendered each frame.
	 * 
	 */
	std::string socket_data = "";

	/**
	 * @brief Returns a string representing connection info.
	 * 
	 * @return std::string A string with connected client info.
	 */
	std::string stringifyInfo();

	/**
	 * @brief Splits a space-delimited string into a list
	 * of arguments.
	 * 
	 * @param data The data to split.
	 * @return std::vector<std::string> The split data. 
	 */
	std::vector<std::string> splitString(std::string data);

	/**
	 * @brief True if the "Return to menu" button was pressed.
	 * 
	 */
	bool wasMenuPressed = false;
};