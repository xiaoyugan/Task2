//C++ Boot Camp - Task 2 - 2018-19 
//Name: Maodan Luo
//Student number: 27042120 

#include "MenuSystem.h"

MenuSystem& MenuSystem::instance()
{
	static MenuSystem s_instance;
	return s_instance;
}

void MenuSystem::list_all_games() const
{
	auto gameVisitorLambda = [](const Game& rGame) {
		std::cout << "id: "<<rGame.get_game_id()<<", title: "+rGame.get_title()<<", price: "<<rGame.get_game_Price()<<", description: "+rGame.get_game_desc()<< "\n";
	};

	DatabaseManager::instance().visit_games(gameVisitorLambda);
}

void MenuSystem::list_all_users() const
{
	auto userVisitorLambda = [](const UserBase& rUser) {
		std::cout << rUser.get_username() << " " << rUser.get_email() << "\n";
	};

	DatabaseManager::instance().visit_users(userVisitorLambda);
}

int MenuSystem::run_login_screen()
{
	m_pUser = nullptr;

	// in this menu we get the username and password.
	std::string username;
	std::string password;

	std::cout << "Login Menu\n";
	std::cout << "Username: ";
	std::cin >> username;

	std::cout << "Password: ";
	std::cin >> password;

	// find the user and check password
	auto pUser = DatabaseManager::instance().find_user(username);
	if (pUser && pUser->get_password() == password)
	{
		m_pUser = pUser;
		std::cout << "Hi " << m_pUser->get_username() << "\n";
	}
	else
	{
		std::cout << "No such username or password!\n";
	}
	return 0;
}

int MenuSystem::run_admin_user_menu()
{
	AdminUser* pAdminUser = static_cast<AdminUser*>(m_pUser);
	int result = 0;
	do
	{
		std::cout << "Admin Menu (" << m_pUser->get_username() << ")\n";
		std::cout << "(1) List All Games\n";
		std::cout << "(2) List All Users\n";
		std::cout << "(3) Add Game\n";
		std::cout << "(4) Add User\n";
		std::cout << "(5) Modify Game\n";//todo
		std::cout << "(6) Remove Game\n";//todo
		std::cout << "(q) Logout\n";

		char option;
		std::cin >> option;

		switch (option)
		{
		case '1': list_all_games(); break;
		case '2': list_all_users(); break;
		case '3':
		{
			//std::cout << "TODO\n"; break;//调用添加游戏代码
			int g_id;
			float g_price;
			std::string g_title;
			std::string g_desc;
			std::cout << "Please Input New Game ID\n";
			std::cin >> g_id;
			std::cout << "Please Input New Game Title\n";
			std::cin >> g_title;
			std::cout << "Please Input New Game Price\n";
			std::cin >> g_price;
			std::cout << "Please Input New Game Description\n";
			std::cin >> g_desc;
			DatabaseManager::instance().add_and_store_game(Game(g_id, g_title, g_price, g_desc));
			std::cout << "Added successfully\n";
			break;
		}
		case '4': 
		{
			//std::cout << "TODO\n"; break;//调用userdatabase里面的add_users
			std::string u_name;
			std::string u_password;
			std::string u_mail;
			char op;
			std::cout << "(1) Add AdminUser\n";
			std::cout << "(2) Add PlayerUser\n";
			std::cin >> op;
			std::cout << "Please Input New User Name\n";
			std::cin >> u_name;
			std::cout << "Please Input New User Passerword\n";
			std::cin >> u_password;
			std::cout<< "Please Input New User Email\n";
			std::cin >> u_mail;
			switch (op)
			{
			case'1':DatabaseManager::instance().add_and_store_user(new AdminUser(u_name, u_password, u_mail)); 
					std::cout << "Added successfully\n"; 
					break;
			case'2':DatabaseManager::instance().add_and_store_user(new PlayerUser(u_name, u_password, u_mail));
					std::cout << "Added successfully\n";
					break;
			default:std::cout<< "INAVLID OPTION\n"; break;
			}
			break;
		}
		case'5':
		{
			//modify game
			int g_id;
			char op;
			list_all_games();
			std::cout << "Please Input The ID Of The Game That You Want to Modify\n";
			std::cin >> g_id;
			auto rGame = DatabaseManager::instance().find_game(g_id);
			std::cout << "(1) Modify The Title\n"
				      << "(2) Modify The Description\n"
					  << "(3) Modify The Price\n"
				      << "(4) Quite\n";
			std::cin >> op;
			switch (op)
			{
			case'1':
			{
				std::string modify_name;
				std::cout << "Please Input The New Title\n";
				std::cin >> modify_name;
				rGame->set_title(modify_name);
				DatabaseManager::instance().update_games_data();
				std::cout << "Modify the title successfully";
				break;
			}
			case'2':
			{
				std::string modify_desc;
				std::cout << "Please Input The New Description\n";
				std::cin >> modify_desc;
				rGame->set_description(modify_desc);
				DatabaseManager::instance().update_games_data();
				std::cout << "Modify the description successfully";
				break;
			}
			case'3':
			{
				float modify_price;
				std::cout << "Please Input The New Price\n";
				std::cin >> modify_price;
				rGame->set_price(modify_price);
				DatabaseManager::instance().update_games_data();
				std::cout << "Modify the description successfully";
				break;
			}
			case'4':break;
			default: std::cout << "INAVLID OPTION\n"; break;
			}
			break;
		}
		case'6':
		{
			//remove game
			int g_id;
			list_all_games();
			std::cout << "Please Input The ID Of The Game That You Want to Remove\n";
			std::cin >> g_id;
			DatabaseManager::instance().remove_game(g_id);
			DatabaseManager::instance().update_games_data();
			std::cout << "Remove the game successfully";
			break;
		}
		case 'q': result = -1; break;
		default:  std::cout << "INAVLID OPTION\n"; break;
		}
	} while (result == 0);

	// force logout.
	m_pUser = nullptr;

	return 0;
}

