#include "Server.hpp"

Server::Server(unsigned short port)
{
	//Attempt to bind to the port.
	if (mListenerSocket.listen(port) != sf::Socket::Done)
	{
		throw std::runtime_error("Could not bind to port " + std::to_string(port) + "!");
	}

	//Set the listener as non-blocking.
	mListenerSocket.setBlocking(false);
}

void Server::updateListener()
{
	//Check for new connections...
	Socket sock(new sf::TcpSocket());

	//If one is received.
	if (mListenerSocket.accept(*sock) != sf::Socket::NotReady)
	{
		//Initialize it.
		sock->setBlocking(false);

		//Push the socket back.
		mClientSockets.push_back(sock);
	}
}

void Server::updateClients()
{
	// Iterate through all current connections.
	for (auto& sock : mClientSockets)
	{
		//Create buffers for the data.
		char data[100] = "";
		std::size_t received;

		sf::Socket::Status status = sock->receive(data, 100, received);
		//If the socket is just disconnected...
		if (status == sf::Socket::Disconnected)
		{
			disconnectSocket(sock);
			continue;
		}
		//If the socket has data to be accepted...
		else if (status != sf::Socket::NotReady)
		{
			//Handle the sent data.
			handleData(std::string(data), sock);
		}
	}
}

void Server::sendData(std::string data, Socket& sock)
{
	//Sent data size.
	std::size_t sent;

	//Attempt to send the data.
	sf::Socket::Status status = sock->send(data.data(), data.size(), sent);

	//Remove the socket if it's disconnected.
	if (status == sf::Socket::Disconnected)
	{
		disconnectSocket(sock);
	}
	else   //Otherwise, if we have sent (some) of the message...
	{
		//If/While we have not sent the full message...
		while (status == sf::Socket::Partial)
		{
			//Continue sending.
			data   = data.substr(sent);
			status = sock->send(data.data(), data.size(), sent);
		}
	}
}

bool Server::isData()
{
	return !mQueuedData.empty();
}

std::pair<Server::Socket, std::string> Server::popData()
{
	//Copy the top element.
	std::pair<Socket, std::string> ret = *(mQueuedData.end() - 1);
	mQueuedData.pop_back();   //Pop the top element.
	return ret;				  //Return the copy.
}

std::vector<Server::Socket>& Server::getClients()
{
	return mClientSockets;
}

void Server::disconnectSocket(Socket& sock)
{
	//Iterate over all clients.
	for (auto client = mClientSockets.begin();
		 client != mClientSockets.end();)
	{
		//If the socket's IP and Port match...
		if ((*client)->getRemoteAddress() == sock->getRemoteAddress() && (*client)->getRemotePort() == sock->getRemotePort())
		{
			//Erase it and break.
			mClientSockets.erase(client);
			break;
		}
		else
		{
			++client;
		}
	}
}

void Server::setRunning(bool running)
{
	mIsRunning = running;
}

bool Server::isRunning()
{
	return mIsRunning;
}

void Server::handleData(std::string data, Socket& sock)
{
	//Push the data back into the queue.
	mQueuedData.push_back(std::make_pair(sock, data));
}
