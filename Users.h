//C++ Boot Camp - Task 2 - 2018-19 
//Name: Maodan Luo
//Student number: 27042120
#pragma once

#include <string>
#include <list>

//--
// UserTypeId represents an identifier for the specific user type.
//--
enum class UserTypeId
{
	kInvalid = 0
	, kPlayerUser
	, kAdminUser
	, kGuestUser
	, kGameStudio
};

//--
// UserBase represents a user base class for the system.
//--
class UserBase
{
public:
	using Username = std::string;

	UserBase(const Username& username, const std::string& password, const std::string& email)
		: m_username(username)
		, m_password(password)
		, m_email(email)
	{}

	virtual ~UserBase() {}

	// mechanism for identifying the user type at runtime.
	virtual const UserTypeId get_user_type() const = 0;
	//virtual const  double get_available_funds() const { return 0.0; }
	const std::string get_username() const { return m_username; }

	const std::string get_password() const { return m_password; }
	void set_password(const std::string& val) { m_password = val; }

	const std::string get_email() const { return m_email; }
	void set_email(const std::string& val) { m_email = val; }

private:
	const Username m_username; // The users username (unique key)
	std::string m_password; // Users password.
	std::string m_email; // Users email address.
};

//--
// PlayerUser represents a system user who owns games
//--
class PlayerUser : public UserBase
{
public:
	using GameList = std::list<Game::GameId>;

	PlayerUser(const Username& username, const std::string& password, const std::string& email, const double&accountFunds, const GameList& mygamelist, const int&age)
		:UserBase(username, password, email)
		, m_accountFunds(accountFunds)
		, m_ownedGames(mygamelist)
		, m_age(age)
	{}

	// define the specific user type.
	virtual const UserTypeId  get_user_type() const override { return UserTypeId::kPlayerUser; }

	const PlayerUser::GameList& get_game_list() const { return m_ownedGames; }

	const double get_available_funds() const  { return m_accountFunds; }

	const int get_myage()const { return m_age; }

	void set_accountFunds(const double& val) { m_accountFunds = val; }

	void add_ownedGame(const Game::GameId& val) 
	{
		if (m_ownedGames.front() == 0)
		{
			m_ownedGames.remove(0);
		}
		m_ownedGames.push_back(val);		
	}

	void pop_ownedGame(const Game::GameId& val) 
	{ 
		if (m_ownedGames.size() == 1)
		{
			m_ownedGames.push_back(0);
		}
		m_ownedGames.remove(val); 
	}

private:
	GameList m_ownedGames; // List of owned games.
	double m_accountFunds; // The players available funds.
	int m_age;// The player`s age.
};

//--
// AdminUser represents a system user who has privileges to modify the system.
//--
class AdminUser : public UserBase
{
public:
	// inherit the constructor.
	using UserBase::UserBase;

	// define the specific user type.
	virtual const UserTypeId get_user_type() const override { return UserTypeId::kAdminUser; }
};

//--
// GameStudio who can only access their own games and set a new version number to their games
//--
class GameStudio : public UserBase
{
public:
	using GameList = std::list<Game::GameId>;

	GameStudio(const Username& username, const std::string& password, const std::string& email, const GameList& mygamelist)
		:UserBase(username, password, email)
		, m_ownedGames(mygamelist)
	{}

	// define the specific user type.
	virtual const UserTypeId  get_user_type() const override { return UserTypeId::kGameStudio; }

	const GameStudio::GameList& accessible_gamelist() const { return m_ownedGames; }

	void add_ownedGame(const Game::GameId& val)
	{
		if (m_ownedGames.front() == 0)
		{
			m_ownedGames.remove(0);
		}
		m_ownedGames.push_back(val);
	}

private:
	GameList m_ownedGames; // List of owned games.
};

//--
// GusetUser who only have email data
//--
class GuestUser
{
public:
	GuestUser(std::string&mail):m_email(mail){}

	const std::string get_email() const { return m_email; }
	// define the specific user type.
	const UserTypeId get_user_type(){ return UserTypeId::kGuestUser; }

private:
	std::string m_email; // Users email address.
};
