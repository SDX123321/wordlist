#pragma once
#include <mysql.h>
#include <string>
#include <vector>
#include <functional>
#include <iostream>
using namespace std;
struct word {
	string name;
	string zh;
	int ID;
	string yinbiao;
};
class words
{
public:
	friend string get_zh_cet_4(int ID);
	friend string get_name_cet_4(int ID);
	friend string get_yinbiao_cet_4(int ID);
	friend string get_zh_cet_6(int ID);
	friend string get_name_cet_6(int ID);
	friend string get_yinbiao_cet_6(int ID);
	

	
	
public:
	friend void add_word();
	
public:
	static words* get_instance()
	{
		static words word_action;
		return &word_action;
	}
public:
	words();
	~words();
private:
	bool check_word(string new_word);
	string get_word(int ID, string select, string sources);
private:
	MYSQL* con2;
	const char* host2 = "localhost";
	const char* user2 = "root";
	const char* pw2 = "123456";
	const char* db_name2 = "wordlist";
	const int port2 = 3300;
};

