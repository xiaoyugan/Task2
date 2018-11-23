//C++ Boot Camp - Task 2 - 2018-19 
//Name: Maodan Luo
//Student number: 27042120
#pragma once

#include <string>

class Game
{
public:
	using GameId = int;

	Game(const GameId& gameId, const std::string& title, const std::string& desc)
		: m_gameId(gameId)
		, m_title(title)
		, m_description(desc)
	{}

	virtual ~Game() {}

	const Game::GameId get_game_id() const { return m_gameId; }

	const std::string get_title() const { return m_title; }

	const std::string get_game_desc() const { return m_description; }

	void set_title(const std::string& val) { m_title = val; }

	void set_description(const std::string&val) { m_description = val; }

private:
	const GameId m_gameId;	// Numerical identifier for a game  (unique key).
	std::string m_title; // The game title
	std::string m_description; // Some information about the game.
};



