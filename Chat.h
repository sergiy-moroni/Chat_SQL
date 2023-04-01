#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <filesystem>
#include <mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

class Chat
{
public:
	
	Chat() = default;
	~Chat() = default;

	void start();

	};

