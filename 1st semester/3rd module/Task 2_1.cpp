//Дано число N < 106 и последовательность пар целых чисел из[-231..231] длиной N.
//Построить декартово дерево из N узлов, характеризующихся парами чисел{ Xi, Yi }.
//Каждая пара чисел{ Xi, Yi } определяет ключ Xi и приоритет Yi в декартовом дереве.
//Добавление узла в декартово дерево выполняйте второй версией алгоритма, рассказанного на лекции :
//При добавлении узла(x, y) выполняйте спуск по ключу до узла P с меньшим приоритетом.Затем разбейте
//найденное поддерево по ключу x так, чтобы в первом поддереве все ключи меньше x, а во втором больше или 
//равны x.Получившиеся два дерева сделайте дочерними для нового узла(x, y).Новый узел вставьте на место узла P.
//
//
//Построить также наивное дерево поиска по ключам Xi методом из задачи 2.
//
//
//2_1.Вычислить разницу глубин наивного дерева поиска и декартового дерева.Разница может быть отрицательна.

#include<iostream>

struct CNode {
	CNode* LeftChild;
	CNode* RightChild;
	CNode* Parent;
	int Key;
	int depth;

	void Clear()
	{
		delete LeftChild;
		delete RightChild;
	}

	CNode() : LeftChild(0), RightChild(0), Parent(0), depth(1) {}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////Реализация ДИНАМИЧЕСКОГО МАССИВА ниже//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////Реализация СТЕКА ниже//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////Реализация ОЧЕРЕДИ ниже//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////Реализация бинарного дерева////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



class CMyTree {
public:
	int MaxDepth = 0;
	int GetMaxDepth();
	CMyTree() : root(0) {}
	bool Add(const int& key);
	bool Has(const int& key) const;
	bool Delete(const int& key);
	bool IsEmpty() const { return (root == 0); };
	void iterativePreorder();
	void Clear();
	queue cleaningqueue;

private:
	CNode* root;

	bool add(CNode& current, const int& key);
	bool has(CNode* current, const int& key) const;
	static bool deleteInSubtree(CNode*& current, const int& key);
	static CNode* merge(CNode*left, CNode*right);
	static CNode& findMinChild(CNode& current);
	static void preorder(CNode& tree);
	int getmaxdepth();
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

void CMyTree::Clear()
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

int CMyTree::GetMaxDepth()
{
	if (&root == nullptr)
		return 0;

	CStack nodeStack;
	nodeStack.Push(*root);
	root->depth = 1;

	while (nodeStack.IsEmpty() == false)
	{
		struct CNode node = nodeStack.Pop();
		

		if (node.RightChild != nullptr)
		{
			nodeStack.Push(*node.RightChild);
			node.RightChild->depth = node.RightChild->Parent->depth + 1;
			
		}
		if (node.LeftChild != nullptr)
		{
			nodeStack.Push(*node.LeftChild);
			node.LeftChild->depth = node.LeftChild->Parent->depth + 1;
		}
	}

	return getmaxdepth();
}

int CMyTree::getmaxdepth()
{
	CStack nodeStack;
	nodeStack.Push(*root);

	while (nodeStack.IsEmpty() == false)
	{
		struct CNode node = nodeStack.Pop();
		if (node.depth > MaxDepth)
		{
			MaxDepth = node.depth;
		}

		if (node.RightChild != nullptr)
		{
			nodeStack.Push(*node.RightChild);
			//if (node.RightChild->depth > MaxDepth)
			//{
			//	MaxDepth = node.RightChild->depth;
			//}
		}
		if (node.LeftChild != nullptr)
		{
			nodeStack.Push(*node.LeftChild);
			//if (node.LeftChild->depth > MaxDepth)
			//{
			//	MaxDepth = node.LeftChild->depth;
			//}
		}
	}

	return MaxDepth;
}//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
///////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////Реализация декартового дерева ниже//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct CTreapDekartNode {
	CTreapDekartNode* LeftChild;
	CTreapDekartNode* RightChild;
	CTreapDekartNode* Parent;
	int Key;
	int Priority;
	int depth;

	void Clear()
	{
		delete LeftChild;
		delete RightChild;
	}

	CTreapDekartNode() : LeftChild(0), RightChild(0), Parent(0), Priority(0), depth(1) {}

	CTreapDekartNode(int _key, int _priority) : Key(_key), Priority(_priority),
		LeftChild(0), RightChild(0) {}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////Реализация ДИНАМИЧЕСКОГО МАССИВА ниже//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Константа, задающая начальный размер буфера
#define INITIAL_CAPACITY 4

// Объявление класса
class MyVectorDekart {
private:
	int size;     // текущий размер массива
	int capacity; // вместимость буфера
	CTreapDekartNode *data;    // указатель на буфер, где хранятся элементы

