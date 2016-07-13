#ifndef TRIE_H
#define TRIE_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <unordered_map>
#include <math.h>
#include <iomanip>

typedef unsigned short Unicode;

using namespace std;

typedef struct dataUnit
{
	Unicode word;		//the each word
	size_t counter;		//the total number of the word, so far: 0 - middle
} dataUint;

class TrieNode	//the node of the Trie
{
public:
	TrieNode();
	~TrieNode();
	TrieNode* findNext(Unicode word);	//get the next TrieNode of the current 'word' 
public:
	typedef unordered_map<Unicode, TrieNode*> NextMap;
	TrieNode* pre;			//store the pre TrieNode
	NextMap* next;			//store the next TrieNodes
	dataUnit dataValue;		//store the current word and counter the end.
};

class Trie
{
public:
	Trie();
	~Trie();
public:
	void build();		//build the tree with the training data.
	void handleBS(size_t len1, size_t len2);	//between the each words or sentences
	void addDict();		//add the data of dict into the tree
	TrieNode* findTrieNode(const vector<Unicode>& words);		//check whether the words is in the Trie or not.
	void insertTrie(const vector<Unicode>& words);				//insert the words into the Trie
	size_t insertWordContainer(const vector<Unicode>& words);	//the statistics of Each parameters: get the various Container
	void insertContainer(const Unicode& word, size_t tag);		//servicing on insertWordContainer
	void deleteNode(TrieNode* node);							//free the Trie: delete all TrieNode
	void showTrie(TrieNode *node, vector<TrieNode*>& words);	//show details of each words in Trie
	void showTrieNode(TrieNode* node);							//servicing on showTrie
	void showContainer();										//output the main data which will be used at next program.
private:
	TrieNode* root;		//the root of Trie

	size_t totalBF;		//the first word is State "B": used in initM
	size_t totalSF;		//the first word is state "S": used in initM

	size_t totalB;		//the total number of State "B": used in probM
	size_t totalM;		//the total number of State "M": used in probM
	size_t totalE;		//the total number of State "E": used in probM
	size_t totalS;		//the total number of State "S": used in probM
	
	size_t totalBM;		//the total number of State "M" after State "B": used in probM
	size_t totalBE;		//the total number of State "E" after State "B": used in probM

	size_t totalMM;		//the total number of State "M" after State "M": used in probM
	size_t totalME;		//the total number of State "E" after State "M": used in probM

	size_t totalEB;		//the total number of State "B" after State "E": used in probM
	size_t totalES;		//the total number of State "S" after State "E": used in probM

	size_t totalSB;		//the total number of State "B" after State "S": used in probM
	size_t totalSS;		//the total number of State "S" after State "S": used in probM

	size_t totalWord;	//the total number of word: used in emitM
	size_t totalWords;	//the total number of the unique word
	unordered_map<Unicode, size_t> BContainer;		//count the time of each word's "B": used in emitM
	unordered_map<Unicode, size_t> MContainer;		//count the time of each word's "M": used in emitM
	unordered_map<Unicode, size_t> EContainer;		//count the time of each word's "E": used in emitM
	unordered_map<Unicode, size_t> SContainer;		//count the time of each word's "S": used in emitM
	unordered_map<Unicode, size_t> wordContainer;	//store each unique word
};

Unicode charToUni(char tmp[3]);					//convert two chars into a Unicode
void UniToChar(Unicode word, char tmp[3]);		//convert a Unicode into two chars
int getSerialOne(char byte);					//caculate the first continuous bit of '1'
void test();									//test reading data and the coding of word

#endif
