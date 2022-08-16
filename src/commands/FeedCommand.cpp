#include "FeedCommand.hpp"
#include <algorithm>
#include <sstream>
#include <chrono>

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

	if (!isTimerOut(ctx.getNickname(), args[0]))
	{
		SQLite::Statement query(m_db, "SELECT Cooldown FROM FEEDINFO WHERE User = ? AND Emoji = ?");
		query.bind(1, ctx.getNickname());
		query.bind(2, args[0]);
		query.executeStep();

		std::stringstream timerMessage;
		time_t time = query.getColumn(0).getInt64() - std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());	
		timerMessage << ctx.getMention() << " Кормить можно раз в 10 минут coMMMMfy . Осталось: " << std::put_time(std::localtime(&time), "%M:%S");
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
	time_t cooldown = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	updateSize << "UPDATE FEEDINFO SET Count = Count + 1, Size = Size + " << randomSize 
		<< ", Cooldown = " << cooldown + 600
		<< " WHERE User = '" << ctx.getNickname() 
		<< "' AND Emoji = '" << args[0] << "'" ;

	m_db.exec(updateSize.str());
	query.reset();
	query.executeStep();

	messageStream << ctx.getMention() << ", Ты покормил(a): "  << args[0] << " , " << query.getColumn(3) << " раз(а). Размер = " << query.getColumn(4) << " см. " <<
		"До следующий кормежки: 10:00";
	ctx.send(messageStream.str());
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

	ctx.send(statusStream.str());

}

bool FeedCommand::isValidEmoji(const std::string &emoji)
{
	bool res = std::find_if(AVAIBLE_EMOJIES.begin(), AVAIBLE_EMOJIES.end(), [&emoji](const char *e){ return !strcmp(emoji.c_str(), e); }) != AVAIBLE_EMOJIES.end();
	return res;
}

bool FeedCommand::isTimerOut(const std::string &user, const std::string &emoji)
{
	SQLite::Statement query(m_db, "SELECT Cooldown FROM FEEDINFO WHERE User = ? AND Emoji = ?");
	query.bind(1, user);
	query.bind(2, emoji);

	query.executeStep();
	if (!query.hasRow())
		return true;

	time_t cooldown = query.getColumn(0);

	if (cooldown > std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()))
		return false;

	return true;
}
