//C++ Boot Camp - Task 2 - 2018-19 
//Name: Maodan Luo
//Student number: 27042120

#include "DatabaseManager.h"
#include <atltime.h>
#include <fstream> 
#include <string>
#include <sstream>
#include <iostream>

DatabaseManager::DatabaseManager()
{

}

DatabaseManager::~DatabaseManager()
{
	// We must delete users since we have take ownership.
	for (auto it : m_users)
	{
		if (it.second) delete it.second;
	}
	//delete guest users.
	for (auto it : m_gUsers)
	{
		if (it.second) delete it.second;
	}
}

DatabaseManager& DatabaseManager::instance()
{
	static DatabaseManager s_instance;
	return s_instance;
}

void DatabaseManager::load_data()
{
	//load games from gamelist.csv =>m_games
	std::ifstream games_stream("gamelist.csv", std::ios::in| std::ios::_Nocreate);
	if (!games_stream.fail())
	{
		std::string g_id;
		std::string g_title;
		std::string g_price;
		std::string g_desc;
		std::string g_version;
		std::string g_age;
		while (std::getline(games_stream, g_id, ',') && std::getline(games_stream, g_title, ',') &&std::getline(games_stream,g_price,',') &&std::getline(games_stream, g_desc, ',')&&std::getline(games_stream,g_version,',')&&std::getline(games_stream,g_age,'\n'))
		{
			add_game(Game(std::stoi(g_id), g_title,std::stod(g_price), g_desc,std::stoi(g_version),std::stoi(g_age)));
		}
		games_stream.close();
	}
	else
	{
		std::cout << "\nAn error has occurred when opening the file.\n";
	}

	//load admin users from adminlist.csv =>m_users
	std::ifstream admin_stream("adminlist.csv",std::ios::in | std::ios::_Nocreate);
	if (!admin_stream.fail())
	{
		std::string u_type;
		std::string u_name;
		std::string u_password;
		std::string u_mail;
		while (std::getline(admin_stream, u_type, ',')&& std::getline(admin_stream, u_name, ',')&& std::getline(admin_stream, u_password, ',') && std::getline(admin_stream, u_mail, '\n'))
		{
			if (std::stoi(u_type)== 2)
			{
				add_user(new AdminUser(u_name, u_password, u_mail));
			}
		}
		admin_stream.close();
	}
	else
	{
		std::cout << "\nAn error has occurred when opening the file.\n";
	}

	//load player users from playerlist.csv =>m_users
	std::ifstream player_stream("playerlist.csv", std::ios::in | std::ios::_Nocreate);
	if (!player_stream.fail())
	{
		std::string u_type;
		std::string u_name;
		std::string u_password;
		std::string u_mail;
		std::string u_account_funds;
		std::string u_owned_game;
		std::string u_age;
		while (std::getline(player_stream, u_type, ',') && std::getline(player_stream, u_name, ',') && std::getline(player_stream, u_password, ',') && std::getline(player_stream, u_mail, ',')&&std::getline(player_stream,u_age,',')&&std::getline(player_stream,u_account_funds,',')&&std::getline(player_stream,u_owned_game,'\n'))
		{
			if (std::stoi(u_type) == 1)
			{
				std::list<int>mygame;
				std::stringstream iss(u_owned_game);
				int game;
				while(iss>>game)
				{
					mygame.push_back(game);
				}
				add_user(new PlayerUser(u_name, u_password, u_mail, std::stod(u_account_funds),mygame,std::stoi(u_age)));
			}
		}
		player_stream.close();
	}
	else
	{
		std::cout << "\nAn error has occurred when opening the file.\n";
	}

	//load game studio users from gamestudiolist.csv =>m_users
	std::ifstream gamestudio_stream("gamestudiolist.csv", std::ios::in | std::ios::_Nocreate);
	if (!gamestudio_stream.fail())
	{
		std::string u_type;
		std::string u_name;
		std::string u_password;
		std::string u_mail;
		std::string u_owned_game;
		while (std::getline(gamestudio_stream, u_type, ',') && std::getline(gamestudio_stream, u_name, ',') && std::getline(gamestudio_stream, u_password, ',') && std::getline(gamestudio_stream, u_mail, ',') && std::getline(gamestudio_stream, u_owned_game, '\n'))
		{
			if (std::stoi(u_type) == 4)
			{
				std::list<int>mygame;
				std::stringstream iss(u_owned_game);
				int game;
				while (iss >> game)
				{
					mygame.push_back(game);
				}
				add_user(new GameStudio(u_name, u_password, u_mail, mygame));
			}
		}
		player_stream.close();
	}
	else
	{
		std::cout << "\nAn error has occurred when opening the file.\n";
	}

	//load guest users from guestlist.csv =>m_gUsers
	std::ifstream gusets_stream("guestlist.csv", std::ios::in | std::ios::_Nocreate);
	if (!gusets_stream.fail())
	{
		std::string mail;
		while (std::getline(gusets_stream,mail, '\n'))
		{
			add_guest(new GuestUser(mail));
		}
		gusets_stream.close();
	}
	else
	{
		std::cout << "\nAn error has occurred when opening the file.\n";
	}
}

