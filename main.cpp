#include "Chat.h"
#include <mysql.h>

using namespace std;

//функция для создания базы данных чата
void createDBforChat() {
	
	MYSQL mysql;
	mysql_init(&mysql);
	if (&mysql == nullptr) {
	cout << "Error: can't create MySQL-descriptor" << endl;
	}
	if (!mysql_real_connect(&mysql, "localhost", "root", "0000", "testdb", NULL, NULL, 0)) {
		cout << "Error: can't connect to database " << mysql_error(&mysql) << endl;
	}
	mysql_query(&mysql, "CREATE TABLE users(id INT AUTO_INCREMENT PRIMARY KEY, name VARCHAR(255) NOT NULL, login VARCHAR(255) NOT NULL, pass VARCHAR(255)) NOT NULL)");
	mysql_query(&mysql, "CREATE TABLE messages(id INT AUTO_INCREMENT PRIMARY KEY, text VARCHAR(255), id_sender int NOT NULL REFERENCES users(id), id_reciever int NOT NULL REFERENCES users(id))");
	mysql_query(&mysql, "INSERT INTO users (name, login, pass) values('admin', 'admin', '0000'");
	mysql_query(&mysql, "INSERT INTO messages (text, id_sender, id_reciever) values('Hello', '1', '1'");
	mysql_close(&mysql);
	system("Pause");
}

int main()
{
//функция по автоматическому созданию таблиц 
	//createDBforChat();

	setlocale(LC_ALL, "Rus");
	
	MYSQL mysql;
	MYSQL_RES* res;
	MYSQL_ROW row;

	mysql_init(&mysql);

	if (&mysql == NULL) {
		cout << "Error: can't create MySQL-descriptor" << endl;
	}

	if (!mysql_real_connect(&mysql, "localhost", "root", "0000", "testdb", 0, NULL, 0)) {
		cout << "Error: can't connect to database " << mysql_error(&mysql) << endl;
	}
	else {
		cout << "Success!" << endl;
	}
	mysql_set_character_set(&mysql, "utf8");
	cout << "connection characterset: " << mysql_character_set_name(&mysql) << endl;
	mysql_query(&mysql, "SELECT * FROM users"); //Делаем запрос к таблице

	if (res = mysql_store_result(&mysql)) {
		while (row = mysql_fetch_row(res)) {
			for (int i = 0; i < mysql_num_fields(res); i++) {
				cout << row[i] << "  ";
			}
			cout << endl;
		}
	}
	else
		cout << "Ошибка MySql номер " << mysql_error(&mysql);
	mysql_close(&mysql);

	Chat ch;
	ch.start();

	return 0;
}
