//C++ Boot Camp - Task 2 - 2018-19 
//Name: Maodan Luo
//Student number: 27042120
#pragma once

#include "Game.h"
#include "Users.h"

#include <map>

//--
// DatabaseManager is the one central database for the whole system
// In this example it has been designed using the singleton pattern.
//--
class DatabaseManager
{
public:
	// Singleton instance definition.
	static DatabaseManager& instance();

	// Initialize the database from storage.
	void load_data();

	// Write all the data to storage.
	void store_data();

	// Real-time store users (app)
	void store_adminuser_data(UserBase* pUser);

	void store_playeruser_data(UserBase*pUser);

	void store_guest_data(GuestUser*gUser);

	void store_owned_games(UserBase*pUser);

	void update_player_data();

	// Real-time store games (app)
	void store_game_data(const Game&rGame);

	// Adds a user to the db.
	void add_user(UserBase* pUser);

	void add_guest(GuestUser*gUser);

	// Add and store users
	void add_and_store_adminuser(UserBase* pUser);

	void add_and_store_playeruser(UserBase*pUser);

	void add_and_stroe_guest(GuestUser*gUser);

	// Add and store games
	void add_and_store_game(Game&rGame);

	// Finds a user by username, return nullptr if the user is not found.
	UserBase* find_user(const UserBase::Username& username);

	GuestUser*find_guest(const std::string&mail);

	// iterating users using visitor pattern
	template<typename Visitor> void visit_users(Visitor& func)
	{
		for (auto it : m_users) { func(*it.second); }
	}

	// Adds a game to the db.
	void add_game(Game&rGame);

	// Remove games
	void remove_game(int id);

	// Update games data把刚修改过的数据传进表中
	void update_games_data();

	// Finds a game by id, returns nullptr if the game is not found.
	Game* find_game(const Game::GameId gameid);

	// iterating games using visitor pattern
	template<typename Visitor> void visit_games(Visitor& func)
	{
		for (auto it : m_games) { func(it.second); }
	}

	void search_game(const int i,const std::string s=" ", const double pri_upper = 0, const double pri_lower = 0);

	void store_purchase_history(PlayerUser*pUser,Game*rGame);

	void check_purchase_history();

	void store_player_activityInfo(PlayerUser*pUser, Game*rGame,double&time,std::string&start_time);

	void check_player_activityInfo();

	std::string get_time();

private:
	// Constructors are private for singleton pattern.
	DatabaseManager();
	~DatabaseManager();


private:
	// Types
	using UserContainer = std::map<UserBase::Username, UserBase*>;
	using GameContainer = std::map<Game::GameId, Game>;
	using GuestUserContainer = std::map<std::string, GuestUser*>;

	UserContainer m_users;
	GameContainer m_games;
	GuestUserContainer m_gUsers;
};

