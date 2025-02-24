#pragma once
#ifndef CHECKIN_H
#define CHECKIN_H
#endif // !CHECKIN_H
#include <mysql.h>
#include <string>
#include <iostream>
#include <stdio.h>

class CheckIn
{
public:
	CheckIn();
	~CheckIn();
public:	
	bool check_in(std::string user_name);
	int get_checkin_days(std::string user_name);
	std::string get_last_checkin_day(std::string user_name);
public:
	static CheckIn* get_instance() 
	{
		static CheckIn CheckIner;
		return &CheckIner;
	}
private:

	MYSQL* con;
	const char* host = "localhost";
	const char* use = "root";
	const char* pw = "123456";
	const char* db_name = "wordlist";
	const int port = 3300;
};

