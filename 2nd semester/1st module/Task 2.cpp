// ФИВТ | ООП на C++ | 2 семестр, 1 модуль, задача A
// https://contest.yandex.ru/contest/3970/problems/A/
// ............Цикл минимальной длины............
//Дан невзвешенный неориентированный граф.Найдите цикл минимальной длины.
//Требуемая сложность O(VE), +1 балл за O(V + E).
// ............ Формат ввода ............
//В первой строке вводятся два натуральных числа N и M, не превосходящих 10000. Далее идут M строк по 
//2 числа(от 0 до N - 1) - индексы вершин между которыми есть ребро.
// ............ Формат вывода ............
//Требуется вывести 1 целое число равное длине минимального цикла.Если цикла нет, то вывести - 1.

#include <vector>
#include <queue>
#include <iostream>

struct bfsVerts {
	char colour = 0;
	int parent = -1;
	int distance = 0;
};

class CUnorientedListGraph {

public:

	explicit CUnorientedListGraph(int input) :
		vertCount(input),
		verts(input)	{ }

	explicit CUnorientedListGraph(CUnorientedListGraph &input) :
		vertCount(input.vertCount),
		verts(input.verts)
	{ }

	void AddEdge(int input1, int input2)
	{ 
		verts[input1].push_back(input2);
		verts[input2].push_back(input1);
	}

	void GetNearVerts(int inputVert, std::vector<int>& inputVector) const
	{
		if (inputVector.empty() != 1) { inputVector.clear(); }
		inputVector = verts[inputVert];
	}

	void BFS()
	{
		bfsNeeds.resize(vertCount);

		for (int i = 0; i < vertCount; i++)
		{
			bfs(i);
			if (minCycleDist == 3) break;
		}

		std::cout << minCycleDist;
		return;
	}

private:

	int vertCount;

	std::vector<std::vector<int>> verts;

	// Current BFS session colours of the vertices
	std::vector<bfsVerts> bfsNeeds;

	int minCycleDist = -1;

	void bfs(int inputVert)
	{
		std::queue<int> bfsQueue;
		bfsQueue.push(inputVert);

		bfsNeeds.clear();
		bfsNeeds.resize(vertCount);

		char foundCycleYet = 0;
		std::vector<int> vertices;

		while ((bfsQueue.empty() == 0))
		{
			int temporaryVert = bfsQueue.front();
			bfsQueue.pop();

			GetNearVerts(temporaryVert, vertices);

			for (int i = 0; i < vertices.size(); i++)
			{
				if (vertices[i] != bfsNeeds[temporaryVert].parent)
				{
					if (bfsNeeds[vertices[i]].colour == 1)
					{
						int foundCycleDist = bfsNeeds[vertices[i]].distance + bfsNeeds[temporaryVert].distance + 1;
						if ((minCycleDist > foundCycleDist) || (minCycleDist == -1)) minCycleDist = foundCycleDist;
						break;
						if (minCycleDist == 3) return;
					}
					bfsQueue.push(vertices[i]);
					bfsNeeds[vertices[i]].colour = 1;
					bfsNeeds[vertices[i]].parent = temporaryVert;
					bfsNeeds[vertices[i]].distance = bfsNeeds[bfsNeeds[vertices[i]].parent].distance + 1;
				}
			}
		}
	}
};



int main(void)
{
	int N, M;
	std::cin >> N;
	std::cin >> M;

	CUnorientedListGraph abc(N);

	for (int i = 0; i < M; i++)
	{
		int temp1, temp2;
		std::cin >> temp1;
		std::cin >> temp2;

		abc.AddEdge(temp1, temp2);
	}

	abc.BFS();
}