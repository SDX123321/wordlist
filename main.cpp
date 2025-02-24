#pragma comment( lib,"winmm.lib" )
#include "login.h"
#include "practice_menu.h"
#include "essyX.h"


int main()
{
	
	std::string user_name = login::get_instance()->login_into_system();
	Wordlist A;
	A.get_name(user_name);
	while (true)
	{
		switch (practice_main_menu::get_instance()->main_choose_menu())
		{
		case 1:A.Wordlist_a();
			break;
		case 2: user::get_instance()->show_error_book();
			break;
		case 3:user::get_instance()->show_history_table();
		}
	}
	
	
	
}
