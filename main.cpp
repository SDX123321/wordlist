#pragma comment( lib,"winmm.lib" )
#include "login.h"
#include "practice_menu.h"



int main()
{
	
	std::string user_name = login::get_instance()->login_into_system();
	Wordlist A;
	/*A.get_name(user_name);*/
//user::get_instance()->user_name = "admin";
//user::get_instance()->get_error_book();
	while (true)
	{
		
		switch (practice_main_menu::get_instance()->main_choose_menu())
		{
		case 1:A.Wordlist_a();
			break;
		case 2: user::get_instance()->show_error_book();
			break;
		case 3:user::get_instance()->show_history_table();
			break;
		case 4:
            user::get_instance()->show_own_wordlist();
			break;
		case 5: 
			user::get_instance()->get_error_book();

			break;
		case 6:
			user::get_instance()->refound_password(user::get_instance()->user_name,true);
			break;
		case 7:
			exit(0);
		}
		
	}
	
	
	
}
