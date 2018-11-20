//C++ Boot Camp - Task 2 - 2018-19 
//Name: Maodan Luo
//Student number: 27042120

#include "DatabaseManager.h"
#include <fstream> 
#include <string>
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
	std::ifstream games_stream("gamelist.csv", std::ios::in);
	if (!games_stream.fail())
	{
		std::string g_id;
		std::string g_title;
		std::string g_desc;
		while (std::getline(games_stream, g_id, ',') && std::getline(games_stream, g_title, ',') && std::getline(games_stream, g_desc, '\n'))
		{
			add_game(new Game(std::stoi(g_id), g_title, g_desc));
		}
		games_stream.close();
	}
	else
	{
		std::cout << "\nAn error has occurred when opening the file.\n";
	}

	//从表中读取玩家数据
	std::ifstream users_stream("userlist.csv",std::ios::in);
	if (!users_stream.fail())
	{
		std::string u_type;
		std::string u_name;
		std::string u_password;
		std::string u_mail;
		while (std::getline(users_stream, u_type, ',')&& std::getline(users_stream, u_name, ',')&& std::getline(users_stream, u_password, ',') && std::getline(users_stream, u_mail, '\n'))
		{
			if (std::stoi(u_type) == 1)
			{
				add_user(new PlayerUser(u_name, u_password, u_mail));
			}
			else if (std::stoi(u_type)== 2)
			{
				add_user(new AdminUser(u_name, u_password, u_mail));
			}
		}
		users_stream.close();
	}
	else
	{
		std::cout << "\nAn error has occurred when opening the file.\n";
	}
}

//存储玩家游玩的数据这些
void DatabaseManager::store_data()
{
	
}

//添加一个就更新一个，实时把用户数据加入到表单，防止程序没有正常结束而使数据丢失
void DatabaseManager::store_user_data(UserBase* pUser)
{
	//store user data 把用户数据再存到表里面
	//定义文件输出流 
	std::ofstream user_stream;
	//打开要输出的文件
	user_stream.open("userlist.csv", std::ios::out | std::ios::app);
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

void DatabaseManager::Store_game_data(Game* rGame)
{
	std::ofstream game_stream;
	game_stream.open("gamelist.csv", std::ios::out | std::ios::app);
	if (!game_stream.fail())
	{
		game_stream << rGame->get_game_id() << "," << rGame->get_title() << "," << rGame->get_game_desc() << std::endl;
		game_stream.close();
	}
	else
	{
		std::cout << "\nAn error has occurred when opening the file.\n";
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

void DatabaseManager::add_and_store_user(UserBase* pUser)
{
	add_user(pUser);
	store_user_data(pUser);
}

void DatabaseManager::add_and_store_game(Game* rGame)
{
	add_game(rGame);
	Store_game_data(rGame);
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

void DatabaseManager::add_game(Game* rGame)
{
	// Store the game indexed by game id.
	m_games.insert(std::make_pair(rGame->get_game_id(), rGame));
}

Game* DatabaseManager::find_game(const Game::GameId gameid)
{
	auto it = m_games.find(gameid);
	if (it != m_games.end())
	{
		return it->second;
	}
	else
	{
		return nullptr;
	}
}