int MenuSystem::run_player_user_menu()
{
	PlayerUser* pPlayerUser = static_cast<PlayerUser*>(m_pUser);
	int result = 0;
	do
	{
		std::cout << "Player Menu (" << m_pUser->get_username() << ")\n";
		std::cout << "Wallet \x9C" << pPlayerUser->get_available_funds() << "\n";
		std::cout << "(1) List All Games\n";
		std::cout << "(2) List My Games\n";
		std::cout << "(3) Buy Game\n";
		std::cout << "(4) Add Funds\n";
		std::cout << "(q) Logout\n";

		char option;
		std::cin >> option;

		switch (option)
		{
		case '1': list_all_games(); break;
		case '2': std::cout << "TODO\n"; break;
		case '3': std::cout << "TODO\n"; break;
		case '4': std::cout << "TODO\n"; break;
		case 'q': result = -1; break;
		default:  std::cout << "INAVLID OPTION\n"; break;
		}
	} while (result == 0);

	// force logout.
	m_pUser = nullptr;

	return 0;
}

int MenuSystem::run_unknown_user_menu()
{
	// in this menu we get the username and password.
	int result = 0;

	std::cout << "Main Menu\n";
	std::cout << "(1) List All Games\n";
	std::cout << "(2) Login\n";
	std::cout << "(q) Quit\n";

	char option;
	std::cin >> option;

	switch (option)
	{
	case '1': list_all_games(); break;
	case '2': run_login_screen(); break;
	case 'q': result = -1;  break;
	default:  std::cout << "INAVLID OPTION\n"; break;
	}

	return result;
}

int MenuSystem::run()
{
	int result = 0;
	do
	{
		if (!m_pUser)
		{
			result = run_unknown_user_menu();
		}
		else
		{
			switch (m_pUser->get_user_type())
			{
				case UserTypeId::kPlayerUser: result = run_player_user_menu(); break;
				case UserTypeId::kAdminUser: result = run_admin_user_menu(); break;
				default: result = -1; break;
			}
		}
	} while (result == 0);

	return 0;
}
