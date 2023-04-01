#include "Chat.h"

const char IP_SERV[] = "10.211.55.3";			// Enter local Server IP address
const int PORT_NUM = 4700;				// Enter Open working server port
const short BUFF_SIZE = 1024;			// Maximum size of buffer for exchange info between server and client

// Key variables for all program
int erStat;
in_addr ip_to_num;

using namespace std;

void Chat::start() {
	erStat = inet_pton(AF_INET, IP_SERV, &ip_to_num);

	if (erStat <= 0) {
		std::cout << "Error in IP translation to special numeric format" << std::endl;

	}
	WSADATA wsData;

	erStat = WSAStartup(MAKEWORD(2, 2), &wsData);

	if (erStat != 0) {
		std::cout << "Error WinSock version initializaion #";
		std::cout << WSAGetLastError();
	}
	else
		std::cout << "WinSock initialization is OK" << std::endl;

	SOCKET ServSock = socket(AF_INET, SOCK_STREAM, 0);

	if (ServSock == INVALID_SOCKET) {
		std::cout << "Error initialization socket # " << WSAGetLastError() << std::endl;
		closesocket(ServSock);
		WSACleanup();
	}
	else
		std::cout << "Server socket initialization is OK" << std::endl;

	// Server socket binding
	sockaddr_in servInfo;
	ZeroMemory(&servInfo, sizeof(servInfo));
	servInfo.sin_family = AF_INET;
	servInfo.sin_addr = ip_to_num;
	servInfo.sin_port = htons(PORT_NUM);

	erStat = bind(ServSock, (sockaddr*)&servInfo, sizeof(servInfo));

	if (erStat != 0) {
		std::cout << "Error Socket binding to server info. Error # " << WSAGetLastError() << std::endl;
		closesocket(ServSock);
		WSACleanup();
	}
	else
		std::cout << "Binding socket to Server info is OK" << std::endl;
	//Starting to listen to any Clients
	erStat = listen(ServSock, SOMAXCONN);

	if (erStat != 0) {
		std::cout << "Can't start to listen to. Error # " << WSAGetLastError() << std::endl;
		closesocket(ServSock);
		WSACleanup();
	}
	else {
		std::cout << "Listening..." << std::endl;
	}

	sockaddr_in clientInfo;
	ZeroMemory(&clientInfo, sizeof(clientInfo));

	int clientInfo_size = sizeof(clientInfo);

	SOCKET ClientConn = accept(ServSock, (sockaddr*)&clientInfo, &clientInfo_size);

	if (ClientConn == INVALID_SOCKET) {
		std::cout << "Client detected, but can't connect to a client. Error # " << WSAGetLastError() << std::endl;
		closesocket(ServSock);
		closesocket(ClientConn);
		WSACleanup();
	}
	else {
		std::cout << "Connection to a client established successfully" << std::endl;
		char clientIP[22];

		inet_ntop(AF_INET, &clientInfo.sin_addr, clientIP, INET_ADDRSTRLEN);
		std::cout << "Client connected with IP address " << clientIP << std::endl;
	}
	MYSQL mysql;
	MYSQL_RES* res;
	MYSQL_ROW row;
	mysql_init(&mysql);
	if (!mysql_real_connect(&mysql, "localhost", "root", "0000", "testdb", 0, NULL, 0)) {
		cout << "Error: can't connect to database " << mysql_error(&mysql) << endl;
	}
	else {
		cout << "Success!" << endl;
	}
	mysql_set_character_set(&mysql, "utf8");
	std::vector <char> servBuff(BUFF_SIZE), clientBuff(BUFF_SIZE);							// Creation of buffers for sending and receiving data
	short packet_size = 0;

	string choise;
	std::string log;
	std::string pas;

	string nameclientISLogin;

	while (true) {
		//servBuff = {};
		packet_size = recv(ClientConn, servBuff.data(), servBuff.size(), 0);
		choise = servBuff[0];
		
		
		if (choise != "0"&& choise != "1"&& choise != "2")
		{
		//	break;
		}
		if (choise == "0") { break; }
		if (choise == "2")
		{
			std::cout << "Client choise registration" << endl;


			packet_size = recv(ClientConn, servBuff.data(), servBuff.size(), 0);

			string name;
			name = servBuff.data();
			name.pop_back();
			cout << name << endl;

			packet_size = recv(ClientConn, servBuff.data(), servBuff.size(), 0);
			string log;
			log = servBuff.data();
			log.pop_back();
			cout << log << endl;

			packet_size = recv(ClientConn, servBuff.data(), servBuff.size(), 0);
			string pass;
			pass = servBuff.data();
			pass.pop_back();
			cout << pass << endl;
			string quer = "INSERT INTO users (name, login, pass) values('" + name + "', '" + log + "', '" + pass + "')";
			size_t y = quer.length();
			const char* qq = quer.c_str();
			mysql_query(&mysql, qq);
			continue;
			
		}
		if (choise == "1")
		{
			std::cout << "Client choise enter" << endl;

			packet_size = recv(ClientConn, servBuff.data(), servBuff.size(), 0);
			string log;
			log = servBuff.data();
			log.pop_back();
			cout << log << endl;

			packet_size = recv(ClientConn, servBuff.data(), servBuff.size(), 0);
			string pass;
			pass = servBuff.data();
			pass.pop_back();
			cout << pass << endl;
			bool correct_login = false;
			mysql_query(&mysql, "SELECT Login from users");
			if (res = mysql_store_result(&mysql)) {
				while (row = mysql_fetch_row(res)) {
					for (int i = 0; i < mysql_num_fields(res); i++) {
						if (row[i] == log) {
							cout << "log is ok" << endl;
							nameclientISLogin = log;
							correct_login = true;
							break;
						};
					}
				}
			}
			mysql_query(&mysql, "SELECT pass from users");
			bool correctpass = false;
			if (res = mysql_store_result(&mysql)) {
				while (row = mysql_fetch_row(res)) {
					for (int i = 0; i < mysql_num_fields(res); i++) {
						if (row[i] == pass) {
							cout << "pass is ok" << endl;
							correctpass = true;
							clientBuff.clear();
							clientBuff.push_back('1');

							packet_size = send(ClientConn, clientBuff.data(), clientBuff.size(), 0);
						};
					}
				}
			}
	
			while (correctpass  && correct_login) {
				std::cout << "Client Entered" << std::endl;

				packet_size = recv(ClientConn, servBuff.data(), servBuff.size(), 0);
				string q = servBuff.data();
				q.pop_back();
				char qq = q[0];
				if (q == "0")
				{
					std::cout << "Good Bye!" << std::endl;
					correctpass = false;
					correct_login = false;
					continue;
				
					}
				string quer01 = "SELECT text from messages where id_reciever = (select id from users where login = \"" + nameclientISLogin + "\")";
				int y = quer01.length();
				const char* qq3 = quer01.c_str();
				switch (qq)
				{
				case '1':
				
					mysql_query(&mysql, qq3);
					if (res = mysql_store_result(&mysql)) {
						while (row = mysql_fetch_row(res)) {
							for (int i = 0; i < mysql_num_fields(res); i++) {
								string e1 = row[i];
								clientBuff.clear();
								for (int j = 0; j < e1.length(); j++)
								{
									clientBuff.push_back(e1[j]);
								}
								clientBuff.push_back('\n');
								packet_size = send(ClientConn, clientBuff.data(), clientBuff.size(), 0);
							}
						}
					}
					break;
				case '2':
					//отправка всех имен клиенту
					mysql_query(&mysql, "SELECT name from users");
					if (res = mysql_store_result(&mysql)) {
						while (row = mysql_fetch_row(res)) {
							for (int i = 0; i < mysql_num_fields(res); i++) {
								string e = row[i];
								clientBuff.clear();
								for (int j = 0; j < e.length(); j++)
								{
									clientBuff.push_back(e[j]);
								}
								clientBuff.push_back('\n');
								packet_size = send(ClientConn, clientBuff.data(), clientBuff.size(), 0);
							}
						}
						
						packet_size = recv(ClientConn, servBuff.data(), servBuff.size(), 0);
						string name_reciever = servBuff.data();
						name_reciever.pop_back();
						cout << name_reciever << endl;
						packet_size = recv(ClientConn, servBuff.data(), servBuff.size(), 0);
						string text_message = servBuff.data();
						text_message.pop_back();
						cout << text_message << endl;
			
						string quer = "INSERT INTO messages (text, id_sender, id_reciever) values('" + text_message + "', (select id from users where login = \"" + nameclientISLogin + "\"), (select id from users where name = \"" + name_reciever + "\"))";
						size_t y = quer.length();
						const char* qq2 = quer.c_str();
						mysql_query(&mysql, qq2);
						break;

					}
				}
				continue;
			}
		}

		closesocket(ServSock);
		closesocket(ClientConn);
		WSACleanup();
	}
}
