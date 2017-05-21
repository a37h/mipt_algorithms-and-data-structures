// Дано число N < 106 и последовательность целых чисел из [-231..231] длиной N.
// Требуется построить бинарное дерево, заданное наивным порядком вставки.
// Т.е., при добавлении очередного числа K в дерево с корнем root, если root→Key ≤ K, то узел K
// добавляется в правое поддерево root; иначе в левое поддерево root.

// 1_2. Выведите элементы в порядке pre-order (сверху вниз).

// Реализация с типом данных int (достаточно этого)
#include<stdio.h>
#include<iostream>
#include<cstddef>
#include<assert.h>

struct CNode {
	CNode* LeftChild;
	CNode* RightChild;
	CNode* Parent;
	int Key;

	void Clear()
	{
		delete LeftChild;
		delete RightChild;
	}

	CNode() : LeftChild(0), RightChild(0), Parent(0) {}
};

///////////////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////////
// Реализация ДИНАМИЧЕСКОГО МАССИВА ниже //
///////////////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////////

// Константа, задающая начальный размер буфера
#define INITIAL_CAPACITY 4

// Объявление класса
class MyVector {
private:
	int size;     // текущий размер массива
	int capacity; // вместимость буфера
	CNode *data;    // указатель на буфер, где хранятся элементы

	void grow();  // функция увеличения размера буфера

public:
	MyVector();                       // конструктор
									  //MyVector(int _capacity);          // конструктор, принимающий параметр (вместимость)
									  //MyVector(const MyVector &other);  // конструктор копирования
	~MyVector();                      // деструктор

									  //MyVector& operator=(const MyVector &other); // оператор присваивания копированием

	void push_back(CNode val); // объявление функции добавления элемента
	int get_size() const;    // функция, возвращающая размер массива

	CNode& operator[](int i);      // оператор доступа к элементу по индексу
	CNode operator[](int i) const; // оператор доступа к элементу по индексу
								 // (константный, без возможности
								 // модифицирования данных, нужен, например,
								 // для функции print_vector)

	void push_on_position(CNode val, int position);
};

// Реализация конструктора
MyVector::MyVector()
	: size(0),
	capacity(INITIAL_CAPACITY)
{

	data = new CNode[capacity];
}

// Реализация деструктора
MyVector::~MyVector()
{

	delete[] data;
}

// Реализация функции увеличения размера
void MyVector::grow()
{
	int new_capacity = capacity * 2;         // вычисляем размер нового буфера
	CNode *new_buffer = new CNode[new_capacity]; // выделяем новый буфер
	for (int i = 0; i < size; ++i)           // копируем данные в новый буфер
		new_buffer[i] = data[i];
	delete[] data;                           // удаляем старый буфер
	data = new_buffer;                       // заменяем указатель на буфер новым
	capacity = new_capacity;                 // обновляем вместимость
}

// Реализация функции добавления элемента в конец массива
void MyVector::push_back(CNode val)
{
	if (size + 1 > capacity)
		grow();

	data[size] = val;
	size++;
}

void MyVector::push_on_position(CNode val, int position)
{
	if (size + 1 > capacity)
		grow();

	data[position] = val;

	if (position == size)
		size++;
}

// Реализация функции получения размера вектора
int MyVector::get_size() const
{
	return size;
}

// Реализация оператора доступа к элементу
CNode& MyVector::operator[](int i)
{
	return data[i];
}

// Реализация константного оператора доступа к элементу
CNode MyVector::operator[](int i) const
{
	return data[i];
}

///////////////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////////
////////// Реализация СТЕКА ниже //////////
///////////////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////////

class CStack {

private:

	MyVector StackVector;
	int StackName; ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// NEWLINE
	int top; // Индекс верхнего элемента.

public:

	CStack() :		// Конструктор
		top(-1)
	{
		//MyVector StackVector();
		top = -1;
	}

	void Push(CNode T)
	{
		top++;
		StackVector.push_on_position(T, top);
	}

	CNode Pop()
	{

		if (top > -1)
		{
			CNode temp = StackVector[top];
			top--;
			return temp;
		}
		else
		{
			CNode kappa;
			return kappa;
		}

	}

	bool IsEmpty()
	{
		return top == -1;			// Проверка на пустоту
	}

};

class queue {
private:

	CStack stack1;
	CStack stack2;

public:
	queue()
	{
		;
	}

	CNode dequeue()
	{

		if (stack2.IsEmpty() == 1)
		{

			if (stack1.IsEmpty() == 1)
			{
				CNode *kappa = nullptr;
				return *kappa;
			}

			while (stack1.IsEmpty() == 0)
			{
				stack2.Push(stack1.Pop());
			}
			return stack2.Pop();

		}

		else

		{
			return stack2.Pop();
		}
	}

	void enqueue(CNode Element)
	{
		stack1.Push(Element);
	}

	bool IsEmpty()
	{
		if (stack1.IsEmpty())
		{
			if (stack2.IsEmpty())
			{
				return 1;
			}
			return 0;
		}
		return 0;
	}
};



