#include "State/Host.hpp"

Host::Host(sf::RenderWindow& window,
		   std::unique_ptr<State>& main_state,
		   unsigned short port)
	: State(window, main_state)
{
	//Initialize the script box.
	TextEditor::LanguageDefinition lua_lang = TextEditor::LanguageDefinition::Lua();
	lua_lang.mAutoIndentation				= true;
	mScriptBox.SetLanguageDefinition(lua_lang);

	//Set the default code.
	mScriptBox.SetText("-- A simple echo script.\n\nx = arg\nprint(x)");



	//Attempt to bind to the port.
	if (mListenerSocket.listen(port) != sf::Socket::Done)
	{
		error("Could not bind to port " + std::to_string(port) + "!");
	}

	//Set the listener as non-blocking.
	mListenerSocket.setBlocking(false);
}

Host::~Host()
{
	//Clear & close all sockets.
	mListenerSocket.close();
	for (auto& sock : mClientSockets)
	{
		sock->disconnect();
	}
	mClientSockets.clear();
}

void Host::updateFrame()
{
	//Button states.
	if (wasMenuPressed)
	{
		//Return to the main menu.
		state().reset(new MainMenu(window(), state()));
		return;
	}
	///////////
	//Toggle read-only depending on the state of isRunning.
	mScriptBox.SetReadOnly(isRunning);

	//Update the socket data string.
	socket_data = stringifyInfo();

	//Socket logic//
	updateListener();

	//Receive data that's ready to be received.
	updateClients();
}

void Host::updateListener()
{
	//Check for new connections...
	std::shared_ptr<sf::TcpSocket> sock(new sf::TcpSocket());

	//If one is received.
	if (mListenerSocket.accept(*sock) != sf::Socket::NotReady)
	{
		//Initialize it.
		sock->setBlocking(false);

		//Push the socket back.
		mClientSockets.push_back(sock);
	}
}

void Host::updateClients()
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
			disconnectSocket(*sock);
			continue;
		}
		//If the socket has data to be accepted...
		else if (status != sf::Socket::NotReady)
		{
			//Handle the sent data.
			handleData(std::string(data), *sock);
		}
	}
}

void Host::handleData(std::string data, sf::TcpSocket& sock)
{
	//If we're not running...
	if (!isRunning)
	{
		//Send back a filler message.
		sendData("Server is online but is not accepting connections.\nTry again later.\n",
				 sock);
	}
	else
	{
		//Otherwise, evaluate the lua script with the given data.
		//TODO: Split data into arguments.
		std::string lua_result = evaluateLua(splitString(data));

		//Return the script result.
		sendData(lua_result, sock);
	}
}

std::string Host::evaluateLua(std::vector<std::string> args)
{
	//The stream to be printed to by the lua state.
	std::stringstream output;

	for (auto& i : args)
	{
		std::cout << i << "\n";
	}

	//Create the print function.
	auto print = sol::overload(
		//THE STRING OVERLOAD
		[&output](std::string str) { output << str; },
		//THE NUMBER OVERLOAD
		[&output](double val) { output << val; },
		//THE LIST OVERLOAD
		[&output](sol::table table) {
			bool isFirst = true;
			for (auto& i : table)
			{
				//Deduce the type of the item in the table.
				std::string item;
				sol::type type = i.second.get_type();
				if (type == sol::type::string)
				{
					item = i.second.as<std::string>();
				}
				else if (type == sol::type::number)
				{
					item = std::to_string(i.second.as<double>());
				}

				//Render the first opening brace if it's the first elem.
				if (isFirst)
				{
					isFirst = false;
					output << "[" << item;
					continue;
				}
				//Otherwise, output the item.
				output << ", " << item;
			}
			//Finish outputting.
			output << "]";
		});

	//To number function.
	auto tonumber = [&](std::string arg) { return std::stod(arg); };
	auto tostring = [&](double number) { return std::to_string(number); };

	//Create the lua state.
	sol::state lua;

	//Bind the standard functions.
	lua.set_function("print", print);
	lua.set_function("tonumber", tonumber);
	lua.set_function("tostring", tostring);

	//Bind the argument array.
	lua.set("arg", sol::as_table(args));

	//Evaluate the script.
	try
	{
		lua.script(mScriptBox.GetText());
	}
	catch (const std::exception& e)   //If we throw an exception
	{
		//Return an error message.
		output.str("");
		output << "Error: Lua exception in server -- perhaps your input is wrong?\n";
		output << "Error message >> " << e.what();
	}

	output << "\n";
	//Return the output.
	return output.str();
}

std::vector<std::string> Host::splitString(std::string data)
{
	//Create the vector to append each string to.
	std::vector<std::string> ret;

	//For appending characters as we iterate.
	std::string current_string = "";
	//Iterate over the string.
	for (auto& i : data)
	{
		//If it is a space..
		if (std::isspace(i))
		{
			//Append current_string and clear it. (unless the string is empty).
			if (!current_string.empty())
			{
				ret.push_back(current_string);
				current_string.clear();
			}
		}
		else   //Otherwise..
		{
			//Append the character to the current string.
			current_string.push_back(i);
		}
	}
	//Append the last bits of the argument.
	if (!current_string.empty())
	{
		ret.push_back(current_string);
	}

	return ret;
}

void Host::sendData(std::string data, sf::TcpSocket& sock)
{
	//Sent data size.
	std::size_t sent;

	//Attempt to send the data.
	sf::Socket::Status status = sock.send(data.data(), data.size(), sent);

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
			status = sock.send(data.data(), data.size(), sent);
		}
	}
}

void Host::disconnectSocket(sf::TcpSocket& sock)
{
	//Iterate over all clients.
	for (auto client = mClientSockets.begin();
		 client != mClientSockets.end();)
	{
		//If the socket's IP and Port match...
		if ((*client)->getRemoteAddress() == sock.getRemoteAddress() && (*client)->getRemotePort() == sock.getRemotePort())
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

void Host::error(std::string msg)
{
	//Reset to the error state.
	state().reset(new Error(window(),
							state(),
							msg));
}

std::string Host::stringifyInfo()
{
	//Create a new stringstream.
	std::stringstream stream;

	//The header..
	stream << "Connected Clients:\n";
	for (auto& sock : mClientSockets)
	{
		//Append each connected client to the stream.
		stream << sock->getRemoteAddress()
			   << ":"
			   << sock->getRemotePort()
			   << "\n";
	}

	//Return the stringified stream.
	return stream.str();
}

void Host::drawSFML()
{
	//Nothing to do...
}

void Host::drawImGui()
{
	//Draw the main window.
	ImGui::SetNextWindowSize(window().getSize());
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::Begin("ScriptableTCP Server",
				 nullptr,
				 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

	//Render the scripting box.
	mScriptBox.Render("Script", ImVec2(480, 250), false);

	//Render a clear button.
	if (ImGui::Button("<> Clear Script"))
	{
		mScriptBox.SetText("");
		isRunning = false;
	}

	//Render the pause/play button.
	std::string running_button = (isRunning) ? ("|| Pause") : ("> Start");
	if (ImGui::Button(running_button.c_str()))
	{
		isRunning = !isRunning;
	}

	//Render the back button.
	wasMenuPressed = ImGui::Button("<- Main Menu");

	ImGui::Text("--------\n");

	//Render the connection info.
	ImGui::TextWrapped("%s", socket_data.c_str());

	ImGui::End();
}