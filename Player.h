#include "Board.h"
#include <string>
#ifndef P
#define P
//player class definition 
class Player {
public:
	Player(std::string name);
	std::string name() const;
	virtual bool isInteractive() const;
	virtual int chooseMove(const Board& b, Side s) const = 0;
	virtual ~Player();
private:
	std::string m_name;

};
//bad player class definition 
class BadPlayer : public Player
{
public:
	BadPlayer(std::string name);
	virtual bool isInteractive() const;
	int chooseMove(const Board& b, Side s)const;
	~BadPlayer();
private:
};
//human player class definition 
class HumanPlayer : public Player {
public:
	HumanPlayer(std::string name);
	virtual bool isInteractive() const;
	int chooseMove(const Board& b, Side s)const;
	~HumanPlayer();
private:
};
//smart player class definition 
class SmartPlayer : public Player {
public:
	SmartPlayer(std::string name);
	virtual bool isInteractive() const;
	int chooseMove(const Board& b, Side s)const;
	~SmartPlayer();

private:
	void chooseM(Side s, const Board& b, int& bestHole, int& value, const int& depth)const; //function generating game tree
	int Eval(const Board& b, Side s, Side opp, const int& depth)const;		//heusterics evaluation function 
	int maxDepth;		//max depth for game tree 
};
#endif // !P
