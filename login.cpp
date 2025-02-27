#include "login.h"	

std::string login::login_into_system() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    bool valid_choice = false;
    std::string result;

    do {
        int choice = user::get_instance()->welcome_page();
        switch (choice) {
        case 1: // 登录
            result = handle_login(hConsole);
            if (!result.empty()) {
                valid_choice = true;
            }
            break;

        case 2: // 注册
            result = handle_register(hConsole);
            if (!result.empty()) {
                valid_choice = true;
            }
            break;

        case 3: // 退出
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
            std::cout << "正在退出程序...\n";
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            std::this_thread::sleep_for(std::chrono::seconds(1));
            exit(0);

        default:
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
            std::cout << "无效的选择，请重试\n";
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            std::this_thread::sleep_for(std::chrono::seconds(1));
            system("cls");
            break;
        }
    } while (!valid_choice);

    return result;
}

std::string login::handle_login(HANDLE hConsole) {
    const int MAX_ATTEMPTS = 5;
    int attempts = 0;
    std::string account, password;
    DWORD mode;
    GetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), &mode);


    // 清空输入缓冲区
    //std::cin.clear();
    //std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    // 获取账号

    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
    std::cout << "请输入账号:\n ";
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    std::getline(std::cin, account);
    int retry = 0;
    do
    {
        if (!user::get_instance()->check_account(account))
        {
            cout << "[输入校验]账号不存在\n";
            retry++;
        }
        if (retry == 3)
        {
            cout << "您已失败三次，失败五次后将强制退出" << endl;
            
            
        }
        if (retry == 5)
        {
            cout << "您已失败五次，强制退出\n";
            exit(-1);
        }
    } while (!user::get_instance()->check_account(account));
    // 获取密码
    SetConsoleTextAttribute(hConsole, 0x0002);
    std::cout << "请输入密码: ";
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
    std::cout << "(密码不会显示在屏幕上)\n";
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    retry = 0;
    do {
        // 密码输入（带星号显示）
        password.clear();
        SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), mode & (~ENABLE_ECHO_INPUT));

        char ch;
        while ((ch = _getch()) != '\r') {
            if (ch == '\b') {
                if (!password.empty()) {
                    password.pop_back();
                    std::cout << "\b \b";
                }
            }
            else if (ch >= 32 && ch <= 126) {
                password.push_back(ch);
                std::cout << '*';
            }
        }
        std::cout << std::endl;
        SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), mode);
        if (!user::get_instance()->check_password(account, password))
        {
            cout << "[输入校验]密码错误\n";
            retry++;
        }
        if (retry == 3)
        {
            cout << "您已失败三次，失败五次后将强制退出" << endl;
            if (user::get_instance()->refound_password(account, false))
            {
                SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
                std::cout << "登录成功！欢迎回来，" << account << "\n";
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

                std::this_thread::sleep_for(std::chrono::milliseconds(800));

                user::get_instance()->user_name = account;
                system("cls");
                std::cout << "欢迎你， " << user::get_instance()->user_name << std::endl;
                // 签到系统
                handle_checkin(account, hConsole);

                return account;
            }
        }
        if (retry == 5)
        {
            cout << "您已失败五次，强制退出\n";
            exit(-1);
        }
    } while (!user::get_instance()->check_password(account, password));
    if (user::get_instance()->check_password(account, password)) {
        SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
        std::cout << "登录成功！欢迎回来，" << account << "\n";
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

        std::this_thread::sleep_for(std::chrono::milliseconds(800));

        user::get_instance()->user_name = account;
        system("cls");
        std::cout << "欢迎你， " << user::get_instance()->user_name << std::endl;
        // 签到系统
        handle_checkin(account, hConsole);

        return account;
    }

}
    

std::string login::handle_register(HANDLE hConsole) {
        current new_user;
        const int MAX_ATTEMPTS = 5;
        int retry = 0;

        // 注册账号
        do {
            
            if (retry != 0)
            {
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
                std::cout << "请重新输入\n";
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            }

            std::cout << "请输入账号 (4-20位字母、数字或下划线): ";
            std::cin >> new_user.name;
            

            if (!user::get_instance()->check_length(new_user.name)) {
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
                std::cout << "[输入校验]账号格式不正确\n";
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                retry++;
                
            }

            if (user::get_instance()->check_account(new_user.name)) {
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
                std::cout << "[输入校验]账号已存在\n";
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                retry++;
                
            }
            

            if (retry == 3)
            {
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
                cout << "您已失败三次，失败五次后将强制退出" << endl;
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            }
            if (retry == 5)
            {
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
                cout << "您已失败五次，强制退出\n";
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                exit(-1);
            }
        } while (user::get_instance()->check_account(new_user.name) ||!user::get_instance()->check_length(new_user.name));

        // 注册密码
        retry = 0;
        
            SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
            std::cout << "请输入密码 (4-20位，字母、数字或下划线): ";
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

            // 密码输入（带星号显示）
            new_user.password.clear();
            DWORD mode;
            GetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), &mode);
            SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), mode & (~ENABLE_ECHO_INPUT));

            char ch;
    do
    {
            while ((ch = _getch()) != '\r') {
                if (ch == '\b') {
                    if (!new_user.password.empty()) {
                        new_user.password.pop_back();
                        std::cout << "\b \b";
                    }
                }
                else if (ch >= 32 && ch <= 126) {
                    new_user.password.push_back(ch);
                    std::cout << '*';
                }
            }
            std::cout << std::endl;
            SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), mode);
            retry++;

            if (!user::get_instance()->check_length(new_user.password)) {
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
                std::cout << "密码格式不正确\n";
                retry++;
                continue;
            }
            if (retry == 3)
            { 
                cout << "您已失败三次，失败五次后将强制退出" << endl;
                
            }
            if (retry == 5)
            {
                cout << "您已失败五次，强制退出\n";
                exit(-1);
            }
            
        } while (!user::get_instance()->check_length(new_user.password));

        // 创建账号
        if (user::get_instance()->insert_user(new_user)) {
            SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
            std::cout << "账号创建成功！\n";
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

            user::get_instance()->user_name = new_user.name;
            system("cls");

            // 创建用户相关表
            if(user::get_instance()->create_history_table(new_user.name))   std::cout<<"创建历史记录表成功！\n";
            if(user::get_instance()->create_error_book(new_user.name))  std::cout<<"创建错题本成功！\n";
            if(user::get_instance()->create_own_wordlist(new_user.name)) std::cout<<"创建词库成功！\n";
            
            
            // 首次签到
            std::cout<<"欢迎你， "<<user::get_instance()->user_name<<std::endl;
            handle_checkin(new_user.name, hConsole);
            std::this_thread::sleep_for(std::chrono::seconds(2));
            return new_user.name;
        }

        SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
        std::cout << "账号创建失败\n";
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        return "";
    }

void login::handle_checkin(const std::string& account, HANDLE hConsole) {
        std::cout << "上次签到日期：" << CheckIn::get_instance()->get_last_checkin_day(account) << "\n";
        std::this_thread::sleep_for(std::chrono::seconds(2));

        if (!CheckIn::get_instance()->check_in(account)) {
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
            std::cout << "签到失败！\n";
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        }

        std::cout << "您已签到 " << CheckIn::get_instance()->get_checkin_days(account) << " 天\n";
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

