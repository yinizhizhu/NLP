#include "../h/Trie.h"

#define OUT
//#define OUTSYMBOL

#ifdef OUTSYMBOL
ofstream outSymbol("RESULT/symbol.txt");
#endif

#ifdef OUT
ofstream out("RESULT/res.txt");
#endif

TrieNode::TrieNode()	//Initial
{
	pre = NULL;
	next = new NextMap;
	if (next == NULL)
	{
		cout << "The room is not enough for TrieNode!" << endl;
		return;
	}
	dataValue.counter = 0;
	return;
}

TrieNode::~TrieNode()	//free the room
{
	delete next;
	return;
}

TrieNode* TrieNode::findNext(Unicode word)	// get the next TrieNode
{
	NextMap::const_iterator iter = next->find(word);
	if (iter == next->end())
		return NULL;
	return iter->second;
}

Trie::Trie()	//initial
{
	root = new TrieNode;
	if (root == NULL)
	{
		cout << "The room is not enough for root!" << endl;
		return;
	}
	totalB = totalM = totalE = totalS = 0;
	totalWord = 0;
	totalWords = 0;
	return;
}

Trie::~Trie()	// free the room 
{
	cout << "In delete Trie" << endl;
	TrieNode* tmp = NULL;
	queue<TrieNode*> container;
	container.push(root);
	unordered_map<Unicode, TrieNode*>::iterator moveIter, endIter;
	while (!container.empty())
	{
		size_t i = 0, len = container.size();
		for (; i < len; i++)
		{
			tmp = container.front();
			if (tmp)
			{
				moveIter = tmp->next->begin();
				endIter = tmp->next->end();
				for (; moveIter != endIter; moveIter++)
					container.push(moveIter->second);
				tmp->next->clear();
			}
			container.pop();		//pop the front element of the queue
			delete tmp;
		}
	}
	//out << "The total number of word is: " << totalWord << endl;
	//out << "The total number of words is: " << totalWords << endl;
#ifdef OUTSYMBOL
	outSymbol.close();
#endif
	cout << "Out delete Trie" << endl;
	return;
}

void Trie::build()		//build up the Trie Tree
{
	cout << "In build" << endl;
	char tmp[3];
	tmp[2] = 0;
	vector<Unicode> words;

	size_t len1 = 0, len2 = 0, len;

	ifstream in("trainingData.txt");
	string container;
	while (!in.eof())
	{
		getline(in, container);
		len = container.size();
		for (size_t i = 0; i < len;)
		{
			tmp[0] = container[i];
			if (getSerialOne(tmp[0]) == 0)
			{
				if (container[i + 1] == 'w' && container[i] == '/')
				{
#ifdef OUTSYMBOL
					insertTrie(words);
#endif
					words.clear();
					len1 = len2 = 0;
				}
				else
				{
					if (!words.empty())
					{
						totalWords++;	//count the words
#ifndef OUTSYMBOL
						insertTrie(words);
						len2 = insertWordContainer(words);
#endif
						handleBS(len1, len2);
						len1 = len2;
						words.clear();
					}
				}
				do
				{
					i++;
				} while (getSerialOne(container[i]) == 0 && i < len);
			}
			else
			{
				tmp[1] = container[i + 1];
				words.push_back(charToUni(tmp));
				i += 2;
			}
		}
	}
	in.close();
	cout << "Out build" << endl;
	return;
}

void Trie::handleBS(size_t len1, size_t len2)
{
	if (len1 == 0)
	{
		if (len2 == 1)
			totalSF++;
		else
			totalBF++;
	}
	else if (len1 == 1)	//S
	{
		if (len2 == 1)	//S->S
			totalSS++;
		else			//S->B
			totalSB++;
	}
	else			//E
	{
		if (len2 == 1)	//E->S
			totalES++;
		else			//E->B
			totalEB++;
	}
	return;
}

void Trie::addDict()
{
	cout << "In addDict" << endl;
	char tmp[3];
	tmp[2] = 0;
	vector<Unicode> words;

	ifstream in("dictTrain.txt");
	string container;
	while (!in.eof())
	{
		getline(in, container);
		for (size_t i = 0; i < container.size();)
		{
			tmp[0] = container[i];
			tmp[1] = container[i + 1];
			words.push_back(charToUni(tmp));
			i += 2;
			if (0 == getSerialOne(container[i]))
				break;
		}
		insertTrie(words);
		words.clear();
	}
	in.close();
	cout << "out addDict" << endl;
	return;
}

