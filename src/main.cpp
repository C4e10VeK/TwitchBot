#include <iostream>
#include <unordered_map>
#include <random>
#include <SQLiteCpp/SQLiteCpp.h>

#include "Utils.hpp"
#include "BotCore.hpp"
#include "FeedCommand.hpp"

int main()
{
	BotCore bot;
	FeedCommand feed{};
	bot.init("config.yaml");

	std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_int_distribution<int8_t> uni(-6, 6);

	bot.connectCallback([&](MessageContext &ctx){
				std::clog << ctx.getNickname() << ": " <<  ctx.getMessage() << std::endl;

				if (ctx.getMessage().compare(0, ctx.getPrefix().size(), ctx.getPrefix()) != 0)
					return;

				std::string line = ctx.getMessage().substr(ctx.getPrefix().size());
				std::string command = line.substr(0, line.find(" "));
				std::string arg = command.size() + 1 <= line.size() ? line.substr(command.size() + 1) : "";	

				if (command == "feed")
				{	
					feed.execute(ctx, arg);
					return;
				}
				if (command == "анимефикация" && !arg.empty())
				{
					ctx.send(arg + " Ты стал анимешником (соц. статуc = -9999999)");
					return;
				}	
				if (command == "забанить" && !arg.empty())
				{
					int8_t res = uni(rng);
					std::string msg = res < 0 ? " Нет. Не банить" : " Да. Забанить";
					ctx.send(ctx.getMention() + msg);
					return;
				}
			});	

	bot.run();
	
	return 0;
}

