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

	Trie test;
	test.build();
	//test.addDict();
	//vector<TrieNode*> words;
	//test.showTrie(NULL, words);
	test.showContainer();

	//seg test;
	//test.readData();
	//test.readSymbol();
	//test.control();
	//test.showInitM();
	//test.showProbM();
	//test.showEmitM();
	//test.showState();

	end = time(NULL);
	cout << "The running time is: " << difftime(end, start) << "s" << endl;
	return 0;
}
