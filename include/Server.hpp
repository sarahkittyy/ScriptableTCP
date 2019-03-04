#pragma once

#include <SFML/Network.hpp>

#include <memory>
#include <tuple>
#include <unordered_map>
#include <vector>

class Server
{
public:
	/**
	 * @brief For convenience.
	 * 
	 */
	typedef std::shared_ptr<sf::TcpSocket> Socket;

	/**
	 * @brief Attempt to listen to the port.
	 * 
	 * @param port The port to listen to for incoming connections.
	 */
	Server(unsigned short port);

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
	 * @brief Sends a string of data safely to the socket.
	 * 
	 * @param data The data to send.
	 * @param sock The socket to send it to.
	 */
	void sendData(std::string data, Socket& sock);

	/**
	 * @brief True if there is data to be handled.
	 *
	 */
	bool isData();

	/**
	 * @brief Pop the top most data-socket pair for handling.
	 * 
	 * @return std::pair<Socket, std::string> The data at the top of the stack.
	 * 
	 * @remarks See isData()
	 */
	std::pair<Socket, std::string> popData();

	/**
	 * @brief Get the vector of connected clients.
	 * 
	 * @return std::vector<std::shared_ptr<sf::TcpSocket>>& The connected clients. 
	 */
	std::vector<Socket>& getClients();

	/**
	 * @brief Disconnects a TCP socket.
	 * 
	 * @param sock The socket to disconnect.
	 */
	void disconnectSocket(Socket& sock);

	/**
	 * @brief Set whether or not the server is running.
	 * 
	 * @param running True/False
	 */
	void setRunning(bool running);

	/**
	 * @brief Returns if the server is running.
	 * 
	 * @return true If it's running.
	 * @return false If it's not running.
	 */
	bool isRunning();

private:
	/**
	 * @brief Handles an incoming string of data.
	 * 
	 * @param data The data recieved.
	 * @param sock The socket it was received from.
	 */
	void handleData(std::string data, Socket& sock);

	/**
	 * @brief Data recieved, stored for handling later.
	 * 
	 */
	std::vector<std::pair<Socket, std::string>> mQueuedData;

	/**
	 * @brief The main socket listener.
	 * 
	 */
	sf::TcpListener mListenerSocket;

	/**
	 * @brief A vector of all connected clients.
	 * 
	 */
	std::vector<Socket> mClientSockets;

	/**
	 * @brief True if the server is running the script on incoming connections.
	 * 
	 */
	bool mIsRunning = false;
};