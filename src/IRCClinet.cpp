#include "IRCClinet.hpp"

#include <iostream>
#include <unistd.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>

IRCClient::IRCClient() : m_socket(-1), m_connected(false) { }

IRCClient::~IRCClient()
{
	disconnect();
	close(m_socket);
}

bool IRCClient::isConnected() const
{
	return m_connected;
}

void IRCClient::connect(const char *host, const char *port)
{
	addrinfo addr{}, *addrList;
	addr.ai_family = AF_UNSPEC;
	addr.ai_socktype = SOCK_STREAM;
	int err = getaddrinfo(host, port, &addr, &addrList);
	if (err < 0)
	{
		std::cerr << "Error while connecting to: " << host << ":" << port << std::endl;
		return;
	}

	m_socket = socket(addrList->ai_family, addrList->ai_socktype, addrList->ai_protocol);
	if (m_socket < 0)
	{
		std::cerr << "Error while connecting to: " << host << ":" << port << std::endl;
		return;
	}

	err = ::connect(m_socket, addrList->ai_addr, addrList->ai_addrlen);
	if (err < 0)
	{
		disconnect();
		std::cerr << "Error while connecting to: " << host << ":" << port << std::endl;
		return;
	}
	m_connected = true;
}

void IRCClient::send(const std::string &msg)
{
	if (!m_connected) return;

	std::string message = msg + "\r\n";

	int res = ::send(m_socket, message.c_str(), message.size(), 0);
	if (res < 0)
	{
		std::cerr << "Error while sending messgae" << std::endl;
		disconnect();
	}
}

std::string IRCClient::receive()
{
	if (!m_connected) return "";

	std::string res(512, '\0');
	int err = recv(m_socket, res.data(), res.size(), 0);
	if (err < 0)
	{
		std::cerr << "Error while receive message" << std::endl;
		disconnect();
	}
	return res;
}

void IRCClient::disconnect()
{
	m_connected = false;
}

