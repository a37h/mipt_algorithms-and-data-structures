// Алгоритм сжатия данных Хаффмана

// Заголовки для тестирования вручную
#include <iostream>
#include <fstream>

// <vector>, <map>, <queue> для хранения промежуточных данных
// <algorithm> для использования std::sort (я очень ленивый, знаю)
#include <algorithm>
#include <vector>
#include <map>
#include <queue>

#include "Huffman.h"

static void copyStream(IInputStream& input, IOutputStream& output)
{
	byte value;
	while (input.Read(value))
	{
		output.Write(value);
	}
} 


// Тип хранимого элемента
typedef char byte;

// Получение бита из байта под номером от 1 до 8 (нумерация слева направо)
bool getBit(unsigned char number, char position)
{
	switch (position)
	{
	case 1:
		return ((number & 128) != 0);
	case 2:
		return ((number & 64) != 0);
	case 3:
		return ((number & 32) != 0);
	case 4:
		return ((number & 16) != 0);
	case 5:
		return ((number & 8) != 0);
	case 6:
		return ((number & 4) != 0);
	case 7:
		return ((number & 2) != 0);
	case 8:
		return ((number & 1) != 0);
	default:
		return 1;
	}
}

// Получение байта из 8 бит (n1 - старший разряд, n8 - младший)
unsigned char getByte(bool n1, bool n2, bool n3, bool n4, bool n5, bool n6, bool n7, bool n8)
{
	return n1 * 128 + n2 * 64 + n3 * 32 + n4 * 16 + n5 * 8 + n6 * 4 + n7 * 2 + n8;
}



/////////////////////
//// ENCODE PART ////
/////////////////////



// Простой счетчик чтобы удобно передавать и принимать в функцию и из неё
struct counter
{
	int count;

	void inc() { count++; };

	counter() : count(0) {};
};

// Структура для хранения кода символа и длины кода
struct encodedSymbol
{
	// Будем хранить 0 и 1
	std::vector<bool> data;
	// Будем иметь их количество
	int length;

	encodedSymbol() : length(0) {};
    
    ~encodedSymbol()
    {
    data.clear();
    }
};

// Объект для хранения пары символ + частота
struct symbolObject
{
	// Частота с которой он встрчается
	int itsPercentage;

	// Массив символов которые он содержит и их количество
	std::vector<unsigned char> symbols;
	int ElemenetsContaining = 0;

	// 0 - левые повороты, 1 - правые повороты.
	// В code хранится 2 поля
	// data - массив из 0 и 1, слева направо получаем повороты от вершины
	// length - количество таких поворотов
	encodedSymbol code;

	symbolObject* LeftChild;
	symbolObject* RightChild;
    
    ~symbolObject()
    {
    delete LeftChild;
    delete RightChild;
    symbols.clear();
    }

	symbolObject(int itsPercentage_, int ElementsContaining_, unsigned char symbol_) :
		itsPercentage(itsPercentage_),
		LeftChild(nullptr),
		RightChild(nullptr),
		ElemenetsContaining(ElementsContaining_)
	{
		symbols.push_back(symbol_);
	};

	symbolObject() :
		itsPercentage(0),
		ElemenetsContaining(0),
		LeftChild(nullptr),
		RightChild(nullptr)
	{};

	symbolObject(int itsPercentage_, int ElementsContaining_, std::vector<unsigned char>& first, std::vector<unsigned char>& second) :
		itsPercentage(itsPercentage_),
		LeftChild(nullptr),
		RightChild(nullptr),
		ElemenetsContaining(ElementsContaining_)
	{
		for (int i = 0; i < first.size(); i++)
		{
			symbols.push_back(first[i]);
		}
		for (int i = 0; i < second.size(); i++)
		{
			symbols.push_back(second[i]);
		}
	};


};

