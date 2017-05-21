#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>


template <typename WeightType>
class CMatrixGraph {
public:
	// constructor of empty Graph with number of vertices
	explicit CMatrixGraph(unsigned int inputVertsCount);
	// constructor of empty Graph without any parameters
	explicit CMatrixGraph();
	// method for initializing empty graph
	void initializeGraph(unsigned int inputVertsCount);
	// adding (or reseting) an edge from 'inputVFrom' into 'inputVTo' with 'inputEWeight' weight of the edge between them
	void SetEdge(int inputVFrom, int inputVTo, WeightType inputEWeight);
	// get vertices count
	unsigned int GetVertsCount(void) const;
	// get edge weight
	WeightType GetEdgeWeight(int inputVFrom, int inputVTo) const;
private:
	// amount of vertices
	int verticesCount = 0;
	// edges between vertices
	std::vector<std::vector<WeightType>> edges;
};

template <typename WeightType>
CMatrixGraph<WeightType>::CMatrixGraph(unsigned int inputVertsCount) :
	verticesCount(inputVertsCount),
	edges(inputVertsCount)
{
	for (int i = 0; i < verticesCount; i++)
	{
		edges[i].resize(verticesCount);
	}
}

template <typename WeightType>
CMatrixGraph<WeightType>::CMatrixGraph() :
	verticesCount(0),
	edges(0)
{
	for (int i = 0; i < verticesCount; i++)
	{
		edges[i].resize(verticesCount);
	}
}

template <typename WeightType>
void CMatrixGraph<WeightType>::initializeGraph(unsigned int inputVertsCount)
{
	verticesCount = inputVertsCount;
	edges.resize(inputVertsCount);
	for (int i = 0; i < verticesCount; i++)
	{
		edges[i].resize(verticesCount);
	}
}

template <typename WeightType>
WeightType CMatrixGraph<WeightType>::GetEdgeWeight(int inputVFrom, int inputVTo) const
{
	return edges[inputVFrom][inputVTo];
}

template <typename WeightType>
unsigned int CMatrixGraph<WeightType>::GetVertsCount(void) const
{
	return verticesCount;
}

template <typename WeightType>
void CMatrixGraph<WeightType>::SetEdge(int inputVFrom, int inputVTo, WeightType inputEWeight)
{
	edges[inputVFrom][inputVTo] = inputEWeight;
}

///

template <typename WeightType>
void Input(CMatrixGraph<WeightType>* Graph)
{
	freopen("floyd.in", "r", stdin);
	int N = 0;
	std::cin >> N;
	Graph->initializeGraph(N);
	for (unsigned int i = 0; i < Graph->GetVertsCount(); i++)
	{
		for (unsigned int j = 0; j < Graph->GetVertsCount(); j++)
		{
			WeightType temp;
			std::cin >> temp;

			Graph->SetEdge(i, j, temp);
		}
	}
	fclose(stdin);
}

template <typename WeightType>
void FloydAlgorithm(CMatrixGraph<WeightType>* Graph)
{
	for (unsigned int between = 0; between < Graph->GetVertsCount(); ++between) {
		for (unsigned int first = 0; first < Graph->GetVertsCount(); ++first) {
			for (unsigned int second = 0; second < Graph->GetVertsCount(); ++second) {
				Graph->SetEdge(first, second, std::min(Graph->GetEdgeWeight(first, second), Graph->GetEdgeWeight(first, between) + Graph->GetEdgeWeight(between, second)));
			}
		}
	}
}

template <typename WeightType>
void ShowGraphMatrix(CMatrixGraph<WeightType>* Graph)
{
	freopen("floyd.out", "w", stdout);
	for (unsigned int i = 0; i < Graph->GetVertsCount(); ++i)
	{
		for (unsigned int j = 0; j < Graph->GetVertsCount(); ++j)
			std::cout << Graph->GetEdgeWeight(i,j) << " ";
		std::cout << std::endl;
	}
	fclose(stdout);
}

template <typename WeightType>
void Solution()
{
	// let's create empty graph
	CMatrixGraph<WeightType> Graph;
	// let's get all of the input
	Input<WeightType>(&Graph);
	// let's do the algorithm
	FloydAlgorithm<WeightType>(&Graph);
	// let's show the result
	ShowGraphMatrix<WeightType>(&Graph);
}

int main(void)
{
	Solution<int>();
	return 0;
}

// Вся программа написана на шаблонах, а в данном случае в консте достаточно всего лишь int
// Так что используем int как инициализацию шаблона