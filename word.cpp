
#include "word.h"
using namespace std;

words::words()
{
	words::con2 = mysql_init(NULL);
	mysql_options(con2, MYSQL_SET_CHARSET_NAME, "gbk");

	if (!mysql_real_connect(con2, host2, user2, pw2, db_name2, port2, NULL, 0))
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

string get_zh_cet_4(int ID)
{
    return words::get_instance()->get_word(ID, "zh", "cet_4");
}

string get_name_cet_4(int ID)
{
	return words::get_instance()->get_word(ID, "name", "cet_4");
}

string get_yinbiao_cet_4(int ID)
{
	return words::get_instance()->get_word(ID, "yinbiao", "cet_4");
}

string get_zh_cet_6(int ID)
{
	return words::get_instance()->get_word(ID, "zh", "cet_6");
}

string get_name_cet_6(int ID)
{
	return words::get_instance()->get_word(ID, "name", "cet_6");
}

string get_yinbiao_cet_6(int ID)
{
	return words::get_instance()->get_word(ID, "yinbiao", "cet_6");
}

void add_word()
{
	struct word new_word; string dest;
	cout << "Please enter the destination you want to insert in:(eg: cet_4 or cet_6)\n";
	cin >> dest; //todo:lack double_check
	cout << "Please enter the word you want to add.\n";
	words* instance = words::get_instance();
	
	//if I check the unique word's name, there's no need to double check other objects
	do {
		cin >> new_word.name;
		if (!instance->check_word(new_word.name)) 
			std::cout << "The word you type already exists!" << endl;
	} while (!instance->check_word(new_word.name));
	std::cout << "Please input the word's Chinese meaning:";
	std::cin >> new_word.zh;
	std::cout << "Please input the word's pronunciation:";
	std::cin >> new_word.yinbiao;
	char sql[1024];
	sprintf_s(sql, "insert into '%s'(name,zh,yinbiao) values('%s','%s','%s')",
		dest.c_str(),new_word.name.c_str(), new_word.zh.c_str(), new_word.yinbiao.c_str());
	if (mysql_query(instance->con2, sql)) //static object, use static ptr
	{
		std::cout << "Success!";
		
	}
	else std::cout << "Failed to insert the word, due to error:" << mysql_error(instance->con2) << endl;
}

//private functions:

string words::get_word(int ID,string select ,string sources )
{
	string word;
	char sql[1024];
	sprintf_s(sql, "SELECT '%s' from '%s' WHERE ID = %d", select.c_str(), sources.c_str(), ID);
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
} //main function, get the word from the database

bool words::check_word(string new_word)
{
	char sql[1024];
	sprintf_s(sql, "SELECT name FROM word WHERE name = '%s'", new_word.c_str());

	if (mysql_query(con2, sql))
	{
		fprintf_s(stderr, "Failed to check password.:Error%s\n", mysql_error(con2));
		return false;
	}
	MYSQL_RES* result = mysql_store_result(con2);
	MYSQL_ROW row;
	if (row = mysql_fetch_row(result)) return false;
	return true;
}



