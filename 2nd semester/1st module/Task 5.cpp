// ФИВТ | ООП на C++ | 2 семестр, 1 модуль, задача D
// https://contest.yandex.ru/contest/3970/problems/D/
// ............Зал круглых столов............
// Единственный способ попасть в Зал Круглых Столов – пройти через Колонный Коридор.
// Стены Коридора изображаются на карте прямыми линиями, которые параллельны осиOY системы координат.
// Вход в Коридор находится снизу, а выход из Коридора в Зал – сверху.
// В Коридоре есть цилиндрические(на карте круглые) Колонны одинакового радиуса R.
// Разработайте алгоритм, который по информации о размерах Коридора, и размещения Колонн определяет диаметр 
// наибольшего из Круглых Столов, который можно пронести через такой Коридор, сохраняя поверхность Стола 
// горизонтальной.
// ............ Формат ввода ............
// В первой строке задано два целых числа XL и XR - x - координаты левой и правой стен Коридора.
// Во второй строке находится целое число R(1≤R≤1000000)(1≤R≤1000000) - радиус всех Колон.
// В третьей - целое число N(1≤N≤200)(1≤N≤200), задающее количество Колон.
// Далее идут N строк, в каждой из которых по два целых числа – x - и y - координаты центра 
// соответствующей Колоны.
// Все входные координаты – целые числа, не превышающие по модулю 1000000.
// ............ Формат вывода ............
// Вывести одно число - искомый диаметр наибольшего Стола. 
// Диаметр нужно выводить с точностью 3 знака после десятичной точки(даже в случае, если он окажется целым).
// Если нельзя пронести ни одного Стола, то ответ должен быть : 0.000
// Точность 3 знака после точки, по обычнам правилам округления, означает, что ответ, который выводится 
// в выходной файл, должен отличатся от точного не более чем на 5×10−45×10−4(то есть на 0.0005). Например,
// если точный ответ 1.234567, то в файле должно находится число 1.235.Если точный ответ 5.0005, то 
// необходимо округлять в большую сторону, то есть в файл необходимо вывести 5.001.

#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <stack>

double makeDoubleFine(double input)
{
	double a = input * 1000;
	double b = double(a);
	double shit = a - b;

	double newResult;
	if (shit >= 0.5)
	{
		return (b / 1000 + 0.001);
	}
	else
	{
		return (b / 1000);
	}
}

struct Pillar 
{ 
	Pillar(double input1, double input2) 
		: x(input1), y(input2) 
	{ }

	double x, y; 
};

struct bfsVerts {
	char colour = 0;

	double parent = -1;
};

class MatrixGraph {
public:

	explicit MatrixGraph(double _verticesCount, double input2) : verticesCount(_verticesCount), edges(_verticesCount), PillarOwnRadius(input2), dfsNeeds(verticesCount) {
		for (double i = 0; i < verticesCount; i++) { edges[i].resize(verticesCount); }
	}

	void AddEdge(double input1, double input2)
	{
		edges[input1][input2] = 1;
		edges[input2][input1] = 1;
	}

	void GetNearVertices(double vertex, std::vector<double>& vertices) const
	{
		if (vertices.empty() != 1)
			vertices.clear();
		for (double i = 0; i < verticesCount; i++)
			if (edges[vertex][i] == 1) vertices.push_back(i);
	}


	void push_Pillar(Pillar *x) { Pillars.push_back(x); Pillars[0]->x; }

	double binSearch(double left, double right)
	{
		double mid = (left + right) / 2;

		while (mid != left && mid != right)
		{
			bool IsThatMidOk = itFits(mid);

			if (IsThatMidOk)
			{
				left = mid;
			}
			else
			{
				right = mid;
			}

			mid = (left + right) / 2;
		}

		return mid;
	}

	// Возвращает 1 если inputCurrentMid подходит и путь есть, 0 иначе.
	bool itFits(double inputCurrentMid)
	{
		// Очистим граф и вершины
		for (double i = 0; i < verticesCount; i++)
		{
			for (double j = 0; j < verticesCount; j++)
			{
				edges[i][j] = 0;
			}
			dfsNeeds[i].parent = -1;
			dfsNeeds[i].colour = 0;
		}

		// Построим ребра сваязанные с левой стеной
		for (double j = 0; j < verticesCount; j++)
		{
			if ((j != 0) && (_ifEdge_(Pillars[0]->x, 0, Pillars[j]->x, 0, inputCurrentMid, 0)))
			{
				AddEdge(0, j);

			}
		}

		// Построим ребра сваязанные с правой стеной
		for (double j = 0; j < verticesCount; j++)
		{
			if ((j != 1) && (_ifEdge_(Pillars[1]->x, 0, Pillars[j]->x, 0, inputCurrentMid, 0)))
			{
				AddEdge(1, j);

			}
		}

		// Построим все остальные ребра
		for (double i = 2; i < verticesCount; i++)
		{
			for (double j = 2; j < verticesCount; j++)
			{
				if ((i != j) && (_ifEdge_(Pillars[i]->x, Pillars[i]->y, Pillars[j]->x, Pillars[j]->y, inputCurrentMid, 1)))
				{
					AddEdge(i, j);
				}
			}
		}

		// Вызовем DFS от 0 и дойдем (или не дойдем) до 1
		bool DidWeMadeItThroughTheCoridor = !dfs(1, 0);
		return DidWeMadeItThroughTheCoridor;
	}

private:

