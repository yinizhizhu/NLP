#ifndef SEGMENT_H
#define SEGMENT_H

#include "Trie.h"

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
	void control();
	void divider(vector<Unicode>& words, vector<char>& state);
	void showInitM();
	void showProbM();
	void showEmitM();
	void showState();
private:
	vector<char> initState;
	vector<double> initM;
	vector<vector<double>> probM;
	unordered_map<Unicode, vector<double>> emitM;
};

#endif