#include "../h/Segment.h"

ofstream segOut("RESULT/segOut.txt");
ofstream unkonwn("RESULT/unknown.txt");

seg::seg()
{
	size_t i;
	initState.push_back('B');
	initState.push_back('M');
	initState.push_back('E');
	initState.push_back('S');
	initM.resize(4);
	for (i = 0; i < 4; i++)
	{
		vector<double> newOne;
		newOne.resize(4);
		probM.push_back(newOne);
	}
	return;
}

seg::~seg()
{
	segOut.close();
	return;
}

void seg::control()
{
	ifstream in("dev.txt");
	//ifstream in("devTest.txt");

	size_t i, len, lineLen;
	string container;

	char tmp[3];
	tmp[2] = 0;
	Unicode one;
	vector<Unicode> words;
	vector<char> state;

	while (!in.eof())
	{
		getline(in, container);
		lineLen = container.size();
		for (i = 0; i < lineLen;)
		{
			tmp[0] = container[i];
			tmp[1] = container[i + 1];
			one = charToUni(tmp);

			len = findSymbol(one);
			if (len)
			{
				if (!words.empty())
				{
					eachWords(words, state);
				}
				//for (size_t j = 0; j < (len * 2) && (i + j) < lineLen; j++)
				//	segOut << container[i + j];
				//segOut << '/';
				i += len * 2;
			}
			else
			{
				words.push_back(one);
				i += 2;
			}
		}
		if (!words.empty())
		{
			eachWords(words, state);
		}
	}

	in.close();
}

void seg::eachWords(vector<Unicode>& words, vector<char>& state)
{
	char tmp[3];
	size_t j;
	if (words.size() > 0)
	{
		divider(words, state);

		//for (j = 0; j < state.size(); j++)
		//	cout << state[j];
		//cout << endl;

		UniToChar(words[0], tmp);
		segOut << tmp;
		if (state[0] == 'S')
		{
			//segOut << '/';
			segOut << endl;
		}
		for (j = 1; j < state.size(); j++)
		{
			UniToChar(words[j], tmp);
			segOut << tmp;
			if (state[j] == 'E' || state[j] == 'S')
			{
				//segOut << '/';
				segOut << endl;
			}
		}
		words.clear();
		state.clear();
	}
	return;
}

void seg::divider(vector<Unicode>& words, vector<char>& state)
{
	//cout << "In Divider: ";

	char tmp[3];
	tmp[2] = 0;

	size_t bestIndex;
	size_t i, j, k, len = words.size();
	unordered_map<Unicode, vector<double>>::iterator iter;

	vector<segNode> all;
	all.resize(len);
	state.resize(len);

	vector<double> prob;	//store the each 4 prob to select the best one
	prob.resize(4);

	iter = emitM.find(words[0]);
	if (iter == emitM.end())
	{
		UniToChar(words[0], tmp);
		//cout << "1.Cannot find the word: " << tmp << endl;
		unkonwn << tmp << endl;
		vector<double> newOne;
		for (j = 0; j < 4; j++)
			newOne.push_back(-15);
		emitM.insert(unordered_map<Unicode, vector<double>>::value_type(words[0], newOne));
		iter = emitM.find(words[0]);
	}
	for (i = 0; i < 4; i++)
	{
		if (initM[i] != -NOT)
			all[0].bestPro[i] = initM[i] * iter->second[i];
		else
			all[0].bestPro[i] = NOT;
		//cout << "<" << all[0].bestSel[i] << "> " << all[0].bestPro[i] << "=";//for test
	}
	//cout << endl;//for test

	for (i = 1; i < len; i++)
	{
		iter = emitM.find(words[i]);
		if (iter == emitM.end())
		{
			UniToChar(words[i], tmp);
			//cout << "2.Cannot find the word: " << tmp << endl;
			unkonwn << tmp << endl;
			vector<double> newOne;
			for (j = 0; j < 4; j++)
				newOne.push_back(-15);
			emitM.insert(unordered_map<Unicode, vector<double>>::value_type(words[i], newOne));
			iter = emitM.find(words[i]);
		}
		for (j = 0; j < 4; j++)	//the cur word
		{
			for (k = 0; k < 4; k++)	//the pre word
			{
				if (all[i - 1].bestPro[k] != NOT)
					prob[k] = all[i - 1].bestPro[k] * probM[k][j] * iter->second[j] / 10;
				else
					prob[k] = NOT;
			}

			bestIndex = 0;			//get the best one( Dynamic )
			for (k = 1; k < 4; k++)
			{
				if (prob[k] < prob[bestIndex])
					bestIndex = k;
			}
			all[i].bestSel[j] = bestIndex;
			all[i].bestPro[j] = prob[bestIndex];

			//cout << "<" << all[i].bestSel[j] << "> " << all[i].bestPro[j] << "--";	//for test
		}
		//cout << endl;//for test
	}

	if (len == 1)
	{
		state[0] = 'S';
		//cout << "Out Divider" << endl;
		return;
	}

	bestIndex = 2;	//E
	if (all[len - 1].bestPro[3] < all[len - 1].bestPro[2])	//S
			bestIndex = 3;

	state[len - 1] = initState[bestIndex];
	for (i = len - 2;; i--)
	{
		bestIndex = all[i + 1].bestSel[bestIndex];
		state[i] = initState[bestIndex];
		//if (bestIndex == 3 || bestIndex == 2) //the pre: E S
		//{
		//	if (state[i + 1] == 'M' || state[i + 1] == 'E')
		//	{
		//		state[i] = 'B';
		//	}
		//	else
		//	{
		//		state[i] = 'S';
		//	}
		//}
		//else								//the next: B M
		//{
		//	if (state[i + 1] == 'B' || state[i + 1] == 'S')
		//	{
		//		state[i] = 'E';
		//	}
		//	else
		//	{
		//		state[i] = 'M';
		//	}
		//}
		if (i == 0)
			break;
	}
	//cout << "Out Divider" << endl;
	return;
}