size_t Trie::getDepth(vector<Unicode>& words, vector<size_t>& T, size_t start)
{
	TrieNode *move = root;
	size_t i, counter, maxC, maxT = 1, len = words.size();

	maxC = counter = 0;
	for (i = start; i < len; i++)
	{
		move = move->findNext(words[i]);
		if (move == NULL)
		{
			T[start] = maxT;
			return maxC;
		}
		counter++;
		if (move->dataValue.counter > 0)
		{
			maxC = counter;
			maxT = move->dataValue.counter;
		}
	}
	T[start] = maxT;
	return maxC;
}

bool Trie::checkI(size_t i, size_t lenI, vector<size_t>& posContainer)
{
	size_t tmpI;
	size_t move = 0, len = posContainer.size();
	for (; move < len; move++)
	{
		tmpI = posContainer[move];
		if (i < tmpI && (i + lenI) > tmpI)
				return false;
	}
	return true;
}

void Trie::opWithMaxLen(vector<size_t>& maxLen, vector<size_t>& T)
{
	set<size_t>::iterator iter;
	set<size_t> index;

	vector<size_t> posContainer;
	size_t maxP;
	size_t i, tmpI, len = maxLen.size();

	for (i = 0; i < len; i++)
		index.insert(i);

	while (!index.empty())
	{
		maxP = 10000;
		iter = index.begin();
		for (; iter != index.end(); iter++)
		{
			tmpI = *iter;
			if (checkI(tmpI, maxLen[tmpI], posContainer))
			{
				if (maxP == 10000)
				{
					maxP = tmpI;
				}
				else
				{
					if (maxLen[tmpI] > maxLen[maxP])
						maxP = tmpI;
					else if (maxLen[tmpI] == maxLen[maxP] && maxLen[tmpI] == 2)
					{
						if (T[tmpI] > T[maxP])
						{
							maxP = tmpI;
						}
					}
				}
			}
		}
		if (maxP == 10000 || maxLen[maxP] < 2)
			return;
		posContainer.push_back(maxP);
		for (i = 1; i < maxLen[maxP]; i++)
		{
			maxLen[maxP + i] = 0;
			index.erase(maxP + i);
		}
		index.erase(maxP);	//delete the index from the ready set
		if (maxP > 0 && 2 == maxLen[maxP - 1])
			maxLen[maxP - 1] = 1;
	}
	return;
}

void Trie::opWithDict(vector<Unicode>& words, vector<char>& state)
{
	vector<size_t> T;
	vector<size_t> maxLen;
	size_t i, j, len = words.size();
	T.resize(len);
	state.resize(len);

	for (i = 0; i < len; i++)
	{
		maxLen.push_back(getDepth(words, T, i));
	}

	opWithMaxLen(maxLen, T);

	for (i = 0; i < len;)
	{
		if (maxLen[i] >= 2)
		{
			state[i] = 'B';
			for (j = 1; j < (maxLen[i] - 1); j++)
				state[i + j] = 'M';
			state[i + j] = 'E';
			i += maxLen[i];
		}
		else
		{
			state[i] = 'N';
			i++;
		}
	}

	//for (i = 0; i < (len - 1); i++)	//try to opitmise
	//{
	//	if (state[i] == 'B' && state[i + 1] == 'B')
	//		state[i+1] = 'S';
	//	else if (state[i] == 'E' && state[i + 1] == 'E')
	//		state[i+1] = 'S';
	//}
	return;
}

TrieNode* Trie::findTrieNode(const vector<Unicode>& words)		//get TrieNode of the words
{
	TrieNode *move = root;
	size_t i = 0, len = words.size();
	for (; i < len; i++)
	{
		move = move->findNext(words[i]);
		if (move == NULL)
			return NULL;
	}
	return move;
}

void Trie::insertTrie(const vector<Unicode>& words)		//insert the words into the Trie
{
	if (words.size() == 0)
		return;

	TrieNode *pre = NULL;
	TrieNode *cur = findTrieNode(words);
	size_t pos = 0, len = words.size();
	totalWord += len;//count the word
	if (cur)
	{
		if (cur->dataValue.counter > 0)
			cur->dataValue.word = words.back();
		cur->dataValue.counter++;
		return;
	}
	else
	{
		pre = cur = root;
		while (cur->next->size())
		{
			cur = cur->findNext(words[pos]);
			if (cur)
			{
				pre = cur;
				pos++;
			}
			else
				break;
		}
		for (; pos < len - 1; pos++)		//step by step
		{
			cur = new TrieNode;
			if (cur == NULL)
			{
				cout << "The room is not enough for cur!" << endl;
				return;
			}
			cur->pre = pre;
			cur->dataValue.word = words[pos];
			pre->next->insert(unordered_map<Unicode, TrieNode*>::value_type(words[pos], cur));
			pre = cur;
		}
		cur = new TrieNode;				//finish the last step
		if (cur == NULL)
		{
			cout << "The room is not enough for cur!" << endl;
			return;
		}
		cur->pre = pre;
		cur->dataValue.word = words[pos];
		cur->dataValue.counter++;
		pre->next->insert(unordered_map<Unicode, TrieNode*>::value_type(words[pos], cur));
	}
	return;
}

