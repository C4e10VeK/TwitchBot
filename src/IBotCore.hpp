#pragma once

#include <string>

class IBotCore
{
public:
	virtual void sendMessageToChannel(const std::string &channel, const std::string& message) = 0;
	virtual const std::string &getPrefix() const = 0;
};

