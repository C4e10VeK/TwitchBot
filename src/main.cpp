#include <iostream>
#include <unordered_map>
#include <random>
#include <range/v3/view.hpp>
#include <range/v3/range.hpp>

#include "Utils.hpp"
#include "BotCore.hpp"
#include "FeedCommand.hpp"

int main()
{
	BotCore bot;
	FeedCommand feed{};
	bot.init("config.yaml");

	bot.connectCallback([&](MessageContext &ctx){
				std::clog << ctx.getNickname() << ": " <<  ctx.getMessage() << std::endl;

				auto words = split(ctx.getMessage(), " ");

				auto filtered = words | ranges::views::filter([](const std::string &s){ return s == "pirat"; }) | ranges::to_vector;

				if (!filtered.empty())
				{
					std::stringstream piratReact;
					for (const auto &s : filtered)
						piratReact << s << " ";
					
					ctx.send(piratReact.str());
				}

				if (ctx.getMessage().compare(0, ctx.getPrefix().size(), ctx.getPrefix()) != 0)
					return;

				std::string line = ctx.getMessage().substr(ctx.getPrefix().size());
				std::string command = line.substr(0, line.find(" "));
				std::string arg = command.size() + 1 <= line.size() ? line.substr(command.size() + 1) : "";
				trim(arg);

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
					int8_t res = random(-6, 6);
					std::string msg = res < 0 ? " Нет. Не банить" : " Да. Забанить";
					ctx.send(ctx.getMention() + msg);
					return;
				}
				if (command == "SHUTDOWN" && ctx.getNickname() == "chelovchik")
					bot.stop();
			});	

	bot.run();
	
	return 0;
}

