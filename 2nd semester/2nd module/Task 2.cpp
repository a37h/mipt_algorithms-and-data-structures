// author: Jukovsky Anatoly
// 
// MIPT | DIHT | OOP on C++ | 2 semester | 2 module | task #2
//  ***** Задача 2. «Trade Arbitrage» (3 балла)
// Необходимо написать торгового советника для поиска арбитража. 
// Определение
// Арбитраж - это торговля по цепочке различных валют в надежде заработать на
// небольших различиях в коэффициентах.Например, есть следующие курсы валют
// (на 03.05.2015) :
// GBP / USD : 0.67
// RUB / GBP : 78.66
// USD / RUB : 0.02
// Имея 1$ и совершив цикл USD->GBP->RUB->USD, получим 1.054$.Таким образом заработав 5.4%.
//  ***** Формат входных данных.
// Первая строка содержит число N – количество возможных валют (размер таблицы котировок)
// Далее следует построчное представление таблицы.Диагональные элементы(i, i) пропущены(подразумевается,
// что курс валюты к себе же 1.0). В элементе таблицы(i, j) содержится обменный курс i->j.
// Если обмен в данном направлении не производится, то - 1.
//  ***** Формат выходных данных.
// Выведите YES, если арбитраж есть, и NO, иначе.

#include <cfloat>
#include <iostream>
#include <vector>
#include <limits>

class MatrixGraph {
public:
	// empty constructor
	explicit MatrixGraph();
	// initalizing empty graph
	void initalizeGraph(int inputVertsCount);
	// adding or reseting edge
	virtual void SetEdge(int inputVFrom, int inputVTo, double inputWeight) ;
	// getting edge weight
	double GetEdgeWeight(int inputVFrom, int inputVTo);
	// getting verts count
	virtual int VerticesCount() const  { return verticesCount; }

private:
	int verticesCount;
	std::vector<std::vector<double>> edges;
};

double MatrixGraph::GetEdgeWeight(int inputVFrom, int inputVTo)
{
	return edges[inputVFrom][inputVTo];
}

MatrixGraph::MatrixGraph() :
	verticesCount(-1),
	edges(0)
{
}

void MatrixGraph::initalizeGraph(int inputVertsCount)
{
	verticesCount = inputVertsCount;
	edges.resize(inputVertsCount);
	for (int i = 0; i < verticesCount; ++i)
		edges[i].resize(inputVertsCount);
}

void MatrixGraph::SetEdge(int inputVFrom, int inputVTo, double inputWeight)
{
	edges[inputVFrom][inputVTo] = inputWeight;
}

//

void GetInput(MatrixGraph *Graph)
{
	int N = 0;
	std::cin >> N;

	// if there's just 
	if (N == 0 || N == 1) {
		std::cout << "NO";
		return;
	}

	// initializing graph with amount of verts
	Graph->initalizeGraph(N);

	// reading graph matrix
	for (int i = 0; i < N; ++i)
	{
		for (int j = 0; j < N; ++j)
		{
			if (i == j)
				Graph->SetEdge(i, j, 1);
			else
			{
				double temp;
				std::cin >> temp;
				if (temp == -1)
					continue;
				Graph->SetEdge(i, j, temp);
			}
		}
	}
}


void BellmanFordAlgorithm(MatrixGraph *Graph)
{
	// getting '-infinite' value
	double MinExchangeRate = std::numeric_limits<double>::min();
	int AmountOfCurrencys = Graph->VerticesCount();
	// vector for keeping information about arbitrage exchange rate while finding cycle
	std::vector<double> ArbitrageExcRate(0);
	
	for (int Currency = 0; Currency < AmountOfCurrencys; ++Currency)
	{
		// preparing ArbitrageExcRate vector
		ArbitrageExcRate.clear();
		ArbitrageExcRate.resize(AmountOfCurrencys);
		for (int i = 0; i < AmountOfCurrencys; ++i)
			ArbitrageExcRate[i] = MinExchangeRate;
		ArbitrageExcRate[Currency] = 1.0;
		
		for (int k = 0; k <= AmountOfCurrencys; ++k)
		{
			for(int i = 0; i < AmountOfCurrencys; ++i)
			{
				for (int j = 0; j < AmountOfCurrencys; ++j)
				{
					if (ArbitrageExcRate[j] < ArbitrageExcRate[i] * Graph->GetEdgeWeight(i, j))
					{
						// if we found needed cycle then we've done it
						if (k == AmountOfCurrencys)
						{
							std::cout << std::endl << "YES";
							return;
						}

						// rebalancing arbitrage exchange rate
						ArbitrageExcRate[j] = ArbitrageExcRate[i] * Graph->GetEdgeWeight(i, j);
					}
				}
			}
		}
	}

	// if we haven't found any cycles then we've got no needed arbitrage
	std::cout << std::endl << "NO";
	return;
}

void Solution(void)
{
	MatrixGraph	Graph;
	GetInput(&Graph);
	BellmanFordAlgorithm(&Graph);
	return;
}

int main() 
{
	Solution();
	return 0;
}

// Не знаю, нужно ли было делать эту задачу на шаблонах, но смысла я в этом не увидел
// Куда применить в данной задаче шаблонизацию - непонятно.