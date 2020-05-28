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
	cout << "���ս����Ŀ : ";
	cin >> numNonTerminal;
	cout << "������ս����";
	for (int i = 0;i < numNonTerminal;i ++)
	{
		char c;
		cin >> c;
		nonTerminals.insert(c);
		nonTerVec.push_back(c);
	}
#pragma endregion NonTerminalsInit

	#pragma region TerminalsInit
    cout << "�ս����Ŀ : ";
	cin >> numTerminal;
	cout << "�Ƿ���ڿշ������ڻظ�1�����ҵȻὫ�շ����ڵ�һλ���룬����ظ�0��";
	cin >> existNull;
	cout << "������ս����";
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

	cout << "����ʽ��Ŀ��";
	cin >> numProduction;
	productions.resize(numProduction);
	cout << "�������ʽ��";
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
						cout << "����δ֪�ַ�";
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
	
				// ����ǿ��ս��ֱ�Ӽ���
				if (firstChar.isNull)
				{
					assert(sentence.size() == 1);
	
					firstSets[currentNonTer].insert(firstChar.c);
	
					isOver = false;
				}
				// ����Ƿǿ��ս��ֱ�Ӽ���
				else if (firstChar.isTer)
				{
					firstSets[currentNonTer].insert(firstChar.c);
	
					isOver = false;
				}
				// ���������S->ABCdE,��A,B,C������epsilon,�����μ���F(A),F(B),F(C),F(d)Ȼ�������ע���ʱ�����ʱ����Ҫȥ����epsilon.
				// ��ֱ�����һ���ַ���First����Ҳ����epsilon����epsilon�������
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