class CMyTree {
public:
	CMyTree() : root(0) {}
	bool Add(const int& key);
	bool Has(const int& key) const;
	bool Delete(const int& key);
	bool IsEmpty() const { return (root == 0); };
	void iterativePreorder();
	void iterativePreorderCleaning();
	queue cleaningqueue;

private:
	CNode* root;

	bool add(CNode& current, const int& key);
	bool has(CNode* current, const int& key) const;
	static bool deleteInSubtree(CNode*& current, const int& key);
	static CNode* merge(CNode*left, CNode*right);
	static CNode& findMinChild(CNode& current);
};

///////////////////////////////////////////////////////// CMyTree :: Add

bool CMyTree::Add(const int& key)
{
	if (root == 0) {
		root = new CNode;
		root->Key = key;
		return true;
	}
	return add(*root, key);
}

///////////////////////////////////////////////////////// CMyTree :: add

bool CMyTree::add(CNode& current, const int& key)
{
	if (current.Key == key) {
		return false;
	}
	if (current.Key > key) {
		if (current.LeftChild == 0) {
			current.LeftChild = new CNode;
			current.LeftChild->Key = key;
			current.LeftChild->Parent = &current;
			return true;
		}
		return add(*current.LeftChild, key);
	}
	else {
		assert(current.Key < key);
		if (current.RightChild == 0) {
			current.RightChild = new CNode;
			current.RightChild->Key = key;
			current.RightChild->Parent = &current;
			return true;
		}
		return add(*current.RightChild, key);
	}
}

///////////////////////////////////////////////////////// CMyTree :: Has

bool CMyTree::Has(const int& key) const
{
	return has(root, key);
}

///////////////////////////////////////////////////////// CMyTree :: has

bool CMyTree::has(CNode* current, const int& key) const
{
	if (current == 0) {
		return false;
	}
	if (current->Key == key) {
		return true;
	}

	return has((current->Key < key ? current->LeftChild : current->RightChild), key);
}

///////////////////////////////////////////////////////// CMyTree :: Delete

bool CMyTree::Delete(const int& key)
{
	return deleteInSubtree(root, key);
}

///////////////////////////////////////////////////////// CMyTree :: deleteinsubtree

bool CMyTree::deleteInSubtree(CNode*& current, const int& key)
{
	if (current == 0)
		return false;
	if (current->Key == key) {
		CNode *newCurrent = merge(current->LeftChild, current->RightChild);
		current->LeftChild = 0;
		current->RightChild = 0;
		delete current;
		current = newCurrent;
		return true;
	}
	return deleteInSubtree(current->Key < key ? current->LeftChild : current->RightChild, key);

}

///////////////////////////////////////////////////////// CMyTree :: merge

CNode* CMyTree::merge(CNode*left, CNode*right)
{
	if ((left = 0) || (right == 0)) {
		return left == 0 ? right : left;
	}
	assert(left->Key < right->Key);
	CNode& rightMinChild = findMinChild(*right);
	rightMinChild.LeftChild = left;

	return right;
}

///////////////////////////////////////////////////////// CMyTree :: findMinChild

CNode& CMyTree::findMinChild(CNode& current)
{
	return current.LeftChild == 0 ? current : findMinChild(*(current.LeftChild));
}

// An iterative process to print preorder traversal of Binary tree
void CMyTree::iterativePreorder()
{
	if (&root == nullptr)
		return;

	CStack nodeStack;
	nodeStack.Push(*root);
	while (nodeStack.IsEmpty() == false)
	{
		struct CNode node = nodeStack.Pop();
		printf("%d ", node.Key);

		if (node.RightChild != nullptr)
			nodeStack.Push(*node.RightChild);
		if (node.LeftChild != nullptr)
			nodeStack.Push(*node.LeftChild);
	}
}

void CMyTree::iterativePreorderCleaning()
{
	cleaningqueue.enqueue(*root);        // корень в очередь
	while (cleaningqueue.IsEmpty() != 1) {
		CNode x = cleaningqueue.dequeue(); 
		if (x.Parent == nullptr)
		{
			x.Clear();
			return;
		}
		if (x.LeftChild != nullptr)   // x.left - левое поддерево
			cleaningqueue.enqueue(*x.LeftChild);
		if (x.RightChild != nullptr)  // x.right - правое поддерево
			cleaningqueue.enqueue(*x.RightChild);
		if (x.RightChild == nullptr)
		{
			if (x.LeftChild == nullptr)
			{
				cleaningqueue.enqueue(*x.Parent);
				x.Clear();
				cleaningqueue.dequeue();
			}
		}

	}
}

int main()
{
	CMyTree tree;

	int n = 0;
	std::cin >> n;
	int temp = 0;

	for (int i = 0; i < n; i++)
	{
		std::cin >> temp;
		tree.Add(temp);
	}

	tree.iterativePreorder();
	tree.iterativePreorderCleaning();

	return 0;
}