	// Возвращает 1 если найден путь из inputVertFROM в inputVertTO
	bool dfs(double inputVertFROM, double inputVertTO)
	{
		// Очередь для реализации обхода в глубину, сразу закидываем туда начальную вершину
		std::stack<double> dfsStack;
		dfsStack.push(inputVertFROM);

		// Вектор для вызова функции GetNearVerts
		std::vector<double> tempVertsVector;

		while ((dfsStack.empty() == 0))
		{
			// Вынимаем элемент из стека
			double temporaryVert = dfsStack.top();
			dfsStack.pop();

			// Получаем список смежных с ним вершин
			GetNearVertices(temporaryVert, tempVertsVector);

			for (int i = 0; i < tempVertsVector.size(); i++)
			{
				// Обрабатываем только НЕ родительские вершины
				if (tempVertsVector[i] != dfsNeeds[temporaryVert].parent)
				{
					// Если вершина еще не помечена то кидаем её в очередь на dfs
					if (dfsNeeds[tempVertsVector[i]].colour == 0)
					{
						dfsStack.push(tempVertsVector[i]);
						dfsNeeds[tempVertsVector[i]].parent = temporaryVert;

						// Отметим её (и не будем вызывать от неё dfs если она уже окрашена)
						dfsNeeds[tempVertsVector[i]].colour = 1;
					}

					// Проверим, пришли ли в конечную вершину
					if (tempVertsVector[i] == inputVertTO)
					{
						for (double i = 0; i < dfsStack.size(); i++)
							dfsStack.pop();
						return 1;
					}

				}
			}
		}
		return 0;
	}

	bool _ifEdge_(double x1, double y1, double x2, double y2, double currentMid, bool wallOrPillar) {

		double DistanceBetweenTwoPillars = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));

		if (wallOrPillar == 0) // Если стена
		{
			// Если ребро нужно строить значит расстояние между стеной и пилларом меньше чем радиус пиллара + диаметр стола
			return(DistanceBetweenTwoPillars < PillarOwnRadius + currentMid);
		}
		else
			if (wallOrPillar == 1) // Если не стена
			{
				// Если ребро нужно строить значит расстояние между двумя пилларами меньше чем 2 радиуса пиллара + диаметр стола
				return(DistanceBetweenTwoPillars < PillarOwnRadius * 2 + currentMid);
			}

	}

	double verticesCount;							// Количество вершин в графе
	double PillarOwnRadius;						// Радиус колон
	std::vector<std::vector<double>> edges;		// Матриац смежности
	std::vector<Pillar*> Pillars;				// Массив колон + стен (левая стена 0, правая стена 1)

	std::vector<bfsVerts> dfsNeeds;

};

int main(void)
{
	double XL, XR, R, N; // XL - х координата левой стены, XR - х координата правой стены
	std::cin >> XL >> XR >> R >> N; // R - радиус колон, N - количество колон

									// Построим граф из N+2 вершин (N колон + 2 стены) + закинем радиус R
	MatrixGraph task(N + 2, R);

	Pillar temp1(XL, -1);
	Pillar temp2(XR, -1);
	task.push_Pillar(&temp1);
	task.push_Pillar(&temp2);

	bool ShouldWeEvenWork = 0;

	// Вводим координаты всех колон
	for (double i = 0; i < N; i++)
	{
		double Ttemp1, Ttemp2;
		std::cin >> Ttemp1;
		std::cin >> Ttemp2;
		Pillar *Ttemp = new Pillar(Ttemp1, Ttemp2);
		task.push_Pillar(Ttemp);

		if ((Ttemp1 >= XL) && (Ttemp1 <= XR))
		{
			ShouldWeEvenWork = 1;
		}
	}

	double result;

	if (ShouldWeEvenWork)
	{
		result = task.binSearch(0, abs(XR - XL));
		result = makeDoubleFine(result);
	}
	else
	{
		result = XR - XL;
		result = makeDoubleFine(result);
	}

	std::cout.setf(std::ios::fixed);
	std::cout << std::setprecision(3) << result;

}