#include"Side.h"
#include <vector>
#ifndef B
#define B

class Board {
public:
	Board(int nHoles, int nInitialBeansPerHole);	//constructor 
	Board(const Board& b);		//copy constructor 
	int holes() const;

	int beans(Side s, int hole) const;


	int beansInPlay(Side s) const;

	int totalBeans() const;
	bool sow(Side s, int hole, Side& endSide, int& endHole);
	bool moveToPot(Side s, int hole, Side potOwner);
	bool setBeans(Side s, int hole, int beans);
private:
	int m_holes;	//number of holes on each side 
	int m_BeansInPots;	//number of beans in each hole 
	int m_Beans_In_TotalN;	//beans at play for norht side
	int m_Beans_In_TotalS;	//beans at play for south side 
	int beans_InPotN;	//beans in north pot 
	int beans_InPotS;	//beans in south pot 
	std::vector<int>holeN;		//north holes bean vector 
	std::vector<int>holeS;		//south holes bean vector 

};
#endif // !B
