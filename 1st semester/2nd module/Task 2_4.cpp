//Во всех задачах данного раздела необходимо реализовать и использовать локальную пирамидальную сортировку 
//(без использования дополнительной памяти). Общее время работы алгоритма O(n log n).
//2_4. Закраска прямой 2.
//На числовой прямой окрасили N отрезков.Известны координаты левого и правого концов каждого отрезка(Li и Ri)
//Найти сумму длин частей числовой прямой, окрашенных ровно в один слой.

#include <iostream>
#include <vector>

// Heap method //
void HeapSIFTDOWN(std::vector<int>& arr, int size, int i);
// Heap method //
void HeapBUILD(std::vector<int>& arr, int size);
// Heap sort //
void HeapSORT(std::vector<int>& arr, int n);
// Solving Function //
int SolveFUNC(int n, std::vector<int>& L, std::vector<int>& R);
// Read function //
int CinFunc(std::vector<int>& first, std::vector<int>& second);
// Memory free function //
void Erase(std::vector<int>& first, std::vector<int>& second);

int main(void)
{
	std::vector<int> LEFT_SIDE, RIGHT_SIDE;

	int amount = CinFunc(LEFT_SIDE, RIGHT_SIDE);

	std::cout << std::endl << SolveFUNC(amount, LEFT_SIDE, RIGHT_SIDE);

	Erase(LEFT_SIDE, RIGHT_SIDE);

	return 0;
}

int CinFunc(std::vector<int>& first, std::vector<int>& second)
{
	int l = 0, r = 0, n = 0;
	std::cin >> n;
	for (int i = 0; i < n; ++i)
	{
		std::cin >> l;
		std::cin >> r;
		first.push_back(l);
		second.push_back(r);
	}

	return n;
}

void Erase(std::vector<int>& first, std::vector<int>& second)
{
	first.clear();
	second.clear();
	return;
}

// Просеивание вниз
void HeapSIFTDOWN(std::vector<int>& arr, int size, int i)
{
	int left = 2 * i;
	int right = 2 * i + 1;
	int largest = i;

	if (left <= size - 1 && arr[left] > arr[largest])
	{ largest = left; }
	if (right <= size - 1 && arr[right] >= arr[largest])
	{ largest = right; }

	if (largest != i)
	{
		std::swap(arr[i], arr[largest]);
		HeapSIFTDOWN(arr, size, largest);
	}
}

// Построение кучи
void HeapBUILD(std::vector<int>& arr, int size)	
{
	for (int i = size / 2 - 1; i >= 0; --i)
	{
		HeapSIFTDOWN(arr, size, i);
	}
}

// Пирамидальная сортировка
void HeapSORT(std::vector<int>& arr, int n)
{
	HeapBUILD(arr, n);

	int heapSize = n;
	while (heapSize >= 2)
	{
		std::swap(arr[0], arr[heapSize - 1]);
		--heapSize;
		HeapSIFTDOWN(arr, heapSize, 0);
	}
}

// Функция решения
int SolveFUNC(int n, std::vector<int>& L, std::vector<int>& R)
{
	int BracketSequenceBalance = 0, ResultLength = 0, Current = 0, LeftArr = 0, RightArr = 0;

	HeapSORT(L, n);
	HeapSORT(R, n);

	for (Current = 0; Current <= R[n - 1]; ++Current) {

		int l = 0, r = 0;
		bool fi = 0, fj = 0;

		if (LeftArr < n)
		{
			l = L[LeftArr];
		}
		while (Current == l)
		{
			if (LeftArr < n)
			{
				++BracketSequenceBalance; // Открывающая
				++LeftArr;
				if (LeftArr < n) { l = L[LeftArr]; }
				fi = 1; // Посетили
			}
			else
			{
				break;
			}
		}

		if (RightArr < n)
		{
			r = R[RightArr];
		}
		while ((Current == r) && (RightArr < n))
		{
			if (RightArr < n)
			{
				--BracketSequenceBalance; // Закрывающая
				++RightArr;
				if (RightArr < n) { r = R[RightArr]; }
				fj = 1; // Посетили
			}
			else
			{
				break;
			}
		}

		if (fi) { l = L[--LeftArr]; }
		if (fj) { r = R[--RightArr]; }

		if (Current >= l) { ++LeftArr; }
		if (Current >= r) { ++RightArr; }

		// Если баланс 1 т.е. открыта одна скобка то закрашиваем
		if (BracketSequenceBalance == 1) { ResultLength++; }
	}

	return ResultLength;
}