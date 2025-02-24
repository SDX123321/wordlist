﻿#include "user.h"

constexpr auto CRT_SECURE_NO_WARNINGS = 1;

using namespace std;
user::user()
{
	con = mysql_init(NULL);
	mysql_options(con, MYSQL_SET_CHARSET_NAME, "gbk");
	
	if (!mysql_real_connect(con, host, use, pw, db_name, port, NULL, 0))
	{
		printf("Error connecting to database:%s\n", mysql_error(con));
		exit(-1);
	}
}
user::~user()
{
	mysql_close(user::con);
}

void user::get_name(std::string new_name)
{
	user::user_name.assign(new_name);
} //界面向user类中传参user_name

bool user::insert_user(current& person)
{
	char sql[1000];
	sprintf_s(sql, "INSERT user SET name = '%s', password = '%s', check_in_count = 1,last_checkin_date = CURDATE();", person.name.c_str(), person.password.c_str());
	if (mysql_query(con, sql))
	{
		fprintf_s(stderr, "Failed to create account.:Error%s\n", mysql_error(con));
		return false;
	}
	else return true;
}


//检查代码函数   已优化为防止sql注入
bool user::check_password(string account, string password) {
	MYSQL_STMT* stmt = mysql_stmt_init(con);
	if (!stmt) {
		fprintf_s(stderr, "Failed to init stmt: %s\n", mysql_error(con));
		return false;
	}

	// 使用预处理语句替代字符串拼接
	const char* sql = "SELECT password FROM user WHERE name = ? AND password = ?;";
	if (mysql_stmt_prepare(stmt, sql, static_cast<unsigned long>(strlen(sql))) != 0) {
		fprintf_s(stderr, "Prepare failed: %s\n", mysql_stmt_error(stmt));
		mysql_stmt_close(stmt);
		return false;
	}

	// 配置参数绑定结构
	MYSQL_BIND bind[2];
	memset(&bind, 0, sizeof(bind));

	// 绑定第一个参数（account）
	bind[0].buffer_type = MYSQL_TYPE_STRING;
	bind[0].buffer = const_cast<char*>(account.c_str());
	bind[0].buffer_length = static_cast<unsigned long>(account.length());
	bind[0].length = nullptr;

	// 绑定第二个参数（password）
	bind[1].buffer_type = MYSQL_TYPE_STRING;
	bind[1].buffer = const_cast<char*>(password.c_str());
	bind[1].buffer_length = static_cast<unsigned long>(password.length());
	bind[1].length = nullptr;
	

	// 执行参数绑定
	if (mysql_stmt_bind_param(stmt, bind) != 0) {
		fprintf_s(stderr, "Bind param failed: %s\n", mysql_stmt_error(stmt));
		mysql_stmt_close(stmt);
		return false;
	}

	// 执行查询
	if (mysql_stmt_execute(stmt) != 0) {
		fprintf_s(stderr, "Execute failed: %s\n", mysql_stmt_error(stmt));
		mysql_stmt_close(stmt);
		return false;
	}

	// 将结果集缓存到客户端
	if (mysql_stmt_store_result(stmt) != 0) {
		fprintf_s(stderr, "Store result failed: %s\n", mysql_stmt_error(stmt));
		mysql_stmt_close(stmt);
		return false;
	}

	// 通过结果集行数判断是否存在匹配记录
	bool ret = (mysql_stmt_num_rows(stmt) > 0 );

	// 清理资源
	mysql_stmt_free_result(stmt);
	mysql_stmt_close(stmt);
	return ret;
}

//注册时检查账号重复函数，已优化为防止sql注入
bool user::check_account(const std::string& account) {
	MYSQL_STMT* stmt = mysql_stmt_init(con);
	if (!stmt) {
		fprintf_s(stderr, "[SECURITY] Statement init failed: %s\n", mysql_error(con));
		return false;
	}

	// 参数化查询
	const char* sql = "SELECT 1 FROM user WHERE name = ? LIMIT 1";  //使用存在性检查
	if (mysql_stmt_prepare(stmt, sql, static_cast<unsigned long>(strlen(sql))) != 0) {
		fprintf_s(stderr, "[SQL防御] 准备失败: %s\n", mysql_stmt_error(stmt));
		mysql_stmt_close(stmt);
		return false;
	}

	// 参数绑定结构
	MYSQL_BIND bind = {};
	bind.buffer_type = MYSQL_TYPE_STRING;
	bind.buffer = const_cast<char*>(account.data());  // 安全转换：只读操作
	bind.buffer_length = static_cast<unsigned long>(account.length());

	// 安全绑定验证
	if (mysql_stmt_bind_param(stmt, &bind) != 0) {
		fprintf_s(stderr, "[参数校验] 绑定失败: %s\n", mysql_stmt_error(stmt));
		mysql_stmt_close(stmt);
		return false;
	}

	// 执行安全查询（启用服务器端预处理）
	if (mysql_stmt_execute(stmt) != 0) {
		fprintf_s(stderr, "[执行异常] 错误代码: %s\n", mysql_stmt_error(stmt));
		mysql_stmt_close(stmt);
		return false;
	}

	// 安全结果处理
	mysql_stmt_store_result(stmt);
	bool exists = (mysql_stmt_num_rows(stmt) > 0);

	// 内存安全释放
	mysql_stmt_free_result(stmt);
	mysql_stmt_close(stmt);

	return exists;
}