// Рекурсивный обход дерева для создания кода элемента (рекурсивный вызов). Левый поворот: 0, правый поворот: 1
void preOrder(symbolObject* root, counter *haffmanTreeAmountOfNodes)
{
	if (root)
	{
		haffmanTreeAmountOfNodes->inc();
		// Если дошли до листа то выходим из функции, продолжать обход нам не надо
		if (root->LeftChild == nullptr && root->RightChild == nullptr)
			return;

		// Если есть левый ребенок то вызываем рекурсивно обход левого поддерева
		// В код левого ребенка добавляем 0, длину левого кода ставим длину кода в данном узле + 1
		if (root->LeftChild != nullptr)
		{
			for (int i = 0; i < root->code.length; i++)
			{
				root->LeftChild->code.data.push_back(root->code.data[i]);
			}

			root->LeftChild->code.data.push_back(0);
			root->LeftChild->code.length = root->code.length + 1;
			preOrder(root->LeftChild, haffmanTreeAmountOfNodes);
		}

		// Если есть правый ребенок то вызываем рекурсивно обход правого поддерева
		// В код правого ребенка добавляем 1, длину правого кода ставим длину кода в данном узле + 1
		if (root->RightChild != nullptr)
		{
			for (int i = 0; i < root->code.length; i++)
			{
				root->RightChild->code.data.push_back(root->code.data[i]);
			}

			root->RightChild->code.data.push_back(1);
			root->RightChild->code.length = root->code.length + 1;
			preOrder(root->RightChild, haffmanTreeAmountOfNodes);
		}
	}
}

// Рекурсивный обход дерева для создания кода элемента (первичный вызов). Левый поворот: 0, правый поворот: 1
void PreOrder(symbolObject* root, counter *haffmanTreeAmountOfNodes)
{
	if (root)
	{
		haffmanTreeAmountOfNodes->inc();

		// Рассмотрели случай если дерево состоит из 1 вершины
		// Т.е. один символ, кодируем его 0 и длина кода составит 1
		if (root->LeftChild == nullptr && root->RightChild == nullptr)
		{
			root->code.data.push_back(0);
			root->code.length = 1;
			return;
		}

		// Иначе же, если есть хотя-бы один ребенок то длина кода корня 1
		root->code.length = 0;

		// Если есть левый ребенок то вызываем рекурсивно обход левого поддерева
		// В код левого ребенка добавляем 0, длину левого кода ставим 1
		if (root->LeftChild != nullptr)
		{
			root->LeftChild->code.data.push_back(0);
			root->LeftChild->code.length = 1;
			preOrder(root->LeftChild, haffmanTreeAmountOfNodes);
		}

		// Если есть правый ребенок то вызываем рекурсивно обход правого поддерева
		// В код правого ребенка добавляем 1, длину правого кода ставим 1
		if (root->RightChild != nullptr)
		{
			root->RightChild->code.data.push_back(1);
			root->RightChild->code.length = 1;
			preOrder(root->RightChild, haffmanTreeAmountOfNodes);
		}
	}
}

// Предикат отношения для сортировки вектора из symbolObject
bool predikat(symbolObject* a, symbolObject* b)
{
	return a->itsPercentage > b->itsPercentage;
}

// InOrder обход чтобы зашифровать дерево
void InOrderHaffmanTree(symbolObject* Root, std::vector<bool>& haffman)
{
	if (Root->LeftChild != nullptr || Root->RightChild != nullptr)
	{
		haffman.push_back(1);
	}
	else
	{
		haffman.push_back(0);

		for (int p = 1; p <= 8; p++)
			haffman.push_back(getBit(Root->symbols[0], p));
	}

	if (Root->LeftChild)
	{
		InOrderHaffmanTree(Root->LeftChild, haffman);
	}

	if (Root->RightChild)
	{
		InOrderHaffmanTree(Root->RightChild, haffman);
	}
}

