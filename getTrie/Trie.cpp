#include "../h/Trie.h"

ofstream out("RESULT/res.txt");

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
	total = 0;
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
	out << "The total number of words is: " << total << endl;
	out.close();
	cout << "Out delete Trie" << endl;
	return;
}

void Trie::build()		//build up the Trie Tree
{
	char tmp[3];
	tmp[2] = 0;
	vector<Unicode> words;

	TrieNode *preTrieNode = root;
	TrieNode *curTrieNode = NULL;

	ifstream in("testData.txt");
	//ifstream in("trainingData.txt");
	string container;
	while (!in.eof())
	{
		getline(in, container);
		for (size_t i = 0; i < container.size();)
		{
			tmp[0] = container[i];
			if (getSerialOne(tmp[0]) == 0)
			{
				if (container[i + 1] == 'w' && container[i] == '/')
				{
					preTrieNode = root;
					words.clear();
				}
				else
				{
					if (!words.empty())
					{
						total++;
						curTrieNode = insertTrie(words);
						if (preTrieNode && curTrieNode)		//store the preTrieNode to get the relation in position
							curTrieNode->dataValue.pre.push_back(preTrieNode);
						preTrieNode = curTrieNode;
						words.clear();
					}
				}
				i++;
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

TrieNode* Trie::insertTrie(const vector<Unicode>& words)		//insert the words into the Trie
{
	if (words.size() == 0)
		return NULL;

	TrieNode *pre = NULL;
	TrieNode *cur = findTrieNode(words);
	if (cur)
	{
		if (cur->dataValue.counter > 0)
			cur->dataValue.word = words.back();
		cur->dataValue.counter++;
		return cur;
	}
	else
	{
		size_t pos = 0, len = words.size();
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
				return NULL;
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
			return NULL;
		}
		cur->pre = pre;
		cur->dataValue.word = words[pos];
		cur->dataValue.counter++;
		pre->next->insert(unordered_map<Unicode, TrieNode*>::value_type(words[pos], cur));
	}
	return cur;
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
			for (size_t i = 0; i < words.size(); i++)
				showTrieNode(words[i]);
			//cout << endl;
			out << endl;
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
	out << tmp << ": " << node->dataValue.counter << ", ";
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