size_t Trie::insertWordContainer(const vector<Unicode>& words)
{
	size_t i = 0, len = words.size();
	if (len == 1)
	{
		insertContainer(words[i], 3);		//Single
		totalS++;
	}
	else if (len > 1)
	{
		insertContainer(words[i], 0);		//Begin
		totalB++;
		if (len == 2)
			totalBE++;			//BE
		else
		{
			totalBM++;			//BM
			totalMM += len - 3;	//MM
			totalME++;			//ME
		}
		for (i = 1; i < (len - 1); i++)
		{
			insertContainer(words[i], 1);	//Middle
			totalM++;
		}
		insertContainer(words[i], 2);		//End
		totalE++;
	}
	return len;
}

void Trie::insertContainer(const Unicode& word, size_t tag)
{
	unordered_map<Unicode, size_t>::iterator iter;

	iter = wordContainer.find(word);	//wordContainer
	if (iter == wordContainer.end())
		wordContainer.insert(unordered_map<Unicode, size_t>::value_type(word, 1));
	else
		iter->second++;

	if (tag == 0)
	{
		iter = BContainer.find(word);	//BContainer
		if (iter == BContainer.end())
			BContainer.insert(unordered_map<Unicode, size_t>::value_type(word, 1));
		else
			iter->second++;
	}
	else if (tag == 1)
	{
		iter = MContainer.find(word);	//MContainer
		if (iter == MContainer.end())
			MContainer.insert(unordered_map<Unicode, size_t>::value_type(word, 1));
		else
			iter->second++;
	}
	else if (tag == 2)
	{
		iter = EContainer.find(word);	//EContainer
		if (iter == EContainer.end())
			EContainer.insert(unordered_map<Unicode, size_t>::value_type(word, 1));
		else
			iter->second++;
	}
	else if (tag == 3)
	{
		iter = SContainer.find(word);	//SContainer
		if (iter == SContainer.end())
			SContainer.insert(unordered_map<Unicode, size_t>::value_type(word, 1));
		else
			iter->second++;
	}
	return;
}

void Trie::deleteNode(TrieNode* node)	//delete the all the nodes(Trie tree)
{
	unordered_map<Unicode, TrieNode*>::iterator iter = node->next->begin();
	for (; iter != node->next->end(); iter++)
		deleteNode(iter->second);
	delete node;
	return;
}

void Trie::showTrie(TrieNode *node, vector<TrieNode*>& words)		//show each word of Trie
{
	TrieNode *move;
	if (node)
		move = node;
	else
		move = root;
	if (move->next->size() == 0)
	{
		if (!words.empty())
		{
#ifdef OUTSYMBOL
			outSymbol << words.size() << " ";
#endif
			for (size_t i = 0; i < words.size(); i++)
				showTrieNode(words[i]);
			//cout << endl;
#ifdef OUTSYMBOL
			outSymbol<<endl;
#endif
#ifdef OUT
			out << endl;
#endif
		}
		return;
	}
	unordered_map<Unicode, TrieNode*>::iterator iter = move->next->begin();
	for (; iter != move->next->end(); iter++)
	{
		words.push_back(iter->second);
		showTrie(iter->second, words);
		words.pop_back();
	}
	return;
}

void Trie::showTrieNode(TrieNode *node)		//show the content of the TrieNode
{
	char tmp[3];
	UniToChar(node->dataValue.word, tmp);
	//cout << tmp << ": " << node->dataValue.counter << ", ";
#ifdef OUTSYMBOL
	outSymbol << tmp;
#endif
#ifdef OUT
	out << tmp << ": " << node->dataValue.counter << ", ";
#endif
	return;
}

