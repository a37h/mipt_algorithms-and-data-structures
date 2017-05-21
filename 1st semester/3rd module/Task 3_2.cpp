#include <iostream>
#include <queue>
#include <stack>
#include <vector>

template <class T>
class AVLNode {
public:
	AVLNode(const T& value) : data_(value), left_(nullptr), right_(nullptr), parent_(nullptr), Count(1) {}
	~AVLNode()
	{
	}

	void	  SetValue(int value) { data_ = value; }
	const T&  GetValue() const { return data_; }

	void      SetLeft(AVLNode* left) { left_ = left; }
	AVLNode*  GetLeft() const { return left_; }

	void      SetRight(AVLNode* right) { right_ = right; }
	AVLNode*  GetRight() const { return right_; }

	void      SetParent(AVLNode* parent) { parent_ = parent; }
	AVLNode*  GetParent() const { return parent_; }

	void      Print() const { std::cout << data_ << std::endl; }

	void IncreaseCount() { Count++; };
	AVLNode();

	T     data_;
	AVLNode* left_;
	AVLNode* right_;
	AVLNode* parent_;
	int Count;
	int CountLeft;
	int CountRight;

private:
};

template <class T>
class AVLTree {
public:
	AVLTree() : root_(NULL) {}
	~AVLTree();

	bool Insert(const T& value);

	AVLNode<T>* GetRoot() const { return root_; }

	AVLNode<T>* Find(AVLNode<T>* root, const T& value) const;
	AVLNode<T>* KStat(AVLNode<T>* root, int keystat);

	int  Height(AVLNode<T>* root) const;
	int  BalanceFactor(AVLNode<T>* root) const;

	void RotateLeft(AVLNode<T>* root);
	void RotateRight(AVLNode<T>* root);
	AVLNode<T>* FindMin(AVLNode<T>* root);
	void Delete(int key);
	void PrintPreOrder(AVLNode<T>* root) const; // Parent, Left, Right
	void PrintInOrder(AVLNode<T>* root) const; // Left, Parent, Right
	void CleanInTheONES(AVLNode<T>* root) const; // Left, Parent, Right
	void APrintPostOrder(AVLNode<T>* root) const;
	void PrintPostOrder(AVLNode<T>* root) const; // Left, Right, Parent
	void BalanceRoot(AVLNode<T>* root);
	void PrintBreadthSearchFirst() const;
	void CountPostOrder(AVLNode<T>* root) const;
	void countPostOrder(AVLNode<T>* root) const;
	void InsertAVLNode(AVLNode<T>* root, AVLNode<T>* ins);
	void DeleteAVLNode(AVLNode<T>* node);
	void BALANCETOTHETOP(AVLNode<T>* root);
	AVLNode<T>* root_;

private:
};


template <class T>
void AVLTree<T>::CountPostOrder(AVLNode<T>* root) const {
	if (root) {
		CountPostOrder(root->GetLeft());  // Left
		CountPostOrder(root->GetRight()); // Right

		if (root->GetParent()) { // Если у корня с которым мы сейчас работаем есть родитель (т.е. мы не в вершине)
			countPostOrder(root->GetParent());
		}
	}
}
//
template <class T>
void AVLTree<T>::countPostOrder(AVLNode<T>* root) const
{
	if (root != nullptr)
	root->IncreaseCount();

	if (root != nullptr) { // Если у корня с которым мы сейчас работаем есть родитель (т.е. мы не в вершине)
		countPostOrder(root->GetParent());
	}
}

template <class T>
AVLTree<T>::~AVLTree() {
	if (root_) {
		DeleteAVLNode(root_);
	}
}

template <class T>
void AVLTree<T>::DeleteAVLNode(AVLNode<T>* node) {
	if (node) {
		DeleteAVLNode(node->GetLeft());
		DeleteAVLNode(node->GetRight());
		delete node; // Post Order Deletion
	}
}

template <class T>
bool AVLTree<T>::Insert(const T& value) {
	AVLNode<T>* new_node = new (std::nothrow) AVLNode<T>(value);

	if (!new_node)
		return true; // Out of memory

	if (!root_) // Special case
	{
		root_ = new_node;
	}
	else
		InsertAVLNode(root_, new_node);

	return false;
}

template <class T>
void AVLTree<T>::InsertAVLNode(AVLNode<T>* root, AVLNode<T>* ins) {
	// Binary Search Tree insertion algorithm
	if (ins->GetValue() <= root->GetValue()) {
		if (root->GetLeft()) // If there is a left child, keep searching
		{
			InsertAVLNode(root->GetLeft(), ins);
		}
		else { // Found the right spot
			root->SetLeft(ins);
			ins->SetParent(root);
		}
	}
	else {
		if (root->GetRight()) // If there is a right child, keep searching
		{
			InsertAVLNode(root->GetRight(), ins);
		}
		else {// Found the right spot
			root->SetRight(ins);
			ins->SetParent(root);
		}
	}

	// AVL balancing algorithm
	BalanceRoot(root);
}

