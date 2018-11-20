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

	//Real-time storage of users
	void store_user_data(UserBase* pUser);

	// Adds a user to the db.
	void add_user(UserBase* pUser);

	// Add and store users
	void add_and_store_user(UserBase* pUser);

	// Finds a user by username, return nullptr if the user is not found.
	UserBase* find_user(const UserBase::Username& username);

	// iterating users using visitor pattern
	template<typename Visitor> void visit_users(Visitor& func)
	{
		for (auto it : m_users) { func(*it.second); }
	}

	// Adds a game to the db.
	void add_game(Game& rGame);

	// Finds a game by id, returns nullptr if the game is not found.
	Game* find_game(const Game::GameId gameid);

	// iterating games using visitor pattern
	template<typename Visitor> void visit_games(Visitor& func)
	{
		for (auto it : m_games) { func(it.second); }
	}

private:
	// Constructors are private for singleton pattern.
	DatabaseManager();
	~DatabaseManager();


private:
	// Types
	using UserContainer = std::map<UserBase::Username, UserBase*>;
	using GameContainer = std::map<Game::GameId, Game>;

	UserContainer m_users;
	GameContainer m_games;

};

