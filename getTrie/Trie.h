#ifndef TRIE_H
#define TRIE_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <unordered_map>
#include <math.h>

typedef unsigned short Unicode;

using namespace std;

typedef struct dataUnit
{
	Unicode word;
	size_t counter;		//the total number of the word, so far
	//vector<TrieNode*> pre;		//the preTrieNode of this TrieNode
} dataUint;

class TrieNode
{
public:
	TrieNode();
	~TrieNode();
	TrieNode* findNext(Unicode word);
public:
	typedef unordered_map<Unicode, TrieNode*> NextMap;
	TrieNode* pre;
	NextMap* next;
	dataUnit dataValue;
};

class Trie
{
public:
	Trie();
	~Trie();
public:
	void build();
	TrieNode* findTrieNode(const vector<Unicode>& words);
	//TrieNode* insertTrie(const vector<Unicode>& words);
	void insertTrie(const vector<Unicode>& words);
	size_t insertWordContainer(const vector<Unicode>& words);
	void insertContainer(const Unicode& word, size_t tag);
	void deleteNode(TrieNode* node);
	void showTrie(TrieNode *node, vector<TrieNode*>& words);
	void showTrieNode(TrieNode* node);
	void showContainer();
private:
	TrieNode* root;

	size_t totalBF;
	size_t totalSF;

	size_t totalB;
	size_t totalM;
	size_t totalE;
	size_t totalS;
	
	size_t totalEB;
	size_t totalES;

	size_t totalBM;
	size_t totalBE;

	size_t totalMM;
	size_t totalME;

	size_t totalSS;
	size_t totalSB;

	size_t totalWord;
	size_t totalWords;
	unordered_map<Unicode, size_t> BContainer;		//the state B
	unordered_map<Unicode, size_t> MContainer;		//the state M
	unordered_map<Unicode, size_t> EContainer;		//the state E
	unordered_map<Unicode, size_t> SContainer;		//the state S
	unordered_map<Unicode, size_t> wordContainer;		//store each word
	//unordered_map<Unicode, size_t> symbolContainer;		//store each symbol
};

Unicode charToUni(char tmp[3]);
void UniToChar(Unicode word, char tmp[3]);
int getSerialOne(char byte);
void test();

#endif
