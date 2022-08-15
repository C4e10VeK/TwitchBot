#include <iostream>
#include <unordered_map>
#include <random>
#include <range/v3/view.hpp>
#include <range/v3/range.hpp>

#include "Utils.hpp"
#include "BotCore.hpp"
#include "commands/FeedCommand.hpp"
#include "commands/BanJokeCommand.hpp"

inline void onReactVeryLark(MessageContext &ctx)
{
	auto words = ctx.getMessage() | 
		ranges::views::split(' ') | 
		ranges::views::transform([](auto &&rng){ return std::string(&*rng.begin(), ranges::distance(rng)); }) |
		ranges::views::filter([](const std::string &s){ return s == "pirat"; }) |
		ranges::to_vector;

	if (!words.empty())
	{
		std::stringstream piratReact;
		for (const auto &s : words)
			piratReact << s << " ";
					
		ctx.send(piratReact.str());
	}
}

int main()
{
	BotCore bot;
	bot.init("config.yaml");
	bot.addCommand<FeedCommand>("feed");
	bot.addCommand<BanJokeCommand>("забанить");

	bot.connectCallback([&](MessageContext &ctx){

				onReactVeryLark(ctx);

				if (ctx.getMessage().compare(0, ctx.getPrefix().size(), ctx.getPrefix()) != 0)
					return;

				std::string line = ctx.getMessage().substr(ctx.getPrefix().size());
				std::string command = line.substr(0, line.find(" "));	

				if (command == "SHUTDOWN" && ctx.getNickname() == "chelovchik")
					bot.stop();
			});	

	bot.run();
	
	return 0;
}

