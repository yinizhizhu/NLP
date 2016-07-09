#ifndef TRIE_H
#define TRIE_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <unordered_map>

typedef unsigned short Unicode;

using namespace std;

class TrieNode
{
public:
	TrieNode();
	~TrieNode();
	TrieNode* findNext(Unicode word);
public:
	typedef struct dataUnit
	{
		Unicode word;
		size_t counter;		//the total number of the word, so far
		vector<TrieNode*> pre;		//the preTrieNode of this TrieNode
	} dataUint;
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
	TrieNode* insertTrie(const vector<Unicode>& words);
	void deleteNode(TrieNode* node);
	void showTrie(TrieNode *node);
	void showTrieNode(TrieNode* node);
private:
	TrieNode* root;
};

Unicode charToUni(char tmp[3]);
void UniToChar(Unicode word, char tmp[3]);
int getSerialOne(char byte);
void test();

#endif
