#include <iostream>
#include <time.h>
#include "h/Segment.h"

using namespace std;

typedef unsigned short uint16_t;
typedef unsigned char uint8_t;

int main()
{
	time_t startI, endI;
	time_t startR, endR;

	startI = time(NULL);
	//Trie t;
	//t.build();
	//t.showContainer();

	seg s;
	s.readData();
	s.readSymbol();
	s.readNum();
	//s.showNum();
	endI = time(NULL);
	cout << "The initial time is: " << difftime(endI, startI) << "s" << endl;
	
	startR = time(NULL);
	s.control();
	//s.showInitM();
	//s.showProbM();
	//s.showEmitM();
	//s.showState();
	endR = time(NULL);
	cout << "The time of dividing is: " << difftime(endR, startR) << "s" << endl;

	return 0;
}
