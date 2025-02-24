#pragma once
#ifndef USER_H
#define USER_H
#endif // !USER_H

#include <mysql.h>
#include <mysql_driver.h>
#include <mysqlx/xdevapi.h>
#include <iostream>
#include <string>
#include <iterator>
#include <regex>
#include <vector>
#include <thread>
#include "practice_menu.h"




using namespace std;
typedef struct current
{
	std::string name;
	std::string password;
}current;
struct error_detail
{
    int main_id;
	int word_id;
	std::string name;
	std::string zh;
	std::string sources;
	int error_times;

};

class user
{
	user();
	virtual ~user();
private:
	void log_error(const std::string& msg)    //打印错误日志
    {
		std::cerr << "[ERROR] " << msg << std::endl;
	}           

	void log_mysql_error(MYSQL_STMT* stmt)            //打印预处理语句错误日志
    {
		std::cerr << "[DB ERROR] " << mysql_stmt_error(stmt) << std::endl;
	}
public:
	static user* get_instance() //单例模式
	{
		static user SQLManager;
		return &SQLManager;
	}
	std::string user_name;

public:
	void get_name(std::string new_name);     //账号传参
	bool insert_user(current &t); //新增用户
	bool check_password(string account, string password); //检查密码
    bool check_account(const std::string& account); //检查账号
    bool check_length(const std::string& account);   //检查账号或者密码长度

	
	//void login();//登录
	int welcome_page(); //欢迎界面，选择登录或注册
	bool create_error_book(std::string user_name);
	bool create_history_table(std::string user_name);
	void show_error_book();
    void show_history_table();

    bool record_info(std::vector<int> num, std::string sources, std::string mode);
    bool record_score(int score, double during, const std::string&source, const std::string& mode);
public:
	
	std::string escape_identifier(const std::string& input);
    class SafeBinder {
    public:
        SafeBinder() : params(nullptr), param_count(0) {}

        ~SafeBinder() {
            if (params) {
                delete[] params;
            }
        }

        void bind_int(unsigned int index, int& value) {
            ensure_capacity(index + 1);
            params[index].buffer_type = MYSQL_TYPE_LONG;
            params[index].buffer = &value;
            params[index].is_null = 0;
            params[index].length = 0;
        }

        void bind_str(unsigned int index, std::string& value, unsigned long length) {
            ensure_capacity(index + 1);
            params[index].buffer_type = MYSQL_TYPE_STRING;
            params[index].buffer = (void*)value.c_str();
            params[index].buffer_length = length;
            params[index].is_null = 0;
            params[index].length = &lengths[index];
            lengths[index] = static_cast<unsigned long>(value.size());
        }

        MYSQL_BIND* get_params() {
            return params;
        }

    private:
        void ensure_capacity(unsigned int count) {
            if (count > param_count) {
                MYSQL_BIND* new_params = new MYSQL_BIND[count];
                unsigned long* new_lengths = new unsigned long[count];
                for (unsigned int i = 0; i < param_count; ++i) {
                    new_params[i] = params[i];
                    new_lengths[i] = lengths[i];
                }
                delete[] params;
                delete[] lengths;
                params = new_params;
                lengths = new_lengths;
                param_count = count;
            }
        }

        MYSQL_BIND* params;
        unsigned long* lengths;
        unsigned int param_count;
    };
	//static string user_name;

private:
	MYSQL* con;
	const char* host = "localhost";
	const char* use = "root";
	const char* pw ="123456";
	const char* db_name = "wordlist";
	const int port = 3300;
};
class ConsoleColor {
public:
	ConsoleColor() { original = GetStdHandle(STD_OUTPUT_HANDLE); }
	~ConsoleColor() { SetConsoleTextAttribute(original, 0x07); }
	void set_red() { SetConsoleTextAttribute(original, FOREGROUND_RED); }
	void set_green() { SetConsoleTextAttribute(original, FOREGROUND_GREEN); }
private:
	HANDLE original;
};