void DatabaseManager::store_adminuser_data(UserBase* pUser)
{
	std::ofstream user_stream;
	user_stream.open("adminlist.csv", std::ios::out | std::ios::app);
	if (!user_stream.fail())
	{
		user_stream << static_cast<std::underlying_type<UserTypeId>::type>(pUser->get_user_type()) << "," << pUser->get_username() << "," << pUser->get_password() << "," << pUser->get_email() << std::endl;
		user_stream.close();
	}
	else
	{
		std::cout << "\nAn error has occurred when opening the file.\n";
	}
}

void DatabaseManager::store_playeruser_data(UserBase*puser)
{
	PlayerUser* pUser = static_cast<PlayerUser*>(puser);
	std::ofstream user_stream;
	user_stream.open("playerlist.csv", std::ios::out | std::ios::app);
	if (!user_stream.fail())
	{
		std::string str_gamelist;
		std::list<Game::GameId>my_game = pUser->get_game_list();
		if (my_game.front()!=0)
		{
			for (int it : my_game)
			{
				str_gamelist = str_gamelist+std::to_string(it) + " ";
			}
		}
		else
		{
			str_gamelist = "0";
		}
		user_stream << static_cast<std::underlying_type<UserTypeId>::type>(pUser->get_user_type()) << "," << pUser->get_username() << "," << pUser->get_password() << "," << pUser->get_email() <<","<<pUser->get_myage()<<","<<pUser->get_available_funds()<<","<< str_gamelist << std::endl;
		user_stream.close();
	}
	else
	{
		std::cout << "\nAn error has occurred when opening the file.\n";
	}
}

void DatabaseManager::store_gamestudio_data(UserBase*puser)
{
	GameStudio* pUser = static_cast<GameStudio*>(puser);
	std::ofstream user_stream;
	user_stream.open("gamestudiolist.csv", std::ios::out | std::ios::app);
	if (!user_stream.fail())
	{
		std::string str_gamelist;
		std::list<Game::GameId>my_game = pUser->accessible_gamelist();
		if (my_game.front() != 0)
		{
			for (int it : my_game)
			{
				str_gamelist = str_gamelist + std::to_string(it) + " ";
			}
		}
		else
		{
			str_gamelist = "0";
		}
		user_stream << static_cast<std::underlying_type<UserTypeId>::type>(pUser->get_user_type()) << "," << pUser->get_username() << "," << pUser->get_password() << "," << pUser->get_email() << "," << str_gamelist << std::endl;
		user_stream.close();
	}
	else
	{
		std::cout << "\nAn error has occurred when opening the file.\n";
	}
}

void DatabaseManager::store_guest_data(GuestUser*gUser)
{
	std::ofstream guest_stream;
	guest_stream.open("guestlist.csv", std::ios::out | std::ios::app);
	if (!guest_stream.fail())
	{
		guest_stream << gUser->get_email()<< std::endl;
		guest_stream.close();
	}
	else
	{
		std::cout << "\nAn error has occurred when opening the file.\n";
	}
}

void DatabaseManager::store_game_data(const Game &rGame)
{
	std::ofstream game_stream;
	game_stream.open("gamelist.csv", std::ios::out | std::ios::app);
	if (!game_stream.fail())
	{
		game_stream << rGame.get_game_id() << "," << rGame.get_title() << "," <<rGame.get_game_Price()<<","<< rGame.get_game_desc() <<","<<rGame.get_version()<<","<<rGame.get_ageRestriction()<< std::endl;
		game_stream.close();
	}
	else
	{
		std::cout << "\nAn error has occurred when opening the file.\n";
	}	
}

