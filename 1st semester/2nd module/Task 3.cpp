//Даны неотрицательные целые числа n,k...
//3_1. Реализуйте стратегию выбора опорного элемента “медиана трёх”. 
//Функцию Partition реализуйте методом прохода двумя итераторами от начала      
//массива к концу. 

// Полностью рабочая версия с медианой 

// Изначально были попытки сделать медиану из первого, последнего и среднего, но, видимо, вы постарались сделать хорошую killer sequence на этот случай
// Так что теперь медиана из "первочетвертного", "серединного", "тричетвертного" элементов (т.е. тот, что находится примерно на позиции 1/4 от размера массива, 1/2 и 3/4, соответственно).
// Теперь все работает и реализована медиана.

#include <iostream>
#include <vector>

int partition(int *array, int left, int right);
int KStatDC(int *array, int n, int k);
int mediana(int a, int  ai, int b, int bi, int c, int ci);

int main(void)
{

	int n = 0, k = 0;
	std::cin >> n;
	std::cin >> k;
	int *newarray = new int[n];
	for (int i = 0; i < n; i++)
	{
		std::cin >> newarray[i];
	}

	std::cout << KStatDC(newarray, n, k);

	delete[] newarray;

	return 0;
}

int partition(int *array, int left, int right)
{
	int mid = (left + right) / 2;
	int top = (left + mid) / 2;
	int bot = (mid + right) / 2;

	int medianofthree = mediana(array[mid],mid,array[top],top,array[bot],bot);
	// Опорный элемент

	std::swap(array[medianofthree], array[right]);
	// Поменяли пивот и последний

	int i = left;
	int j = left;
	// i указывает на начало группы элементов, строго бОльших опорного
	// j больше i, итератор j указывает на первый нерассмотренный элемент 

	while (j < right)
	{
		if (array[j] > array[right])
		{
			j++;
		}
		else
		{
			std::swap(array[i], array[j]);
			i++;
			j++;
		}
	}

	std::swap(array[i], array[right]);

	return i;

}

int KStatDC(int *array, int n, int k)
{
	int left = 0, right = n - 1;
	while (true) {
		int mid = partition(array, left, right);

		if (mid == k) {
			return array[mid];
		}
		else if (k < mid) {
			right = mid - 1;
		}
		else {
			left = mid + 1;
		}
	}
}

int mediana(int a, int  ai, int b, int bi, int c, int ci)
{
	if (a > b) { // ba ?c
		if (c > a) // bac
			return ai;
		return (b > c) ? bi : ci;
	}
	// ab ? c
	if (c > b) // abc
		return bi;
	return (a > c) ? ai : ci;
}