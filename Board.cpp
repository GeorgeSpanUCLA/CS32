#include "Board.h"
#include<vector>
#include <iostream>
using namespace std;
//Board constructor 
Board::Board(int nHoles, int nInitialBeansPerHole)
:m_holes(nHoles), m_BeansInPots(nInitialBeansPerHole)
{
	if (m_holes <= 0) //check if holes are more than 0
		m_holes = 1;
	if (m_BeansInPots < 0)	//check if holes are not negative 
		m_BeansInPots = 0;
	holeN.push_back(POT);		//set value for pot of north player in the first postion of vector 
	holeS.push_back(POT);		//set value for pot of north player in the first postion of vector 
	for (int i = 0; i < m_holes; i++) {	//push holes with beans on the north and south vectors 
		holeN.push_back(m_BeansInPots);
		holeS.push_back(m_BeansInPots);
	}
	
	m_Beans_In_TotalN = m_holes * m_BeansInPots;	//set total beans at play for north 
	m_Beans_In_TotalS = m_holes * m_BeansInPots;	//set total beans at play for south 
	beans_InPotN = POT;	//set beans in north pot variable
	beans_InPotS = POT;	//set beans in south pot variable 

}

Board::Board(const Board& b)	//board copy constructor
:m_holes(b.m_holes), m_BeansInPots(b.m_BeansInPots) {	//set hole and beans values 
	holeN.push_back(b.beans_InPotN);		//set pot values 
	holeS.push_back(b.beans_InPotS);
	for (int i = 0; i <m_holes; i++) {
		holeN.push_back(b.holeN[i+1]);		//set new bean vectors 
		holeS.push_back(b.holeS[i+1]);
	}

	m_Beans_In_TotalN = b.m_Beans_In_TotalN;		//set beans at play values 
	m_Beans_In_TotalS = b.m_Beans_In_TotalS;
	beans_InPotN = b.beans_InPotN;
	beans_InPotS = b.beans_InPotS;
}

int Board::holes() const {		//returns number of holes on each side 
	return m_holes;
}

int Board::beans(Side s, int hole) const {		//returns beans in a specific hole 
	if (s == SOUTH)
		return holeS[hole];
	
	
	return holeN[hole];
}


int Board::beansInPlay(Side s) const {		//returns total beans in play for each side 
	if (s == SOUTH)
		return m_Beans_In_TotalS;
	return m_Beans_In_TotalN;
}

int  Board::totalBeans() const {		//returns total beans in both sides including pots 
return  m_Beans_In_TotalS + m_Beans_In_TotalN + beans_InPotN +beans_InPotS;

}
bool Board::sow(Side s, int hole, Side& endSide, int& endHole) {	//sows beans when a move is made 
	
	if (hole > m_holes || hole < 0 ||  hole == 0)	//check if its a valid hole and not a pot 
		return false;
	if (s == SOUTH && holeS[hole] == 0)	//check if hole has beans 
		return false;
	if (s == NORTH && holeN[hole] == 0)
		return false;
	if (s == SOUTH) {
		int BeansToSow = holeS[hole];	//put selected holes beans in a variable 
		holeS[hole] = 0;		//empty selected hole 
		int i = hole + 1;		//start for the next hole counterclockwise 
		while (BeansToSow > 0) {	//loop until there are no more beans to sow 
			for (; i <= m_holes && BeansToSow>0; i++) {	//iterate through holes putting one bean in each and reducing the beans to sow counter 
				holeS[i]++;
				BeansToSow--;
				endHole = i;	//set final hole 
				endSide = SOUTH;	//set final side
				
			}
			if (BeansToSow>0) {	//check if you can add a bean in the pot 
				holeS[0] ++;
				BeansToSow--;
				m_Beans_In_TotalS--;	//if yes reduce the number of beans at play 
				beans_InPotS++;		//increase the number of beans in the pot counter 
				endHole = 0;
			}
			i = m_holes;
			if (BeansToSow > 0) {		//move to the north side and continue sowing counterclockwise until no more beans are left but skip pot 
				for (; i > 0 && BeansToSow > 0; i--) {
					holeN[i]++;
					BeansToSow--;
					endHole = i;
					m_Beans_In_TotalN++;
					m_Beans_In_TotalS--;
				}
				endSide = NORTH;		//set final side to north
			}
			i = 1;
		}
	
		return true;

	}
	else {	//do same as before but start from north side 
		int BeansToSow = holeN[hole];
		holeN[hole] = 0;
		int i = hole - 1;
		while (BeansToSow > 0) {
			for (; i >= 0 && BeansToSow > 0; i--) {
				holeN[i]++;
				BeansToSow--;
				endHole = i;
				endSide = NORTH;
				if (i == 0) {
					beans_InPotN++;
					m_Beans_In_TotalN--;
					endHole = 0;
				}
			}
				if (BeansToSow > 0) {
					for (int i =1; i <=m_holes && BeansToSow > 0; i++) {
						holeS[i]++;
						BeansToSow--;
						endHole = i;
						m_Beans_In_TotalS++;
						m_Beans_In_TotalN--;
					}
					endSide = SOUTH;
				}
				i = m_holes;
		}
		
		return true;
	}
}
bool	Board::moveToPot(Side s, int hole, Side potOwner) {		//empty selected hole and move beans to selected pot 
	if (hole > m_holes || hole < 0 ||  hole == 0)	//check if hole is valid
		return false;
	//check different combinations of hole owners and bean recepients 
	if (s ==NORTH && potOwner == NORTH) {	
		holeN[0] += holeN[hole];
		m_Beans_In_TotalN -= holeN[hole];
		holeN[hole] = 0;
		beans_InPotN = holeN[0];
	}
	else if(s == NORTH && potOwner == SOUTH) {
		holeS[0] += holeN[hole];
		m_Beans_In_TotalN -= holeN[hole];
		holeN[hole] = 0;
		beans_InPotS = holeS[0];
	}
	else if (s == SOUTH && potOwner == SOUTH) {
		holeS[0] += holeS[hole];
		m_Beans_In_TotalS -= holeS[hole];
		holeS[hole] = 0;
		beans_InPotS = holeS[0];
	}
	else  {
		holeN[0] += holeS[hole];
		m_Beans_In_TotalS -= holeN[hole];
		holeS[hole] = 0;
		beans_InPotN = holeN[0];
	}
	
	
	return true;	//return true if task is successful
}

bool  Board::setBeans(Side s, int hole, int beans) {	//change the number of beans in indicated hole 
	if (hole<0 || hole>m_holes || beans < 0)		//check if hole if valid
		return false;
	if (s == SOUTH) {
		if (hole == 0) {
			beans_InPotS = beans;	//if hole is a pot update the pot number 
		}
		else {
			m_Beans_In_TotalS = m_Beans_In_TotalS - holeS[hole] + beans;	//update the beans at play number 
		}
		holeS[hole] = beans;
}
	else {
		//do same for north side 
		if (hole == 0) {
			beans_InPotN = beans;
		}
		else {
			m_Beans_In_TotalN = m_Beans_In_TotalN - holeN[hole] + beans;
		}
		holeN[hole] = beans;
	}
	return true; //return true if action done successfully 
}


