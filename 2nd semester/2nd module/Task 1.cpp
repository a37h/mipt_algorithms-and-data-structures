// author: Jukovsky Anatoly
// 
// MIPT | DIHT | OOP on C++ | 2 semester | 2 module | task #1
//  ***** Задача 1. «Города» (3 балла)
// Требуется отыскать самый выгодный маршрут между городами.Требуемое время работы O((N + M)logN), 
// где N - количество городов, M - известных дорог между ними.
//  ***** Формат входных данных.
// Первая строка содержит число N – количество городов.
// Вторая строка содержит число M - количество дорог.
// Каждая следующая строка содержит описание дороги(откуда, куда, время в пути).
// Последняя строка содержит маршрут(откуда и куда нужно доехать).
//  ***** Формат выходных данных.
// Вывести длину самого выгодного маршрута.

#include <iostream>
#include <vector>
#include <limits>

template <typename WeightType>
class CMatrixGraph {
public:
	// constructor of Graph with 'inputVertsCount' vertices in it
	explicit CMatrixGraph(unsigned int inputVertsCount);
	// adding an edge from 'inputVFrom' into 'inputVTo' with 'inputEWeight' weight of the edge between them
	void AddEdge(int inputVFrom, int inputVTo, WeightType inputEWeight);
	// put into vector 'outputVerts' vertices, which are connected with vertice 'inputV'
	void GetConnectedVertices(int inputV, std::vector<int>& outputVerts) const;
	// get vertices count
	unsigned int GetVertsCount(void) const;
	// get edge weight
	WeightType GetEdgeWeight(int inputVFrom, int inputVTo) const;
private:
	// amount of vertices
	int verticesCount;
	// edges between vertices
	std::vector<std::vector<WeightType>> edges;
};

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
void CMatrixGraph<WeightType>::AddEdge(int inputVFrom, int inputVTo, WeightType inputEWeight)
{
	edges[inputVFrom][inputVTo] = inputEWeight;
	edges[inputVTo][inputVFrom] = inputEWeight;
}

template <typename WeightType>
void CMatrixGraph<WeightType>::GetConnectedVertices(int vertex, std::vector<int>& vertices) const
{
	if (vertices.empty() != 1)
		vertices.clear();
	for (int i = 0; i < verticesCount; i++)
		if (edges[vertex][i] != 0) vertices.push_back(i);
}

// returns shortest path from inputVStart to inputVEnd
template <typename WeightType>
WeightType DijkstrasAlgorithm(int inputVStart, int inputVEnd, CMatrixGraph<int>* Graph)
{
	// ***************** PREPARING PART *******************

	// getting WeightType max value which we are going to assume as infinity
	WeightType WeightTypeMaxValue = std::numeric_limits<WeightType>::max();

	// getting vertices count
	unsigned int verticesCount = Graph->GetVertsCount();

	// vector with distances to verts
	std::vector<WeightType> distanceToVerts;
	distanceToVerts.resize(verticesCount);
	// vector for keeping information about used verts
	std::vector<int> usedVerts;
	usedVerts.resize(verticesCount);
	// temporary vector for getting next vertices
	std::vector<int> tempNextVertsNeeds;

	// shortest path to the starting vertice is 0
	for (unsigned int i = 0; i < verticesCount; ++i)
	{
		distanceToVerts[i] = WeightTypeMaxValue;
	}
	distanceToVerts[inputVStart] = 0;

	// ***************** ALGORITHM PART *******************

	for (unsigned int i = 0; i < verticesCount; ++i)
	{
		// ***** preparing part *****

		// just a temporary vertice
		int tempV; // doesn't matter what's in here
		// infinity is equal to 32000
		WeightType tempVDist = WeightTypeMaxValue;

		// ***** finding next vertice to work with *****

		// get unused vert with shortest path to it
		for (unsigned int f = 0; f < verticesCount; ++f) 
		{
			if ((tempVDist != WeightTypeMaxValue) && (tempVDist < distanceToVerts[f]) || usedVerts[f] == 1)
			{
				continue;
			}
			tempV = f;
			tempVDist = distanceToVerts[tempV];
		}

		// ***** getting shortest path to it *****

		Graph->GetConnectedVertices(tempV, tempNextVertsNeeds);
		// lookup every next vertice from tempV
		for (unsigned int j = 0; j < tempNextVertsNeeds.size(); ++j)
		{
			int tempVv = tempNextVertsNeeds[j];
			WeightType tempVvWeight = Graph->GetEdgeWeight(tempV, tempNextVertsNeeds[j]);

			if ((distanceToVerts[tempV] + tempVvWeight < distanceToVerts[tempVv]) || (distanceToVerts[tempVv] == WeightTypeMaxValue))
			{
				distanceToVerts[tempVv] = distanceToVerts[tempV] + tempVvWeight;
			}
		}

		// ***** ending *****

		// mark vertice 'tempV' as used as long as shortest path to it found
		usedVerts[tempV] = 1;
	}

	return distanceToVerts[inputVEnd];
}

template <typename WeightType>
void CompleteTask(void)
{
	// N - amount of vertices, M - amount of edges
	int N = 0, M = 0;
	std::cin >> N >> M;

	// Graph - our graph (captain obvious)
	CMatrixGraph<WeightType> Graph(N);
	for (int i = 0; i < M; ++i)
	{
		int temp1 = 0, temp2 = 0, temp3 = 0;
		std::cin >> temp1 >> temp2 >> temp3;
		Graph.AddEdge(temp1, temp2, temp3);
	}

	// reading input (2 vertices which we want to use for getting distance between them)
	int inputVertFrom = 0, inputVertTo = 0;
	std::cin >> inputVertFrom >> inputVertTo;

	// starting DijkstrasAlgorithm
	int result = 0;
	result = DijkstrasAlgorithm<WeightType>(inputVertFrom, inputVertTo, &Graph);

	// end
	std::cout << result;
	return;
}

int main(void)
{
	CompleteTask<int>();
	return 0;
}

// Вся программа написана на шаблонах, а в данном случае в консте достаточно всего лишь int
// Так что используем int как инициализацию шаблона