void DatabaseManager::update_games_data()
{
	//clean data
	std::ofstream games_stream;
	games_stream.open("gamelist.csv", std::ios::out);
	games_stream.close();
	//store data
	for (auto it : m_games)
	{
		store_game_data(it.second);
	}
}

void DatabaseManager::update_player_data()
{
	//clean
	std::ofstream player_stream;
	player_stream.open("playerlist.csv", std::ios::out);
	player_stream.close();
	//store
	for (auto it : m_users)
	{
		if (it.second->get_user_type() == UserTypeId::kPlayerUser)
		{
			store_playeruser_data(it.second);
		}
	}
}

void DatabaseManager::add_user(UserBase* pUser)
{
	// Store the user instance in the user map, indexed by username.
	// We are taking ownership of the memory pointer to.
	if (pUser)
	{
		m_users.insert(std::make_pair(pUser->get_username(), pUser));
	}
}

void DatabaseManager::add_guest(GuestUser*gUser)
{
	if (gUser)
	{
		m_gUsers.insert(std::make_pair(gUser->get_email(),gUser));
	}
}

void DatabaseManager::add_game(Game&rGame)
{
	// Store the game indexed by game id.
	m_games.insert(std::make_pair(rGame.get_game_id(), rGame));
}

void DatabaseManager::add_and_store_adminuser(UserBase* pUser)
{
	add_user(pUser);
	store_adminuser_data(pUser);
}

void DatabaseManager::add_and_store_playeruser(UserBase*pUser)
{
	add_user(pUser);
	store_playeruser_data(pUser);
}

void DatabaseManager::add_and_store_gamestudio(UserBase*pUser)
{
	add_user(pUser);
	store_gamestudio_data(pUser);
}

void DatabaseManager::add_and_store_game(Game&rGame)
{
	add_game(rGame);
	store_game_data(rGame);
}

void DatabaseManager::add_and_stroe_guest(GuestUser*gUser)
{
	add_guest(gUser);
	store_guest_data(gUser);
}

UserBase* DatabaseManager::find_user(const UserBase::Username& username)
{
	auto it = m_users.find(username);
	if (it != m_users.end())
	{
		return it->second;
	}
	else
	{
		return nullptr;
	}
}

GuestUser*DatabaseManager::find_guest(const std::string&mail)
{
	auto it = m_gUsers.find(mail);
	if (it != m_gUsers.end())
	{
		return it->second;
	}
	else
	{
		return nullptr;
	}
}

Game* DatabaseManager::find_game(const Game::GameId gameid)
{
	auto it = m_games.find(gameid);
	if (it != m_games.end())
	{
		return &it->second;
	}
	else
	{
		return nullptr;
	}
}

void DatabaseManager::remove_game(int id)
{
	m_games.erase(id);
}

void DatabaseManager::give_away(UserBase*puser, std::string name,int id)
{
	auto user = DatabaseManager::instance().find_user(name);

	if (puser->get_user_type() == UserTypeId::kPlayerUser)
	{
		//if the giver is player
		PlayerUser* pPlayerUser = static_cast<PlayerUser*>(puser);
		//whether the friend exists
		if (user)
		{
			if (user->get_user_type() == UserTypeId::kPlayerUser)
			{			
				bool own = false;
				//if player own the game
				for (int it : pPlayerUser->get_game_list())
				{
					if (id == it)
					{
						own = true;
						break;
					}
				}
				if (own)
				{
					PlayerUser* pUser = static_cast<PlayerUser*>(user);
					bool friendown = false;
					//if friend own the game
					for (int it : pUser->get_game_list())
					{
						if (id == it)
						{
							friendown = true;
							std::cout << "Your friend already owns the game\n\n";
							break;
						}
					}
					//if(friend does not have this game){judge the friend`s age}
					if (!friendown)
					{
						auto rGame = DatabaseManager::instance().find_game(id);
						if (pUser->get_myage() >= rGame->get_ageRestriction())
						{
							pPlayerUser->pop_ownedGame(id);
							pUser->add_ownedGame(id);
							DatabaseManager::instance().update_player_data();
							std::cout << "Give away successfully\n\n";
						}
						else
						{
							std::cout << "Your friend do not suitable this game(age)\n\n";
						}
					}
				}
				else
				{
					std::cout << "You do not have this game\n\n";
				}
			}
			else 
			{
				std::cout << "You can't give the game to the user\n\n";
			}
		}
		else
		{
			std::cout << "The user does not exist\n\n";
		}
	}
	else if (puser->get_user_type() == UserTypeId::kGameStudio)
	{
		//if the giver is gamestudio
		GameStudio*pPlayerUser = static_cast<GameStudio*>(puser);
		////whether the player exists
		if (user)
		{
			if (user->get_user_type() == UserTypeId::kPlayerUser)
			{				
				bool own = false;
				//if gamestudio own this game
				for (int it : pPlayerUser->accessible_gamelist())
				{
					if (id == it)
					{
						own = true;
						break;
					}
				}
				if (own)
				{
					//if player own the game
					PlayerUser* pUser = static_cast<PlayerUser*>(user);
					bool friendown = false;
					for (int it : pUser->get_game_list())
					{
						if (id == it)
						{
							friendown = true;
							std::cout << "Your friend already owns the game\n\n";
							break;
						}
					}
					//if(player does not have this game){judge the player`s age}
					if (!friendown)
					{
						auto rGame = DatabaseManager::instance().find_game(id);
						if (pUser->get_myage() >= rGame->get_ageRestriction())
						{
							pUser->add_ownedGame(id);
							DatabaseManager::instance().update_player_data();
							std::cout << "Give away successfully\n\n";
						}
						else
						{
							std::cout << "Your friend do not suitable this game(age)\n\n";
						}
					}
				}
				else
				{
					std::cout << "You do not have this game\n\n";
				}
			}
			else 
			{
				std::cout << "You can't give the game to the user\n\n";
			}
		}
		else
		{
			std::cout << "The user does not exist\n\n";
		}
	}	
}

