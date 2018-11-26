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

	// Real-time store data(ios::app).used by add_and_store_XXX function.
	void store_adminuser_data(UserBase*pUser);
	void store_playeruser_data(UserBase*pUser);
	void store_gamestudio_data(UserBase*pUser);
	void store_guest_data(GuestUser*gUser);
	void store_game_data(const Game&rGame);

	// Pass the newly modified data into the table.Prevent when the program from ending abnormally,the data loss.
	void update_player_data();
	void update_games_data();

	// Adds a user to the m_users.
	void add_user(UserBase* pUser);
	// Adds a guest to the m_gUsers.
	void add_guest(GuestUser*gUser);
	// Adds a game to the m_games.
	void add_game(Game&rGame);

	// Add one to update one, add user data to the form in real time. Prevent when the program from ending abnormally,the data loss.
	// Add and store data.
	void add_and_store_adminuser(UserBase* pUser);
	void add_and_store_playeruser(UserBase*pUser);
	void add_and_store_gamestudio(UserBase*pUser);
	void add_and_stroe_guest(GuestUser*gUser);
	void add_and_store_game(Game&rGame);

	// Finds a user by username, return nullptr if the user is not found.
	UserBase* find_user(const UserBase::Username& username);
	// Finds a guest by email, returns nullptr if the guest is not found.
	GuestUser*find_guest(const std::string&mail);
	// Finds a game by id, returns nullptr if the game is not found.
	Game* find_game(const Game::GameId gameid);

	// iterating users using visitor pattern.
	template<typename Visitor> void visit_users(Visitor& func)
	{
		for (auto it : m_users) { func(*it.second); }
	}
	// iterating games using visitor pattern.
	template<typename Visitor> void visit_games(Visitor& func)
	{
		for (auto it : m_games) { func(it.second); }
	}

	// store player`s purchase history(player, games and data of purchase) to purchaseHistory.csv
	void store_purchase_history(PlayerUser*pUser, Game*rGame);
	// display purchase history
	void check_purchase_history();

	// store player`s activities(the date and time as well as the length of time each player spends in each game) to activityInfo.csv
	void store_player_activityInfo(UserBase*pUser, Game*rGame, double&time, std::string&start_time);
	//if i=1,print the player activity info,if i=2,print the most populargame.
	void check_player_activityInfo(const int i);

	// GameStudio or Player gift a game to a friend,and if(Giver is player){the game will be transferred from one bag to the other}else if(Giver is GameStudio){no transferred}
	void give_away(UserBase*pUser, std::string name, int id);
	// play game and timing.store_player_activityInfo.
	void play_game(UserBase*pUser, int id);
	void remove_game(int id);
	// i=1,search by title,i=2,search by description,i=3,search by price,i=4,search by age restriction.
	void search_game(const int i,const std::string s=" ", const double pri_upper = 0, const double pri_lower = 0,const int age=0);
	double average_price();

	// get current time and return a string
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

