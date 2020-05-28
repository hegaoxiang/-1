#include<vector>
#include <unordered_map>
#include <iostream>
#include <unordered_set>
#include <cassert>

using namespace std;
#define _In_
using GramaType = char;
int numNonTerminal;
int numTerminal;
int numProduction;
bool existNull;
char nullChar;
struct Production 
{
	struct CHAR
	{
		char c;
		bool isTer;
		bool isNull;
	};
	char lNonTerminal;
	vector<CHAR>	rSentence;
};
using ChatSet = unordered_set<char>;
ChatSet nonTerminals;
vector<char>nonTerVec;
ChatSet terminals;
vector<char>terVec;
vector<Production> productions;
unordered_map<char, ChatSet> firstSets;
unordered_map<char, ChatSet> followSets;

bool ExistNullInSet(const ChatSet& set)
{
	assert(existNull);
	auto&& iter = set.find(nullChar);
	return iter != set.end();
}
void Init()
{
	
	#pragma region NonTerminalsInit
	cout << "非终结符数目 : ";
	cin >> numNonTerminal;
	cout << "输入非终结符：";
	for (int i = 0;i < numNonTerminal;i ++)
	{
		char c;
		cin >> c;
		nonTerminals.insert(c);
		nonTerVec.push_back(c);
	}
#pragma endregion NonTerminalsInit

	#pragma region TerminalsInit
    cout << "终结符数目 : ";
	cin >> numTerminal;
	cout << "是否存在空符，存在回复1，并且等会将空符放在第一位输入，否则回复0：";
	cin >> existNull;
	cout << "输入非终结符：";
	for (int i = 0; i < numTerminal; i++)
	{
		char c;
		cin >> c;

		if (existNull)
		{
			nullChar = c;
		}
		
		terminals.insert(c);
		terVec.push_back(c);
	}
#pragma endregion TerminalsInit

	#pragma region ProductionsInit

	cout << "产生式数目：";
	cin >> numProduction;
	productions.resize(numProduction);
	cout << "输入产生式：";
	char c;
	int num;
	for (size_t i = 0; i < numProduction; i++)
	{
		cin >> c >> num;
		productions[i].lNonTerminal = c;
		while (num--)
		{
			char temp;
			cin >> temp;

			auto IsTer = [temp] (const unordered_set<char>& ter, const unordered_set<char>& nonTer){
				auto&& iter = ter.find(temp);

				if (iter!= ter.end())
				{
					return true;
				}
				else
				{
					auto&& iter = nonTer.find(temp);

					if (iter != nonTer.end())
					{
						return false;
					}
					else
					{
						cout << "出现未知字符";
						exit(0);
					}
				}
			};

			bool isTer = IsTer(terminals, nonTerminals);
			bool isNull = false;
			if (isTer&&existNull&&temp == nullChar)
			{
				isNull = true;
			}
			productions[i].rSentence.push_back({ temp, isTer,isNull });
		}
	}
#pragma endregion ProductionsInit

	#pragma region First Set
	{
	// PreInit First Set
		for (size_t i = 0; i < numTerminal; i++)
		{
			firstSets[terVec[i]].insert(terVec[i]);
		}
		// First Set finish
		bool isOver = false;
		while (isOver = !isOver)
		{
			for (size_t i = 0; i < numProduction; i++)
			{
				auto& prod = productions[i];
				auto& sentence = prod.rSentence;
				auto& firstChar = prod.rSentence[0];
				auto& currentNonTer = prod.lNonTerminal;
	
				int bfSize = firstSets[currentNonTer].size();
	
				// 如果是空终结符直接加入
				if (firstChar.isNull)
				{
					assert(sentence.size() == 1);
	
					firstSets[currentNonTer].insert(firstChar.c);
	
					isOver = false;
				}
				// 如果是非空终结符直接加入
				else if (firstChar.isTer)
				{
					firstSets[currentNonTer].insert(firstChar.c);
	
					isOver = false;
				}
				// 否则加入若S->ABCdE,若A,B,C均还有epsilon,则依次加入F(A),F(B),F(C),F(d)然后结束，注意此时加入的时候需要去除掉epsilon.
				// 若直到最后一个字符的First集合也存在epsilon，则讲epsilon加入进来
				else
				{
					for (size_t i = 0; i < sentence.size(); i++)
					{
						auto nextCharSet = firstSets[firstChar.c];
	
						bool eNull = false;
						if (ExistNullInSet(nextCharSet))
						{
							nextCharSet.erase(nullChar);
							existNull = true;
						}
						
						firstSets[currentNonTer].merge(nextCharSet);
	
						if (eNull)
						{
							if (i == sentence.size() - 1)
							{
								firstSets[currentNonTer].insert(nullChar);
							}
							break;
						}
					}
				}
	
				if (bfSize!= firstSets[currentNonTer].size())
				{
					isOver = false;
				}
				
			}
	
		}
	}
#pragma endregion First Set

	{
		bool isOver = false;
		while (isOver = !isOver)
		{

		}
	}
}

int main()
{
	Init();

	return 0;
}