void seg::readData()
{
	ifstream in("RESULT/res.txt");

	Unicode word;
	char tmp[3], number[30];
	tmp[2] = 0;
	unordered_map<Unicode, vector<double>>::iterator iter;

	size_t i, j, k, len;
	string container;

	getline(in, container);
	len = atoi(container.c_str());
	for (i = 0; i < len; i++)	//read the Unicode of each word
	{
		getline(in, container);
		tmp[0] = container[0];
		tmp[1] = container[1];
		word = charToUni(tmp);
		vector<double> newOne;
		for (j = 0; j < 4; j++)
			newOne.push_back(-15);
		emitM.insert(unordered_map<Unicode, vector<double>>::value_type(word, newOne));
	}

	for (k = 0; k < 4; k++)		//read the data which will be insert into the emitM Matrix
	{	//read the state:
		/*
		0 - B
		1 - M
		2 - E
		3 - S
		*/
		getline(in, container);
		len = atoi(container.c_str());
		for (i = 0; i < len; i++)
		{
			getline(in, container);
			tmp[0] = container[0];
			tmp[1] = container[1];
			word = charToUni(tmp);
			iter = emitM.find(word);
			if (iter == emitM.end())
			{
				cout << "3.No the word in emitM: " << tmp << endl;
				continue;
			}
			for (j = 3; j < container.size(); j++)
				number[j-3] = container[j];
			number[j-3] = 0;
			iter->second[k] = atof(number);
		}
	}

	getline(in, container);
	len = atoi(container.c_str());
	for (i = 0; i < len; i++)	//read the data which will be inserted into the probM
	{
		size_t pos = 0;
		getline(in, container);
		for (j = 0, k = 0; j < container.size(); j++)
		{
			if (container[j] == ' ')
			{
				number[k] = 0;
				probM[i][pos++] = atof(number);
				k = 0;
			}
			else
				number[k++] = container[j];
		}
	}

	getline(in, container);
	getline(in, container);
	i = 0;
	for (j = 0, k = 0; j < container.size(); j++)	// read the data which will be inserted into the initM martrix
	{
		if (container[j] == ' ')
		{
			number[k] = 0;
			initM[i++] = atof(number);
			k = 0;
		}
		else
			number[k++] = container[j];
	}


	in.close();
}

void seg::readSymbol()
{
	ifstream in("RESULT/symbol.txt");
	Unicode word;

	char tmp[3], number[9];
	tmp[2] = 0;
	string container;
	for (size_t i = 0; i < 43; i++)
	{
		getline(in, container);

		number[0] = container[0];	//get the length
		number[1] = '\0';

		tmp[0] = container[2];		//get the first word
		tmp[1] = container[3];
		word = charToUni(tmp);

		symbol.insert(unordered_map<Unicode, size_t>::value_type(word, atoi(number)));
	}
	in.close();
	return;
}

size_t seg::findSymbol(Unicode word)
{
	unordered_map<Unicode, size_t>::iterator iter = symbol.find(word);
	if (iter != symbol.end())
		return iter->second;
	return 0;
}

void seg::showInitM()
{
	cout << "The Init matrix is: " << endl;
	for (size_t i = 0; i < 4; i++)
		cout << initM[i] << " ";
	cout << endl << endl;
	return;
}

void seg::showProbM()
{
	cout << "The Prob matrix is: " << endl;
	size_t i, j;
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
			cout << probM[i][j]<<" ";
		cout << endl;
	}
	cout << endl;
	return;
}

void seg::showEmitM()
{
	cout << "The Emit matrix is: " << emitM.size() << endl;

	size_t i, counter = 0;

	char tmp[3];
	tmp[2] = 0;
	unordered_map<Unicode, vector<double>>::iterator iter;
	iter = emitM.begin();
	for (; iter != emitM.end(); iter++)
	{
		UniToChar(iter->first, tmp);
		cout << tmp << ": ";
		for (i = 0; i < 4; i++)
			cout << iter->second[i] << " ";
		cout << endl;
		counter++;
		if (counter > 9)
			break;
	}
	cout << endl;
	return;
}

void seg::showState()
{
	for (size_t i = 0; i < 4; i++)
		cout << initState[i];
	cout << endl;
	return;
}

void seg::showWords(vector<Unicode>& words)
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