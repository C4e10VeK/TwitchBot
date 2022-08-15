#pragma once

#include "../MessageContext.hpp"
#include <vector>

class ICommand
{
public:
	virtual ~ICommand() = default;

	virtual void execute(MessageContext &ctx, const std::vector<std::string> &args) = 0;
};

