#include <iostream>
#include "mysql/mysql.h"

using namespace std;

int main() {
	MYSQL mysql; // Дескриптор соединения c MySql
	mysql_init(&mysql);

	if (&mysql == NULL) {
		// Если дескриптор не получен — выводим сообщение об ошибке
		cout << "Error: can't create MySQL-descriptor" << endl;
	}

	// Подключаемся к серверу
	if (!mysql_real_connect(&mysql, "localhost", "eduard", "300974", "testdb", 0, NULL, 0)) {
		// Если нет возможности установить соединение с БД выводим сообщение об ошибке
		cout << "Error: can't connect to database " << mysql_error(&mysql) << endl;
	}
	else {
		// Если соединение успешно установлено выводим фразу — "Success!"
		cout << "Success!" << endl;
	}

	// Закрываем соединение с базой данных
	mysql_close(&mysql);

	system("Pause");
	return 0;
}



