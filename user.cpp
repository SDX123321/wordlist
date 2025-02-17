#include "user.h"


user::user()
{
	MYSQL* con = mysql_init(NULL);
	mysql_options(con, MYSQL_SET_CHARSET_NAME, "GBK");
	if (!mysql_real_connect(con, host, use, pw, db_name, port, NULL, 0))
	{
		printf("Error connecting to database:%s\n", mysql_error(con));
		exit(-1);
	}
}
user::~user()
{
	mysql_close(user::con);
}

bool user::insert_user(current& person)
{
	char sql[1000];
	sprintf(sql, "insert into user(name,password) values('%s','%s')", person.name.c_str(), person.password.c_str());
	if (mysql_query(con, sql))
	{
		fprintf(stderr, "Failed to create account.:Error%s\n", mysql_error(con));
		return false;
	}
	return true;
}

bool user::check_password(int account, char pwd)
{
	char sql[1024];
	sprintf(sql, "SELECT * FROM user WHERE name = '%s' AND password = '%s'", account, pwd);
	if (mysql_query(con, sql)|| !mysql_store_result(con))
		{
		return false;
		}
	return true;
}

int user::welcome_page(int choice)
{
	std::cout << "\n====背单词系统====\n";
	std::cout << "1.登录\n";
	std::cout << "2.注册\n";
	std::cout << "3.直接退出\n";
	do {
		std::cout << "请输入您的选择:";
		std::cin >> choice;
	} while (choice < 1 || choice>3);
	if (choice == 3)
	{
		exit(0);
	}
	return choice;
	
}


