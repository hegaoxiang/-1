#include<vector>
#include <unordered_map>
#include <iostream>
#include <unordered_set>
#include <cassert>

using namespace std;
using GramaType = char;

int numNonTerminal;
int numTerminal;
int numProduction;
bool existNull;
char nullChar;
const char endChar = '#';

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

int main()
{
#pragma region NonTerminalsInit
	cout << "非终结符数目 : ";
	cin >> numNonTerminal;
	cout << "输入非终结符：";
	for (int i = 0; i < numNonTerminal; i++)
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

		terminals.insert(c);
		terVec.push_back(c);
	}
	if (existNull)
	{
		assert(terVec.size() >= 1);
		nullChar = terVec[0];
	}
#pragma endregion TerminalsInit

#pragma region ProductionsInit

	cout << "产生式数目：";
	cin >> numProduction;
	productions.resize(numProduction);
	
	char c;
	int num;
	for (size_t i = 0; i < numProduction; i++)
	{
		cout << "输入产生式" << i << ": ";
		cin >> c >> num;
		productions[i].lNonTerminal = c;
		while (num--)
		{
			char temp;
			cin >> temp;

			auto IsTer = [temp](const unordered_set<char>& ter, const unordered_set<char>& nonTer) {
				auto&& iter = ter.find(temp);

				if (iter != ter.end())
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
			if (isTer && existNull && temp == nullChar)
			{
				isNull = true;
			}
			productions[i].rSentence.push_back({ temp, isTer,isNull });
		}
	}
#pragma endregion ProductionsInit

#pragma region First Set
	cout << "---------------开始构建First集合----------------\n";
	{
		// PreInit First Set
		for (size_t i = 0; i < numTerminal; i++)
		{
			firstSets[terVec[i]].insert(terVec[i]);
		}
		// First Set construct
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

			
				}
				// 如果是非空终结符直接加入
				else if (firstChar.isTer)
				{
					firstSets[currentNonTer].insert(firstChar.c);

				}
				// 否则加入若S->ABCdE,若A,B,C均还有epsilon,则依次加入F(A),F(B),F(C),F(d)然后结束，注意此时加入的时候需要去除掉epsilon.
				// 若直到最后一个字符的First集合也存在epsilon，则讲epsilon加入进来
				else
				{
					for (size_t i = 0; i < sentence.size(); i++)
					{
						auto nextCharSet = firstSets[sentence[i].c];

						bool eNull = false;
						if (ExistNullInSet(nextCharSet))
						{
							nextCharSet.erase(nullChar);
							eNull = true;
						}

						firstSets[currentNonTer].merge(nextCharSet);

						if (eNull)
						{
							if (i == sentence.size() - 1)
							{
								firstSets[currentNonTer].insert(nullChar);
							}
						}
						else
							break;
					}
				}

				if (bfSize != firstSets[currentNonTer].size())
				{
					isOver = false;
				}

			}

		}
	}
	cout << "---------------构建First集合完毕----------------\n";
#pragma endregion First Set

#pragma region Follow Set
	cout << "---------------开始构建Follow集合----------------\n";
	{
		// preinitialize follow set
		auto& startChar = nonTerVec[0];
		followSets[startChar].insert(endChar);

		// follow set construct
		bool isOver = false;
		while (isOver = !isOver)
		{
			for (size_t i = 0; i < numProduction; i++)
			{
				auto& prod = productions[i];
				auto& sentence = prod.rSentence;
				auto& currentNonTer = prod.lNonTerminal;
				

				// S->zAc,若把First(c)中除空符以外的所有字符加入到Follow(A)中去，
				// 顺序循环遍历至n-1，若当前字符为NonTer，讲其后字符的First集合内除空字符加入
				for (size_t i = 0; i < sentence.size() - 1; i++)
				{
					auto& cC = sentence[i];
					auto& cFollowSet = followSets[cC.c];
					auto size = cFollowSet.size();

					auto nCFirstSet = firstSets[sentence[i + 1].c];
					if (ExistNullInSet(nCFirstSet))
					{
						nCFirstSet.erase(nullChar);
					}

					if (!cC.isTer)
					{
						cFollowSet.merge(nCFirstSet);
						if (size != cFollowSet.size())
						{
							isOver = false;
						}
					}
				}

				// S->zA或者S->zAc且First(c)中存在epsilon，则将Follow(S)加入至Follow(A)中
				// 倒序循环，当前元素为x，则将Follow(S)加入至Follow(x)，
				// 若x的First(x)集合内不包括epsilon,跳出，反之继续
				for (int i = sentence.size() - 1; i >= 0; i--)
				{
					auto& cC = sentence[i];
					auto& cFollowSet = followSets[cC.c];
					auto size = cFollowSet.size();
					auto cNTFS = followSets[currentNonTer];

					cFollowSet.merge(cNTFS);

					if (size != cFollowSet.size())
					{
						isOver = false;
					}
					if (!ExistNullInSet(firstSets[cC.c]))
					{
						break;
					}
				}
			}
		}
	}
	cout << "---------------构建Follow集合完毕----------------\n";
#pragma endregion Follow Set

	// 打印
	for (auto& nT:nonTerVec)
	{
		cout << "First( " << nT << " ) = { ";
		for (auto c : firstSets[nT])
			cout << c << " ";
		cout << "}\n";
	}

	for (auto& t : nonTerVec)
	{
		cout << "Follow( " << t << " ) = { ";
		for (auto c : followSets[t])
			cout << c << " ";
		cout << "}\n";
	}

	return 0;
}