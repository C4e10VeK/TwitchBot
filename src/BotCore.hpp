#pragma once

#include <yaml-cpp/yaml.h>

#include "IRCClinet.hpp"
#include "IBotCore.hpp"
#include "UserInfo.hpp"
#include "MessageContext.hpp"
#include "Callback.hpp"

enum class CallbackType
{
	MESSAGE,
	COMMAND
};

class BotCore : public IBotCore
{
private:
	using MessageCallback = Callback<MessageContext &>;

	IRCClient m_client;
	std::string m_prefix;
	MessageCallback m_callbackMsg;

public:
	MessageCallback::IType &onMessageCallback;

	BotCore();

	void init(const std::string &path);
	
	void joinChannel(const std::string &channel);

	template<CallbackType CT, typename... T>
	void connectCallback(T &&...some);

	void sendMessageToChannel(const std::string &channel, const std::string &message) override;

	void run();

	void stop();

	bool isConnected() const;

	const std::string &getPrefix() const override;
private:
	void autoPong();
};

template<CallbackType CT, typename... T>
void BotCore::connectCallback(T &&...some)
{
	if constexpr (CT == CallbackType::MESSAGE)
		onMessageCallback = createCallback(std::forward<T>(some)...);
	else
		return;
}

