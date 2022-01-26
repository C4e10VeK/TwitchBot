#pragma once

#include "UserInfo.hpp"
#include "IBotCore.hpp"

class MessageContext
{
private:
	UserInfo m_userInfo;
	IBotCore &m_botCore;
public:
	MessageContext(const UserInfo &user, IBotCore &botCore) : m_userInfo(user), m_botCore(botCore) { }

	void send(const std::string &message)
	{
		m_botCore.sendMessageToChannel(m_userInfo.channel, message);
	}

	const std::string &getPrefix() const
	{
		return m_botCore.getPrefix();
	}

	const std::string &getNickname() const
	{
		return m_userInfo.nickname;
	}

	const std::string &getMention() const
	{
		return m_userInfo.mention;
	}

	const std::string &getMessage() const
	{
		return m_userInfo.message;
	}
};

