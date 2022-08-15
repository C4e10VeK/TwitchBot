#pragma once

#include "ICommand.hpp"
#include "../Utils.hpp"

class BanJokeCommand : public ICommand
{
public:
	void execute(MessageContext &ctx, const std::vector<std::string> &args) override
	{
		if (args.empty()) return;
		int8_t res = random(-6, 6);
		std::string msg = res < 0 ? " Нет. Не банить " : " Да. Забанить ";
		msg += args[0];
		ctx.send(ctx.getMention() + msg);
	}
};

