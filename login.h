#pragma once
#include "user.h"
#include <string>
#include <windows.h>
#include <thread>
#include <chrono>
#include <conio.h>
class login
{
public:
	static login* get_instance()
	{
		static login login_action;
		return &login_action;
	}
public:
	current new_login;
	current new_register;
	void login_into_system(); 
	
	
};

