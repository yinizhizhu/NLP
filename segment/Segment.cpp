#include "../h/Segment.h"

#define GET

#ifdef GET
ofstream segOut("RESULT/segOut.txt");		//output the result of segment: each words per line
ofstream unknown("RESULT/unknown.txt");		//output the word which never appear in the training data
ofstream stateOut("RESULT/state.txt");		//output the word which never appear in the training data
#endif

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
	dict.build();
	dict.showContainer();
	dict.addDict();
	return;
}

seg::~seg()
{
#ifdef GET
	segOut.close();
	unknown.close();
#endif
	return;
}

void seg::readNum()
{
	ifstream in("number.txt");
	Unicode word;

	char tmp[3];
	tmp[2] = 0;
	string container;
	for (size_t i = 0; i < 16; i++)
	{
		getline(in, container);

		tmp[0] = container[0];		//get the first word
		tmp[1] = container[1];
		word = charToUni(tmp);

		if (i < 15)
			num.push_back(word);
		else
			special = word;
	}
	in.close();
	return;
}

bool seg::checkNum(Unicode tmp)
{
	Unicode res = tmp - num[0];
	if (res >= 0 && res < 10)
		return true;
	for (size_t i = 10; i < 15; i++)
		if (num[i] == tmp)
			return true;
	return false;
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
				cout << "3.None of the word in emitM: " << tmp << endl;
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
	for (size_t i = 0; i < 52; i++)
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

void seg::control()
{
	ifstream in("dev.txt");
	//ifstream in("devTest.txt");

	size_t i, len, lineLen;
	string container;

	char tmp[3];
	tmp[2] = 0;
	Unicode one;
	vector<char> state;
	vector<Unicode> words;

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
				if (checkNum(one))		//handle with the number, decimal, time
				{
					do{
						segOut << container[i] << container[i + 1];
						i += 2;
						if (i >= lineLen || !findSymbol(one))
							break;
						tmp[0] = container[i];
						tmp[1] = container[i + 1];
						one = charToUni(tmp);
					} while (checkNum(one));
					segOut << endl;
				}
				else if (one == special)	//handle with the specail symbol '-'
				{
					do{
						segOut << container[i] << container[i + 1];
						i += 2;
						if (i >= lineLen)
							break;
						tmp[0] = container[i];
						tmp[1] = container[i + 1];
						one = charToUni(tmp);
					} while (one == special);
					segOut << endl;
				}
				else
				{
#ifdef GET
					for (size_t j = 0; j < (len * 2) && (i + j) < lineLen; j++)
						segOut << container[i + j];
					segOut << endl;
#endif
					i += len * 2;
				}
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
	size_t i, j, len = words.size();
	if (len > 0)
	{
		showWords(words);	//for test
		stateOut << endl;

		dict.opWithDict(words, state);	//check in dict firstly and divide the sentence into pieces.

		for (i = 0; i < len;)
		{
			if (state[i] == 'N')
			{
				state[i] = 'S';			//to get the best score
				for (j = i + 1; j < len; j++)
				{
					if (state[j] != 'N')
						break;
					state[j] = 'S';		//to get the best score
				}
				//divider(words, i, j, state);
				i = j + 1;
			}
			else
				i++;
		}

		for (j = 0; j < state.size(); j++)	//for test
			stateOut << state[j];
		stateOut << endl;

		outPut(words, state);
	}
	return;
}

void seg::outPut(vector<Unicode>& words, vector<char>& state)
{
	char tmp[3];
	size_t j, len = words.size();
	UniToChar(words[0], tmp);
#ifdef GET
	segOut << tmp;
#endif
	if (state[0] == 'S')
	{
		//segOut << '/';
#ifdef GET
		segOut << endl;
#endif
	}
	for (j = 1; j < len; j++)
	{
		UniToChar(words[j], tmp);
#ifdef GET
		segOut << tmp;
#endif
		if (state[j] == 'E' || state[j] == 'S' || j == (len - 1))
		{
			//segOut << '/';
#ifdef GET
			segOut << endl;
#endif
		}
	}
	words.clear();
	state.clear();
	return;
}

void seg::divider(vector<Unicode>& words, size_t start, size_t tail, vector<char>& state)
{
	size_t len = tail - start;
	if (len == 0)
		return;
	else if (len == 1)
	{
		state[start] = 'S';
		return;
	}

	char tmp[3];
	tmp[2] = 0;

	size_t bestIndex;
	size_t i, j, k;
	vector<segNode> all;		//stor all the nodes of segment(words)
	all.resize(len);
	vector<double> prob;	//store the each 4 prob to select the best one
	prob.resize(4);

	unordered_map<Unicode, vector<double>>::iterator iter;
	iter = emitM.find(words[start]);
	if (iter == emitM.end())
	{
		UniToChar(words[start], tmp);
#ifdef GET
		unknown << tmp << endl;
#endif
		vector<double> newOne;
		for (j = 0; j < 4; j++)
			newOne.push_back(-15);
		emitM.insert(unordered_map<Unicode, vector<double>>::value_type(words[start], newOne));
		iter = emitM.find(words[start]);
	}
	for (j = 0; j < 4; j++)
	{
		if (initM[j] != -NOT)
			all[0].bestPro[j] = initM[j] * iter->second[j] / 10;
		else
			all[0].bestPro[j] = NOT;
		//cout << "<" << all[0].bestSel[i] << "> " << all[0].bestPro[i] << " &";//for test
	}
	//cout << endl;//for test

	for (i = start + 1; i < tail; i++)
	{
		iter = emitM.find(words[i]);
		if (iter == emitM.end())
		{
			UniToChar(words[i], tmp);
#ifdef GET
			unknown << tmp << endl;
#endif
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
				if (all[i - start - 1].bestPro[k] != NOT)
					prob[k] = all[i - start - 1].bestPro[k] * probM[k][j] * iter->second[j] / 10;
				else
					prob[k] = NOT;
			}

			bestIndex = 0;			//get the best one( Dynamic )
			for (k = 1; k < 4; k++)
			{
				if (prob[k] < prob[bestIndex])
					bestIndex = k;
			}
			all[i - start].bestSel[j] = bestIndex;
			all[i - start].bestPro[j] = prob[bestIndex];

			//cout << "<" << all[i].bestSel[j] << "> " << all[i].bestPro[j] << " &";	//for test
		}
		//cout << endl;//for test
	}


	//bestIndex = 0;	//E
	//for (i = 1; i < 4; i++)
	//	if (all[len - 1].bestPro[i] < all[len - 1].bestPro[bestIndex])	//S
	//		bestIndex = i;
	bestIndex = 2;	//E
	if (all[len - 1].bestPro[3] < all[len - 1].bestPro[bestIndex])	//S
		bestIndex = 3;

	state[tail - 1] = initState[bestIndex];
	for (i = len - 2;; i--)
	{
		bestIndex = all[i].bestSel[bestIndex];
		state[start + i] = initState[bestIndex];
		if (i == 0)
			break;
	}
	return;
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
		stateOut << tmp;
	}
	return;
}

void seg::showNum()
{
	char tmp[3];
	for (size_t i = 0; i < 12; i++)
	{
		UniToChar(num[i], tmp);
		cout << tmp;
		//cout << num[i] << endl;
	}
	cout << endl;

	UniToChar(special, tmp);	//show the specail symbol
	cout << tmp << endl;
	return;
}