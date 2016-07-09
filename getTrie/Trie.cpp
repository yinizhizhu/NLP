#include "../h/Trie.h"

TrieNode::TrieNode()
{
	pre = NULL;
	next = NULL;
	dataValue.counter = 0;
	return;
}

TrieNode::~TrieNode()
{
	next->clear();
	delete pre;
	delete next;
	return;
}

TrieNode* TrieNode::findNext(Unicode word)
{
	if (next == NULL)
		return NULL;
	NextMap::const_iterator iter = next->find(word);
	if (iter == next->end())
		return NULL;
	return iter->second;
}

Trie::Trie()
{
	root = new TrieNode();
	root->pre = NULL;
	root->next = new unordered_map<Unicode, TrieNode*>;
	return;
}

Trie::~Trie()
{
	TrieNode* tmp;
	queue<TrieNode*> container;
	container.push(root);
	unordered_map<Unicode, TrieNode*>::iterator moveIter, endIter;
	while (!container.empty())
	{
		size_t i = 0, len = container.size();
		for (; i < len; i++)
		{
			tmp = container.front();
			if (tmp->next)
			{
				moveIter = tmp->next->begin();
				endIter = tmp->next->end();
				for (; moveIter != endIter; moveIter++)
					container.push(moveIter->second);
			}
			delete tmp;
		}
	}
	return;
}

void Trie::build()
{
	char tmp[3];
	tmp[2] = 0;
	vector<Unicode> words;

	TrieNode *preTrieNode = NULL;
	TrieNode *curTrieNode = NULL;

	ifstream in("testData.txt");
	string container;
	while (!in.eof())
	{
		getline(in, container);
		for (unsigned int i = 0; i < container.size();)
		{
			tmp[0] = container[i];
			if (getSerialOne(tmp[0]) == 0)
			{
				curTrieNode = insertTrie(words);
				if (preTrieNode && curTrieNode)		//store the preTrieNode to get the relation in position
						curTrieNode->dataValue.pre.push_back(preTrieNode);
				preTrieNode = curTrieNode;

				if (tmp[0] == 'w')
				{
					preTrieNode = NULL;
					words.clear();
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
		cout << container << endl;
	}
	in.close();
	return;
}

TrieNode* Trie::findTrieNode(const vector<Unicode>& words)
{
	TrieNode *move = root;
	if (move->next)
	{
		size_t i = 0, len = words.size();
		for (; i < len; i++)
		{
			move = move->findNext(words[i]);
			if (move == NULL)
				return NULL;
		}
	}
	return move;
}

TrieNode* Trie::insertTrie(const vector<Unicode>& words)
{
	if (words.size() == 0)
		return NULL;
	TrieNode *pre = NULL;
	TrieNode *cur = findTrieNode(words);
	if (cur)
	{
		if (cur->dataValue.counter)
			cur->dataValue.word = words.back();
		cur->dataValue.counter++;
		return cur;
	}
	else
	{
		size_t pos = 0, len = words.size();
		cur = root;
		while (cur->next->size())
		{
			pre = cur;
			cur = cur->findNext(words[pos]);
			if (cur)
				pos++;
			else
				break;
		}
		for (; pos < len-1; pos++)		//step by step
		{
			cur = new TrieNode;
			pre->next->insert(unordered_map<Unicode, TrieNode*>::value_type(words[pos], cur));
			cur->pre = pre;
			pre = cur;
		}
		cur = new TrieNode;				//finish the last step
		pre->next->insert(unordered_map<Unicode, TrieNode*>::value_type(words[pos], cur));
		cur->pre = pre;
		cur->dataValue.word = words[pos];
		cur->dataValue.counter++;
	}
	return cur;
}

void Trie::deleteNode(TrieNode* node)
{
	if (node->next)
	{
		unordered_map<Unicode, TrieNode*>::iterator iter = node->next->begin();
		for (; iter != node->next->end(); iter++)
			deleteNode(iter->second);
		delete node;
	}
	return;
}

Unicode charToUni(char tmp[3])
{
	return (((Unicode)tmp[0] << 8) | ((Unicode)tmp[1] & 0x00FF));
}

void UniToChar(Unicode word, char tmp[3])
{
	tmp[0] = (word >> 8);
	tmp[1] = (word & 0xFF);
	tmp[2] = '\0';
	return;
}

int getSerialOne(char byte)
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

void test()
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