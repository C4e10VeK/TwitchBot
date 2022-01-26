#include <iostream>
#include <fstream>
#include <regex>
#include <thread>

#include "IRCClinet.hpp"

struct ChatUser
{
	std::string rawMessage;
	std::string nickname;
	std::string channel;
	std::string message;
};

ChatUser getChatUserInfo(const std::string &msg)
{
	ChatUser user;
	std::regex r(":([a-z0-9_]+)!\\1@\\1.tmi.twitch.tv PRIVMSG #([a-z0-9_]+) :(.*)");
	std::smatch matches;

	if (!std::regex_search(msg, matches, r)) return { "", "", "", "" };

	user.rawMessage = matches[0].str();
	user.nickname = matches[1].str();
	user.channel = matches[2].str();
	user.message = matches[3].str();

	return user;
}

int main()
{
	std::string host;
	std::string port;
	std::string token;

	std::ifstream configFile;
	configFile.open("config.conf");
	
	std::getline(configFile, host);
	std::getline(configFile, port);
	std::getline(configFile, token);

	configFile.close();

	std::string autho = "PASS " + token;
	std::string nick = "NICK chelovchik";
	std::string join = "JOIN #chelovchik";

	IRCClient client;

	client.connect(host.c_str(), port.c_str());

	client.send(autho);
	client.send(nick);

	token.clear();
	autho.clear();

	auto msg = client.receive();

	std::cout << msg << std::endl;

	client.send(join);

	std::thread autoPong([&]{
				while (client.isConnected())
				{
					client.send("PONG :tmi.twitch.tv");
					std::this_thread::sleep_for(std::chrono::milliseconds(240000));
				}
			});

	while (client.isConnected())
	{
		msg = client.receive();

		std::clog << msg << std::endl;	

		if (msg.find("PRIVMSG") != std::string::npos)
		{
			ChatUser usr = getChatUserInfo(msg);
			if (usr.message == "~аниме")
				client.send("PRIVMSG #" + usr.channel + " :@" + usr.nickname + " Говно твое аниме!!! coMMMMfy");
			if (usr.nickname == "chelovchik" && usr.message == "~stop_bot")
				client.disconnect();
		}
	}

	autoPong.join();
	
	return 0;
}

