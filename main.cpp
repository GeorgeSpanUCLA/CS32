
#include "Game.h"
#include "Player.h"
#include "Board.h"
#include "Side.h"
#include <iostream>
#include <cassert>
using namespace std;
#if defined(_MSC_VER)  &&  !defined(_DEBUG)
#include <iostream>
#include <windows.h>
#include <conio.h>

struct KeepWindowOpenUntilDismissed
{
	~KeepWindowOpenUntilDismissed()
	{
		DWORD pids[1];
		if (GetConsoleProcessList(pids, 1) == 1)
		{
			std::cout << "Press any key to close this window . . . ";
			_getch();
		}
	}
} keepWindowOpenUntilDismissed;
#endif
void doPlayerTests()
{
	HumanPlayer hp("Marge");//test human player constructor 
	assert(hp.name() == "Marge" && hp.isInteractive());	//test human interactive function 
	BadPlayer bp("Homer");	//test bad player constructor 
	assert(bp.name() == "Homer" && !bp.isInteractive());	//test badplayer interactive  function
	SmartPlayer sp("Lisa");	//test smart player constructor 
	assert(sp.name() == "Lisa" && !sp.isInteractive());	//test smartplayer interactive function
	Board b(3, 2);
	b.setBeans(SOUTH, 2, 0);
	cout << "=========" << endl;
	int n = hp.chooseMove(b, SOUTH);	//test human player choose move function 
	cout << "=========" << endl;
	assert(n == 1 || n == 3);
	n = bp.chooseMove(b, SOUTH);	//test bad player choose move function 
	assert(n == 1 || n == 3);
	n = sp.chooseMove(b, SOUTH);	//test smart player choose move function 
	assert(n == 1 || n == 3);
}



void doBoardTests()
{
	Board b(3, 2);	//create simple board 
	assert(b.holes() == 3 && b.totalBeans() == 12 &&
		b.beans(SOUTH, POT) == 0 && b.beansInPlay(SOUTH) == 6);	//test if board was created with correct number of holes, beans in holes, beans in pots
	//test set beans and move to pot functions with valid inputs 
	b.setBeans(SOUTH, 1, 1);
	b.moveToPot(SOUTH, 2, SOUTH);
	assert(b.totalBeans() == 11 && b.beans(SOUTH, 1) == 1 &&
		b.beans(SOUTH, 2) == 0 && b.beans(SOUTH, POT) == 2 &&
		b.beansInPlay(SOUTH) == 3);
	Side es;
	int eh;
	//test sow function when move is possible 
	b.sow(SOUTH, 3, es, eh);
	assert(es == NORTH && eh == 3 && b.beans(SOUTH, 3) == 0 &&
		b.beans(NORTH, 3) == 3 && b.beans(SOUTH, POT) == 3 &&
		b.beansInPlay(SOUTH) == 1 && b.beansInPlay(NORTH) == 7);
	//test constructor for invalid input 
	Board a(-1, -1);	
	assert(a.holes() == 1 && a.totalBeans() == 0 &&
		a.beans(SOUTH, POT) == 0 && a.beansInPlay(SOUTH) ==0 );
	//test copy constructor 
	Board c(a);
	assert(c.holes() == 1 && c.totalBeans() == 0 &&
		c.beans(SOUTH, POT) == 0 && c.beansInPlay(SOUTH) == 0);
	//test for invalid sow hole 
	assert(a.sow(SOUTH, 3, es, eh) == false);
	//test for empty sow hole 
	assert(a.sow(SOUTH, 1, es, eh) == false);
	//test for sow attempt at pot 
	assert(a.sow(SOUTH, 0, es, eh) == false);


}


