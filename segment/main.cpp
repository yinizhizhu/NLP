#include <iostream>
#include <time.h>
#include "h/Segment.h"

using namespace std;

typedef unsigned short uint16_t;
typedef unsigned char uint8_t;

int main()
{
	//test();
	//TrieNode test;
	//cout << test.dataValue.counter << endl;

	time_t start, end;
	start = time(NULL);

	//Trie t;
	//t.build();
	//t.addDict();
	//vector<TrieNode*> words;
	//test.showTrie(NULL, words);
	//t.showContainer();

	seg s;
	s.readData();
	s.readSymbol();
	s.control();
	//s.showInitM();
	//s.showProbM();
	//s.showEmitM();
	//s.showState();

	end = time(NULL);
	cout << "The running time is: " << difftime(end, start) << "s" << endl;
	return 0;
}
