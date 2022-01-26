#pragma once

#include <string>

class IRCClient
{
private:
	int m_socket;
	bool m_connected;
public:
	IRCClient();
	~IRCClient();

	bool isConnected() const;

	void connect(const char *host, const char *port);

	void send(const std::string &msg);

	std::string receive();

	void disconnect();
};