template <class T>
void AVLTree<T>::PrintPreOrder(AVLNode<T>* root) const {
	if (root) {
		root->Print();                   // Parent
		PrintPreOrder(root->GetLeft());  // Left
		PrintPreOrder(root->GetRight()); // Right
	}
}

template <class T>
void AVLTree<T>::PrintInOrder(AVLNode<T>* root) const {
	if (root) {
		PrintInOrder(root->GetLeft());  // Left
		root->Print();                  // Parent
		PrintInOrder(root->GetRight()); // Right
	}
}
template <class T>
void AVLTree<T>::APrintPostOrder(AVLNode<T>* root) const {
	if (root) {
		APrintPostOrder(root->GetLeft());  // Left
		APrintPostOrder(root->GetRight()); // Right
		root->Print();                    // Parent
		std::cout << " it has " << root->Count << " elements in his subtree + 1" << std::endl;
	}
}


// Depth-First Search
template <class T>
AVLNode<T>* AVLTree<T>::Find(AVLNode<T>* root, const T& value) const {
	if (root) {
		//std::cout << root->GetValue() << std::endl;
		if (root->GetValue() == value)
			return root; // Found
		else if (value < root->GetValue())
			return Find(root->GetLeft(), value);
		else
			return Find(root->GetRight(), value);
	}

	return NULL;
}

template <class T>
AVLNode<T>* AVLTree<T>::KStat(AVLNode<T>* root, int keystat) {
	if (root) {
		int mid = 0;
		if (root->GetLeft())
			mid = (root->GetLeft()->Count);

		if (mid == keystat)
		{
			return root;
		}
		else if (keystat < mid)
		{
			return KStat(root->GetLeft(), keystat);
		}
		else
		{
			return KStat(root->GetRight(), keystat - mid - 1);
		}
	}
	else
		return nullptr;
}

template <class T>
int AVLTree<T>::Height(AVLNode<T>* root) const {
	int height = 0;
	if (root) {
		int left = Height(root->GetLeft());
		int right = Height(root->GetRight());
		height = 1 + ((left > right) ? left : right);
	}
	return height;
}

template <class T>
int  AVLTree<T>::BalanceFactor(AVLNode<T>* root) const {
	int balance = 0;
	if (root) {
		balance = Height(root->GetLeft()) - Height(root->GetRight());
	}
	return balance;
}

template <class T>
void AVLTree<T>::RotateLeft(AVLNode<T>* root) {
	AVLNode<T>* newroot = root->GetRight();
	root->SetRight(newroot->GetLeft());
	newroot->SetParent(root->GetParent());
	newroot->SetLeft(root);

	if (root->GetParent() == NULL) {
		root_ = newroot;
		newroot->SetParent(NULL);
	}
	else {
		if (root->GetParent()->GetLeft() == root) {
			root->GetParent()->SetLeft(newroot);
		}
		else {
			root->GetParent()->SetRight(newroot);
		}
	}
}

template <class T>
void AVLTree<T>::RotateRight(AVLNode<T>* root) {
	// Rotate node
	AVLNode<T>* newroot = root->GetLeft();
	root->SetLeft(newroot->GetRight());
	newroot->SetParent(root->GetParent());
	newroot->SetRight(root);

	// Adjust tree
	if (root->GetParent() == NULL) {
		root_ = newroot;
		newroot->SetParent(NULL);
	}
	else {
		if (root->GetParent()->GetLeft() == root) {
			root->GetParent()->SetLeft(newroot);
		}
		else {
			root->GetParent()->SetRight(newroot);
		}
	}
}

template <class T>
void AVLTree<T>::CleanInTheONES(AVLNode<T>* root) const {
	if (root) {
		root->Count = 1;                   // Parent
		CleanInTheONES(root->GetLeft());  // Left
		CleanInTheONES(root->GetRight()); // Right
	}
}

template <class T>
AVLNode<T>* AVLTree<T>::FindMin(AVLNode<T>* root)
{
	while (root->GetLeft())
	{
		root = root->GetLeft();
	}
	return root;
}

template <class T>
void AVLTree<T>::BalanceRoot(AVLNode<T>* root)
{
	int balance = BalanceFactor(root);
	if (balance > 1)		// left tree unbalanced
	{
		if (BalanceFactor(root->GetLeft()) < 0) // right child of left tree is the cause
			RotateLeft(root->GetLeft()); // double rotation required
		RotateRight(root);
	}
	else if (balance < -1)	// right tree unbalanced
	{
		if (BalanceFactor(root->GetRight()) > 0) // left child of right tree is the cause
			RotateRight(root->GetRight());
		RotateLeft(root);
	}
}

