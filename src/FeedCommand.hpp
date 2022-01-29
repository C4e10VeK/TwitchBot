#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <SQLiteCpp/SQLiteCpp.h>

#include "MessageContext.hpp"

class FeedCommand
{
private:
	SQLite::Database m_db;
	const std::vector<const char*> AVAIBLE_EMOJIES = { "VeryPag", "VeryPog", "VeryLark", "AAUGH" };
	std::unordered_map<std::string, std::time_t> m_cooldownTimer;
public:
	FeedCommand(); 

	void execute(MessageContext &ctx, const std::string &arg);

private:
	void sendStatus(MessageContext &ctx);	

	bool isValidEmoji(const std::string &emoji);

	bool isTimerOut(const std::string &user);
};
