#include <iostream>
#include <string>
#include <termios.h>
#include <unistd.h>
#include "mysql/mysql.h"

using namespace std;

char _getch(){
	char buf = 0;
		struct termios old = { 0 };
		fflush(stdout);
		if (tcgetattr(0, &old) < 0)
			perror("tcsetattr()");
		old.c_lflag &= ~ICANON;
		old.c_lflag &= ~ECHO;
		old.c_cc[VMIN] = 1;
		old.c_cc[VTIME] = 0;
		if (tcsetattr(0, TCSANOW, &old) < 0)
			perror("tcsetattr ICANON");
		if (read(0, &buf, 1) < 0)
			perror("read()");
		old.c_lflag |= ICANON;
		old.c_lflag |= ECHO;
		if (tcsetattr(0, TCSADRAIN, &old) < 0)
			perror("tcsetattr ~ICANON");
		
		return buf;
}

int main() {
	MYSQL mysql; // Дескриптор соединения c MySql
	MYSQL_RES* res;
	MYSQL_ROW row;
	mysql_init(&mysql);
	

	string str_tmp;
	char username_db[20]{};
	char password_db[20]{};
	const char* database_name = "chat_test";
	bool database = false;
	const char* table_users_name = "users";
	bool table = false;
	bool connetion_db = false;
	int notdb = 1049;

	if (&mysql == NULL) {
		// Если дескриптор не получен — выводим сообщение об ошибке
		cout << "Error: can't create MySQL-descriptor" << endl;
	}

	while(connetion_db == false){
		cout << " Enter username of your database: ";
		for (int i = 0; i < 20; i++){
			username_db[i] = _getch();
			cout << username_db[i];
			if (username_db[i] == 10){
				username_db[i] = '\0';
				cout << endl;
				break;
			}
		}	

		cout << " Enter password of your database: ";
			for (int i = 0; i < 20; i++){
			password_db[i] = _getch();
			cout << "*";
			if (password_db[i] == 10){
				password_db[i] = '\0';
				cout << endl;
				break;
			}
		}	
	
		// Подключаемся к серверу
		if (!mysql_real_connect(&mysql, "localhost", username_db, password_db, database_name, 3306, NULL, 0)) {
			// Если нет возможности установить соединение с БД выводим сообщение об ошибке
			cout << "Error: can't connect to database " << mysql_error(&mysql) << endl;
			if (mysql_errno(&mysql) == notdb){
				connetion_db = true;
				cout << " need for create database" << endl;
				database = true;
				
			}
		
		}
		else {
			// Если соединение успешно установлено выводим фразу — "Success!"
			cout << "Success!" << endl;
			connetion_db = true;
		}
	}

	if (database == true){
		mysql_real_connect(&mysql, "localhost", username_db, password_db, NULL, 0, NULL, 0);
		mysql_query(&mysql, "CREATE DATABASE chat_test");
		cout << " Database " << database_name << " created" << endl;
		mysql_query(&mysql, "use chat_test");
		mysql_query(&mysql, "CREATE TABLE users(id INT AUTO_INCREMENT PRIMARY KEY, name VARCHAR(255)))");
		mysql_query(&mysql, "INSERT INTO users(id, name) VALUES (default, 'ALL USERS')");
		cout << " Table created" << endl;
	}

	mysql_set_character_set(&mysql, "utf8");
	cout << " connection characterset " << mysql_character_set_name(&mysql) << endl; 
	
	mysql_query(&mysql,"SELECT * FROM users");

	if( res = mysql_store_result(&mysql)){
		while(row = mysql_fetch_row(res)){
			for (int i = 0; i < mysql_num_fields(res); i++){
				cout << row[i] << " ";
			}
			cout << endl;
		}
	}
	else cout << " Ошика mysql номер: " << mysql_error(&mysql);

	// Закрываем соединение с базой данных
	mysql_close(&mysql);

	system("Pause");
	return 0;
}