//登录或注册时检查长度与合法字符
bool user::check_length(const std::string& account) {

	if (!std::regex_match(account, std::regex("^[a-zA-Z0-9_]{4,20}$"))) {
		fprintf_s(stderr, "[输入校验] 非法账号格式\n");
		return false;
	}
	else {
		printf_s("[输入校验] 账号格式正确\n");
		return true;
	}
}


int user::welcome_page() {
	while (true) {
		system("cls");  // 清屏
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

		// 显示菜单
		SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		std::cout << "\n==== 背单词系统 ====\n";
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		std::cout << "1. 登录\n"
			<< "2. 注册\n"
			<< "3. 退出\n"
			<< "请输入你的选择 (1-3): ";

		std::string input;
		std::getline(std::cin, input);

		// 输入验证
		try {
			int choice = std::stoi(input);
			if (choice >= 1 && choice <= 3) {
				return choice;
			}
		}
		catch (...) {
			// 转换失败，继续循环
		}

		// 无效输入处理
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
		std::cout << "请输入有效的选项 (1-3)\n";
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}
	


bool user::create_error_book(std::string user_name)
{
	char sql[512];
	sprintf_s(sql, "CREATE TABLE IF NOT EXISTS `%s_error_book` ("
		"main_id int PRIMARY KEY AUTO_INCREMENT,"
		"ID int,"
		"name VARCHAR(50) CHARACTER SET utf8mb4,"
		"zh VARCHAR(100) CHARACTER SET utf8mb4,"
		"sources VARCHAR(20) CHARACTER SET utf8mb4,"
		"error_times INT NOT NULL DEFAULT 0"
		") CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;",
		escape_identifier(user_name).c_str());

	if (mysql_query(con, sql)) {
		log_error("创建错题本失败: " + std::string(mysql_error(con)));
		return false;
	}
	return true;
}

bool user::create_history_table(std::string user_name) {
	char sql[512];
	sprintf_s(sql, "CREATE TABLE IF NOT EXISTS `%s_history_table` ("
		"ID int PRIMARY KEY AUTO_INCREMENT,"
		"score varchar(50) CHARACTER SET utf8mb4,"
		"time datetime,"
		"mode varchar(20) CHARACTER SET utf8mb4,"
		"last double unsigned,"
		"source varchar(20) CHARACTER SET utf8mb4"
		") CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;",
		escape_identifier(user_name).c_str());

	if (mysql_query(con, sql)) {
		log_error("创建历史记录表失败: " + std::string(mysql_error(con)));
		return false;
	}
	return true;
}

void user::show_error_book() {
	if (user_name.empty()) {
		log_error("未登录用户");
		return;
	}

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	// 预处理语句初始化
	MYSQL_STMT* stmt = mysql_stmt_init(con);
	if (!stmt) {
		log_error("MySQL 语句初始化失败");
		return;
	}

	// 使用参数化查询，限制返回数量为50
	const char* sql = "SELECT main_id, name, zh, sources, ID FROM `%s_error_book` "
		"ORDER BY main_id DESC LIMIT 50";

	char formatted_sql[256];
	sprintf_s(formatted_sql, sql, escape_identifier(user_name).c_str());

	if (mysql_stmt_prepare(stmt, formatted_sql, strlen(formatted_sql))) {
		log_error("预处理语句准备失败: " + std::string(mysql_stmt_error(stmt)));
		mysql_stmt_close(stmt);
		return;
	}

	// 执行查询
	if (mysql_stmt_execute(stmt)) {
		log_error("执行查询失败: " + std::string(mysql_stmt_error(stmt)));
		mysql_stmt_close(stmt);
		return;
	}

	// 准备结果绑定
	MYSQL_BIND result_bind[5];
	memset(result_bind, 0, sizeof(result_bind));

	// 为结果分配内存
	struct {
		int main_id;
		char name[50];
		char zh[100];
		char sources[20];
		int word_id;
		unsigned long name_length;
		unsigned long zh_length;
		unsigned long sources_length;
	} result_data;

	// 绑定结果列
	result_bind[0].buffer_type = MYSQL_TYPE_LONG;
	result_bind[0].buffer = &result_data.main_id;

	result_bind[1].buffer_type = MYSQL_TYPE_STRING;
	result_bind[1].buffer = result_data.name;
	result_bind[1].buffer_length = sizeof(result_data.name);
	result_bind[1].length = &result_data.name_length;

	result_bind[2].buffer_type = MYSQL_TYPE_STRING;
	result_bind[2].buffer = result_data.zh;
	result_bind[2].buffer_length = sizeof(result_data.zh);
	result_bind[2].length = &result_data.zh_length;

	result_bind[3].buffer_type = MYSQL_TYPE_STRING;
	result_bind[3].buffer = result_data.sources;
	result_bind[3].buffer_length = sizeof(result_data.sources);
	result_bind[3].length = &result_data.sources_length;

	result_bind[4].buffer_type = MYSQL_TYPE_LONG;
	result_bind[4].buffer = &result_data.word_id;

	if (mysql_stmt_bind_result(stmt, result_bind)) {
		log_error("绑定结果失败: " + std::string(mysql_stmt_error(stmt)));
		mysql_stmt_close(stmt);
		return;
	}

	// 存储结果集
	if (mysql_stmt_store_result(stmt)) {
		log_error("存储结果失败: " + std::string(mysql_stmt_error(stmt)));
		mysql_stmt_close(stmt);
		return;
	}

	// 显示错题本标题
	SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	std::cout << "\n==== 错题本 ====\n";
	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

	// 获取并显示结果
	int count = 0;
	while (mysql_stmt_fetch(stmt) == 0) {
		count++;
		SetConsoleTextAttribute(hConsole, 0x0003 | FOREGROUND_INTENSITY);
		std::cout << "\n错题 #" << count << "\n";
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

		std::cout << "单词: " << std::string(result_data.name, result_data.name_length) << "\n"
			<< "中文: " << std::string(result_data.zh, result_data.zh_length) << "\n"
			<< "来源: " << std::string(result_data.sources, result_data.sources_length) << "\n";
	}

	if (count == 0) {
		SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
		std::cout << "目前没有错题记录，继续保持！\n";
	}
	else {
		SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
		std::cout << "\n共显示 " << count << " 条错题记录\n";
	}

	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

	// 清理资源
	mysql_stmt_free_result(stmt);
	mysql_stmt_close(stmt);
}

void user::show_history_table() {
	if (user_name.empty()) {
		log_error("未登录用户");
		return;
	}

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	// 预处理语句初始化
	MYSQL_STMT* stmt = mysql_stmt_init(con);
	if (!stmt) {
		log_error("MySQL 语句初始化失败");
		return;
	}

	// 使用参数化查询，限制返回数量为50，按时间降序排序
	const char* sql = "SELECT ID, score, time, mode, source, last FROM `%s_history_table` "
		"ORDER BY time DESC LIMIT 50";

	char formatted_sql[256];
	sprintf_s(formatted_sql, sql, escape_identifier(user_name).c_str());

	if (mysql_stmt_prepare(stmt, formatted_sql, strlen(formatted_sql))) {
		log_error("预处理语句准备失败: " + std::string(mysql_stmt_error(stmt)));
		mysql_stmt_close(stmt);
		return;
	}

	// 执行查询
	if (mysql_stmt_execute(stmt)) {
		log_error("执行查询失败: " + std::string(mysql_stmt_error(stmt)));
		mysql_stmt_close(stmt);
		return;
	}

	// 准备结果绑定
	MYSQL_BIND result_bind[6];
	memset(result_bind, 0, sizeof(result_bind));

	// 为结果分配内存
	struct {
		int id;
		char score[51];
		MYSQL_TIME time;
		char mode[21];
		char source[21];
		double last;
		unsigned long score_length;
		unsigned long mode_length;
		unsigned long source_length;
	} result_data;

	// 绑定结果列
	result_bind[0].buffer_type = MYSQL_TYPE_LONG;
	result_bind[0].buffer = &result_data.id;

	result_bind[1].buffer_type = MYSQL_TYPE_STRING;
	result_bind[1].buffer = result_data.score;
	result_bind[1].buffer_length = sizeof(result_data.score);
	result_bind[1].length = &result_data.score_length;

	result_bind[2].buffer_type = MYSQL_TYPE_DATETIME;
	result_bind[2].buffer = &result_data.time;

	result_bind[3].buffer_type = MYSQL_TYPE_STRING;
	result_bind[3].buffer = result_data.mode;
	result_bind[3].buffer_length = sizeof(result_data.mode);
	result_bind[3].length = &result_data.mode_length;

	result_bind[4].buffer_type = MYSQL_TYPE_STRING;
	result_bind[4].buffer = result_data.source;
	result_bind[4].buffer_length = sizeof(result_data.source);
	result_bind[4].length = &result_data.source_length;

	result_bind[5].buffer_type = MYSQL_TYPE_DOUBLE;
	result_bind[5].buffer = &result_data.last;

	if (mysql_stmt_bind_result(stmt, result_bind)) {
		log_error("绑定结果失败: " + std::string(mysql_stmt_error(stmt)));
		mysql_stmt_close(stmt);
		return;
	}

	// 存储结果集
	if (mysql_stmt_store_result(stmt)) {
		log_error("存储结果失败: " + std::string(mysql_stmt_error(stmt)));
		mysql_stmt_close(stmt);
		return;
	}

	// 显示历史记录标题
	SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	std::cout << "\n==== 历史答题记录 ====\n";
	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

	// 获取并显示结果
	int count = 0;
	while (mysql_stmt_fetch(stmt) == 0) {
		count++;
		SetConsoleTextAttribute(hConsole, 0x0003 | FOREGROUND_INTENSITY);
		std::cout << "\n记录 #" << count << "\n";
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

		// 格式化时间
		char time_str[20];
		sprintf_s(time_str, "%04d-%02d-%02d %02d:%02d:%02d",
			result_data.time.year, result_data.time.month, result_data.time.day,
			result_data.time.hour, result_data.time.minute, result_data.time.second);

		std::cout << "时间: " << time_str << "\n"
			<< "分数: " << std::string(result_data.score, result_data.score_length) << "\n"
			<< "词库: " << std::string(result_data.source, result_data.source_length) << "\n"
			<< "模式: " << std::string(result_data.mode, result_data.mode_length) << "\n"
			<< "用时: " << result_data.last << " 秒\n";
	}

	if (count == 0) {
		SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
		std::cout << "暂无答题记录，快去开始你的第一次答题吧！\n";
	}
	else {
		SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
		std::cout << "\n共显示 " << count << " 条历史记录\n";
	}

	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

	// 清理资源
	mysql_stmt_free_result(stmt);
	mysql_stmt_close(stmt);
}


bool user::record_info(std::vector<int> ID_list, std::string sources, std::string mode)
{
	if (user_name.empty() || sources.empty() || mode.empty() || ID_list.empty()) {
		log_error("参数错误：必要参数为空");
		return false;
	}
	
	// 设置连接字符集为 utf8mb4
	if (mysql_set_character_set(con, "utf8mb4")) {
		log_error("设置字符集失败: " + std::string(mysql_error(con)));
		return false;
	}

	// 预处理语句初始化
	MYSQL_STMT* stmt = mysql_stmt_init(con);
	if (!stmt) {
		log_error("MySQL 语句初始化失败");
		return false;
	}

	// 使用参数化查询防止SQL注入
	const char* sql = "INSERT INTO `%s_error_book` (ID, name, zh, sources) VALUES (?, ?, ?, ?)";
	char formatted_sql[256];
	sprintf_s(formatted_sql, sql, escape_identifier(user_name).c_str());

	if (mysql_stmt_prepare(stmt, formatted_sql, static_cast<unsigned long>(strlen(formatted_sql)))) 
	{
		log_error("预处理语句准备失败: " + std::string(mysql_stmt_error(stmt)));
		mysql_stmt_close(stmt);
		return false;
	}

	// 准备绑定参数
	MYSQL_BIND bind[4];
	memset(bind, 0, sizeof(bind));

	// 为每个错题记录执行插入
	words* word_instance = words::get_instance();
	bool success = true;

	for (int word_id : ID_list) {
		// 获取单词信息
		std::string word_name = (sources == "CET4") ?
			word_instance->get_name_cet_4(word_id) :
			word_instance->get_name_cet_6(word_id);

		std::string word_zh = (sources == "CET4") ?
			word_instance->get_zh_cet_4(word_id) :
			word_instance->get_zh_cet_6(word_id);

		// 设置绑定参数
		int id_value = word_id;
		bind[0].buffer_type = MYSQL_TYPE_LONG;
		bind[0].buffer = &id_value;
		bind[0].is_null = 0;

		bind[1].buffer_type = MYSQL_TYPE_STRING;
		bind[1].buffer = (void*)word_name.c_str();
		bind[1].buffer_length = static_cast<unsigned long>(word_name.length());
		bind[1].is_null = 0;

		bind[2].buffer_type = MYSQL_TYPE_STRING;
		bind[2].buffer = (void*)word_zh.c_str();
		bind[2].buffer_length = static_cast<unsigned long>(word_zh.length());
		bind[2].is_null = 0;

		bind[3].buffer_type = MYSQL_TYPE_STRING;
		bind[3].buffer = (void*)sources.c_str();
		bind[3].buffer_length = static_cast<unsigned long>(sources.length());
		bind[3].is_null = 0;

		// 绑定参数并执行
		if (mysql_stmt_bind_param(stmt, bind)) {
			log_error("参数绑定失败: " + std::string(mysql_stmt_error(stmt)));
			success = false;
			break;
		}

		if (mysql_stmt_execute(stmt)) {
			log_error("执行插入失败: " + std::string(mysql_stmt_error(stmt)));
			success = false;
			break;
		}
	}

	// 清理资源
	mysql_stmt_close(stmt);
	return success;
}

bool user::record_score(int score, double during, const std::string&source, const std::string& mode) {
	if (user_name.empty()) {
		log_error("未登录用户");
		return false;
	}
	if (mysql_set_character_set(con, "utf8mb4")) {
		log_error("设置字符集失败: " + std::string(mysql_error(con)));
		return false;
	}

	// 预处理语句初始化
	MYSQL_STMT* stmt = mysql_stmt_init(con);
	if (!stmt) {
		log_error("MySQL 语句初始化失败");
		return false;
	}

	// 使用参数化查询防止SQL注入
	const char* sql = "INSERT INTO `%s_history_table` (score, time, mode, last,source) "
		"VALUES (?, NOW(), ?, ?,?)";

	char formatted_sql[256];
	sprintf_s(formatted_sql, sql, escape_identifier(user_name).c_str());

	if (mysql_stmt_prepare(stmt, formatted_sql, strlen(formatted_sql))) {
		log_error("预处理语句准备失败: " + std::string(mysql_stmt_error(stmt)));
		mysql_stmt_close(stmt);
		return false;
	}

	// 准备绑定参数
	MYSQL_BIND bind[4];
	memset(bind, 0, sizeof(bind));

	// 转换score为字符串
	std::string score_str = std::to_string(score);

	// 绑定score参数
	bind[0].buffer_type = MYSQL_TYPE_STRING;
	bind[0].buffer = (void*)score_str.c_str();
	bind[0].buffer_length = score_str.length();
	bind[0].is_null = 0;
	bind[0].length = nullptr;

	// 绑定mode参数
	bind[1].buffer_type = MYSQL_TYPE_STRING;
	bind[1].buffer = (void*)mode.c_str();
	bind[1].buffer_length = mode.length();
	bind[1].is_null = 0;
	bind[1].length = nullptr;

	// 绑定during参数
	bind[2].buffer_type = MYSQL_TYPE_DOUBLE;
	bind[2].buffer = (void*)&during;
	bind[2].is_null = 0;
	bind[2].length = nullptr;

	//绑定source参数
	bind[3].buffer_type = MYSQL_TYPE_STRING;
	bind[3].buffer = (void*)source.c_str();
	bind[3].is_null = 0;
	bind[3].length = nullptr;

	// 执行绑定
	if (mysql_stmt_bind_param(stmt, bind)) {
		log_error("参数绑定失败: " + std::string(mysql_stmt_error(stmt)));
		mysql_stmt_close(stmt);
		return false;
	}

	// 执行插入
	if (mysql_stmt_execute(stmt)) {
		log_error("执行插入失败: " + std::string(mysql_stmt_error(stmt)));
		mysql_stmt_close(stmt);
		return false;
	}

	// 清理资源
	mysql_stmt_close(stmt);
	return true;
}

std::string user::escape_identifier(const std::string& input) {         //检查名称防止sql注入
	std::string output;
	output.reserve(input.length() * 2);
	for (char c : input) {
		if (!isalnum(c) && c != '_') continue;
		output.push_back(c);
	}
	return output.empty() ? "invalid" : output;
}





