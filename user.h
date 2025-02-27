#pragma once
#ifndef USER_H
#define USER_H
#endif // !USER_H

#include <mysql.h>
#include <mysql_driver.h>
#include <mysqlx/xdevapi.h>
#include <iostream>
#include <string>
#include <conio.h>
#include <iterator>
#include <regex>
#include <stdio.h>
#include <vector>
#include <thread>
#include "practice_menu.h"
#include <type_traits>


using namespace std;
typedef struct current
{
	std::string name;
	std::string password;
}current;

typedef struct {
    int main_id;
    string name;
    string zh;
    string sources;
    int word_id;
    unsigned long name_length;
    unsigned long zh_length;
    unsigned long sources_length;
} result_data;


class user
{
public:
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
   // void get_host() {
        //std::string new_host;
        //cout << "请输入主机名，默认留空为localhost" << endl;
        //cin>>new_host;
        //if (new_host.empty())
       // {
       //     new_host = "localhost";
       // }
       // host = new_host;
   // }
    //void set_pwd(){
       // cout << "是否要使用远端服务器密码？" << endl;
       // string response;
        //cin >> response;
   // if (response == "y")
       // {
        //   //////////////////////////
       // }
       
   // }
    void get_name(std::string new_name);     //账号传参
	bool insert_user(current &t); //新增用户
	
    bool refound_password(string account, bool restore);
    bool check_password(string account, string password); //检查密码
    bool update_password(string account, string password);
    bool check_account(const std::string& account); //检查账号
    bool check_length(const std::string& account);   //检查账号或者密码长度

	
	//void login();//登录
	int welcome_page(); //欢迎界面，选择登录或注册
	
    
    bool create_error_book(std::string user_name);
	bool create_history_table(std::string user_name);
    bool create_own_wordlist(std::string user_name);
	
    
    void show_error_book();
    bool get_error_book();
    void show_history_table();
    void show_own_wordlist();

    bool errorpp(vector <string> wrong_list);
    bool errorcc(vector <string> right_list);

    bool addw();
    bool deletew();
    bool searchw();
    bool showall();
    
    bool record_info(std::vector<int> num, std::string sources, std::string mode);
    bool record_score(int score, double during, const std::string&source, const std::string& mode);
public:
	
	std::string escape_identifier(const std::string& input);
    std::vector <result_data> result_get;

        MYSQL_BIND* get_params() {
            return params;
        }
        
        
        bool first_connect = true;
        ///////////////////////////
//////////////////////////////

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
private:
    MYSQL* con;
    const char* use = "root";
    const char* db_name = "wordlist";
    const int port = 3300;
};
	//static string user_name;



class ConsoleColor {
public:
	ConsoleColor() { original = GetStdHandle(STD_OUTPUT_HANDLE); }
	~ConsoleColor() { SetConsoleTextAttribute(original, 0x07); }
	void set_red() { SetConsoleTextAttribute(original, FOREGROUND_RED); }
	void set_green() { SetConsoleTextAttribute(original, FOREGROUND_GREEN); }
private:
	HANDLE original;
};