template <class T>
void AVLTree<T>::BALANCETOTHETOP(AVLNode<T>* root)
{
	BalanceRoot(root);
	AVLNode<int> *Temp;
	Temp = root;
	while (Temp->GetParent())
	{
		Temp = Temp->GetParent();
		BalanceRoot(Temp);
	}
}

template <class T>
void AVLTree<T>::Delete(int key)
{
	AVLNode<int> *Node;
	Node = Find(root_, key);

	if (Node == nullptr)
		return;

	// Итак, мы нашли узел который нужно удалить
	// У нас есть варианты: он - лист, у него есть правое поддерево, есть левое поддерево, есть оба поддерева]

	if (Node == root_)
	{

		if ((Node->GetLeft() == nullptr) && (Node->GetRight() == nullptr))
		{
			root_ = nullptr;
			Node->~AVLNode();
		}

		// Если левый и правый ребенки не нулевые
		if ((Node->GetLeft() != nullptr) && (Node->GetRight() != nullptr))
		{
			AVLNode<int> *TempNode;
			TempNode = FindMin(Node->GetRight());
			Node->data_ = TempNode->data_;

			if (TempNode == Node->GetRight())
			{
				Node->SetRight(nullptr);
				Node->GetRight()->~AVLNode();
			}
			else
			{
				TempNode->GetParent()->SetLeft(nullptr);
				TempNode->~AVLNode();
			}

			BALANCETOTHETOP(Node);
			return;
		}

		// Если левый не нулевой
		if (Node->GetLeft() != nullptr)
		{
			root_ = root_->left_;
			BALANCETOTHETOP(Node);
			return;
		}

		// Если правый не нулевой
		if (Node->GetRight() != nullptr)
		{
			root_ = root_->right_;
			BALANCETOTHETOP(Node);
			return;
		}

	}
	else
	{

		if ((Node->GetLeft() == nullptr) && (Node->GetRight() == nullptr))
		{
			if (Node->parent_->left_ == Node)	// Если это левый ребенок родителя
			{
				Node->parent_->left_ = nullptr;
				Node->~AVLNode();
			}
			if (Node->parent_->right_ == Node)	// Если это правый ребенок родителя
			{
				Node->parent_->right_ = nullptr;
				Node->~AVLNode();
			}
			BALANCETOTHETOP(Node);
			return;
		}

		// Если левый и правый ребенки не нулевые
		if ((Node->GetLeft() != nullptr) && (Node->GetRight() != nullptr))
		{
			AVLNode<int> *TempNode;
			TempNode = FindMin(Node->GetRight());
			Node->data_ = TempNode->data_;

			if (TempNode == Node->GetRight())
			{
				Node->SetRight(nullptr);
				Node->GetRight()->~AVLNode();
			}
			else
			{
				TempNode->GetParent()->SetLeft(nullptr);
				TempNode->~AVLNode();
			}

			BALANCETOTHETOP(Node);
			return;
		}

		// Если левый не нулевой
		if (Node->GetLeft() != nullptr)
		{
			Node->data_ = Node->left_->data_;
			Node->left_ = Node->left_->GetLeft();
			Node->right_ = Node->left_->GetRight();
			BALANCETOTHETOP(Node);
			return;
		}

		// Если правый не нулевой
		if (Node->GetRight() != nullptr)
		{
			Node->data_ = Node->right_->data_;
			Node->left_ = Node->right_->GetLeft();
			Node->right_ = Node->right_->GetRight();
			BALANCETOTHETOP(Node);
			return;
		}
	}
}

int main(void)
{
		AVLTree<int> Kappa;
		std::vector<int> keeeeepo;

		int n = 0;
		std::cin >> n;

		for (int i = 0; i < n; i++)
		{
			int temp1 = 0;
			int temp2 = 0;

			std::cin >> temp1;
			std::cin >> temp2;

			if (temp1 > 0)
			{
				Kappa.Insert(temp1);
			}
			if (temp1 < 0)
			{
				Kappa.Delete(-temp1);
			}


			Kappa.CleanInTheONES(Kappa.GetRoot());
			Kappa.CountPostOrder(Kappa.GetRoot());
			AVLNode<int> *Temp;
			Temp = Kappa.KStat(Kappa.GetRoot(), temp2);
			keeeeepo.push_back(Temp->GetValue());
		}

		for (int i = 0; i < n; i++)
		{
			std::cout << keeeeepo[i] << std::endl;
		}

	getchar();
	getchar();
}