	void grow();  // функция увеличения размера буфера

public:
	MyVectorDekart();                       // конструктор
											//MyVectorDekart(int _capacity);          // конструктор, принимающий параметр (вместимость)
											//MyVectorDekart(const MyVectorDekart &other);  // конструктор копирования
	~MyVectorDekart();                      // деструктор

											//MyVectorDekart& operator=(const MyVectorDekart &other); // оператор присваивания копированием

	void push_back(CTreapDekartNode val); // объявление функции добавления элемента
	int get_size() const;    // функция, возвращающая размер массива

	CTreapDekartNode& operator[](int i);      // оператор доступа к элементу по индексу
	CTreapDekartNode operator[](int i) const; // оператор доступа к элементу по индексу
											  // (константный, без возможности
											  // модифицирования данных, нужен, например,
											  // для функции print_vector)

	void push_on_position(CTreapDekartNode val, int position);
};

// Реализация конструктора
MyVectorDekart::MyVectorDekart()
	: size(0),
	capacity(INITIAL_CAPACITY)
{

	data = new CTreapDekartNode[capacity];
}

// Реализация деструктора
MyVectorDekart::~MyVectorDekart()
{

	delete[] data;
}

// Реализация функции увеличения размера
void MyVectorDekart::grow()
{
	int new_capacity = capacity * 2;         // вычисляем размер нового буфера
	CTreapDekartNode *new_buffer = new CTreapDekartNode[new_capacity]; // выделяем новый буфер
	for (int i = 0; i < size; ++i)           // копируем данные в новый буфер
		new_buffer[i] = data[i];
	delete[] data;                           // удаляем старый буфер
	data = new_buffer;                       // заменяем указатель на буфер новым
	capacity = new_capacity;                 // обновляем вместимость
}

// Реализация функции добавления элемента в конец массива
void MyVectorDekart::push_back(CTreapDekartNode val)
{
	if (size + 1 > capacity)
		grow();

	data[size] = val;
	size++;
}

void MyVectorDekart::push_on_position(CTreapDekartNode val, int position)
{
	if (size + 1 > capacity)
		grow();

	data[position] = val;

	if (position == size)
		size++;
}

// Реализация функции получения размера вектора
int MyVectorDekart::get_size() const
{
	return size;
}

// Реализация оператора доступа к элементу
CTreapDekartNode& MyVectorDekart::operator[](int i)
{
	return data[i];
}

// Реализация константного оператора доступа к элементу
CTreapDekartNode MyVectorDekart::operator[](int i) const
{
	return data[i];
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////Реализация СТЕКА ниже//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CStackDekart {

private:

	MyVectorDekart StackVector;
	int StackName; ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// NEWLINE
	int top; // Индекс верхнего элемента.

public:

	CStackDekart() :		// Конструктор
		top(-1)
	{
		//MyVectorDekart StackVector();
		top = -1;
	}

	void Push(CTreapDekartNode T)
	{
		top++;
		StackVector.push_on_position(T, top);
	}

	CTreapDekartNode Pop()
	{

		if (top > -1)
		{
			CTreapDekartNode temp = StackVector[top];
			top--;
			return temp;
		}
		else
		{
			CTreapDekartNode kappa;
			return kappa;
		}

	}

	bool IsEmpty()
	{
		return top == -1;			// Проверка на пустоту
	}

};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////Реализация ОЧЕРЕДИ ниже//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class queueDekart {
private:

	CStackDekart stack1;
	CStackDekart stack2;

public:
	queueDekart()
	{
		;
	}

