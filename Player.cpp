
#include "Player.h"

#include <string>
#include <iostream>
using namespace std;

//player implementation 
Player::Player(string name)//player constructor 
:m_name(name) {}
string Player::name() const {	//return name function 
	return m_name ;
}
bool Player::isInteractive() const {	//function showing if player is human 
	return false;
}
Player:: ~Player() {}	//virtual destructor 


//bad player implementation 
BadPlayer::BadPlayer(string name)
	:Player(name)
{}
bool BadPlayer::isInteractive() const {//return false 
	return false;
}
int BadPlayer::chooseMove(const Board& b, Side s)const {
	for (int i = 1; i <= b.holes(); i++) {	//loop from left to right 
		if (b.beans(s, i) != 0)		//check if hole is empty 
			return i;		//if not select this hole for a move 
	}
	return -1;		//if all holes are empty return -1
}
BadPlayer::~BadPlayer() {}

//human player implementation 
HumanPlayer::HumanPlayer(string name)
	:Player(name)
{}
bool HumanPlayer::isInteractive() const {
	return true;
}
int HumanPlayer::chooseMove(const Board& b, Side s)const {

	if (b.beansInPlay(s) == 0)	//if there are  no more active beans on players side return -1 
		return -1;
	int move;
	bool val = false;	//check if input is a valid move 
	while (val == false) {
		cout << "Enter a move" << endl;	//prompt player for move 
		cin >> move;	//accept input 
		if (move > 0 && move <= b.holes() && b.beans(s, move) != 0) {//check if input is a hole number and if hole is not empty 
			val = true;
		}
	}

	return move;
}
HumanPlayer::~HumanPlayer() {}


//smart player implementation 
SmartPlayer::SmartPlayer(string name)	
	:Player(name), maxDepth(6)
{}
bool SmartPlayer::isInteractive() const {
	return false;
}
int SmartPlayer::chooseMove(const Board& b, Side s) const {
	int besth;
	int val;
	int depth = 0;	//set initial depth as 0
	chooseM(s, b, besth, val, depth);		//call the game tree recursive function
	
	return besth;

}
void SmartPlayer::chooseM(Side s, const Board& b, int& bestHole, int& value, const int& depth)const {
	if (s == SOUTH) {
		if (b.beansInPlay(s) == 0) {	//if there are no more beans at play for this side return -1 
			bestHole = -1;
			value = Eval(b, SOUTH, NORTH, depth);
			return;
		}
		if (depth >= maxDepth) {	//check if maximum depth has bean reached 
			bestHole = -1;
			value = Eval(b, SOUTH, NORTH, depth);	//check the value of this leaf using heusterics 

			return;		//stop the return to a previous node of the tree
		}
		for (int i = 1; i <= b.holes(); i++) {	//check all the holes of a this side of the board 
			if (b.beans(SOUTH, i) != 0) {		//if hole has beans 
				int l;
				Side v;
				Board* a = new Board(b);	//create new test board 
				a->sow(SOUTH, i, v, l);		//make the move for this hole 

				int val;
				int i2 = i;
				chooseM(NORTH, *a, i2, val, depth + 1);	//using this copy board recusivly play a move for the opposite side and increase the depth 
				delete a;		//delete the test copy board 
				if (val >= value) {	//check if the current value is of this leaf is higher than that of the current maximum leaf 
					bestHole = i;	//if yes set the move to this node of the game tree 
					value = val;		//set the new best value to this leafs value 
				}

			}
		}

		return;
	}
	//do the same for the north side 
	if (s == NORTH) {
		if (b.beansInPlay(s) == 0) {	
			bestHole = -1;
			value = Eval(b, NORTH, SOUTH, depth);
			return;
		}
		if (depth >= maxDepth) {
			bestHole = -1;
			value = Eval(b, NORTH, SOUTH, depth);

			return;
		}
		for (int i = 1; i <= b.holes(); i++) {
			if (b.beans(NORTH, i) != 0) {
				int l;
				Side v;
				Board* a = new Board(b);
				a->sow(NORTH, i, v, l);

				int val;
				int i2 = i;
				chooseM(SOUTH, *a, i2, val, depth + 1);
				delete a;
				if (val > value) {
					bestHole = i;
					value = val;
				}
			}
		}
		return;
	}
}


int SmartPlayer::Eval(const Board& b, Side s, Side opp, const int& depth)const {
	int score = 0;
	if (b.beansInPlay(opp) == 0 || b.beansInPlay(s) == 0) {	//check if this leaf means the game will end 
		if (b.beans(s, 0) < b.beans(opp, 0))	//check is this side ends the game with less beans in the pot than the opponent 
			score = -9999;	//if yes return that this is a loosing game move 
		else
			score = 9999;		//if not return this is a winning game move 
	}
	else {	//if game does not end
		score += 30 * (b.beansInPlay(opp) - b.beansInPlay(s)) / b.totalBeans();	//check ratio of beans this side has vs opponent side compared to total beans 
		//the less active beans the better 
		score += 60 * (b.beans(s, 0) - b.beans(opp, 0)) / b.totalBeans();	//check ration of beans in pot vs opponents pot beans to the total number of beans 
		//the more beans in pot the better 
		int count1 = 0;	//check for number of empty holes for this side 
		int count2 = 0;	//check number of empty holes for opponent side 
		for (int i = 0; i <= b.holes(); i++) {
			if (b.beans(s, i) == 0)
				count1++;
			if (b.beans(opp, i) == 0)
				count2++;
		}
		
		score += 10 * ((count1 - count2) / b.holes());	//compare ration of number of empty holes to total holes 
		//the more empty holes the better 
		for (int i = 1; i <= b.holes(); i++) {	//go through the holes and check if the number of beans is becomes large 
			if (b.beans(s, i) >6) {
				score -= 5; //if yes reduce score as it leaves player open to captures 
			}
		}
	}
	if (depth % 2 == 1)	//if depth is odd number it means the better the score the worst for root side 
		score = -1 * score;		//thus give it bad sign
	return score;
}
SmartPlayer::~SmartPlayer() {}

