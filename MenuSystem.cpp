//C++ Boot Camp - Task 2 - 2018-19 
//Name: Maodan Luo
//Student number: 27042120 

#include "MenuSystem.h"
#include <ctime>
MenuSystem& MenuSystem::instance()
{
	static MenuSystem s_instance;
	return s_instance;
}

void MenuSystem::list_all_games() const
{
	auto gameVisitorLambda = [](const Game& rGame) {
		std::cout << "id: "<<rGame.get_game_id()<<", title: "+rGame.get_title()<<", price: "<<rGame.get_game_Price()<<", age restriction: "<<rGame.get_ageRestriction()<<", description: "+rGame.get_game_desc()<< "\n";
	};

	DatabaseManager::instance().visit_games(gameVisitorLambda);
}

void MenuSystem::list_my_games(UserBase*puser)const
{
	std::list<Game::GameId> mygames;
	if (puser->get_user_type() == UserTypeId::kPlayerUser)
	{
		PlayerUser* pUser = static_cast<PlayerUser*>(puser);
		mygames = pUser->get_game_list();
	}
	else if (puser->get_user_type() == UserTypeId::kGameStudio)
	{
		GameStudio*pUser = static_cast<GameStudio*>(puser);
		mygames = pUser->accessible_gamelist();
	}
	
	if (mygames.front() != 0)
	{
		for (auto it : mygames)
		{
			auto rGame = DatabaseManager::instance().find_game(it);
			std::cout << "id: " << rGame->get_game_id() << ", title: " + rGame->get_title() << ", price: " << rGame->get_game_Price() << ", description: " + rGame->get_game_desc() <<", game version: " <<rGame->get_version()<<"\n";
		}
	}
	else
	{
		std::cout << "You do not have any games\n\n";
	}
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

int MenuSystem::run_guest_screen()
{
	m_gUser = nullptr;
	std::string mail;
	std::cout << "Please input your email\n";
	std::cin >> mail;
	auto gUser = DatabaseManager::instance().find_guest(mail);
	if (!gUser)
	{
		DatabaseManager::instance().add_and_stroe_guest(new GuestUser(mail));
		auto guser= DatabaseManager::instance().find_guest(mail);
		std::cout << "Hi " << guser->get_email() << "\n";
		m_gUser = guser;
	}
	else
	{
		std::cout << "Hi " << gUser->get_email() << "\n";
		std::cout << "This is not your first landing! Welcome to Ragister! \n";
		m_gUser = gUser;
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
		std::cout << "(7) Check The Game Purchase History\n";
		std::cout << "(8) Check Player Activity Information\n";
		std::cout << "(9) Check out the most popular games\n"; 
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
			std::cout << "Please Input New Game ID\n";
			std::cin >> g_id;
			//判断ID是否已存在
			if (!DatabaseManager::instance().find_game(g_id))
			{
				std::string g_title;
				double g_price;
				std::string g_desc;
				int versionNumber;
				int ageRestriction;
				std::cout << "Please Input New Game Title\n";
				std::cin.ignore();
				std::getline(std::cin, g_title, '\n');
				std::cout << "Please Input New Game Price\n";
				std::cin >> g_price;
				std::cout << "Please Input New Game Description\n";
				std::cin.ignore();
				std::getline(std::cin, g_desc, '\n');
				std::cout << "Please Input New Game Version Numbers\n";
				std::cin >> versionNumber;
				std::cout << "Please Input New Game Age Restriction\n";
				std::cin >> ageRestriction;
				DatabaseManager::instance().add_and_store_game(Game(g_id, g_title, g_price, g_desc,versionNumber,ageRestriction));
				std::cout << "Added successfully\n";
			}
			else
			{
				std::cout << "The game already exists\n\n";
			}		
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
			std::cout << "(3) Add GameStudio\n";
			std::cin >> op;
			std::cout << "Please Input New User Name\n";
			std::cin >> u_name;
			//判断用户是否存在
			if (!DatabaseManager::instance().find_user(u_name))
			{

				std::cout << "Please Input New User Passerword\n";
				std::cin >> u_password;
				std::cout << "Please Input New User Email\n";
				std::cin >> u_mail;
				switch (op)
				{
				case'1':
				{
					DatabaseManager::instance().add_and_store_adminuser(new AdminUser(u_name, u_password, u_mail));
					std::cout << "Added successfully\n";
					break;
				}
				case'2':
				{
					std::list<Game::GameId>gamelist;//默认初始化为空
					gamelist.push_back(0);
					int age;
					std::cout << "Please Input New User Age\n";
					std::cin >> age;
					DatabaseManager::instance().add_and_store_playeruser(new PlayerUser(u_name, u_password, u_mail, 0.0, gamelist,age));
					std::cout << "Added successfully\n";
					break;
				}
				case'3':
				{
					//gamestudio
					std::list<Game::GameId>gamelist;//默认初始化为空
					gamelist.push_back(0);
					DatabaseManager::instance().add_and_store_gamestudio(new GameStudio(u_name, u_password, u_mail, gamelist));
					std::cout << "Added successfully\n";
					break;
				}
				default:std::cout << "INAVLID OPTION\n"; break;
				}
			}
			else 			
			{
				std::cout << "The user already exists\n\n";
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
				double modify_price;
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
		case'7':
		{
			//CHECK THE GAME PURCHASE HISTORY
			DatabaseManager::instance().check_purchase_history();
			break;
		}
		case'8':
		{
			DatabaseManager::instance().check_player_activityInfo(1);
			break;
		}
		case'9':
		{
			DatabaseManager::instance().check_player_activityInfo(2);
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
		std::cout << "Wallet :" << pPlayerUser->get_available_funds() << "\n";
		std::cout << "(1) List All Games\n";
		std::cout << "(2) List My Games\n";
		std::cout << "(3) Buy Game\n";
		std::cout << "(4) Search Game\n";
		std::cout << "(5) Give Away\n";
		std::cout << "(6) Play Game\n";
		std::cout << "(7) Add Funds\n";
		std::cout << "(8) Check out the most popular games\n";
		std::cout << "(q) Logout\n";

		char option;
		std::cin >> option;

		switch (option)
		{
		case '1': list_all_games(); break;
		case '2': 
		{
			//"(2) List My Games\n";
			list_my_games(pPlayerUser);
			break;
		}
		case '3': 
		{
			//"(3) Buy Game\n";
			list_all_games();
			int id;
			std::cout << "Please input the id that the game you want to buy\n";
			std::cin >> id;
			bool isin = false;
			//判断是否已经拥有
			for (int it : pPlayerUser->get_game_list())
			{
				if (id == it)
				{
					std::cout << "You already own the game\n\n";
					isin = true;
					break;
				}
			}
			//如果没有拥有
			if (!isin)
			{
				auto rGame = DatabaseManager::instance().find_game(id);
				if (rGame)
				{
					//判断年龄
					if (rGame->get_ageRestriction() <= pPlayerUser->get_myage())
					{
						//正常购买
						double game_price = rGame->get_game_Price();
						double available_funds = pPlayerUser->get_available_funds();
						if (game_price <= available_funds)
						{
							//如果钱够，就买下
							pPlayerUser->set_accountFunds(available_funds - game_price);
							//添进游戏库(判断第一个值是不是0)
							if (pPlayerUser->get_game_list().front() != 0)
							{
								pPlayerUser->add_ownedGame(id);
							}
							else
							{
								pPlayerUser->add_ownedGame(id);
								pPlayerUser->pop_ownedGame(0);
							}
							DatabaseManager::instance().update_player_data();
							DatabaseManager::instance().store_purchase_history(pPlayerUser, rGame);
							std::cout << "Buy successfully\n\n";
						}
						else
						{
							std::cout << "You do not have enough money\n\n";
						}
					}
					else
					{
						std::cout << "This game is not  suitabile for you(age)\n\n";
					}					
				}
				else
				{
					std::cout << "Vapor do not have this game\n\n";
				}
			}			
			break;
		}
		case'4':
		{
			//search game
			char how;
			std::cout << "How do you want to search\n"
				<< "(1)By title\n"
				<< "(2)By description\n"
				<< "(3)By price\n"
				<< "(4)By age restriction\n";
			std::cin >> how;
			switch (how)
			{
			case'1':
			{
				std::cout << "Please input the title you want to search\n";
				std::string s_title;
				std::cin.ignore();
				std::getline(std::cin, s_title, '\n');
				DatabaseManager::instance().search_game(1,s_title);
				break;
			}
			case'2':
			{
				std::cout << "Please input the description you want to search\n";
				std::string s_description;
				std::cin.ignore();
				std::getline(std::cin, s_description, '\n');
				DatabaseManager::instance().search_game(2, s_description);
				break;
			}
			case'3':
			{
				double pri_upper, pri_lower;
				std::cout << "Please input the price cap\n";
				std::cin >> pri_upper;
				std::cout << "Please input the price floor\n";
				std::cin >> pri_lower;
				DatabaseManager::instance().search_game(3," ", pri_upper, pri_lower);
				break;
			}
			case'4':
			{
				int age;
				std::cout << "Please input the age restriction\n";
				std::cin >> age;
				DatabaseManager::instance().search_game(4, " ",0,0,age);
				break;
			}
			default:  std::cout << "INAVLID OPTION\n"; break;
			}
			break;
		}
		case'5':
		{
			//give away
			std::cout << "Your games:\n";
			list_my_games(pPlayerUser);
			std::string u_name;
			int id;
			std::cout << "Input the name you want to give it to\n";
			std::cin >> u_name;
			std::cout << "Please select one of your games you want to give away and Input the id\n";
			std::cin >> id;
			DatabaseManager::instance().give_away(pPlayerUser, u_name, id);
			break;
		}
		case'6':
		{
			//play game
			if (pPlayerUser->get_game_list().front()==0)
			{
				std::cout << "You do not have any games\n\n";
			}
			else
			{
				int id;
				std::cout << "Your games:\n";
				list_my_games(pPlayerUser);
				std::cout << "\nWhich game do you want to play?Input the game id\n";
				std::cin >> id;
				DatabaseManager::instance().play_game(pPlayerUser, id);
			}			
			break;
		}
		case '7': 
		{
			//(4) Add Funds\n"
			double topup;
			std::cout << "Please input the amount you want to top up\n";
			std::cin >> topup;
			pPlayerUser->set_accountFunds(topup+pPlayerUser->get_available_funds());
			DatabaseManager::instance().update_player_data();
			std::cout << "Top up successfully\n";
			break;
		}
		case'8':
		{
			DatabaseManager::instance().check_player_activityInfo(2);
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

int MenuSystem::run_gamestudio_menu()
{
	GameStudio *pGS=static_cast<GameStudio*>(m_pUser);
	int result = 0;
	do
	{
		std::cout << "GameStudio Menu (" << m_pUser->get_username() << ")\n";
		std::cout << "(1) List My Games\n";
		std::cout << "(2) Give Away\n";
		std::cout << "(3) Play My Game\n";
		std::cout << "(4) Upload My Game\n";
		std::cout << "(5) Update Version\n";
		std::cout << "(q) Logout\n";

		char option;
		std::cin >> option;
		switch (option)
		{
		case'1':
		{
			list_my_games(pGS);
			break;
		}
		case'2':
		{
			//give away
			std::cout << "Your games:\n";
			list_my_games(pGS);
			std::string u_name;
			int id;
			std::cout << "Input the name you want to give it to\n";
			std::cin >> u_name;
			std::cout << "Please select one of your games you want to give away and Input the id\n";
			std::cin >> id;
			DatabaseManager::instance().give_away(pGS, u_name, id);
			break;
		}
		case'3':
		{
			//play game
			if (pGS->accessible_gamelist().front() == 0)
			{
				std::cout << "You do not have any games\n\n";
			}
			else
			{
				int id;
				std::cout << "Your games:\n";
				list_my_games(pGS);
				std::cout << "\nWhich game do you want to play?Input the game id\n";
				std::cin >> id;
				DatabaseManager::instance().play_game(pGS, id);
			}
			break;
		}
		case'4':
		{
			int g_id;
			std::cout << "Please Input Your Game ID\n";
			std::cin >> g_id;
			//判断ID是否已存在
			if (!DatabaseManager::instance().find_game(g_id))
			{
				std::string g_title;
				double g_price;
				std::string g_desc;
				int versionNumber;
				int ageRestriction;
				std::cout << "Please Input Your Game Title\n";
				std::cin.ignore();
				std::getline(std::cin, g_title, '\n');
				std::cout << "Please Input Your Game Price\n";
				std::cin >> g_price;
				std::cout << "Please Input Your Game Description\n";
				std::cin.ignore();
				std::getline(std::cin, g_desc, '\n');
				std::cout << "Please Input Your Game Version Numbers\n";
				std::cin >> versionNumber;
				std::cout << "Please Input Your Game Age Restriction\n";
				std::cin >> ageRestriction;
				DatabaseManager::instance().add_and_store_game(Game(g_id, g_title, g_price, g_desc, versionNumber, ageRestriction));
				pGS->add_ownedGame(g_id);
				std::cout << "Added successfully\n";
			}
			else
			{
				std::cout << "The game already exists\n\n";
			}
			break;
		}
		case'5':
		{
			//upload version
			if (pGS->accessible_gamelist().front() == 0)
			{
				std::cout << "You do not have any games\n\n";
			}
			else
			{
				int id;
				std::cout << "Your games:\n";
				list_my_games(pGS);
				std::cout << "\nInput the new version`s game id\n";
				std::cin >> id;

				bool own = false;
				//判断是否拥有
				for (int it : pGS->accessible_gamelist())
				{
					if (id == it)
					{
						own = true;
						break;
					}
				}
				if (own)
				{
					auto rGame = DatabaseManager::instance().find_game(id);
					std::cout << "The old version number: " << rGame->get_version() << std::endl;
					int version;
					std::cout << "What is the new version number?Please input\n";
					std::cin >> version;
					rGame->set_version(version);
					DatabaseManager::instance().update_games_data();
					std::cout << "Here simulates uploading a new version of the game\n";
					std::cout<< "The new version number: " << rGame->get_version() << std::endl;
					std::cout << "Update Version Successfully\n";
				}
				else 
				{
					std::cout << "You cannot update this game`s version\n\n";
				}				
			}
			break;
		}
		case 'q': result = -1; break;
		default:  std::cout << "INAVLID OPTION\n"; break;
		}
	} while (result == 0);
	m_pUser = nullptr;
	return 0;
}

int MenuSystem::run_guest_user_menu()
{

	int result = 0;
	do
	{
		std::cout << "Guest Menu (" << m_gUser->get_email() << ")\n";
		std::cout << "(1) List All Games\n";
		std::cout << "(2) Sign-up\n";
		std::cout << "(q) Exit\n";

		char option;
		std::cin >> option;

		switch (option)
		{
		case'1':
		{
			list_all_games();
			break;
		}
		case'2':
		{
			std::cout << "The ‘Sign-up’ facility is not to be implemented here\n\n";
			break;
		}
		case 'q': result = -1; break;
		default:  std::cout << "INAVLID OPTION\n"; break;
		}
	} while (result == 0);

	m_gUser = nullptr;
	return 0;
}

int MenuSystem::run_unknown_user_menu()
{
	// in this menu we get the username and password.
	int result = 0;

	std::cout << "Main Menu\n";
	std::cout << "(1) List All Games\n";
	std::cout << "(2) Login\n";
	std::cout << "(3) Guest\n";
	std::cout << "(q) Quit\n";

	char option;
	std::cin >> option;

	switch (option)
	{
	case '1': list_all_games(); break;
	case '2': run_login_screen(); break;
	case '3': run_guest_screen(); break;
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
		if (!m_pUser&&!m_gUser)
		{
			result = run_unknown_user_menu();
		}
		else if(m_pUser&&!m_gUser)
		{
			switch (m_pUser->get_user_type())
			{
				case UserTypeId::kPlayerUser: result = run_player_user_menu(); break;
				case UserTypeId::kAdminUser: result = run_admin_user_menu(); break;
				case UserTypeId::kGameStudio:result = run_gamestudio_menu(); break;
				default: result = -1; break;
			}
		}
		else if (!m_pUser&& m_gUser)
		{		
			result = run_guest_user_menu();
		}
	} while (result == 0);

	return 0;
}
