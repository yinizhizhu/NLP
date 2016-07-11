#include "../h/Segment.h"

ofstream segOut("RESULT/segOut.txt");

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
	return;
}

void seg::control()
{
	cout << "In control" << endl;
	ifstream in("devTest.txt");

	size_t i, len;
	string container;

	char tmp[3];
	tmp[2] = 0;

	vector<Unicode> words;
	vector<char> state;

	getline(in, container);
	len = container.size();
	for (i = 0; i < len; i += 2)
	{
		tmp[0] = container[i];
		tmp[1] = container[i + 1];
		words.push_back(charToUni(tmp));
	}

	divider(words, state);
	for (i = 0; i < len; i++)
		cout << state[i];
	cout << endl;
	cout << "Out control" << endl;

	in.close();
}

void seg::divider(vector<Unicode>& words, vector<char>& state)
{
	char tmp[3];
	tmp[2] = 0;

	size_t bestIndex;
	size_t i, j, k, len = words.size();
	unordered_map<Unicode, vector<double>>::iterator iter;

	vector<segNode> all;
	all.resize(len);
	state.resize(len);

	vector<double> prob;
	prob.resize(4);

	for (i = 0; i < 4; i++)
	{
		iter = emitM.find(words[0]);
		if (iter == emitM.end())
		{
			UniToChar(words[0], tmp);
			cout << "Cannot find the word: " << tmp << endl;
			return;
		}
		all[0].bestPro[i] = initM[i] * iter->second[i];
		cout << all[0].bestPro[i] <<" ";//for test
	}
	cout << endl;//for test

	for (i = 1; i < len; i++)
	{
		iter = emitM.find(words[i]);
		if (iter == emitM.end())
		{
			UniToChar(words[i], tmp);
			cout << "Cannot find the word: " << tmp << endl;
			return;
		}
		for (j = 0; j < 4; j++)	//the next word
		{
			for (k = 0; k < 4; k++)	//the pre word
			{
				prob[k] = all[i - 1].bestPro[k] * probM[k][j] * iter->second[j];
			}

			bestIndex = 0;			//get the best one( Dynamic )
			for (k = 1; k < 4; k++)
			{
				if (prob[k] < prob[bestIndex])
					bestIndex = k;
			}
			all[i].bestSel[j] = bestIndex;
			all[i].bestPro[j] = prob[bestIndex];
		}
	}

	bestIndex = 0;
	for (i = 1; i < 4; i++)
	{
		if (all[len - 1].bestPro[i] < all[len - 1].bestPro[bestIndex])
			bestIndex = i;
	}
	state[len - 1] = initState[bestIndex];
	for (i = len - 2; i >= 1; i--)
	{
		bestIndex = all[i + 1].bestSel[bestIndex];
		state[i] = initState[bestIndex];
	}
	cout << "Out divider" << endl;
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
			newOne.push_back(-1e-34);
		emitM.insert(unordered_map<Unicode, vector<double>>::value_type(word, newOne));
	}

	for (k = 0; k < 4; k++)		//read the data which will be insert into the emitM Matrix
	{
		getline(in, container);
		len = atoi(container.c_str());
		for (i = 0; i < len; i++)
		{	//read the state:
			/*
			0 - B
			1 - M
			2 - E
			3 - S
			*/
			getline(in, container);
			tmp[0] = container[0];
			tmp[1] = container[1];
			word = charToUni(tmp);
			iter = emitM.find(word);
			if (iter == emitM.end())
			{
				cout << "No the word in emitM: " << tmp << endl;
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