void DatabaseManager::play_game(UserBase*puser, int id)
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
	bool own = false;
	for (int it : mygames)
	{
		if (id == it)
		{
			own = true;
			break;
		}
	}
	if (own)
	{
		//play
		auto rGame = DatabaseManager::instance().find_game(id);
		std::string start_time = DatabaseManager::instance().get_time();
		//start
		time_t begin, end;
		double dur;
		begin = clock();

		std::cout << "You are playing " << rGame->get_title() << "\n\n";
		char quit;
		do
		{
			std::cout << "Quit the game(Y/N)\n";
			std::cin >> quit;
			std::cout << "You are playing " << rGame->get_title() << "\n\n";
		} while (quit == 'N' || quit == 'n');
		std::cout << "You have successfully exited\n\n";
		//end
		end = clock();
		dur = double(end - begin) / CLOCKS_PER_SEC;
		DatabaseManager::instance().store_player_activityInfo(puser, rGame, dur, start_time);
	}
	else
	{
		std::cout << "You do not have this game\n\n";
	}
}

void DatabaseManager::search_game(const int i, const std::string s, const double pri_upper, const double pri_lower,const int age )
{
	bool is1 = false;
	bool is2 = false;
	bool is3 = false;
	bool is4 = false;
	for (auto it : m_games)
	{
		auto rGame = it.second;
		switch (i)
		{
		case 1:
		{
			//search by title
			std::string title = rGame.get_title();
			std::string::size_type idx = title.find(s);
			if (idx != std::string::npos)
			{
				std::cout << "id: " << rGame.get_game_id() << ", title: " + rGame.get_title() << ", price: " << rGame.get_game_Price() << ", description: " + rGame.get_game_desc() << "\n";
				is1 = true;
			}
			break;
		}
		case 2:
		{
			//search by description
			std::string desc = rGame.get_game_desc();
			std::string::size_type idx = desc.find(s);
			if (idx != std::string::npos)
			{
				std::cout << "id: " << rGame.get_game_id() << ", title: " + rGame.get_title() << ", price: " << rGame.get_game_Price() << ", description: " + rGame.get_game_desc() << "\n";
				is2 = true;
			}
			break;
		}
		case 3:
		{
			//search by price
			double price = rGame.get_game_Price();
			if (price <= pri_upper && price >= pri_lower)
			{
				std::cout << "id: " << rGame.get_game_id() << ", title: " + rGame.get_title() << ", price: " << rGame.get_game_Price() << ", description: " + rGame.get_game_desc() << "\n";
				is3 = true;
			}
			break;
		}
		case 4:
		{
			//search by age restriction
			int ageRestri = rGame.get_ageRestriction();
			if (age>=ageRestri)
			{
				std::cout << "id: " << rGame.get_game_id() << ", title: " + rGame.get_title() << ", price: " << rGame.get_game_Price() << ", description: " + rGame.get_game_desc() <<", age restriction: "<< ageRestri<< "\n";
				is4 = true;
			}
			break;
		}
		default:std::cout << "Parameter error\n"; break;
		}
	}
	if (!is1&&!is2&&!is3&&!is4)
	{
		std::cout << "Search for games that don't meet the criteria\n\n";
	}
}

