//Во всех вариантах данной задачи необходимо реализовать и использовать сортировку вставками.
//1_1.Ящики.
//На склад привезли много пустых ящиков.Все ящики пронумерованы по порядку поступления от 0.
//Известно, что их все можно сложить один в один(то есть так, что каждый следующий помещается 
//в предыдущий).Один ящик можно вложить в другой, если его можно перевернуть так, что 
//размеры одного ящика по всем осям станут строго меньше размеров другого ящика по 
//соответствующим осям.Требуется определить, в какой последовательности они будут вложены 
//друг в друга.Вывести номера ящиков.

#include <iostream> 

struct SBoxes {					// Структура "коробок"
	long long position;				// Номер коробки
	long long volume;			// Объём коробки (на самом деле сумма лин.размеров)ы
};

int main()
{
	long long n=0;
	std::cin >> n;					// Ввод n

	SBoxes *Boxes = new SBoxes[n];	// Структура для решения

	for (long long i = 0; i < n; i++)		// Ввод данных
	{
		long long a = 0, b = 0, c = 0;
		std::cin >> a;
		std::cin >> b;
		std::cin >> c;
		Boxes[i].position = i;
		Boxes[i].volume = a+b+c;
	}								
			
	SBoxes tmp;						// Начало реализации решения ======

	for (long long i = 1; i < n; ++i)
	{	
		tmp.position = Boxes[i].position;
		tmp.volume = Boxes[i].volume;

		long long j = i - 1;

		for (; j>= 0 && tmp.volume < Boxes[j].volume; --j)
		{
			Boxes[j + 1].position = Boxes[j].position;
			Boxes[j + 1].volume = Boxes[j].volume;
		}
		Boxes[j+1].position = tmp.position;
		Boxes[j+1].volume = tmp.volume;
	}								// Конец реализации решения ======
	
	for (long long i = 0; i < n; ++i)			// Вывод ответа
	{
		std::cout << Boxes[i].position << " ";
	}

	delete[] Boxes;						// Очистка памяти

	return 0;
} 