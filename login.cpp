#include <mysql_driver.h>  // 驱动
#include <mysql_connection.h>  //管理DB链接
#include <cppconn/statement.h>  //执行SQL语句
#include <cppconn/resultset.h>   //获取查询结果
#include <cppconn/exception.h>  //error
#include <iostream>
#include <string.h>
typedef struct user
{
	int user_id;
	std::string name;
	std::string password;
}user;


int main() {
	sql::mysql::MySQL_Driver* driver = sql::mysql::get_driver_instance();
	sql::Connection* con = driver->connect("tcp://127.0.0.1:3300", "root", "12345678Aa");
	con->setSchema("wordlist");
	
	int choice;
	do
	{
		std::cout << "\n====背单词系统====\n";
		std::cout << "1.登录\n";
		std::cout << "2.注册\n";
		std::cout << "3.直接退出\n";
	}
}