#include "huffman.h"
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
using namespace std;
struct Node
{
	char ch;
	int parent;
	int zero;
	int one;
	bool branch;
};
int huffman(char * Filename)
{

	int weight[0x100];
	for (auto &i : weight)
		i = 0;
	{
		ifstream f(Filename);
		while (!f.eof())
		{
			unsigned char ch;
			f.read((char *)&ch, sizeof(ch));
			++weight[ch];
		}
	}

	multimap<int, int> sortedWeight;
	vector<Node> tree;
	map<char, int> charMap;
	for (size_t i = 0; i < 0x100; ++i)
	{
		if (weight[i] > 0)
		{
			tree.push_back(Node{ (char)i,-1,-1,-1,false });
			charMap[i] = tree.size() - 1;
			sortedWeight.insert(make_pair(weight[i], tree
				.size() - 1));
		}
	}
	while (sortedWeight.size() > 1)
	{
		int w0 = begin(sortedWeight)->first;
		int n0 = begin(sortedWeight)->second;
		sortedWeight.erase(begin(sortedWeight));
		int w1 = begin(sortedWeight)->first;
		int n1 = begin(sortedWeight)->second;
		sortedWeight.erase(begin(sortedWeight));
		tree.push_back(Node{ '\0',-1,n0,n1,false });
		tree[n0].parent = tree.size() - 1;
		tree[n0].branch = false;
		tree[n1].parent = tree.size() - 1;
		tree[n1].branch = true;
		sortedWeight.insert(make_pair(w0 + w1, tree.size() - 1));
	}
	vector<bool> data;
	
	{
	ifstream f(Filename);
	while (!f.eof())
	{
		unsigned char ch;
		f.read((char *)&ch, sizeof(ch));
		vector<bool> compressedChar;
		auto n = tree[charMap[ch]];
		
		while (n.parent != -1)
		{
			compressedChar.push_back(n.branch);
			n = tree[n.parent];
		}
		data.insert(end(data), compressedChar.rbegin(), compressedChar.rend());
	}
}
	int len = strlen(Filename);
	memset(Filename + strlen(Filename), 'h' , 1);
	*(Filename + len + 1) = '\0';
	ofstream f(Filename);
	int treeSize = tree.size();
	f.write((char*)&treeSize, sizeof(treeSize));
	for (auto i : tree)
		f.write((char*)&i, sizeof(i));
	for (size_t i = 0; i <= data.size() / 8; ++i)
	{
		unsigned char ch = 0;
		for (int j = 0; j < 8; ++j)
			if ((i * 8 + j) >= data.size()) break;
			else
			if (data[i * 8 + j])
				ch |= (1 << j);
		f.write((char*)&ch, sizeof(ch));
	}
	
	return 0;
}