void DatabaseManager::store_purchase_history(PlayerUser*pUser, Game*rGame)
{
	std::string cur_time = get_time();
	std::ofstream purchase_stream;
	purchase_stream.open("purchaseHistory.csv", std::ios::out | std::ios::app);
	if (!purchase_stream.fail())
	{
		purchase_stream <<pUser->get_username()<<","<<"Game Id: "<< rGame->get_game_id() << ","<<"Game Title: " << rGame->get_title() <<","<<"Purchase Time: "<<cur_time<< std::endl;
		purchase_stream.close();
	}
	else
	{
		std::cout << "\nAn error has occurred when opening the file.\n";
	}
}

void DatabaseManager::check_purchase_history()
{
	std::ifstream purchase_stream("purchaseHistory.csv", std::ios::in);
	if (!purchase_stream.fail())
	{
		std::string pur_history;
		while (std::getline(purchase_stream, pur_history, '\n'))
		{
			if (!pur_history.empty())
			{
				std::cout << pur_history << std::endl;
			}
			else
			{
				std::cout << "No Purchase History" << std::endl;
			}			
		}
		purchase_stream.close();
	}
	else
	{
		std::cout << "No Purchase History" << std::endl;
	}
}

void DatabaseManager::store_player_activityInfo(UserBase*pUser, Game*rGame, double&time,std::string&start_time)
{
	std::ofstream info_stream;
	std::string play_time = std::to_string(time);
	info_stream.open("activityInfo.csv", std::ios::out | std::ios::app);
	if (!info_stream.fail())
	{
		info_stream << rGame->get_game_id() << ","<< rGame->get_title() << "," <<pUser->get_username() << "," <<start_time<<","<< play_time << std::endl;
		info_stream.close();
	}
	else
	{
		std::cout << "\nAn error has occurred when opening the file.\n";
	}
}

//if i=1,print the player activity info,if i=2,print the most populargame
void DatabaseManager::check_player_activityInfo(const int i)
{
	std::ifstream info_stream("activityInfo.csv", std::ios::in);
	if (!info_stream.fail())
	{
		std::string gameid;
		std::string gamename;
		std::string starttime;
		std::string playingtime;
		std::string username;
		std::string populargame="There are no most popular games";
		double max = 0;
		while (std::getline(info_stream, gameid, ',')&& std::getline(info_stream, gamename, ',')&&std::getline(info_stream, username, ',')&& std::getline(info_stream, starttime, ',')&& std::getline(info_stream, playingtime, '\n'))
		{
			if (i == 1)
			{
				if (!gameid.empty())
				{
					std::cout << "gameid : " << gameid
						<< "game name: " << gamename
						<< "user name: " << username
						<< "login time: " << starttime
						<< "playing time: " << playingtime << "s" << std::endl;
				}
				else
				{
					std::cout << "No Activity Info" << std::endl;
				}
			}
			else if(i==2)
			{
				double durtime = std::stod(playingtime);
				if (durtime >= max)
				{
					max = durtime;
					populargame = gamename;
				}				
			}			
		}
		if (i == 2)
		{
			std::cout << "The most popular game is: "+ populargame<<std::endl;
		}
		info_stream.close();
	}
	else
	{
		std::cout << "No Activity Info" << std::endl;
	}
}

double DatabaseManager::average_price()
{
	if (!m_games.empty())
	{
		double price = 0;
		for (auto it : m_games)
		{
			auto rGame = it.second;
			price += rGame.get_game_Price();
		}
		return price / m_games.size();
	}
	else
	{
		return 0;
	}
}

std::string DatabaseManager::get_time()
{
	CTime tmp;
	tmp = CTime::GetCurrentTime();
	int y = tmp.GetYear();
	int m = tmp.GetMonth();
	int d = tmp.GetDay();
	int h = tmp.GetHour();
	int mi = tmp.GetMinute();
	int se = tmp.GetSecond();
	std::string cur_time = std::to_string(y) + "/" + std::to_string(m) + "/" + std::to_string(d) + " " + std::to_string(h) + ":" + std::to_string(mi) + ":" + std::to_string(se);
	return cur_time;
}