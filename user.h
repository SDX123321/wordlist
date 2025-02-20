#pragma once
#include <mysql.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
typedef struct current
{
	
	std::string name;
	std::string password;
}current;
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
	bool check_password(string account, string password); //检查密码
	bool check_account(string account);
	//void login();//登录
	int welcome_page(); //欢迎界面，选择登录或注册
	
	
private:
	MYSQL* con;
	const char* host = "localhost";
	const char* use = "root";
	const char* pw ="123456";
	const char* db_name = "wordlist";
	const int port = 3300;
};
