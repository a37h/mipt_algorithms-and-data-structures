// ФИВТ | ООП на C++ | 2 семестр, 1 модуль, задача B
// https://contest.yandex.ru/contest/3970/problems/B/
// ............Количество различных путей............
// Дан невзвешенный неориентированный граф.В графе может быть несколько кратчайших путей между какими - то 
// вершинами.Найдите количество различных кратчайших путей между заданными вершинами.Требуемая сложность 
// O(V + E).Ввод: v:кол - во вершин(макс. 50000), n : кол - во ребер(макс. 200000), 
// n пар реберных вершин, пара вершин v, w для запроса.Вывод : количество кратчайших путей от v к w
// ............ Формат ввода ............
// ............ Формат вывода ............

#include <vector>
#include <queue>
#include <iostream>

struct bfsVerts {
	// Если цвет 0 то выполняем %actions% и вызываем bfs
	// Если цвет 1 то выполняем %actions% но не вызываем bfs (чтобы не дублироваться)
	char colour = 0;
	// Храним расстояние от начального узла
	int distance = 0;
	// Храним количество уникальных путей в вершину
	int uniqWays = 0;
	
	int parent = -1;

	int shortestDistance = 0;
};

class CUnorientedListGraph {

public:

	explicit CUnorientedListGraph(int input) :
		vertCount(input),
		verts(input),
		bfsNeeds(input)
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

	void bfs(int inputVertFROM, int inputVertTO)
	{
		// Очередь для реализации обхода в ширину, сразу закидываем туда начальную вершину
		std::queue<int> bfsQueue;
		bfsQueue.push(inputVertFROM);

		// Вектор для вызова функции GetNearVerts
		std::vector<int> tempVertsVector;

		// Длина минимального пути
		int minWayLen = -1;

		// Кол-во уникальных путей в начальную вершину возьмём за единицу
		bfsNeeds[inputVertFROM].uniqWays = 1;

		while ((bfsQueue.empty() == 0))
		{
			// Вынимаем передний элемент из очереди
			int temporaryVert = bfsQueue.front();
			bfsQueue.pop();

			// Получаем список смежных с ним вершин
			GetNearVerts(temporaryVert, tempVertsVector);

			for (unsigned int i = 0; i < tempVertsVector.size(); i++)
			{
				// Обрабатываем только НЕ родительские вершины
				if (tempVertsVector[i] != bfsNeeds[temporaryVert].parent)
				{
					// Если вершина еще не помечена то кидаем её в очередь на bfs
					if (bfsNeeds[tempVertsVector[i]].colour == 0)
					{
						bfsQueue.push(tempVertsVector[i]);
						bfsNeeds[tempVertsVector[i]].parent = temporaryVert;
						// Обновим ей расстояние до неё же (расстояние от родителя + 1)
						bfsNeeds[tempVertsVector[i]].distance = bfsNeeds[temporaryVert].distance + 1;
						// Отметим её (и не будем вызывать от неё bfs если она уже окрашена)
						bfsNeeds[tempVertsVector[i]].colour = 1;
					}
					// Проверим, пришли ли в конечную вершину, если да, то возьмем эталонной длиной minWayLen
					if ((tempVertsVector[i] == inputVertTO) && (minWayLen == -1))
						minWayLen = bfsNeeds[inputVertTO].distance;
					// Если длина уже больше эталонной (минимальной) то завершаем bfs
					if ((minWayLen != -1)&&(bfsNeeds[tempVertsVector[i]].distance > minWayLen))
						break;
					// Количество уникальных путей = кол-во уникальных путей в данный момент + кол-во уникальных путей от данного родителя
					if (bfsNeeds[temporaryVert].distance==bfsNeeds[tempVertsVector[i]].distance-1)
					bfsNeeds[tempVertsVector[i]].uniqWays += bfsNeeds[temporaryVert].uniqWays;
				}
			}
		}
		std::cout << bfsNeeds[inputVertTO].uniqWays;
		return;
	}

private:

	// Кол-во вершин
	int vertCount;

	// Хранение списков смежности
	std::vector<std::vector<int>> verts;

	// Current BFS session colours of the vertices
	std::vector<bfsVerts> bfsNeeds;

};

int main(void)
{
	int V, N;
	std::cin >> V;
	std::cin >> N;

	CUnorientedListGraph abc(V);

	for (int i = 0; i < N; i++)
	{
		int temp1, temp2;
		std::cin >> temp1;
		std::cin >> temp2;

		abc.AddEdge(temp1, temp2);
	}
	
	int from, to;
	std::cin >> from;
	std::cin >> to;

	abc.bfs(from,to);
	return 0;
}