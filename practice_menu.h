#pragma once
#define DEF 10
#include <mysql.h>

#include<iostream>
#include<time.h>
#include<cstdlib>
#include <windows.h>
#include<ctime>
#include<stdio.h>
#include<vector>
#include<algorithm>
#include <string>
#include <functional>
#include<io.h>
#include<fcntl.h>//ʹ��_setmode(_fileno(stdout), _O_U16TEXT)����ӵ�ͷ�ļ�
#include"practice_menu.h"
#include "user.h"
struct word {
	std::string name;
	std::string zh;
	int ID;
	std::string pronunciation;
};

class words
{
public:
	std::string get_zh_cet_4(int ID);
	std::string get_name_cet_4(int ID);
	std::string get_pronunciation_cet_4(int ID);
	std::string get_zh_cet_6(int ID);
	std::string get_name_cet_6(int ID);
	std::string get_pronunciation_cet_6(int ID);
	bool check_word(std::string word);
	std::string get_word(int ID, std::string select, std::string sources);

	

public:
	friend void add_word();
	friend void delete_word();

public:
	static words* get_instance()
	{
		static words word_action;
		return &word_action;
	}
public:
	words();
	virtual ~words();

private:
	MYSQL* con2;
	const char* host2 = "localhost";
	const char* user2 = "root";
	const char* pw2 = "123456";
	const char* db_name2 = "wordlist";
	const int port2 = 3300;
};

class word_action : public words
{
public:
	friend class words;
	friend void add_word();
	friend void delete_word();
	friend void view_words();
};

class practice_sub_menu
{
public:
	friend class Wordlist;
	int practice_choose_menu();
	int practice_choose_menu_b();
	static practice_sub_menu* get_instance()
	{
		static practice_sub_menu n;
		return &n;
	}
};
class practice_main_menu
{
	friend class Wordlist;
public:
	int main_choose_menu();
	static practice_main_menu* get_instance()
	{
		static practice_main_menu n;
		return &n;
	}
};
class Wordlist
{
    friend class error_book;
	friend class main_b;
	friend class error_book;
public:
	void get_name(std::string user_name)
	{ 
		user = user_name;
	}
	std::string user;
	int accept_CET;
	int accept_mode;
	static int e;//����qu�����е��±�
	void Wordlist_a();
	void Init();
	int error_a(std::string correct);//����Ӣ�жϣ������1
	int error_b(std::string correct);//Ӣ�����ж�
	void remind();//��������
	void drop(int& score);//�۷�
	int show(int pcur, std::string name, std::string zh, std::string pronunciation, int ID, int second);//չʾ����
	void random_a(int choice);
	static Wordlist* get()
	{
		static Wordlist n;
		return &n;
	}
};
class error_book
{
public:
	error_book();
	void show_error_book();
	static error_book* get_instance() {
		static error_book n;
        return &n;
	}
};
