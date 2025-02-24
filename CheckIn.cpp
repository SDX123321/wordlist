#include "CheckIn.h"

CheckIn::CheckIn()
{
	con = mysql_init(NULL);
	mysql_options(con, MYSQL_SET_CHARSET_NAME, "gbk");

	if (!mysql_real_connect(con, host, use, pw, db_name, port, NULL, 0))
	{
		printf_s("Error connecting to database:%s\n", mysql_error(con));
		exit(-1);
	}
}

CheckIn::~CheckIn()
{
	if (con) mysql_close(con);
}

bool CheckIn::check_in(std::string user_name) {
    if (!con) return false;

    char query[512];
    snprintf(query, sizeof(query),
        "UPDATE user SET check_in_count = IF(last_checkin_date < CURDATE(), check_in_count + 1, check_in_count), last_checkin_date = IF(last_checkin_date < CURDATE(), CURDATE(), last_checkin_date) WHERE name = '%s';",user_name.c_str());

    if (mysql_query(con, query)) {
        std::cerr << "Error: " << mysql_error(con) << std::endl;
        return false;
    }

    return true;
}


int CheckIn::get_checkin_days(std::string user_name) {
    if (user_name.empty() || user_name.length() > 64) {
        std::cerr << "Invalid username" << std::endl;
        return -2;
    }

    MYSQL_STMT* stmt = mysql_stmt_init(con);
    const char* query = "SELECT check_in_count FROM user WHERE name = ?";

    // 绑定参数
    MYSQL_BIND params;
    memset(&params, 0, sizeof(params));
    params.buffer_type = MYSQL_TYPE_STRING;
    params.buffer = (char*)user_name.c_str();
    params.buffer_length = user_name.length();

    if (mysql_stmt_prepare(stmt, query, strlen(query)) ||
        mysql_stmt_bind_param(stmt, &params)) {
        std::cerr << "Prepare error: " << mysql_stmt_error(stmt);
        mysql_stmt_close(stmt);
        return -1;
    }

    if (mysql_stmt_execute(stmt)) {
        std::cerr << "Execute error: " << mysql_stmt_error(stmt);
        mysql_stmt_close(stmt);
        return -1;
    }

    // 获取结果
    MYSQL_BIND result_bind;
    int checkin_days = 0;
    memset(&result_bind, 0, sizeof(result_bind));
    result_bind.buffer_type = MYSQL_TYPE_LONG;
    result_bind.buffer = &checkin_days;
    mysql_stmt_bind_result(stmt, &result_bind);

    int ret = mysql_stmt_fetch(stmt);
    mysql_stmt_close(stmt);

    return (ret == 0) ? checkin_days :
        (ret == MYSQL_NO_DATA) ? 0 : -1;
}


std::string CheckIn::get_last_checkin_day(std::string user_name)
{
    std::string date;
    char sql[512];
    snprintf(sql, sizeof(sql), "SELECT last_checkin_date FROM user WHERE name = '%s';", user_name.c_str());
    if (mysql_query(con, sql)) {
        std::cerr << "Error: " << mysql_error(con) << std::endl;
        exit(-1);
    }
    MYSQL_RES* result = mysql_store_result(con);
    while (MYSQL_ROW row = mysql_fetch_row(result))
    {
        date = row[0];
    }
    return date; 
}


