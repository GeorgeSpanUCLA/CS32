
#include "Game.h"
#include "Board.h"
#include"Player.h"
#include <vector>
#include <iostream>
using namespace std;
//game contructor 
Game::Game(const Board& b, Player* south, Player* north)
:m_b(b),So(south),No(north)
{}
void Game :: display() const {	//display function 
	cout << No->name() << endl;
	for (int i = 1; i <= m_b.holes(); i++) {
		cout << "  " << m_b.beans(NORTH, i) ;
	}
	cout << endl;
	cout << m_b.beans(NORTH,0) << "    --- "<<m_b.beans(SOUTH,0)<<endl;
	for (int i = 1; i <= m_b.holes(); i++) {
		cout << "  "<< m_b.beans(SOUTH, i);
	}
	cout << endl;
	cout << So->name() << endl;
}
void Game::status(bool& over, bool& hasWinner, Side& winner) const {
	//check if either side has no active beans
	if (m_b.beansInPlay(SOUTH) != 0 && m_b.beansInPlay(NORTH) != 0) {
		over = false;	// if neither side no beans game is not over 
		hasWinner = false;
		return;
	}
	//if one of the sides is empty 
	if (m_b.beans(SOUTH, 0) == m_b.beans(NORTH, 0)) {	//check if both side have same number of pot beans meaning tie 
		over = true;
		hasWinner = false;	//no winner since its a tie 
	}
	//if not a tie check who has more pot beans and set them as the winning side 
	else if (m_b.beans(SOUTH, 0) > m_b.beans(NORTH, 0)) {	
		hasWinner = true;
		over = true;
		winner = SOUTH;
	}
	else {
		hasWinner = true;
		winner = NORTH;
		over = true;
	}
}

bool Game::move(Side s) {//move function 
	
	if (s == NORTH) {
		int move = No->chooseMove(m_b, s);		//call for the players move function 
		bool rep;
		bool val = judge(move, s, rep,SOUTH);	//judge that players move 
		if (!val)	//if no move is possible return false 
			return false;
		while (val == true && rep == true) {	// if move is possible check if player can make another move through rep variable
			display();	//display board 
			move = No->chooseMove(m_b, s);	//call for player to make another move 
			val = judge(move, s, rep, SOUTH);	// judge if move is valid and if another move should be made by the same player 
			if (!val)	//check if a move has been made 
				return false;
			
		}
		
		return true; //return true is move is made 

	}
	//do same for south side 
	if (s == SOUTH) {
		int move = So->chooseMove(m_b, s);
		bool rep;
		bool val = judge(move, s, rep, NORTH);
		if (!val)
			return false;
		while (val == true && rep == true) {
			display();
			move = So->chooseMove(m_b, s);
			
			val = judge(move, s, rep, NORTH);

		}
		if (!val)
			return false;
		return true;
	}

	return true;
}
//play function
void Game:: play() {
	bool turnSouth = true;	//start with south side making first move 
		bool over;	//checks if game is over 
		bool w;	//checks if there is a winner or game is a tie 
		Side wi;	//winning side 
		int count = 0;	//number of turns counter 
		do {	//loop with game is not over 
			display();	//display board 
			if (turnSouth) {	//check if its south players turn 
				move(SOUTH);	//is true call on south player to make a move 
			}
			else
				move(NORTH);	// if its norths turn call on him to make a move 
			if (So->isInteractive() == false && No->isInteractive() == false && count%5==0) {
				//if neither player is human make sure game does not instantly finish 
				cout << "PRESS A KEY and ENTER TO CONTINUE" << endl;
				char a;
				cin >> a;
			}
			if (turnSouth)	//if south side just played make turnSouth false 
				turnSouth = false;
			else	//if north side just played make turnSouth true 
				turnSouth = true;
			count++;	//increase number of moves 
			status(over, w, wi);	//check if game is over 
			cout << endl << endl << endl;
		} while (over == false);	//end of game loop if game is over 
		//check for game winner 
		if (w && wi == SOUTH)
			cout << "The winner is " << So->name() << endl;
		else if (w && wi == NORTH)
			cout << "The winner is " << No->name() << endl;
		else
			cout << "TIE";
		return;
}
int Game::beans(Side s, int hole) const {	//return number of beans in hole 
	if (hole<0 || hole>m_b.holes()) {	//check if hole is valid 
		return -1;
	}
	return m_b.beans(s, hole);
}


//judge move private function 
bool Game::judge(int move, Side s, bool& repeat, Side opponent) {
	repeat = false;	//set the intial repetition variable to false 
	int lasthole;
	Side a;
	if (move == -1) {	// if no move can be made move all of a the opponents beans to their pot 
		for (int i = 1; i <= m_b.holes(); i++) {
			m_b.moveToPot(opponent, i, opponent);
		}
		return false;	//end function 
	}
	m_b.sow(s, move, a, lasthole);	//if move possible excecute it 
	if (lasthole == 0) {	//if last hole is pot 
		repeat = true;	//set repeat to true 
		return true;
	}
	//if last hole was on own players side and was previously empty and opponents opposide hole is not empty 
	else if (a == s && m_b.beans(s, lasthole) == 1 && m_b.beans(opponent, lasthole) != 0) {
		m_b.moveToPot(opponent, lasthole, s);	//empty opponents hole and put beans in players pot 
		m_b.moveToPot(s, lasthole, s);	//put last bean playced in own hole into pot 
		return true;	//end turn with no repeat 
	}
	//if non of the above is applicable end turn 
	return true;
}