// Функция кодировки (Главная функция)
void Encode(IInputStream& original, IOutputStream& compressed)
{
	std::vector<unsigned char> string;

	// Ассоциативный массив: индексы - символы, значение - частота символа
	std::map<unsigned char, std::vector<int>> indivSymbols;

	// 2 идентичных массива с индивидуальными символами: значения - указатели на объекты symbolObject*
	// Обновляем массив percentage актуальными данными
	std::vector <symbolObject*> symbols;
	std::vector <symbolObject*> symbolsCOPY;

	//std::ifstream fin;
	//fin.open("SOURCE.txt");
	//char abc;
	//while (fin >> abc)
	//{
	//	string.push_back(abc);

	//	indivSymbols[abc].push_back(1);

	//};

	byte value;
	while (original.Read(value))
	{
		string.push_back(value);


		indivSymbols[value].push_back(1);
	}

	for (std::map<unsigned char, std::vector<int>>::iterator it = indivSymbols.begin(); it != indivSymbols.end(); ++it)
	{
		symbolObject *temp = new symbolObject();
		temp->symbols.push_back(it->first);
		temp->itsPercentage = it->second.size();

		symbols.push_back(temp);
		symbolsCOPY.push_back(temp);

	}

	int symbols_count = symbols.size();

	int n = string.size();
	//fin.close();

	// Если ничего не ввели то завершились
	if (string.size() == 0)
		return;

	// Предикат сравнения для создания очереди из символов
	struct customLess {
		bool operator()(symbolObject *a, symbolObject *b)
		{
			return a->itsPercentage > b->itsPercentage;
		}
	};

	// Создание очереди из символов
	std::priority_queue<symbolObject*, std::vector<symbolObject*>, customLess> queue;
	for (int i = 0; i < symbols_count; i++)
	{
		queue.push(symbols[i]);
	}

	// Создание дерева Хаффмана
	for (int i = 1; i < symbols_count; i++)
	{
		symbolObject *temp = new symbolObject();

		temp->LeftChild = queue.top();
		queue.pop();
		temp->RightChild = queue.top();
		queue.pop();

		temp->itsPercentage = temp->LeftChild->itsPercentage + temp->RightChild->itsPercentage;

		queue.push(temp);
	}

	// Получили корень дерева Хаффмана
	symbolObject *HaffmanTreeRoot;
	HaffmanTreeRoot = queue.top();
	queue.pop();

	// Считаем путь до каждого символа рекурсивно изнутри этой функции, путь пишется внутри объекта SymbolObject
	counter *haffmanTreeAmountOfNodes = new counter;
	PreOrder(HaffmanTreeRoot, haffmanTreeAmountOfNodes);

	// Строим таблицу хаффмана
	std::map<unsigned char, symbolObject*> table;
	for (int i = 0; i < symbols_count; i++)
	{
		table[symbolsCOPY[i]->symbols[0]] = symbolsCOPY[i];
	}

	// Сохраняем дерево в побитовый массив haffman
	std::vector<bool> haffman;
	InOrderHaffmanTree(HaffmanTreeRoot, haffman);

	// Сохранили бит шифрования, хаффмана и зашифрованную строку в битовый массив
	std::vector<bool> output_coded;
	output_coded.push_back(0);
	for (int f = 0; f < haffman.size(); f++)
		output_coded.push_back(haffman[f]);

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	int codedmessageSize = 0;
	for (int i = 0; i < n; i++)
	{
		for (int p = 0; p < table[string[i]]->code.length; p++)
		{
			output_coded.push_back(table[string[i]]->code.data[p]);
			codedmessageSize++;
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////

	int countofEightMinusBitsinlastbyte = (8 - output_coded.size() % 8) % 8;
	for (int f = 0; f < countofEightMinusBitsinlastbyte; f++)
		output_coded.push_back(0);
	for (int f = 1; f <= 8; f++)
		output_coded.push_back(getBit((8 - countofEightMinusBitsinlastbyte) % 8, f));

	bool e;

	/* Если зашифрованное весит больше чем сама строка то 0 */
	/* Если зашифрованное весит меньше чем сама строка то 1 */
	(output_coded.size()) >= ((string.size() * 8) + 8) ? e = false : e = true;

	// Делаем битовый выходной массив
	if (e)								// Если выводим зашифрованное
	{
		output_coded[0] = e;
	}
	else {							// Если выводим исходное
		output_coded.clear();
		output_coded.push_back(e);
		for (int f = 1; f <= 7; f++)
			output_coded.push_back(0);
		for (int f = 0; f < string.size(); f++)
		{
			for (int p = 1; p <= 8; p++)
				output_coded.push_back(getBit(string[f], p));
		}
	}

	// Делаем байтовый выходной массив
	std::vector<unsigned char> output_byte;
	for (int f = 0; f < output_coded.size() / 8; f++)
	{
		output_byte.push_back(getByte(output_coded[f * 8 + 0], output_coded[f * 8 + 1], output_coded[f * 8 + 2], output_coded[f * 8 + 3], output_coded[f * 8 + 4], output_coded[f * 8 + 5], output_coded[f * 8 + 6], output_coded[f * 8 + 7]));
	}

	//// ТЕСТОВЫЙ ВЫВОД ДЛЯ ТЕСТИРОВАНИЯ | В ПОСЛЕДСТВИИ ЗАМЕНИТЬ НА ВЫВОД ИЗ ПОТОКА ВЫВОДА //
	//std::ofstream fout;
	//fout.open("ENCODED.txt");
	//for (int f = 0; f < output_byte.size(); f++) { fout << output_byte[f]; };
	//fout.close();
	//// ТЕСТОВЫЙ ВВОД ДЛЯ ТЕСТИРОВАНИЯ | В ПОСЛЕДСТВИИ ЗАМЕНИТЬ НА ВВОД ИЗ ПОТОКА ВВОДА //

	for (int f = 0; f < output_byte.size(); f++) { compressed.Write(output_byte[f]); };
    
    string.clear();
    indivSymbols.clear();
    symbols.clear();
    symbolsCOPY.clear();
    table.clear();
    haffman.clear();
    output_coded.clear();
    output_byte.clear();

	return;
} // Неужели



  /////////////////////
  //// DECODE PART ////
  /////////////////////

struct string_BIT_struct
{
	std::vector<bool> data;
	int lastindex;

	~string_BIT_struct()
    {
    data.clear();
    }
    
};

void buildhaffmantree(symbolObject* Root, string_BIT_struct* string_BIT)
{
	string_BIT->lastindex++;

	if (string_BIT->data[string_BIT->lastindex])		// Если 1 т.е. узел
	{
		Root->LeftChild = new symbolObject();
		buildhaffmantree(Root->LeftChild, string_BIT);

		Root->RightChild = new symbolObject();
		buildhaffmantree(Root->RightChild, string_BIT);
	}
	else												// Если 0 т.е. лист
	{
		Root->LeftChild = nullptr;
		Root->RightChild = nullptr;

		unsigned char temp = 0;
		temp = getByte(string_BIT->data[string_BIT->lastindex + 1], string_BIT->data[string_BIT->lastindex + 2], string_BIT->data[string_BIT->lastindex + 3], string_BIT->data[string_BIT->lastindex + 4], string_BIT->data[string_BIT->lastindex + 5], string_BIT->data[string_BIT->lastindex + 6], string_BIT->data[string_BIT->lastindex + 7], string_BIT->data[string_BIT->lastindex + 8]);
		string_BIT->lastindex = string_BIT->lastindex + 8;
		Root->symbols.push_back(temp);
	}
}

void BuildHaffManTree(symbolObject* Root, string_BIT_struct* string_BIT)
{

	string_BIT->lastindex = 1;

	Root->LeftChild = new symbolObject();
	buildhaffmantree(Root->LeftChild, string_BIT);

	Root->RightChild = new symbolObject();
	buildhaffmantree(Root->RightChild, string_BIT);

}



void Decode(IInputStream& compressed, IOutputStream& original)
{
	std::vector<unsigned char> OUTPUT;
	std::vector<unsigned char> string_BYTE;
	// ТЕСТОВЫЙ ВВОД ДЛЯ ТЕСТИРОВАНИЯ | В ПОСЛЕДСТВИИ ЗАМЕНИТЬ НА ВВОД ИЗ ПОТОКА ВВОДА //
	//std::ifstream fin;
	//fin.open("ENCODED.txt");
	//char abc;
	//while (fin >> abc) { string_BYTE.push_back(abc); };
	//int n = string_BYTE.size();
	//fin.close();
	// ТЕСТОВЫЙ ВВОД ДЛЯ ТЕСТИРОВАНИЯ | В ПОСЛЕДСТВИИ ЗАМЕНИТЬ НА ВВОД ИЗ ПОТОКА ВВОДА //

	byte value;
	while (compressed.Read(value))
	{
		string_BYTE.push_back(value);
	}

	// Если ничего не ввели
	if (string_BYTE.size() == 0)
		return;

	// Вектор хранящий закодированный файл в битовом формате
	string_BIT_struct string_BIT;
	for (int i = 0; i < string_BYTE.size(); i++)
	{
		for (int p = 1; p <= 8; p++)
			string_BIT.data.push_back(getBit(string_BYTE[i], p));
	}

	if (string_BIT.data[0] == 0)	// Если строка пришла в исходном формате
	{
		// Просто вывести ее
		int amountofbytesinencodedstring = string_BYTE.size();
		// Нужно начать выводить байты с 9-го бита

		for (int i = 1; i < amountofbytesinencodedstring; i++)
		{
			byte temp = getByte(string_BIT.data[i * 8], string_BIT.data[i * 8+1], string_BIT.data[i * 8+2], string_BIT.data[i * 8+3], string_BIT.data[i * 8+4], string_BIT.data[i * 8+5], string_BIT.data[i * 8+6], string_BIT.data[i * 8+7]);
			OUTPUT.push_back(temp);
		}

		//// ТЕСТОВЫЙ ВЫВОД ДЛЯ ТЕСТИРОВАНИЯ | В ПОСЛЕДСТВИИ ЗАМЕНИТЬ НА ВЫВОД ИЗ ПОТОКА ВЫВОДА //
		//std::ofstream fout;
		//fout.open("DECODED.txt");
		//for (int f = 0; f < OUTPUT.size(); f++) { fout << OUTPUT[f]; };
		//fout.close();
		//// ТЕСТОВЫЙ ВВОД ДЛЯ ТЕСТИРОВАНИЯ | В ПОСЛЕДСТВИИ ЗАМЕНИТЬ НА ВВОД ИЗ ПОТОКА ВВОДА //


		for (int f = 0; f < OUTPUT.size(); f++) { original.Write(OUTPUT[f]); };
        
        OUTPUT.clear();
		string_BYTE.clear();
        

		return;
	}
	else
	{

		symbolObject *HaffmanTreeRoot = new symbolObject();
		if (string_BIT.data[1] == 1)				// Т.е. в начале дерева Хаффмана узел => имеет минимум 2 ребенка
		{
			BuildHaffManTree(HaffmanTreeRoot, &string_BIT);
			// lastindex ссылается на ячейку в которой начинается код зашифрованной строки
			string_BIT.lastindex++;
		}
		else										// Т.е. в начале дерева Хаффмана лист => состоит из одного узла
		{
			unsigned char temp = getByte(string_BIT.data[2], string_BIT.data[3], string_BIT.data[4], string_BIT.data[5], string_BIT.data[6], string_BIT.data[7], string_BIT.data[8], string_BIT.data[9]);
			HaffmanTreeRoot->symbols.push_back(temp);
			// lastindex ссылается на ячейку в которой начинается код зашифрованной строки
			string_BIT.lastindex = 10;


		}

		int countOfBitsInPreLastByte = 0;
		int abk = string_BIT.data.size() - 1;
		countOfBitsInPreLastByte = string_BIT.data[abk - 3] * 8 + string_BIT.data[abk - 2] * 4 + string_BIT.data[abk - 1] * 2 + string_BIT.data[abk];

		int indexOfLastBitCodingString;
		if (countOfBitsInPreLastByte == 0)
			indexOfLastBitCodingString = string_BIT.data.size() - 8 - 1;
		else
			indexOfLastBitCodingString = string_BIT.data.size() - 8 - 8 + countOfBitsInPreLastByte - 1;

		//////////////////////////////////////

		std::vector<bool> codedstringONLYCODEDSYMBOLS;

		// Считаем количество значащих бит в предпоследнем байте
		int amountofbitsintheprelastbyte = countOfBitsInPreLastByte;

		// lastindex - храним индекс ячейки в которой лежит последний бит зашифрованных символов
		int lastindex = indexOfLastBitCodingString;

		// В массиве codedstringONLYCODEDSYMBOLS лежат закодированные символы
		for (int p = string_BIT.lastindex; p <= lastindex; p++)
		{
			codedstringONLYCODEDSYMBOLS.push_back(string_BIT.data[p]);
		}


		// Осталось расшифровать строку при помощи дерева
		symbolObject *Node;
		Node = HaffmanTreeRoot;

		if (string_BIT.data[1] == 1) // Т.е. минимум 2 символа
		{
			for (int p = 0; p < codedstringONLYCODEDSYMBOLS.size(); p++) {

				if (codedstringONLYCODEDSYMBOLS[p] == 0)
					Node = Node->LeftChild;
				if (codedstringONLYCODEDSYMBOLS[p] == 1)
					Node = Node->RightChild;

				if ((Node->LeftChild == nullptr) &( Node->RightChild == nullptr))
				{
					OUTPUT.push_back(Node->symbols[0]);
					Node = HaffmanTreeRoot;
				}
			}
		}
		else
		{
			for (int p = 0; p < codedstringONLYCODEDSYMBOLS.size(); p++)
			{
				OUTPUT.push_back(HaffmanTreeRoot->symbols[0]);
			}

			int abbsdbfasd = 0;
		}


		for (int f = 0; f < OUTPUT.size(); f++) { original.Write(OUTPUT[f]); };
        
        codedstringONLYCODEDSYMBOLS.clear();
        
        
	}


	//// ТЕСТОВЫЙ ВЫВОД ДЛЯ ТЕСТИРОВАНИЯ | В ПОСЛЕДСТВИИ ЗАМЕНИТЬ НА ВЫВОД ИЗ ПОТОКА ВЫВОДА //
	//std::ofstream fout;
	//fout.open("DECODED.txt");
	//for (int f = 0; f < OUTPUT.size(); f++) { fout << OUTPUT[f]; };
	//fout.close();
	//// ТЕСТОВЫЙ ВВОД ДЛЯ ТЕСТИРОВАНИЯ | В ПОСЛЕДСТВИИ ЗАМЕНИТЬ НА ВВОД ИЗ ПОТОКА ВВОДА //

	OUTPUT.clear();
    string_BYTE.clear();

	return;
}