	CTreapDekartNode dequeueDekart()
	{

		if (stack2.IsEmpty() == 1)
		{

			if (stack1.IsEmpty() == 1)
			{
				CTreapDekartNode *kappa = nullptr;
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

	void enqueueDekart(CTreapDekartNode Element)
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

// Класс декартова дерева.
class CTreapDekart {
public:
	int MaxDepth;
	// Конструктор создает пустое дерево.
	CTreapDekart() : root(0), MaxDepth(0) {}
	queueDekart cleaningqueueDekart;
	// Деструктор удаляет все элементы дерева.

	// Добавить узел (key, priority) в дерево.
	// key и priority должны быть уникальны по определению декартова дерева.
	void Insert(int key, int priority);
	// Вывести дерево в консоль обходом слева-направо.
	// Вывести дерево в консоль, положив его не бок: родитель находится слева, "левый"
	// ребенок располагается сверху справа от него, "правый" ребенок - снизу справа.
	void Clear();
	int getmaxdepth();
	int GetMaxDepth();

	~CTreapDekart();

private:
	CTreapDekartNode* root; // Корень дерева.

	void deleteSubTree(CTreapDekartNode* node);
	void split(CTreapDekartNode* node, int key, CTreapDekartNode*& leftTree, CTreapDekartNode*& rightTree);
	CTreapDekartNode* merge(CTreapDekartNode* leftTree, CTreapDekartNode* rightTree);
	void clear(CTreapDekartNode* current);
	void getmaxdepth(CTreapDekartNode& tree, int tempdepth);
};

// Разделяет кучу на две части.
void CTreapDekart::split(CTreapDekartNode* node, int key, CTreapDekartNode*& leftTree, CTreapDekartNode*& rightTree)
{
	if (node == 0) {
		leftTree = 0;
		rightTree = 0;
		return;
	}

	if (node->Key <= key) {
		split(node->RightChild, key, node->RightChild, rightTree);
		leftTree = node;
	}
	else {
		split(node->LeftChild, key, leftTree, node->LeftChild);
		rightTree = node;
	}
}

// Сливает два декартова дерева в одно. Элементы в leftTree должны быть меньше элементов rightTree.
CTreapDekartNode* CTreapDekart::merge(CTreapDekartNode* leftTree, CTreapDekartNode* rightTree)
{
	if (leftTree == 0) {
		return rightTree;
	}
	if (rightTree == 0) {
		return leftTree;
	}

	if (rightTree->Priority > leftTree->Priority) {
		rightTree->LeftChild = merge(leftTree, rightTree->LeftChild);
		return rightTree;
	}
	else {
		leftTree->RightChild = merge(leftTree->RightChild, rightTree);
		return leftTree;
	}
}

// Реализована неэффективная версия вставки.
void CTreapDekart::Insert(int key, int priority)
{
	if (root == 0) {
		root = new CTreapDekartNode(key, priority);
		return;
	}

	CTreapDekartNode* leftTree;
	CTreapDekartNode* rightTree;
	split(root, key, leftTree, rightTree);

	root = merge(leftTree, new CTreapDekartNode(key, priority));
	root = merge(root, rightTree);
	if (leftTree != nullptr)
		leftTree->Parent = root;
	if (rightTree != nullptr)
		rightTree->Parent = root;
}


void CTreapDekart::Clear()
{
	if (root == nullptr)
	{
		return;
	}
	if (root->LeftChild != nullptr)
		clear(root->LeftChild);

	if (root->RightChild != nullptr)
		clear(root->RightChild);

	delete root;
	root = nullptr;
}

void CTreapDekart::clear(CTreapDekartNode* current)
{
	if (current->LeftChild != nullptr)
		clear(current->LeftChild);
	if (current->RightChild != nullptr)
		clear(current->RightChild);

	delete current;
}

CTreapDekart::~CTreapDekart()
{
	deleteSubTree(root);
}

// Разрушает поддерево, корнем которого является node.
void CTreapDekart::deleteSubTree(CTreapDekartNode* node)
{
	if (node == 0) {
		// Дошли до конца дерева.
		return;
	}

	deleteSubTree(node->LeftChild);
	node->LeftChild = 0;
	deleteSubTree(node->RightChild);
	node->RightChild = 0;

	delete node;
}


int CTreapDekart::GetMaxDepth()
{
	if (root == 0) { return 0; }


	if ((root->LeftChild == nullptr) && (root->RightChild == nullptr))
	{
		return 1;
	}

	if (root->LeftChild != 0)
	{
		getmaxdepth(*root->LeftChild, 2);
	}

	if (root->RightChild != 0)
	{
		getmaxdepth(*root->RightChild, 2);
	}

	return MaxDepth;
}

void CTreapDekart::getmaxdepth(CTreapDekartNode& tree, int tempdepth)
{
	if (MaxDepth < tempdepth)
	{
		MaxDepth = tempdepth;
	}

	if (tree.LeftChild != 0)
	{
		getmaxdepth(*tree.LeftChild, tempdepth + 1);
	}

	if (tree.RightChild != 0)
	{
		getmaxdepth(*tree.RightChild, tempdepth + 1);
	}

	return;
}

int main() {
	int n = 0;
	std::cin >> n;

	CMyTree TreeBinary;
	CTreapDekart TreeTreap;

	for (int i = 0; i < n; i++)
	{
		int temp12 = 0;
		std::cin >> temp12;
		TreeBinary.Add(temp12);
		int temp2;
		std::cin >> temp2;
		TreeTreap.Insert(temp12, temp2);
	}

	int TreeBinaryMaxDepth = TreeBinary.GetMaxDepth();
	int TreeTreapTwoMaxDepth = TreeTreap.GetMaxDepth();

	std::cout << std::endl << TreeBinaryMaxDepth - TreeTreapTwoMaxDepth;

	TreeBinary.Clear();
	TreeTreap.Clear();

	return 0;
}