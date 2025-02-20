#include "login.h"	
#include "user.h"
void login::login_into_system()
{

	//current new_add{ "zzzcx", "12345678Aa" };
	//user::get_instance()->insert_user(new_add);
	int choice = user::get_instance()->welcome_page(); //1.登录 2.注册 3.退出
	string account;
	string password;
	switch (choice)
	{
	case 1:
	{
		int retry_time = 0;
		do
		{
			std::cout << "Please enter your account and password.\n";

			std::cin >> account >> password;
			std::cout << "Checking your account and password...\n";
			retry_time++;
			if (retry_time == 3)
			{
				std::cout << "You have tried 3 times, please try again later.\n";
				std::this_thread::sleep_for(std::chrono::seconds(10)); //debug:程序休眠10s
				system("cls");
				break;
			}
			if (retry_time == 5)
			{
				std::cout << "You have tried 5 times, please  don't try again.\n";
				std::this_thread::sleep_for(std::chrono::seconds(5)); //debug:程序休眠10s
				exit(-1);
			}
		} while (!user::get_instance()->check_password(account, password));
		std::cout << "Authenticate succeed!\n" << endl;
		system("cls");
		break;
	}

	case 2:
	{
		current new_user; int j;
		std::cout << "Please enter your account ,it should be limited in 20 characters.\n";
		int retry_time = 0;
		do
		{
			std::cin >> new_user.name;
			retry_time++;
			if (retry_time == 3)
			{
				std::cout << "You have tried 3 times, please try again later.\n";
				std::this_thread::sleep_for(std::chrono::seconds(10)); //debug:程序休眠10s
				system("cls");
				
			}
			if (retry_time == 5)
			{
				std::cout << "You have tried 5 times, please  don't try again.\n";
				std::this_thread::sleep_for(std::chrono::seconds(5)); //debug:程序休眠10s
				exit(-1);
			}
		} while (new_user.name.size() > 20&& user::get_instance()->check_account(new_user.name));
		
		HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
		DWORD mode = 0;
		GetConsoleMode(hStdin, &mode);
		SetConsoleMode(hStdin, mode & (~ENABLE_ECHO_INPUT));

		std::cout << "Please enter your password, it should be limited in 20 characters.\n";
		retry_time = 0;
		do
		{
			std::cin >> new_user.password;
			retry_time++;
			if (retry_time == 3)
			{
				std::cout << "You have tried 3 times, please try again later.\n";
				std::this_thread::sleep_for(std::chrono::seconds(10)); //debug:程序休眠10s
				system("cls");

			}
			if (retry_time == 5)
			{
				std::cout << "You have tried 5 times, please  don't try again.\n";
				std::this_thread::sleep_for(std::chrono::seconds(5)); //debug:程序休眠10s
				exit(-1);
			}
		} while (new_user.password.size() > 20);

		if (!user::get_instance()->insert_user(new_user))
		{
			fprintf_s(stderr, "Failed to create your account.");
		}
		

	}
	}
}