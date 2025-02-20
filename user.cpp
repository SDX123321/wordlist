#include "user.h"
constexpr auto CRT_SECURE_NO_WARNINGS = 1;
using namespace std;
user::user()
{
	con = mysql_init(NULL);
	mysql_options(con, MYSQL_SET_CHARSET_NAME, "gbk");
	
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
	sprintf_s(sql, "insert into user(name,password) values('%s','%s')", person.name.c_str(), person.password.c_str());
	if (mysql_query(con, sql))
	{
		fprintf_s((__acrt_iob_func(2)), "Failed to create account.:Error%s\n", mysql_error(con));
		return false;
	}
	else return true;
}

bool user::check_password(string account,string password)
{
	char sql[1024];
	sprintf_s(sql, "SELECT password FROM user WHERE name = '%s'AND password = '%s' ", account.c_str(),password.c_str());
	
	if (mysql_query(con, sql))
	{
		fprintf_s(stderr, "Failed to check password.:Error%s\n", mysql_error(con));
		return false;
	}
	MYSQL_RES* result = mysql_store_result(con);
	MYSQL_ROW row;
	if (row = mysql_fetch_row(result)) return true;
	return false;
}

bool user::check_account(string account)
{
	char sql[1024];
	sprintf_s(sql, "SELECT password FROM user WHERE name = '%s'", account.c_str());

	if (mysql_query(con, sql))
	{
		fprintf_s(stderr, "Failed to check password.:Error%s\n", mysql_error(con));
		return false;
	}
	MYSQL_RES* result = mysql_store_result(con);
	MYSQL_ROW row;
	if (row = mysql_fetch_row(result)) return false;
	return true;
}





int user::welcome_page()
{
	int choice;
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
		std::cout << "Exiting..." << endl;
		exit(0);
	}
	return choice;
	
}


