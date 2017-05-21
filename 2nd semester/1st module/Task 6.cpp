// ФИВТ | ООП на C++ | 2 семестр, 1 модуль, задача E
// https://contest.yandex.ru/contest/3970/problems/E/
// ............Дополнение до сильносвязного............
// Дан ориентированный граф. Определите, какое минимальное
// число ребер необходимо добавить, чтобы граф стал сильносвязным.
// ............ Формат ввода ............
// В первой строке указывается число вершин графа V, во второй – 
// число ребер E, в последующих – E пар вершин, задающих ребра.
// ............ Формат вывода ............
// Минимальное число ребер k.

#include <vector>
#include <stack>
#include <iostream>

	struct dfsVerts {
	char colourOfDFS = 0;
	int parent = -1;

	int colourOfKosarajus = 0; // 0 - нет цвета
};

class MatrixGraph {
public:

	explicit MatrixGraph(int _verticesCount) :
		verticesCount(_verticesCount),
		edges(_verticesCount),
		dfsNeeds(_verticesCount)
	{
		for (int i = 0; i < verticesCount; i++)
		{
			edges[i].resize(verticesCount);
		}
	}

	void AddEdge(int from, int to)
	{
		edges[from][to] = 1;
	}

	void GetNextVertices(int vertex, std::vector<int>& vertices) const
	{
		if (vertices.empty() != 1)
			vertices.clear();
		for (int i = 0; i < verticesCount; i++)
			if (edges[vertex][i] == 1) vertices.push_back(i);
	}

	void GetPrevVertices(int vertex, std::vector<int>& vertices) const
	{
		if (vertices.empty() != 1)
			vertices.clear();
		for (int i = 0; i < verticesCount; i++)
			if (edges[i][vertex] == 1) vertices.push_back(i);
	}

	void dfsBuildKosarajusQueue(void) {
		for (int i = 0; i < verticesCount; i++)
		{
			if (dfsNeeds[i].colourOfDFS == 0)
				dfsBuildKosarajusQueueReqursive(i);
		}
	}

	void dfsBuildKosarajusQueueReqursive(int v) {
		dfsNeeds[v].colourOfDFS = 1;
		std::vector<int> tempVertsVector;
		GetNextVertices(v, tempVertsVector);
		for (unsigned i = 0; i<tempVertsVector.size(); ++i)
			if (dfsNeeds[tempVertsVector[i]].colourOfDFS == 0)
				dfsBuildKosarajusQueueReqursive(tempVertsVector[i]);
		KosarajusQueue.push_back(v);
	}

	// Обходим граф в глубину начиная с вершин которые лежат под наибольишми номерами в KosarajusQueue
	// Выделим компоненты сильносвязности
	void dfsBuildColoredTrees(void)
	{
		dfsNeeds.clear();
		// Массив предназначеный для хранения информации о вершинах
		dfsNeeds.resize(verticesCount);
		// Очередь для реализации обхода в глубину
		std::stack<int> dfsStack;
		// Обойдем в глубину весь граф
		for (int iter = verticesCount - 1; iter >= 0; iter--)
		{
			int currStartVert = KosarajusQueue[iter];
			// Вызовем dfs только от тех что ранее небыли помечены
			if (dfsNeeds[currStartVert].colourOfDFS == 0)
			{
				// Закинем вершинку
				dfsStack.push(currStartVert);
				dfsNeeds[currStartVert].colourOfKosarajus = ++counterKosajarusColors;
				dfsNeeds[currStartVert].colourOfDFS = 1;
				// Вектор для вызова функции GetNeededVertices
				std::vector<int> tempVertsVector;
				while ((dfsStack.empty() == 0))
				{
					// Вынимаем элемент из стека
					int temporaryVert = dfsStack.top();
					dfsStack.pop();
					// Получаем список нужных вершин
					GetPrevVertices(temporaryVert, tempVertsVector);
					for (unsigned int i = 0; i < tempVertsVector.size(); i++)
					{
						// Обрабатываем только НЕ родительские вершины не помеченные вершины
						if ((dfsNeeds[tempVertsVector[i]].colourOfDFS == 0))
						{
							// Закинем в стек
							dfsStack.push(tempVertsVector[i]);
							// Отметим её (и не будем вызывать от неё dfs если она уже окрашена)
							dfsNeeds[tempVertsVector[i]].colourOfDFS = 1;

							if (dfsNeeds[tempVertsVector[i]].colourOfKosarajus == 0) // Если вершина tempVertsVector[i] еще не покрашена то красим в цвет родителя
							{
								dfsNeeds[tempVertsVector[i]].colourOfKosarajus = dfsNeeds[temporaryVert].colourOfKosarajus;
							}
						}
					}
					tempVertsVector.clear();
				}
			}
		}
	}

