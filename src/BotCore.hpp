#pragma once


#include <unordered_map>
#include <yaml-cpp/yaml.h>

#include "IRCClinet.hpp"
#include "IBotCore.hpp"
#include "UserInfo.hpp"
#include "MessageContext.hpp"
#include "Callback.hpp"
#include "commands/ICommand.hpp"

class BotCore : public IBotCore
{
private:
	using MessageCallback = Callback<MessageContext &>;
	using CommandCallback = Callback<MessageContext &, const std::string &>;

	IRCClient m_client;
	std::string m_prefix;
	MessageCallback m_callbackMsg;

	std::unordered_map<std::string, std::unique_ptr<ICommand>> m_commands;
public:
	MessageCallback::IType &onMessageCallback;

	BotCore();

	void init(const std::string &path);
	
	void joinChannel(const std::string &channel);

	template<typename T, typename ...Args>
	void addCommand(const std::string &name, Args &&...args);

	template<typename... T>
	void connectCallback(T &&...some);

	void sendMessageToChannel(const std::string &channel, const std::string &message) override;

	void run();

	void stop();

	bool isConnected() const;

	const std::string &getPrefix() const override;
private:
	void autoPong();
};

template<typename T, typename ...Args>
void BotCore::addCommand(const std::string &name, Args &&...args)
{
	if (m_commands.find(name) != m_commands.end()) return;

	m_commands[name] = std::make_unique<T>(std::forward<Args>(args)...);
}

template<typename... T>
void BotCore::connectCallback(T &&...some)
{
	onMessageCallback = createCallback(std::forward<T>(some)...);
}

