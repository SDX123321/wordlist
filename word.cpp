
#include <mysql_driver.h>  // 驱动
#include <mysql_connection.h>  //管理DB链接
#include <cppconn/statement.h>  //执行SQL语句
#include <cppconn/resultset.h>   //获取查询结果
#include <cppconn/exception.h>  //error
#include <iostream>
#include <string.h>#include "word.h"
void view_words()
{
	try
	{
		sql::mysql::MySQL_Driver* driver;
		sql::Connection* con;
		sql::Statement* stmt;
		sql::ResultSet* res;

		driver = sql::mysql::get_mysql_driver_instance();
		con = driver->connect("tcp://127.0.0.1:3300", "root", "123456");
		con->setSchema("wordlist");
		stmt = con->createStatement();
		res = stmt->executeQuery("Select * from word");
		while (res->next())
		{
			std::cout << "word_id: " << res->getInt("id") << "\n";
			std::cout << "word: " << res->getString("name") << "\n";
			std::cout << "translate: " << res->getString("zh") << "\n";
			std::cout << "profile: " << res->getString("cixing") << "\n";
			std::cout << "_____________________________" << "\n";
		}

	}

	catch (sql::SQLException& e)
	{
		std::cerr << "MySQL error:" << e.what() << std::endl;
	}

}