void doGameTests()
{
	
	BadPlayer bp1("Bart");
	BadPlayer bp2("Homer");
	Board b(3, 0);
	//test game setBeans functions 
	b.setBeans(SOUTH, 1, 2);
	b.setBeans(NORTH, 2, 1);
	b.setBeans(NORTH, 3, 2);
	//test game constructor 
	Game g(b, &bp1, &bp2);
	bool over;
	bool hasWinner;
	Side winner;
	//    Homer
	//   0  1  2
	// 0         0
	//   2  0  0
	//    Bart
	//test status function when game not over 
	g.status(over, hasWinner, winner);
	assert(!over && g.beans(NORTH, POT) == 0 && g.beans(SOUTH, POT) == 0 &&
		g.beans(NORTH, 1) == 0 && g.beans(NORTH, 2) == 1 && g.beans(NORTH, 3) == 2 &&
		g.beans(SOUTH, 1) == 2 && g.beans(SOUTH, 2) == 0 && g.beans(SOUTH, 3) == 0);
	
	//test move and status function when move possible and game not over for south side 
	g.move(SOUTH);
	//   0  1  0
	// 0         3
	//   0  1  0
	g.status(over, hasWinner, winner);
	assert(!over && g.beans(NORTH, POT) == 0 && g.beans(SOUTH, POT) == 3 &&
		g.beans(NORTH, 1) == 0 && g.beans(NORTH, 2) == 1 && g.beans(NORTH, 3) == 0 &&
		g.beans(SOUTH, 1) == 0 && g.beans(SOUTH, 2) == 1 && g.beans(SOUTH, 3) == 0);
	//test move and status function when move possible and game not over for north side 
	g.move(NORTH);
	//   1  0  0
	// 0         3
	//   0  1  0
	g.status(over, hasWinner, winner);
	assert(!over && g.beans(NORTH, POT) == 0 && g.beans(SOUTH, POT) == 3 &&
		g.beans(NORTH, 1) == 1 && g.beans(NORTH, 2) == 0 && g.beans(NORTH, 3) == 0 &&
		g.beans(SOUTH, 1) == 0 && g.beans(SOUTH, 2) == 1 && g.beans(SOUTH, 3) == 0);

	g.move(SOUTH);
	//   1  0  0
	// 0         3
	//   0  0  1
	g.status(over, hasWinner, winner);
	assert(!over && g.beans(NORTH, POT) == 0 && g.beans(SOUTH, POT) == 3 &&
		g.beans(NORTH, 1) == 1 && g.beans(NORTH, 2) == 0 && g.beans(NORTH, 3) == 0 &&
		g.beans(SOUTH, 1) == 0 && g.beans(SOUTH, 2) == 0 && g.beans(SOUTH, 3) == 1);
	//test when game is over with south side winner 
	g.move(NORTH);
	//   0  0  0
	// 1         4
	//   0  0  0
	g.status(over, hasWinner, winner);
	assert(over && g.beans(NORTH, POT) == 1 && g.beans(SOUTH, POT) == 4 &&
		g.beans(NORTH, 1) == 0 && g.beans(NORTH, 2) == 0 && g.beans(NORTH, 3) == 0 &&
		g.beans(SOUTH, 1) == 0 && g.beans(SOUTH, 2) == 0 && g.beans(SOUTH, 3) == 0);
	assert(hasWinner && winner == SOUTH);
}




int main()
{
	SmartPlayer bp1("Start");
	HumanPlayer bp2("Me");
	Board c(6, 4);
	Game g(c, &bp1, &bp2);
	g.play();
	Board b(6, 4);
	b.setBeans(NORTH, 0, 7);
	b.setBeans(NORTH, 1, 2);
	b.setBeans(NORTH, 2, 1);
	b.setBeans(NORTH, 3, 0);
	b.setBeans(NORTH, 4, 1);
	b.setBeans(NORTH, 5, 1);
	b.setBeans(NORTH, 6, 1);
	b.setBeans(SOUTH, 0, 16);
	b.setBeans(SOUTH, 1, 10);
	b.setBeans(SOUTH, 2, 7);
	b.setBeans(SOUTH, 3, 0);
	b.setBeans(SOUTH, 4, 1);
	b.setBeans(SOUTH, 5, 1);
	b.setBeans(SOUTH, 6, 0);
	SmartPlayer bp4("Start");
	HumanPlayer bp3("Me");
	Game f(b,& bp4, & bp3);
	f.play();

		

}






