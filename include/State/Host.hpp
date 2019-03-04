#pragma once

#include <cctype>
#include <iostream>
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

#include "sol/sol.hpp"

/**
 * @brief Hosts the server, hosts the scripting box and manages server
 * connections.
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
	 * @brief Destroy the Host object
	 * 
	 */
	~Host();

	/**
	 * @brief Updates & parses socket info.
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
	 * @brief Check the listener for new incoming connections.
	 * 
	 */
	void updateListener();

	/**
	 * @brief Iterate over all sockets and recieve data.
	 * 
	 */
	void updateClients();

	/**
	 * @brief Handles an incoming string of data.
	 * 
	 * @param data The data recieved.
	 * @param sock The socket it was received from.
	 */
	void handleData(std::string data, sf::TcpSocket& sock);

	/**
	 * @brief Sends a string of data safely to the socket.
	 * 
	 * @param data The data to send.
	 * @param sock The socket to send it to.
	 */
	void sendData(std::string data, sf::TcpSocket& sock);

	/**
	 * @brief Evaluates the current lua code with the given args.
	 * 
	 * @param args The arguments passed by TCP.
	 * @return std::string The output of the script.
	 */
	std::string evaluateLua(std::vector<std::string> args);

	/**
	 * @brief Splits a space-delimited string into a list
	 * of arguments.
	 * 
	 * @param data The data to split.
	 * @return std::vector<std::string> The split data. 
	 */
	std::vector<std::string> splitString(std::string data);

	/**
	 * @brief Disconnects a TCP socket.
	 * 
	 * @param sock The socket to disconnect.
	 */
	void disconnectSocket(sf::TcpSocket& sock);

	/**
	 * @brief Just calls the state to reset to error with the given message.
	 * 
	 */
	void error(std::string msg);

	/**
	 * @brief Returns a string representing connection info.
	 * 
	 * @return std::string A string with connected client info.
	 */
	std::string stringifyInfo();

	/**
	 * @brief The main socket listener.
	 * 
	 */
	sf::TcpListener mListenerSocket;

	/**
	 * @brief A vector of all connected clients.
	 * 
	 */
	std::vector<std::shared_ptr<sf::TcpSocket>> mClientSockets;

	/**
	 * @brief Whether or not TCP data is being handled.
	 * 
	 */
	bool isRunning = false;

	/**
	 * @brief True if the "Return to menu" button was pressed.
	 * 
	 */
	bool wasMenuPressed = false;

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
};