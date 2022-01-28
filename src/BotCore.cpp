#include "BotCore.hpp"

#include <iostream>
#include <thread>

BotCore::BotCore() : m_client(), m_prefix(), onMessageCallback(m_callbackMsg) { }

void BotCore::init(const std::string &path)
{
	YAML::Node config = YAML::LoadFile(path);
	m_client.connect("irc.chat.twitch.tv", "6667");

	std::string autho = "PASS " + config["token"].as<std::string>();
	std::string nick = "NICK " + config["botname"].as<std::string>();
	m_prefix = config["cmdprefix"].as<std::string>();

	m_client.send(autho);
	m_client.send(nick);

	std::clog << "Bot start" << std::endl;

	auto channels = config["channels"].as<std::vector<std::string>>();

	for (auto &c : channels)
	{
		joinChannel(c);
	}
}

void BotCore::joinChannel(const std::string &channel)
{
	std::clog << "Join to channel: " << channel << std::endl;
	std::stringstream ircMessageSStream;
	ircMessageSStream << "JOIN #" << channel;
	m_client.send(ircMessageSStream.str());
}

void BotCore::sendMessageToChannel(const std::string &channel, const std::string &message)
{	
	std::stringstream ircMessageSStream;
	ircMessageSStream << "PRIVMSG #" << channel << " :" << message;
	m_client.send(ircMessageSStream.str());
}

void BotCore::run()
{
	std::thread autoPongThread(&BotCore::autoPong, this);

	while (m_client.isConnected())
	{
		std::string ircMessage = m_client.receive();	
		if (ircMessage.find("PRIVMSG") != std::string::npos)
		{
			UserInfo user = UserInfo::getChatUserInfo(ircMessage);
			MessageContext ctx(user, *this);
			m_callbackMsg(ctx);
		}
	}

	autoPongThread.join();
}

void BotCore::stop()
{
	m_client.disconnect();
}

bool BotCore::isConnected() const
{
	return m_client.isConnected();
}

const std::string &BotCore::getPrefix() const
{
	return m_prefix;
}

void BotCore::autoPong()
{
	static std::string pongMsg = "PONG :tmi.twitch.tv";
	while (m_client.isConnected())
	{
		m_client.send(pongMsg);
		std::this_thread::sleep_for(std::chrono::milliseconds(240000));
	}
}

