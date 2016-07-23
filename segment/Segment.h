#ifndef SEGMENT_H
#define SEGMENT_H

#include "Trie.h"
#define NOT 3.14e+100

using namespace std;

typedef struct segNode		//To store the each node which will be used to select the best way.
{
	vector<size_t> bestSel;	//store the pre index of best one
	vector<double> bestPro;	//store the best pro
	segNode()
	{
		bestSel.resize(4);
		bestPro.resize(4);
	}
}segNode;

class seg
{
public:
	seg();
	~seg();
	void readNum();						//get the number and '-'
	bool checkNum(Unicode tmp);
	void readData();					//get the initM, the probM, the emitM from file
	void readSymbol();					//read the symbol from file
	size_t findSymbol(Unicode word);	//check whether the Word is in the symbol or not
	void control();		//the controller
	void eachWords(vector<Unicode>& words, vector<char>& state);	//output the each word according to the state
	void outPut(vector<Unicode>& words, vector<char>& state);
	void divider(vector<Unicode>& words, size_t start, size_t tail, vector<char>& state);		//divider the words with hMM: state store the result of dividing
	void showInitM();	//show the initM
	void showProbM();	//show the probM
	void showEmitM();	//show the emitM
	void showState();	//show the state
	void showWords(vector<Unicode>& words);		//show the word
	void showNum();
private:
	Unicode special;		//handle the symbol "-"
	vector<Unicode> num;		//hand with the numbers and the %
	vector<char> initState;								//store all the states
	vector<double> initM;								//initM: initial matrix
	vector<vector<double>> probM;						//probM: trans matrix
	unordered_map<Unicode, vector<double>> emitM;		//emitM: emit matrix
	unordered_map<Unicode, size_t> symbol;				//store all the symbols;
	Trie dict;
};

#endif