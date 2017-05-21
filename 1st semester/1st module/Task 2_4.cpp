//2_4.“Считалочка”.В круг выстроено N человек, пронумерованных числами от 1 до N.
//Будем исключать каждого k - ого до тех пор, пока не уцелеет только один человек. 
//(Например, если N = 10, k = 3, то сначала умрет 3 - й, потом 6 - й, затем 9 - й, 
//затем 2 - й, затем 7 - й, потом 1 - й, потом 8 - й, за ним - 5 - й, и потом 10 - й.
//Таким образом, уцелеет 4 - й.) Необходимо определить номер уцелевшего. N, k ≤ 10000.

#include <iostream>
#include <vector>

struct Node
{
	Node* Next;
	Node* Prev;
	int Number;

	Node(int Key) :Number(Key) {}
	~Node() {}
};

struct List
{
	Node* Root;
	Node* Last;

	List() : Root(nullptr) {};

	void add(int key);
	void show();
	void del(int key);
	void DEL(Node* Temp);
	Node* findlast();
};

void List::add(int key)
{
	if (Root == nullptr)
	{
		Root = new Node(key);
		Root->Next = Root;
		Root->Prev = Root;
		return;
	}

	Node* Temp;

	Temp = findlast();

	Node* Element = new Node(key);
	Element->Next = Root;
	Root->Prev = Element;
	Element->Prev = Temp;
	Temp->Next = Element;
}

void List::show()
{
	if (Root)
	{
		Node* Temp;
		Temp = Root;

		std::cout << Temp->Number << " ";
		Temp = Temp->Next;
		while (Temp != Root)
		{
			std::cout << Temp->Number << " ";
			Temp = Temp->Next;
		}
	}
}

Node* List::findlast()
{
	Node* temp;
	temp = Root;

	while (temp->Next != Root)
	{
		temp = temp->Next;
	}

	return temp;
}

void List::del(int key)
{
	if (Root)
	{
		Node *Temp;
		Temp = Root;
		while (Temp->Number != key)
		{
			Temp = Temp->Next;
		}

		if (Temp == Root)
		{
			Root = nullptr;
			Temp->~Node();
			return;
		}
		
		Temp->Prev->Next = Temp->Next;
		Temp->Next->Prev = Temp->Prev;
		Temp->~Node();
	}
}

void List::DEL(Node* Temp)
{
	if (Root)
	{
		if (Temp == Root)
		{
			Root->Next->Prev = Root->Prev;
			Root->Prev->Next = Root->Next;
			Root = Root->Next;
			Temp->~Node();
			return;
		}

		Temp->Prev->Next = Temp->Next;
		Temp->Next->Prev = Temp->Prev;
		Temp->~Node();
	}
}

//int main(void)
//{
//	List Kappa;
//
//	int n = 0;
//	int k = 0;
//	std::cin >> n;
//	std::cin >> k;
//
//	for (int i = 1; i <= n; i++)
//	{
//		Kappa.add(i);
//	}
//
//	Node *Temp;
//	Temp = Kappa.findlast();
//
//
//
//	for (int i = 1; i < n; i++)
//	{
//		for (int p = 1; p <= k; p++)
//		{
//			Temp = Temp->Next;
//		}
//
//		Kappa.DEL(Temp);
//	}
//
//	std::cout << Kappa.Root->Number;
//}

int SolvingFunc(int n, int k)
{
	if (n == 1) { return 1; }
	int temp = SolvingFunc(n - 1, k);
	int kappa = (temp + k - 1) % n + 1;
	return kappa;
}

int main(void)
{
	int n = 0, k = 0;
	std::cin >> n;
	std::cin >> k;

	int result = SolvingFunc(n, k);

	std::cout << result;
	return 0;
}