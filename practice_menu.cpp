#include "practice_menu.h"
#include "user.h"
#include <thread>
#include <chrono>
#include <limits>

std::vector <int> ID;//统计错题
std::vector <int> siliu;
std::vector <int> zhying;
std::vector <int> qu;
std::vector <int> ag;

std::vector<string> mistake;
std::vector<string> correct;

int Wordlist::e = 0;



int practice_main_menu::main_choose_menu()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	// 显示菜单
	
	bool valid_choice = false;
	int choice;
	do {
		system("cls");
		SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		std::cout << "\n==== 用户主界面 ====\n";
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		std::cout << "1.开始答题" << std::endl;
		std::cout << "2.错题本" << std::endl;
		std::cout << "3.历史答题记录" << std::endl;
		std::cout << "4.个性化单词本" << std::endl;
		std::cout << "5.重练错题" << std::endl;
		std::cout << "6.修改密码" << std::endl;
		std::cout << "7.退出程序" << std::endl;
		std::cout << "请输入你的选择：\n";
		std::cin >> choice;
		if (choice >= 1 && choice <= 6)
		{
			valid_choice = true;
		}
		else if (choice == 7)
		{
			std::cout << "退出程序" << std::endl;
			exit(0);
		}
		else {
			std::cin.clear();
			std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
		}

	}while (!valid_choice);
	system("cls");
	return choice;
}

