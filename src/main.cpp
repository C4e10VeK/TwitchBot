#include <iostream>
#include "BotCore.hpp"

int main()
{	
	BotCore bot;
	bot.init("config.yaml");

	bot.setMessageCallback([](MessageContext &ctx){
				std::clog << ctx.getNickname() << ": " <<  ctx.getMessage() << std::endl;

				if (ctx.getMessage().compare(0, ctx.getPrefix().size(), ctx.getPrefix()) != 0)
					return;

				if (ctx.getNickname() == "a2p1k04")
				{
					ctx.send("@a2p1k04 У тебя перманентный бан в боте coMMMMfy");
					return;
				}

				std::string line = ctx.getMessage().substr(ctx.getPrefix().size());
				std::string command = line.substr(0, line.find(" "));
				std::string arg = line.substr(command.size());

				if (command == "киндер" || command == "kinder")
					ctx.send(ctx.getMention() + ", ты реально подумал что я буду тоже как все киндер делать? VeryPag");
				if (command == "аниме")
					ctx.send(ctx.getMention() + " Говно твое аниме!!! coMMMMfy");
				if (command == "буллинг" && !arg.empty())
					ctx.send(arg + " Ты стал анимешником (соц. статуc = -9999999)");
				if (command == "пистолетов")
					ctx.send("Говно ваш Пистолетов coMMMMfy");
			});	

	bot.run();	
	
	return 0;
}

