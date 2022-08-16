#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <SQLiteCpp/SQLiteCpp.h>

#include "ICommand.hpp"
#include "../MessageContext.hpp"

class FeedCommand : public ICommand
{
private:
	SQLite::Database m_db;
	const std::vector<const char*> AVAIBLE_EMOJIES = { "VeryPag", "VeryPog", "VeryLark", "AAUGH", "VeryPirate", "VeryBased" };
public:
	FeedCommand(); 

	void execute(MessageContext &ctx, const std::vector<std::string> &arg) override;

private:
	void sendStatus(MessageContext &ctx);

	void sendUserStatus(MessageContext &ctx, const std::string &nickname);

	bool isValidEmoji(const std::string &emoji);

	bool isTimerOut(const std::string &user, const std::string &emoji);
};