int practice_sub_menu::practice_choose_menu() {
	int sub_choice;
	bool valid_input = false;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	do 
	{
		system("cls");
		SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		std::cout << "\n==== 请选择词汇来源 ====\n";
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		std::cout << "1.CET4" << std::endl;
		std::cout << "2.CET6" << std::endl;
		std::cout << "敬请期待" << std::endl;

		if (std::cin >> sub_choice) { // 尝试读取整数
			if (sub_choice == 1 || sub_choice == 2) {
				valid_input = true;    // 有效输入
			}
			else {
				std::cout << "请重新输入有效选项（1或2）" << std::endl;
				std::this_thread::sleep_for(std::chrono::seconds(1));
			}
		}
		else { // 非整数输入处理
			std::cin.clear(); // 清除错误状态
			std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n'); // 清空缓冲区
			std::cout << "输入无效，请输入数字1或2。" << std::endl;
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
	} while (!valid_input);
	system("cls");
	return sub_choice;
}

int practice_sub_menu::practice_choose_menu_b()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	bool valid_input = false;
	int sub_choice = 0;
	do {
		system("cls");
		SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		std::cout << "\n==== 请选择题目类型 ====\n";
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		std::cout << "1.给中文写英文" << std::endl;
		std::cout << "2.给英文写中文" << std::endl;
		
		if (std::cin >> sub_choice) 
		{ // 尝试读取整数
			if (sub_choice == 1 || sub_choice == 2)
			{
				valid_input = true;    // 有效输入
			}
			else 
			{
				std::cout << "请重新输入有效选项（1或2）" << std::endl;
				std::this_thread::sleep_for(std::chrono::seconds(1));
			}
		}
		else { // 非整数输入处理
			std::cin.clear(); // 清除错误状态
			std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n'); // 清空缓冲区
			std::cout << "输入无效，请输入数字1或2。" << std::endl;
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
	}while (!valid_input);
	system("cls");
	return sub_choice;
}




void Wordlist::Wordlist_a()
{
	int score = 100;
	DWORD start, end;
	
	accept_CET = practice_sub_menu::get_instance()->practice_choose_menu();
	accept_mode = practice_sub_menu::get_instance()->practice_choose_menu_b();
	random_a(accept_CET);
	if (accept_CET == 1)
			{
				words B;
				std::cout << "欢迎进入CET4\n";
				int every = 0;
				if (accept_mode == 1)
				{
					std::cout << "欢迎进入中译英模式\n";
					start = timeGetTime();         //begin timer
					for (int aa = 1; aa < DEF; aa++)
					{
						std::cout << "第" << aa << "题" << std::endl;
						std::string zh = B.get_zh_cet_4(qu[e]);
						std::string pronunciation = B.get_pronunciation_cet_4(qu[e]);
						std::string name = B.get_name_cet_4(qu[e]);
						every = show(accept_CET, name, zh, pronunciation, qu[e], accept_mode);
						e++;
						score += every;
					}
					end = timeGetTime();;         //end timer
					double during = (end - start) / 1000.0;
					std::cout << "您的答题时间为： " << during << " " << "s" << endl;;
					std::cout << "答题结束" << std::endl;
					// 在 Wordlist_a 函数中调用
					if (!user::get_instance()->record_info(ID, "CET4", "中译英")) {
						std::cout << "错题记录失败" << std::endl;
					}
					else {
						std::cout << "错题记录成功" << std::endl;
					}

					if (!user::get_instance()->record_score(score, during, "CET4", "中译英")) {
						std::cout << "\n成绩记录失败" << std::endl;
					}
					else {
						std::cout << "成绩记录成功\n" << std::endl;
					}
				}
				if (accept_mode == 2)
				{
					start = timeGetTime();
					std::cout << "欢迎进入英译中模式\n";
					for (int aa = 1; aa < DEF; aa++)
					{
						std::cout << "第" << aa << "题" << std::endl;
						std::string zh = B.get_zh_cet_4(qu[e]);
						std::string pronunciation = B.get_pronunciation_cet_4(qu[e]);
						std::string name = B.get_name_cet_4(qu[e]);
						e++;
						every = show(accept_CET, name, zh, pronunciation, qu[e], accept_mode);
						score += every;
					}
					end = timeGetTime();;         //end timer
					double during = (end - start) / 1000.0;
					std::cout << "答题结束" << std::endl;
					if (!user::get_instance()->record_info(ID, "CET4", "英译中")) {
						std::cout << "错题记录失败" << std::endl;
					}
					else {
						std::cout << "错题记录成功" << std::endl;
					}
					if (!user::get_instance()->record_score(score, during, "CET4", "英译中")) {
						std::cout << "\n成绩记录失败" << std::endl;
					}
					else {
						std::cout << "成绩记录成功\n" << std::endl;
					}
				}
			}
			if (accept_CET == 2)
			{
				int every = 0;
				words B;
				std::cout << "欢迎进入CET6\n";
				if (accept_mode == 1)
				{
					start = timeGetTime();
					std::cout << "欢迎进入中译英模式\n";
					for (int aa = 1; aa < DEF; aa++)
					{
						std::cout << "第" << aa << "题" << std::endl;
						std::string zh = B.get_zh_cet_6(qu[e]);
						std::string pronunciation = B.get_pronunciation_cet_6(qu[e]);
						std::string name = B.get_name_cet_6(qu[e]);
						e++;
						every = show(accept_CET, name, zh, pronunciation, qu[e], accept_mode);
						score += every;
					}
					std::cout << "答题结束" << std::endl;
					end = timeGetTime();;         //end timer
					double during = (end - start) / 1000.0;
					if (!user::get_instance()->record_info(ID, "CET6", "中译英")) {
						std::cout << "错题记录失败" << std::endl;
					}
					else {
						std::cout << "错题记录成功" << std::endl;
					}
					if (!user::get_instance()->record_score(score, during, "CET6", "中译英")) {
						std::cout << "\n成绩记录失败" << std::endl;
					}
					else {
						std::cout << "成绩记录成功\n" << std::endl;
					}
				}
				if (accept_mode == 2)
				{
					start = timeGetTime();
					std::cout << "欢迎进入英译中模式\n";
					for (int aa = 1; aa < DEF; aa++)
					{
						std::cout << "第" << aa << "题" << std::endl;
						std::string zh = B.get_zh_cet_6(qu[e]);
						std::string pronunciation = B.get_pronunciation_cet_6(qu[e]);
						std::string name = B.get_name_cet_6(qu[e]);
						e++;
						every = show(accept_CET, name, zh, pronunciation, qu[e], accept_mode);
						score += every;
					}
					std::cout << "答题结束" << std::endl;
					end = timeGetTime();;         //end timer
					double during = (end - start) / 1000.0;
					if (!user::get_instance()->record_info(ID, "CET6", "英译中")) {
						std::cout << "错题记录失败" << std::endl;
					}
					else {
						std::cout << "错题记录成功" << std::endl;
					}
					if (!user::get_instance()->record_score(score, during, "CET6", "英译中")) {
						std::cout << "\n成绩记录失败" << std::endl;
					}
					else {
						std::cout << "成绩记录成功\n" << std::endl;
					}
				}
			}
			ag.push_back(score);
			std::cout << "你的分数是:\t" << score << std::endl;
			std::this_thread::sleep_for(std::chrono::seconds(3));//等待三秒展示分数
}

int Wordlist::error_a(std::string correct)
 {
	SetConsoleOutputCP(936);    // 设置控制台为 GBK 
	std::string n;
	 std::cout << "请输入英文" << std::endl;
	 std::cin >> n;
	 if (n == correct)
	 {
		 return 0;
	 }
	 remind();
	 return 1;
 }


int Wordlist::show(int pcur,std::string name,std::string zh,std::string pronunciation,int ID_a,int second)
{
	int score = 0;
	if (second == 1)//中译英，输入英文
	{
		SetConsoleOutputCP(936);  // 设置控制台为 GBK
		std::cout << zh << std::endl;
		int a = 0;
		while (error_a(name))
		{
			a++;
			if (a == 1)
			{
				drop(score);
				zhying.push_back(1);
				ID.push_back(ID_a);
			if (pcur == 1)
			{
				siliu.push_back(1);
			}
			else
			{
				siliu.push_back(2);
			}
			}
			if (a == 3)
			{
				
				std::cout << "错误次数过多，进入下一题\n";
				break;
			}
			
			std::cout << "提示:";
			SetConsoleOutputCP(65001);  // 设置控制台为 UTF-8
			std::cout << pronunciation << std::endl;
			SetConsoleOutputCP(936);    // 设置控制台为 GBK
			
		}
	}
	if (second == 2)//英译中，输入中文
	{
		SetConsoleOutputCP(936);    // 设置控制台为 GBK
		std::cout << name << std::endl;
		int a = 0;
		while (error_b(zh))
		{
			if (a == 1) {
				drop(score);
				zhying.push_back(2);
				ID.push_back(ID_a);
				if (pcur == 1)
				{
					siliu.push_back(1);
				}
				else
				{
					siliu.push_back(2);
				}
			}
            a++;
			if (a == 3)
			{
				std::cout << "错误次数过多，进入下一题\n";
				break;
			}
			std::cout << "提示:";
			SetConsoleOutputCP(65001);  // 设置控制台为 UTF-8
			std::cout << pronunciation << std::endl;
			SetConsoleOutputCP(936);    // 设置控制台为 GBK
		}
	}
	return score;
}

void  Wordlist::drop(int& score)
 {
	score -=10;
 }

void Wordlist::random_a(int choice)
{
	std::vector <int> s1;
	if (choice == 1)
	{
		for (int g = 1; g <= 4428; g++)
		{
			s1.push_back(g);
		}
		srand(time(0));
		shuffle(s1.begin(), s1.end(), std::mt19937{ std::random_device{}() });
		for (int d = 0; d <= 9; d++)
		{
			qu.push_back(s1.at(d));
		}
	}
	else {
		for (int g = 1; g <= 5035; g++)
		{
			s1.push_back(g);
		}
		srand(time(0));
		shuffle(s1.begin(), s1.end(), std::mt19937{ std::random_device{}() });
		for (int d = 0; d <= 9; d++)
		{
			qu.push_back(s1.at(d));
		}
	}
}

void Wordlist ::remind()
 {
	SetConsoleOutputCP(936);    // 设置控制台为 GBK
	std::cout << "你答错了，请再来一遍" << std::endl;
		 printf("\a");
 }



void error_book::show_error_book()
{
	return user::get_instance()->show_error_book();
}


words::words()
{
	words::con2 = mysql_init(NULL);
	mysql_options(con2, MYSQL_SET_CHARSET_NAME, "gbk");

	if (!mysql_real_connect(con2, user::get_instance()->host.c_str(), user2, user::get_instance()->pwd.c_str(), db_name2, port2, NULL, 0))
	{
		printf("Error connecting to database:%s\n", mysql_error(con2));
		exit(-1);
	}
}

words::~words()
{
	mysql_close(words::con2);
}

//Search for certain word_object from the ID

std::string words::get_zh_cet_4(int ID)
{
	return words::get_instance()->get_word(ID, "zh", "cet_4");
}

std::string words::get_name_cet_4(int ID)
{
	return get_word(ID, "name", "cet_4");
}

std::string words::get_pronunciation_cet_4(int ID)
{
	return words::get_instance()->get_word(ID,"pronunciation", "cet_4");
}

std::string words::get_zh_cet_6(int ID)
{
	return words::get_instance()->get_word( ID,"zh", "cet_6");
}

std::string words::get_name_cet_6(int ID)
{
	return words::get_instance()->get_word(ID, "name", "cet_6");
}

std::string words::get_pronunciation_cet_6(int ID)
{
	return get_word(ID, "pronunciation", "cet_6");
}

//private functions:

std::string words::get_word(int ID, std::string select, std::string sources)
{
	std::string word;
	char sql[1024];
	if (select == "pronunciation") {
		
		mysql_set_character_set(con2, "utf8mb4");
	}
	else {
		
		mysql_set_character_set(con2, "gbk");
	}
	
	sprintf_s(sql, "SELECT %s from %s WHERE ID=%d;", select.c_str(), sources.c_str(), ID);
	if (mysql_query(words::con2, sql))
	{
		printf_s("Error eo fetch zh, due to error:\n");
		return (mysql_error(con2));
	}
	MYSQL_RES* result = mysql_store_result(con2);
	MYSQL_ROW row;
	while (row = mysql_fetch_row(result))
	{
		word = row[0];
	}
	return word;
}

bool words::check_word(std::string word)
{
	char sql[1024];
	sprintf_s(sql, "SELECT name FROM word WHERE name = '%s'", word.c_str());

	if (mysql_query(con2, sql))
	{
		fprintf_s(stderr, "Failed to check word.:Error%s\n", mysql_error(con2));
		return false;
	}
	MYSQL_RES* result = mysql_store_result(con2);
	MYSQL_ROW row;
	if (row = mysql_fetch_row(result)) return false;
	return true;
}

void error_book::error_book_a(std::string name, string zh)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	int ff = 1;
	int f = 0;
	static int b = 1;
	int a = error_book::get_instance()->random_b();
	if (a == 1)
	{
		
		SetConsoleTextAttribute(hConsole, 0x0003);
		std::cout << "第" << b << "题" << std::endl;
		SetConsoleTextAttribute(hConsole, 0x0001|0x0002|0x0004);
		b++;
		std::cout << "中文：" << zh << std::endl;
		while (Wordlist::get()->error_a(name))
		{
			ff++;
			f = 1;
			mistake.push_back(name);//将错误的单词加入到mistake中
			if (ff == 3)
			{
                std::cout << "错误次数过多，自动进入下一题\n";
			}
			SetConsoleOutputCP(936);//重新启用gbk编码保证输出正确
		}
		if (f == 0) {
			correct.push_back(name);//将正确的单词加入到correct中
		}
	}
	else
	{
		SetConsoleTextAttribute(hConsole, 0x0003);
		std::cout << "第" << b << "题" << std::endl;
		SetConsoleTextAttribute(hConsole, 0x0001 | 0x0002 | 0x0004);
		b++;
		std::cout << name << std::endl;
		while (Wordlist::get()->error_b(zh))
		{
			mistake.push_back(name);//将错误的单词加入到mistake中
			std::cout << "不答对无法退出" << std::endl;
			SetConsoleOutputCP(936);//重新启用gbk编码保证输出正确
		}
		if (f == 0) {
			correct.push_back(name);//将正确的单词加入到correct中
		}
	}
}

int error_book::random_b()
{
	int a = 0;
	srand(time(0));
	a = (rand() % 2) + 1;
	return a;
}

int Wordlist::error_b(std::string zh)
{
	SetConsoleOutputCP(936);    // 设置控制台为 GBK 
	std::vector<wchar_t> zh_vector(zh.begin(), zh.end());
	zh_vector.push_back(',');
	std::vector<wchar_t> correct;
	std::cout << "请输入中文" << std::endl;
	std::string shuru;
	std::cin >> shuru;
	std::vector <wchar_t> shuru_vector(shuru.begin(), shuru.end());
	for (auto it = std::find(zh_vector.begin(), zh_vector.end(), '.') + 2; it != zh_vector.end(); it++)
	{
		if ((*it == 65443 && *(it + 1) == 65452) || (*(it - 1) == 65443 && *it == 65452) || *it == 44 || (*it == 65443 && *(it + 1) == 65467) || (*(it - 1) == 65443 && *it == 65467))
		{
			if (shuru_vector == correct)
			{
				
				std::cout << "correct" << std::endl;
				return 0;
			}
			else
			{
				correct.clear();
			}
		}
		else
		{
			correct.push_back(*it);
		}

	}
	remind();
	return 1;
}