//3_1.Дан отсортированный массив целых чисел A[0..n - 1] и массив целых чисел B[0..m - 1].Для каждого элемента массива B[i] 
//найдите минимальный индекс k минимального элемента массива A, равного или превосходящего B[i]: A[k] >= B[i].Если такого элемента 
//нет, выведите n.Время работы поиска k для каждого элемента B[i] : O(log(k)).
//n, m ≤ 10000.
//Формат входных данных.
//В первой строчке записаны числа n и m.Во второй и третьей массивы A и B соответственно.

#include <iostream>

int BinarySearch(int *ArrayOfA, int n, int Number);
void SolvingFunction(int *ArrayOfB, int *SolvingArray, int n, int m, int *ArrayOfA);

int main(void)
{

	int n = 0, m = 0;	// Инициализация n и m
	std::cin >> n >> m;	// Ввод n и m
	int *ArrayOfA = new int[n];	// Объявление массива A
	int *ArrayOfB = new int[m];	// Объявление массива B
	for (int i = 0; i < n; i++) { std::cin >> ArrayOfA[i]; };	// Ввод массива A
	for (int i = 0; i < m; i++) { std::cin >> ArrayOfB[i]; };	// Ввод массива B

	int *SolvingArray = new int[m];	// Объявление массива для решения
	SolvingFunction(ArrayOfB, SolvingArray, n, m, ArrayOfA); // Вызов функции решения

	for (int i = 0; i < m; i++) { std::cout << SolvingArray[i] << " "; }; // Вывод значений массива решения

	delete[] ArrayOfA;	// Очистка памяти
	delete[] ArrayOfB;	// Очистка памяти
	delete[] SolvingArray;	// Очистка памяти

}

void SolvingFunction(int *ArrayOfB, int *SolvingArray, int n, int m, int *ArrayOfA)
{

	for (int i = 0; i < m; i++)
	{
		SolvingArray[i] = BinarySearch(ArrayOfA, n, ArrayOfB[i]);
	}
}

// Напишем бинарный поиск, ибо что еще писать, как не бинарный поиск
// Бинарный поиск должен вернуть найденный нами ИНДЕКС, бинарный поиск - жизнь
// Бинарный поиск должен получить массив (ArrayOfA в кот. мы ищем), его размер (n), искомое число (Number)
// Ееее бинарный поиск

int BinarySearch(int *ArrayOfA, int n, int Number)
{

	int Left = 0, Right = n-1, Mid = Left + (Right - Left) / 2, Index = n;
	int action = 1;


	if (Number < ArrayOfA[0])
	{
		Index = 0;
	}
	else
	if (Number > ArrayOfA[n - 1])
	{
		Index = n;
	}
	else
	{
		while (Left < Right)
		{

			Mid = Left + (Right - Left) / 2;

			if (Number <= ArrayOfA[Mid])
			{
				Right = Mid;
			}
			else
			{
				Left = Mid + 1;
			}
		}

		Index = Left;
	}

	//std::cout << "Left" << " " << Left << " ";
	//std::cout << "Right" << " " << Right << " ";
	//std::cout << "<Index" << " " << Index << "> ";

	return Index;

}