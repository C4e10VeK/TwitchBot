#include <iostream>
#include <unordered_map>
#include <random>
#include <SQLiteCpp/SQLiteCpp.h>

#include "Utils.hpp"
#include "BotCore.hpp"

class FeedCommand
{
private:
	SQLite::Database m_db;
	const std::vector<const char*> AVAIBLE_EMOJIES = { "VeryPag", "VeryPog", "VeryLark", "AAUGH" };
	std::unordered_map<std::string, std::time_t> m_cooldownTimer;
public:
	FeedCommand() : m_db("test.db", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE) { }

	void execute(MessageContext &ctx, const std::string &arg)
	{
		if (!isValidEmoji(arg)) return;

		if (!isTimerOut(ctx.getNickname()))
		{
			ctx.send(ctx.getMention() + " Кормить можно раз в 30 минут coMMMMfy");
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

		messageStream << ctx.getMention() << ", Ты покормил: "  << arg << " , " << query.getColumn(3) << " раз(а). Рахмер = " << query.getColumn(4) << " см";
		ctx.send(messageStream.str());
		m_cooldownTimer[ctx.getNickname()] = std::time(nullptr) + 1800;
	}
private:
	bool isValidEmoji(const std::string &emoji)
	{
		bool res = std::find_if(AVAIBLE_EMOJIES.begin(), AVAIBLE_EMOJIES.end(), [&emoji](const char *e){ return !strcmp(emoji.c_str(), e); }) != AVAIBLE_EMOJIES.end();
		return res;
	}

	bool isTimerOut(const std::string &user)
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
};

int main()
{
	BotCore bot;
	FeedCommand feed{};
	bot.init("config.yaml");

	std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_int_distribution<int8_t> uni(0, 1);

	bot.connectCallback<CallbackType::MESSAGE>([&](MessageContext &ctx){
				std::clog << ctx.getNickname() << ": " <<  ctx.getMessage() << std::endl;

				if (ctx.getMessage().compare(0, ctx.getPrefix().size(), ctx.getPrefix()) != 0)
					return;

				std::string line = ctx.getMessage().substr(ctx.getPrefix().size());
				std::string command = line.substr(0, line.find(" "));
				std::string arg = line.substr(command.size() + 1);

				if (command == "feed" && !arg.empty())
				{	
					feed.execute(ctx, arg);
					return;
				}
				if (command == "анимефикация" && !arg.empty())
				{
					ctx.send(arg + " Ты стал анимешником (соц. статуc = -9999999)");
					return;
				}
				if (command == "пистолетов")
				{
					ctx.send("Говно ваш Пистолетов coMMMMfy");
					return;
				}
				if (command == "забанить" && !arg.empty())
				{
					bool res = uni(rng);
					std::string msg = res ? " Нет" : " Да";
					msg += ". Забанить " + arg;
					ctx.send(ctx.getMention() + msg);
					return;
				}
			});	

	bot.run();
	
	return 0;
}

