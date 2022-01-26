#include "UserInfo.hpp"

#include <regex>

UserInfo UserInfo::getChatUserInfo(const std::string &msg)
{
	UserInfo user;
	std::regex r(":([a-z0-9_]+)!\\1@\\1.tmi.twitch.tv PRIVMSG #([a-z0-9_]+) :(.*)");
	std::smatch matches;

	if (!std::regex_search(msg, matches, r)) return { "", "", "", "", "" };

	user.rawMessage = matches[0].str();
	user.nickname = matches[1].str();
	user.channel = matches[2].str();
	user.message = matches[3].str();
	user.mention = "@" + user.nickname;

	return user;
}