void Trie::showContainer()
{
	char tmp[3];
	unordered_map<Unicode, size_t>::iterator iter;
	
	iter = wordContainer.begin();
#ifdef OUT
	out << wordContainer.size() << endl;
#endif
	for (; iter != wordContainer.end(); iter++)
	{
		UniToChar(iter->first, tmp);
		//cout << tmp << ": " << iter->second << ",";
#ifdef OUT
		out << tmp << endl;
#endif
	}

#ifdef OUT
	out << BContainer.size() << endl;
#endif
	iter = BContainer.begin();
	for (; iter != BContainer.end(); iter++)
	{
		UniToChar(iter->first, tmp);
		//cout << tmp << ": " << iter->second << ",";
#ifdef OUT
		out << tmp << " " << setprecision(16) << log(double(iter->second*1.0 / totalWord)) << endl;
#endif
	}
	//cout << endl;

#ifdef OUT
	out << MContainer.size() << endl;
#endif
	iter = MContainer.begin();
	for (; iter != MContainer.end(); iter++)
	{
		UniToChar(iter->first, tmp);
		//cout << tmp << ": " << iter->second << ",";
#ifdef OUT
		out << tmp << " " << setprecision(16) << log(double(iter->second*1.0 / totalWord)) << endl;
#endif
	}
	//cout << endl;

#ifdef OUT
	out << EContainer.size() << endl;
#endif
	iter = EContainer.begin();
	for (; iter != EContainer.end(); iter++)
	{
		UniToChar(iter->first, tmp);
		//cout << tmp << ": " << iter->second << ",";
#ifdef OUT
		out << tmp << " " << setprecision(16) << log(double(iter->second*1.0 / totalWord)) << endl;
#endif
	}
	//cout << endl;

#ifdef OUT
	out << SContainer.size() << endl;
#endif
	iter = SContainer.begin();
	for (; iter != SContainer.end(); iter++)
	{
		UniToChar(iter->first, tmp);
		//cout << tmp << ": " << iter->second << ",";
#ifdef OUT
		out << tmp << " " << setprecision(16) << log(double(iter->second*1.0 / totalWord)) << endl;
#endif
	}

#ifdef OUT
	out << "4"<< endl;
	out << "-3.14e+100 " << setprecision(16) << log(double(totalBM*1.0 / totalB)) << " " << setprecision(16) << log(double(totalBE*1.0 / totalB)) << " -3.14e+100 " << endl;
	out << "-3.14e+100 " << setprecision(16) << log(double(totalMM*1.0 / totalM)) << " " << setprecision(16) << log(double(totalME*1.0 / totalM)) << " -3.14e+100 " << endl;
	out << setprecision(16) << log(double(totalEB*1.0 / totalE)) << " -3.14e+100 -3.14e+100 " << setprecision(16) << log(double(totalES*1.0 / totalE)) << " " << endl;
	out << setprecision(16) << log(double(totalSB*1.0 / totalS)) << " -3.14e+100 -3.14e+100 " << setprecision(16) << log(double(totalSS*1.0 / totalS)) << " " << endl;

	out << "1" << endl;
	out << setprecision(16) << log(double(totalBF*1.0 / totalWord)) << " -3.14e+100 -3.14e+100 " << setprecision(16) << log(double(totalSF*1.0 / totalWord)) << " " << endl;
#endif
#ifdef OUT
	out.close();
#endif
	return;
}

void Trie::showWords(vector<Unicode>& words)
{
	char tmp[3];
	size_t i, len = words.size();
	for (i = 0; i < len; i++)
	{
		UniToChar(words[i], tmp);
		cout << tmp;
	}
	return;
}

Unicode charToUni(char tmp[3])		//Unicode into two chars
{
	return (((Unicode)tmp[0] << 8) | ((Unicode)tmp[1] & 0x00FF));
}

void UniToChar(Unicode word, char tmp[3])		//two chars into Unicode
{
	tmp[0] = (word >> 8);
	tmp[1] = (word & 0xFF);
	tmp[2] = '\0';
	return;
}

int getSerialOne(char byte)		//calculate the number of first continuous one
{
	int counter = 0;
	unsigned int tag = 1 << 7;
	while (byte & tag)
	{
		counter++;
		tag = tag >> 1;
	}
	return counter;
}

void test()		//for test the reading data from file
{
	ifstream in("testData.txt");
	string container;
	char tmp[3];
	tmp[2] = 0;
	while (!in.eof())
	{
		getline(in, container);
		for (unsigned int i = 0; i < container.size();)
		{
			tmp[0] = container[i];
			if (getSerialOne(tmp[0]) == 0)
				cout << "	" << dec << i++ << ": " << tmp[0] << endl;
			else
			{
				tmp[1] = container[i + 1];
				Unicode res = charToUni(tmp);
				UniToChar(res, tmp);
				cout << tmp << ": " << getSerialOne(tmp[0]) << "-->>" << getSerialOne(container[i]) << endl;
				i += 2;
			}
		}
		cout << container << endl;
	}
	in.close();
	return;
}