	void findConnectionsBetweenStronglyConnectedComponents(void)
	{
		// Матрица смежности для сконденсированного графа (где вершины - SCC - сильносвязные компоненты)
		std::vector<std::vector<int>> SCCMatrix;
		// Сразу делаем ему resize и готовим матрицу к заполнению единичками (1)
		SCCMatrix.resize(counterKosajarusColors);
		for (int i = 0; i < counterKosajarusColors; i++)
		{
			SCCMatrix[i].resize(counterKosajarusColors);
		}

		// Обходим все вершины и смотрим на их детей, если есть другого цвета - строим SCC ребро:)
		for (int i = 0; i < verticesCount; i++)
		{
			std::vector<int> nextVerticesOfCurrentVertice;
			GetNextVertices(i, nextVerticesOfCurrentVertice);

			for (int j = 0; j < nextVerticesOfCurrentVertice.size(); j++)
			{
				if (dfsNeeds[i].colourOfKosarajus != dfsNeeds[nextVerticesOfCurrentVertice[j]].colourOfKosarajus)
				{
					SCCMatrix[dfsNeeds[i].colourOfKosarajus - 1][dfsNeeds[nextVerticesOfCurrentVertice[j]].colourOfKosarajus - 1] = 1;
				}
			}
		}

		int count1 = 0;
		int count2 = 0;

		for (int i = 0; i < counterKosajarusColors; i++)
		{
			int sum1 = 0;
			int sum2 = 0;
			for (int j = 0; j < counterKosajarusColors; j++)
			{
				sum1 += SCCMatrix[i][j];
				sum2 += SCCMatrix[j][i];
			}
			if (sum1 == 0)
				count1++;
			if (sum2 == 0)
				count2++;
		}

		std::cout << ((count1 >= count2) ? count1 : count2);
	}

	void calculate()
	{

		dfsBuildKosarajusQueue();

		dfsBuildColoredTrees();

		if (counterKosajarusColors == 1)
		{
			std::cout << 0;
		}
		else
		{
			findConnectionsBetweenStronglyConnectedComponents();
		}

	}

private:

	int verticesCount;
	// Первый индекс идет по "вертикали", второй индекс по "горизонтали"
	std::vector<std::vector<int>> edges;
	std::vector<dfsVerts> dfsNeeds;
	std::vector<int> KosarajusQueue;
	int counterKosajarusColors = 0;

};

int main(void)
{
	// Ввод данных
	int N, M;
	std::cin >> N;
	std::cin >> M;
	MatrixGraph a(N);
	for (int i = 0; i < M; i++)
	{
		int temp1, temp2;
		std::cin >> temp1;
		std::cin >> temp2;
		a.AddEdge(temp1 - 1, temp2 - 1);
	}

	//Решение задачи
	a.calculate();

	return 0;
}

// Алгоритм решения задачи состоит из двух частей : в первой мы выделяем компоненты сильносвязности графа и
// "окрашиваем" его в соответствующие цвета, затем проводим обход каждой вершины и просматриваем информацию о 
// её потомках.

//- Метод Косарайю обеспечивает поиск сильных компонент связности графа за линейное время и память.

// Доказательство : Этот метод состоит из двух процедур поиска в глубину, подвергнутых незначительным изменениям,
// в результате время его выполнения пропорционально V² в случае насыщенных графов и V + E в случае разреженных графов.

// - Обход каждой из вершин обеспечивает получение количества компонент сильносвязности из которых не выходит ни
// одного ребра в другую компоненту сильносвязности, и наоборот, не входит ни одного ребра из других компонент 
// сильносвязности.

// Доказательство : Этот метод состоит из процедуры обхода в цикле каждой вершины, а затем рассмотрения всех потомков,
// так что в результате время его выполнения пропорционально V² в случае насыщенных графов и V + E в случае разреженных 
// графов.