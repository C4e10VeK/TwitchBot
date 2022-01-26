#pragma once

#include "IBotCore.hpp"

struct UserInfo
{
	std::string rawMessage;
	std::string nickname;
	std::string channel;
	std::string message;
	std::string mention;

	static UserInfo getChatUserInfo(const std::string &msg);
};

