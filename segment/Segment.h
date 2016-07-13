#ifndef SEGMENT_H
#define SEGMENT_H

#include "Trie.h"
#define NOT 3.14e+100

using namespace std;

typedef struct segNode
{
	vector<size_t> bestSel;	//store the pre index of best way
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
	void readData();
	void readSymbol();					//read the symbol from file
	size_t findSymbol(Unicode word);	//check whether the Word is in the symbol or not
	void control();
	void eachWords(vector<Unicode>& words, vector<char>& state);
	void divider(vector<Unicode>& words, vector<char>& state);
	void showInitM();
	void showProbM();
	void showEmitM();
	void showState();
	void showWords(vector<Unicode>& words);
private:
	vector<char> initState;
	vector<double> initM;
	vector<vector<double>> probM;
	unordered_map<Unicode, vector<double>> emitM;
	unordered_map<Unicode, size_t> symbol;
};

#endif