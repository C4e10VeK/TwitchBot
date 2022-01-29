#include "FeedCommand.hpp"

FeedCommand::FeedCommand() : m_db("test.db", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE) { }

void FeedCommand::execute(MessageContext &ctx, const std::string &arg)
{
	if (arg == "rprtr258" || ctx.getNickname() == "rprtr258" || ctx.getNickname() == "trooba_bot")
	{
		ctx.send("@rprtr258, если ты nullptr, то твоя комманда не считается. Пиши эссе на 8 страничек coMMMMfy");
		return;
	}
	if (arg.empty())
	{
		ctx.send(ctx.getMention() + ", можно кормить -> VeryPog VeryPag VeryLark AAUGH");
		return;
	}
	if (arg == "status")
	{
		sendStatus(ctx);
		return;
	}
	
	if (!isValidEmoji(arg)) return;

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
	char updateCmd[512] = {0};
	SQLite::Statement query(m_db, "SELECT * FROM FEEDINFO WHERE User = ? AND Emoji = ?");
	query.bind(1, ctx.getNickname());
	query.bind(2, arg);

	query.executeStep();

	if (!query.hasRow())
	{
		char insertCmd[512] = {0};
		sprintf(insertCmd, "INSERT INTO FEEDINFO (User, Emoji, Count, Size) VALUES ('%s', '%s', %d, %f)", ctx.getNickname().c_str(), arg.c_str(), 0, 0.f);	
		m_db.exec(insertCmd);
		query.reset();
	}

	sprintf(updateCmd, "UPDATE FEEDINFO SET Count = Count + 1, Size = Size + 0.125 WHERE User = '%s' AND Emoji = '%s'", ctx.getNickname().c_str(), arg.c_str());
	m_db.exec(updateCmd);
	query.reset();
	query.executeStep();

	messageStream << ctx.getMention() << ", Ты покормил(a): "  << arg << " , " << query.getColumn(3) << " раз(а). Размер = " << query.getColumn(4) << " см";
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
