#include "FeedCommand.hpp"
#include <algorithm>
#include <sstream>

#include "../Utils.hpp"

FeedCommand::FeedCommand() : m_db("test.db", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE) { }

void FeedCommand::execute(MessageContext &ctx, const std::vector<std::string> &args)
{	
	if (args.empty())
	{
		ctx.send(ctx.getMention() + ", можно кормить -> VeryPog VeryPag VeryLark AAUGH VeryPirate VeryBased");
		return;
	}
	if (args[0] == "status")
	{
		if (args.size() > 1)
		{
			sendUserStatus(ctx, args[1]);
			return;
		}

		sendStatus(ctx);
		return;
	}
	
	if (!isValidEmoji(args[0])) return;

	if (!isTimerOut(ctx.getNickname()))
	{
		std::stringstream timerMessage;
		int seconds = m_cooldownTimer[ctx.getNickname()] - std::time(nullptr);
		int minutes = seconds / 60;
		char temp[32];
		sprintf(temp, "%d:%02d", (minutes % 60), (seconds % 60));
		timerMessage << ctx.getMention() << " Кормить можно раз в 10 минут coMMMMfy . Осталось: " << temp;
		ctx.send(timerMessage.str());
		return;
	}

	std::stringstream messageStream;
	std::stringstream updateSize;
	SQLite::Statement query(m_db, "SELECT * FROM FEEDINFO WHERE User = ? AND Emoji = ?");
	query.bind(1, ctx.getNickname());
	query.bind(2, args[0]);

	query.executeStep();

	if (!query.hasRow())
	{
		char insertCmd[512] = {0};
		sprintf(insertCmd, "INSERT INTO FEEDINFO (User, Emoji, Count, Size) VALUES ('%s', '%s', %d, %f)", ctx.getNickname().c_str(), args[0].c_str(), 0, 0.f);	
		m_db.exec(insertCmd);
		query.reset();
	}

	float randomSize = random(0.005f, 0.5f);
	updateSize << "UPDATE FEEDINFO SET Count = Count + 1, Size = Size + " << randomSize << " WHERE User = '" << ctx.getNickname() << "' AND Emoji = '" << args[0] << "'" ;
	m_db.exec(updateSize.str());
	query.reset();
	query.executeStep();

	messageStream << ctx.getMention() << ", Ты покормил(a): "  << args[0] << " , " << query.getColumn(3) << " раз(а). Размер = " << query.getColumn(4) << " см";
	ctx.send(messageStream.str());
	m_cooldownTimer[ctx.getNickname()] = std::time(nullptr) + 600;
}

void FeedCommand::sendStatus(MessageContext &ctx)
{
	std::stringstream statusStream;
	SQLite::Statement query(m_db, "SELECT Emoji, Count, Size FROM FEEDINFO WHERE User = ?");
	query.bind(1, ctx.getNickname());

	query.executeStep();
	if (!query.hasRow())
	{
		statusStream << ctx.getMention() << ", ты еще никого не кормил(а).";
		ctx.send(statusStream.str());
		return;
	}
	query.reset();

	statusStream << ctx.getMention() << ", ты покормил(а): ";
	while (query.executeStep())
	{
		statusStream << query.getColumn(0) << " - " << query.getColumn(1) << " раз(a), размер = " << query.getColumn(2) << "; ";
	}

	if (!isTimerOut(ctx.getNickname()))
	{
		int seconds = m_cooldownTimer[ctx.getNickname()] - std::time(nullptr);
		int minutes = seconds / 60;
		char temp[32] = {0};
		sprintf(temp, "%d:%02d", (minutes % 60), (seconds % 60));
		statusStream << "До следующей кормежки осталось: " << temp;
	}

	ctx.send(statusStream.str());
}

void FeedCommand::sendUserStatus(MessageContext &ctx, const std::string &nickname)
{
	std::stringstream statusStream;
	SQLite::Statement query(m_db, "SELECT Emoji, Count, Size FROM FEEDINFO WHERE User = ?");
	query.bind(1, nickname);

	query.executeStep();
	if (!query.hasRow())
	{
		statusStream << ctx.getMention() << " " << nickname << ", еще никого не кормил(а).";
		ctx.send(statusStream.str());
		return;
	}
	query.reset();

	statusStream << ctx.getMention() << " " << nickname << ", покормил(а): ";
	while (query.executeStep())
	{
		statusStream << query.getColumn(0) << " - " << query.getColumn(1) << " раз(a), размер = " << query.getColumn(2) << "; ";
	}

	if (!isTimerOut(nickname))
	{
		int seconds = m_cooldownTimer[ctx.getNickname()] - std::time(nullptr);
		int minutes = seconds / 60;
		char temp[32] = {0};
		sprintf(temp, "%d:%02d", (minutes % 60), (seconds % 60));
		statusStream << "До следующей кормежки осталось: " << temp;
	}

	ctx.send(statusStream.str());

}

bool FeedCommand::isValidEmoji(const std::string &emoji)
{
	bool res = std::find_if(AVAIBLE_EMOJIES.begin(), AVAIBLE_EMOJIES.end(), [&emoji](const char *e){ return !strcmp(emoji.c_str(), e); }) != AVAIBLE_EMOJIES.end();
	return res;
}

bool FeedCommand::isTimerOut(const std::string &user)
{
	if (m_cooldownTimer.find(user) != m_cooldownTimer.end())
	{
		std::time_t now = std::time(nullptr);
		if (m_cooldownTimer[user] > now)
		{
			return false;
		}
	}

	return true;
}
