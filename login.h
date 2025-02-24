#pragma once
#ifndef LOGIN_H
#define LOGIN_H
#endif // !LOGIN_H

#include "user.h"
#include <string>
#include <windows.h>
#include <thread>
#include <chrono>
#include <conio.h>
#include "CheckIn.h"
class login
{
public:
	static login* get_instance()
	{
		static login login_action;
		return &login_action;
	}
public:
private:
	std::string handle_login(HANDLE hConsole);
	std::string handle_register(HANDLE hConsole);
	void handle_checkin(const std::string& account, HANDLE hConsole);
public:
	std::string login_into_system();
	//std::string user_name;
	
	
};

