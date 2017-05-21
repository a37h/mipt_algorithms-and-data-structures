// ФИВТ | ООП на C++ | 2 семестр, 1 модуль, задача C
// https://contest.yandex.ru/contest/3970/problems/C/
// ............А есть ли сток?............
// Дана матрица смежности ориентированного графа.Проверьте, содержит ли граф вершину
// - сток.То есть вершину, в которую ведут ребра из всех вершин, и из которой не 
// выходит ни одного ребра.Требуемая сложность O(V).
// ............ Формат ввода ............
// ............ Формат вывода ............

#include <vector>
#include <iostream>

class MatrixGraph{
public:

	explicit MatrixGraph(int _verticesCount) :
		verticesCount(_verticesCount),
		edges(_verticesCount),
		horisontalSum(_verticesCount)
	{
	}

	void GetNextVertices(int vertex, std::vector<int>& vertices) const
	{
		if (vertices.empty() != 1)
			vertices.clear();

		if (vertices.empty() != 1)
			vertices.clear();
		for (int i = 0; i < verticesCount; i++)
			if (edges[vertex][i] == 1) vertices.push_back(i);
	}

	void GetPrevVertices(int vertex, std::vector<int>& vertices) const
	{
		if (vertices.empty() != 1)
			vertices.clear();

		if (vertices.empty() != 1)
			vertices.clear();
		for (int i = 0; i < verticesCount; i++)
			if (edges[i][vertex] == 1) vertices.push_back(i);
	}

	void ShowMatrix() const
	{
		std::cout << "   ";
		for (int i = 0; i < verticesCount; i++)
			std::cout << i << " ";

		std::cout << std::endl << std::endl;

		for (int i = 0; i < verticesCount; i++)
		{
			std::cout << i << "  ";
			for (int j = 0; j < verticesCount; j++)
			{
				if (edges[i][j] == 1)
				{
					std::cout << "W ";
				}
				else
				{
					std::cout << "* ";
				}
			}
			std::cout << std::endl << std::endl;
		}
	}

	void IsThereStock()
	{
		for (int i = 0; i < verticesCount; i++)
		{
			if (horisontalSum[i] == 0)
			{
				int sum = 0;
				for (int j = 0; j < verticesCount; j++)
					sum += edges[j][i];
				if (sum == verticesCount - 1)
				{
					std::cout << "YES";
					return;
				}
			}
		}
		std::cout << "NO";
		return;
	}

	// Первый индекс идет по "вертикали", второй индекс по "горизонтали"
	std::vector<std::vector<int>> edges;

	std::vector<int> horisontalSum;

private:
	int verticesCount;

};

int main(void)
{
	int n;
	std::cin >> n;
	MatrixGraph abc(n);
	
	for (int i = 0; i < n; i++)
	{
		int sum = 0;

		for (int j = 0; j < n; j++)
		{
			int temp;
			std::cin >> temp;
			abc.edges[i].push_back(temp);

			sum += temp;
		}

		abc.horisontalSum[i] = sum;
	}

	abc.IsThereStock();
	return 0;
}