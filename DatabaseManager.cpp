//C++ Boot Camp - Task 2 - 2018-19 
//Name: Maodan Luo
//Student number: 27042120

#include "DatabaseManager.h"
#include <fstream> 
#include <string>
#include<sstream>
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
}

DatabaseManager& DatabaseManager::instance()
{
	static DatabaseManager s_instance;
	return s_instance;
}

void DatabaseManager::load_data()
{
	//从表中读取游戏数据
	std::ifstream games_stream("gamelist.csv", std::ios::in| std::ios::_Nocreate);
	if (!games_stream.fail())
	{
		std::string g_id;
		std::string g_title;
		std::string g_price;
		std::string g_desc;
		while (std::getline(games_stream, g_id, ',') && std::getline(games_stream, g_title, ',') &&std::getline(games_stream,g_price,',') &&std::getline(games_stream, g_desc, '\n'))
		{
			add_game(Game(std::stoi(g_id), g_title,std::stod(g_price), g_desc));
		}
		games_stream.close();
	}
	else
	{
		std::cout << "\nAn error has occurred when opening the file.\n";
	}

	//从表中读取管理员数据
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

	//从表中读取玩家数据
	std::ifstream player_stream("playerlist.csv", std::ios::in | std::ios::_Nocreate);
	if (!player_stream.fail())
	{
		std::string u_type;
		std::string u_name;
		std::string u_password;
		std::string u_mail;
		std::string u_account_funds;
		std::string u_owned_game;
		while (std::getline(player_stream, u_type, ',') && std::getline(player_stream, u_name, ',') && std::getline(player_stream, u_password, ',') && std::getline(player_stream, u_mail, ',')&&std::getline(player_stream,u_account_funds,',')&&std::getline(player_stream,u_owned_game,'\n'))
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
				//split<std::list<int>>(u_owned_game, mygame, '//');
				add_user(new PlayerUser(u_name, u_password, u_mail, std::stod(u_account_funds),mygame));
			}
		}
		player_stream.close();
	}
	else
	{
		std::cout << "\nAn error has occurred when opening the file.\n";
	}
}

//存储玩家游玩的数据这些
void DatabaseManager::store_data()
{
	// 存储修改后和删除后的游戏数据
}

//添加一个就更新一个，实时把用户数据加入到表单，防止程序没有正常结束而使数据丢失
void DatabaseManager::store_adminuser_data(UserBase* pUser)
{
	//store user data 把用户数据再存到表里面
	//定义文件输出流 
	std::ofstream user_stream;
	//打开要输出的文件
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
	//打开要输出的文件
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
		user_stream << static_cast<std::underlying_type<UserTypeId>::type>(pUser->get_user_type()) << "," << pUser->get_username() << "," << pUser->get_password() << "," << pUser->get_email() <<","<<pUser->get_available_funds()<<","<< str_gamelist << std::endl;
		user_stream.close();
	}
	else
	{
		std::cout << "\nAn error has occurred when opening the file.\n";
	}
}

//单个插入
void DatabaseManager::store_game_data(const Game &rGame)
{
	std::ofstream game_stream;
	game_stream.open("gamelist.csv", std::ios::out | std::ios::app);
	if (!game_stream.fail())
	{
		game_stream << rGame.get_game_id() << "," << rGame.get_title() << "," <<rGame.get_game_Price()<<","<< rGame.get_game_desc() << std::endl;
		game_stream.close();
	}
	else
	{
		std::cout << "\nAn error has occurred when opening the file.\n";
	}	
}

void DatabaseManager::store_owned_games(UserBase*User)
{
	PlayerUser* pUser = static_cast<PlayerUser*>(User);

}

// 防止添加完成后没有正常退出造成的数据丢失
void DatabaseManager::update_games_data()
{
	//clean
	std::ofstream games_stream;
	games_stream.open("gamelist.csv", std::ios::out);
	games_stream.close();
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

void DatabaseManager::add_and_store_game(Game&rGame)
{
	add_game(rGame);
	store_game_data(rGame);
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

void DatabaseManager::add_game(Game&rGame)
{
	// Store the game indexed by game id.
	m_games.insert(std::make_pair(rGame.get_game_id(), rGame));
}

void DatabaseManager::remove_game(int id)
{
	m_games.erase(id);
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

void DatabaseManager::search_game(const int i, const std::string s, const double pri_upper, const double pri_lower )
{
	bool is1 = false;
	bool is2 = false;
	bool is3 = false;
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
		default:std::cout << "Parameter error\n"; break;
		}
	}
	if (!is1&&!is2&&!is3)
	{
		std::cout << "Search for games that don't meet the criteria\n\n";
	}
}