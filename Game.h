//C++ Boot Camp - Task 2 - 2018-19 
//Name: Maodan Luo
//Student number: 27042120
#pragma once

#include <string>

class Game
{
public:
	using GameId = int;
	using GamePrice = double;

	Game(const GameId& gameId, const std::string& title, const GamePrice&price, const std::string& desc)
		: m_gameId(gameId)
		, m_title(title)
		,m_gameprice(price)
		, m_description(desc)
	{}

	virtual ~Game() {}

	const Game::GameId get_game_id() const { return m_gameId; }

	const std::string get_title() const { return m_title; }

	const std::string get_game_desc() const { return m_description; }

	const Game::GamePrice get_game_Price() const { return m_gameprice; }

	void set_title(const std::string& val) { m_title = val; }

	void set_description(const std::string&val) { m_description = val; }

	void set_price(const Game::GamePrice&val) { m_gameprice = val; }

private:
	const GameId m_gameId;	// Numerical identifier for a game  (unique key).
	GamePrice m_gameprice; 
	std::string m_title; // The game title
	std::string m_description; // Some information about the game.
};



