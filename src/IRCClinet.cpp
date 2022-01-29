#include "IRCClinet.hpp"

#include <iostream>
#include <sstream>

#ifdef _WIN32
#include <WinSock2.h>
#include <WS2tcpip.h>
#else
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#endif

IRCClient::IRCClient() : m_socket(-1), m_connected(false) { }

IRCClient::~IRCClient()
{
	disconnect();
#ifdef _WIN32
	closesocket(m_socket);
	WSACleanup();
#else
	close(m_socket);
#endif
}

bool IRCClient::isConnected() const
{
	return m_connected;
}

void IRCClient::connect(const char *host, const char *port)
{
	int err;

#ifdef _WIN32
	WSADATA wsaData;
	err = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (err != 0)
	{
		std::cerr << "WSAStartup failed. Error = " << err << std::endl;
		return;
	}
#endif

	addrinfo addr{}, *addrList;
	addr.ai_family = AF_UNSPEC;
	addr.ai_socktype = SOCK_STREAM;
	err = getaddrinfo(host, port, &addr, &addrList);
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
	freeaddrinfo(addrList);
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

	std::string res(1024, '\0');
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

