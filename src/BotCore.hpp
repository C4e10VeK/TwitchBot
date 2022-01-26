#pragma once

#include <yaml-cpp/yaml.h>

#include "IRCClinet.hpp"
#include "IBotCore.hpp"
#include "UserInfo.hpp"
#include "MessageContext.hpp"

class BotCore : public IBotCore
{
private:
	using MessageCallback = void(*)(MessageContext &);

	IRCClient m_client;
	std::string m_prefix;
	MessageCallback m_callbackMsg;

public:
	BotCore();

	void init(const std::string &path);

	void setMessageCallback(MessageCallback callback);

	void joinChannel(const std::string &channel);

	void sendMessageToChannel(const std::string &channel, const std::string &message) override;

	void run();

	void stop();

	bool isConnected() const;

	const std::string &getPrefix() const override;
private:
	void autoPong();
};
