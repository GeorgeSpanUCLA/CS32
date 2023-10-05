#include "Board.h"
#include "Player.h"
#include "Side.h"
#include <vector>
#ifndef G
#define G
//game definition 
class Game {
public:
	Game(const Board& b, Player* south, Player* north);
	void display() const;
	void status(bool& over, bool& hasWinner, Side& winner) const;
	bool move(Side s);
	void play();
	int beans(Side s, int hole) const;

private:
	bool judge(int i, Side s, bool& repeat, Side opponent);	//function makes move and dodges if another move can be made by player 
	Board m_b;		//game board 
	 Player* So;	//South player 
	Player* No;		//norht player 
	
};


#endif // !G
