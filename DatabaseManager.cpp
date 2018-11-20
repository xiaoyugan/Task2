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
	//// add some games.
	//add_game(Game(4789, "Bounceback", "A platform puzzle game for PSP"));
	//add_game(Game(5246, "Piecefall", "A tetris like 3d puzzle game for PS4"));

	//从表中读取玩家数据
	std::ifstream iFile("userlist.csv",std::ios::in);
	if (!iFile.fail())
	{
		std::string type;
		std::string name;
		std::string password;
		std::string mail;
		while (std::getline(iFile, type, ',')&& std::getline(iFile, name, ',')&& std::getline(iFile, password, ',') && std::getline(iFile, mail, '\n'))
		{
			if (std::stoi(type) == 1)
			{
				add_user(new PlayerUser(name, password, mail));
			}
			else if (std::stoi(type)== 2)
			{
				add_user(new AdminUser(name, password, mail));
			}
		}
		iFile.close();
	}
	else
	{
		std::cout << "\nAn error has occurred when opening the file.\n";
	}
}

//存储玩家游玩的数据其他不用
void DatabaseManager::store_data()
{
	// You need a mechinsm for storing data here

	////store user data 把用户数据再存到表里面
	////定义文件输出流 
	//std::ofstream oFile;
	////打开要输出的文件 会清空原来的数据重新写入
	//oFile.open("userlist.txt", std::ios::out /*| std::ios::app*/);
	//if (!oFile.fail())
	//{
	//	for (auto it : m_users)
	//	{
	//		auto *p = it.second;
	//		oFile << static_cast<std::underlying_type<UserTypeId>::type>(p->get_user_type()) << "," << p->get_username() << "," << p->get_password() << "," << p->get_email() << std::endl;
	//	}
	//	oFile.close();
	//}
	//else
	//{
	//	std::cout << "\nAn error has occurred when opening the file.\n";
	//}
	
}

//添加一个就更新一个，实时把用户数据加入到表单，防止程序没有正常结束而使数据丢失
void DatabaseManager::store_user_data(UserBase* pUser)
{
	//store user data 把用户数据再存到表里面
	//定义文件输出流 
	std::ofstream oFile;
	//打开要输出的文件
	oFile.open("userlist.csv", std::ios::out | std::ios::app);
	if (!oFile.fail())
	{
		oFile << static_cast<std::underlying_type<UserTypeId>::type>(pUser->get_user_type()) << "," << pUser->get_username() << "," << pUser->get_password() << "," << pUser->get_email() << std::endl;
		oFile.close();
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
	add_and_store_user(pUser);
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

void DatabaseManager::add_game(Game& rGame)
{
	// Store the game indexed by game id.
	m_games.insert(std::make_pair(rGame.get_game_id(), rGame));
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
