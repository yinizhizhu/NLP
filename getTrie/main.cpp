#include <iostream>
#include "h/Trie.h"

using namespace std;

typedef unsigned short uint16_t;
typedef unsigned char uint8_t;

int main()
{
	//test();
	//TrieNode test;
	//cout << test.dataValue.counter << endl;
	Trie test;
	test.build();
	vector<TrieNode*> words;
	test.showTrie(NULL, words);
	return 0;
}
