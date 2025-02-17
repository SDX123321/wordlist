#pragma once
#include <mysql.h>
#include <iostream>
#include <string>

using namespace std;
typedef struct current
{
	int user_id;
	std::string name;
	std::string password;
}user;
class user
{
	user();
	~user();
public:
	static user* get_instance()
	{
		static user SQLManager;
		return &SQLManager;
	}
public:
	bool insert_user(current &t); //新增用户
	bool check_password(int account, char pwd); //检查密码
	
private:
	MYSQL* con;
	const char* host = "localhodt";
	const char* use = "root";
	const char* pw = "123456";
	const char* db_name = "wordlist";
	const int port = 3300;
};
