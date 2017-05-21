#include <vector>
#include <queue>
#include <iostream>
#include <math.h>

#include <fstream>
#include <iostream>

// храним ребра
struct CPair {
	int from, to, weight;

	explicit CPair(int _from, int _to, int _weight) :
		from(_from), to(_to), weight(_weight)
	{};
};

// кастомный компоратор для приоритикью
class mycomp
{
	bool rev;
public:
	mycomp(const bool& revpar = false)
	{
		rev = revpar;
	}
	bool operator()(const CPair& a, const CPair& b)
	{
		if (!rev)return a.weight>b.weight;
		else return a.weight<b.weight;
	}

};

// хранение графа
class CArcGraph {
public:
	explicit CArcGraph(int _verticesCount);
	CPair& SetEdge(bool which, int from, int to, int weight);
	void DragPairsQueue(std::priority_queue<CPair, std::vector<CPair>, mycomp>& TempQueue);
private:
	std::vector<CPair> GraphVerticesPairs;
	std::vector<CPair> MSTVerticesPairs;
	int GraphPairsAmount;
	int MSTPairsAmount;
	int verticesCount;
};

CArcGraph::CArcGraph(int _verticesCount) :
	verticesCount(_verticesCount),
	GraphPairsAmount(0),
	MSTPairsAmount(0) {}

CPair& CArcGraph::SetEdge(bool which, int from, int to, int weight) {
	CPair AddedPair(from, to, weight);
	if (which) {
		GraphVerticesPairs.push_back(AddedPair);
		GraphPairsAmount++;
	}
	else {
		MSTVerticesPairs.push_back(AddedPair);
		MSTPairsAmount++;
	}
	return AddedPair;
}

void CArcGraph::DragPairsQueue(std::priority_queue<CPair, std::vector<CPair>, mycomp>& TempQueue) {
	for (int i = 0; i < GraphPairsAmount; ++i) TempQueue.push(GraphVerticesPairs[i]);
	return;
}

class HashTable {
public:
	HashTable(int m_) :
		data(m_),
		m(m_)
	{
		data.resize(m);
		for (int i = 0; i < m; ++i)
			data[i] = nullptr;
	};

	void AddEntity(CPair* entity) {
		int from = entity->from;
		int to = entity->to;
		if (from > to)
		{
			int temp = from;
			from = to;
			to = temp;
		}
		int hash = Hash(from, to);
		if (IsPresent(from, to) == nullptr)
		{
			data[hash] = entity;
			return;
		}
		int offset = 1;
		CPair* temp = data[hash + offset];
		while (temp != nullptr)
		{
			temp = data[hash + offset];
			offset++;
		}
		data[hash] = entity;
	}

	CPair* IsPresent(int from, int to) {
		if (from > to)
		{
			int temp = from;
			from = to;
			to = temp;
		}

		int hash = Hash(from, to);
		CPair* temp = data[hash];
		if (temp == nullptr)
			return nullptr;
		int offset = 0;
		while ((temp != nullptr) && (!(temp->from == from && temp->to == to)))
		{
			temp = data[hash + offset];
			offset++;
		}
		return temp;
	}

private:
	int Hash(int a, int b) {
		double a_cos = abs(cos(a)), b_cos = abs(cos(b)), a_sin = abs(cos(a)), b_sin = abs(cos(b));
		double a_cos_twohungreed = abs(a_cos * 200000), b_cos_twohungreed = abs(a_cos * 200000);
		double hash_of_a = a_cos_twohungreed - a_cos * 10501 + a_sin * 300203;
		double hash_of_b = b_cos_twohungreed - b_cos * 30010 + b_sin * 506200;
		int result = abs(hash_of_a) + abs(hash_of_b);
		result = result % m;
		return result;
	}
	std::vector<CPair*> data;
	int m;
};

class Union {
public:
	Union(int n) : parent(n) {
		for (int i = 0; i < n; i++) parent[i] = i;
	}
	int find_set(int v) {
		if (v == parent[v]) return v;
		return find_set(parent[v]);
	}
	void union_sets(int a, int b) {
		a = find_set(a);
		b = find_set(b);
		if (a != b) parent[b] = a;
	}
private:
	std::vector<int> parent;
};

int main(void)
{
// <vvod>
	std::ifstream fin("kruskal.in");

	int n = 0, m = 0;
	fin >> n;
	fin >> m;
	CArcGraph graph(n);
	HashTable hasht(m);
	CPair* Temp;
	for (int i = 0; i < m; i++)
	{
		int temp1 = 0, temp2 = 0, temp3 = 0;
		fin >> temp1;
		temp1--;
		fin >> temp2;
		temp2--;
		fin >> temp3;

		Temp = hasht.IsPresent(temp1, temp2);

		if (Temp == nullptr)
		{
			CPair* temp = &graph.SetEdge(1, temp1, temp2, temp3);
			hasht.AddEntity(temp);
		}
		else
		{
			if (Temp->weight > temp3)
				Temp->weight = temp3;
		}
	}
	fin.close();
// </vvod>

	std::priority_queue<CPair, std::vector<CPair>, mycomp> g;
	graph.DragPairsQueue(g);

	int cost = 0;

	Union Union_(n);

	for (int i = 0; i < n; ++i)
		Union_.union_sets(i, i);
	for (int i = 0; i < m; ++i) {
		int a = g.top().from;
		int b = g.top().to;
		int l = g.top().weight;
		g.pop();
		if (Union_.find_set(a) != Union_.find_set(b)) {
			cost += l;
			graph.SetEdge(0, g.top().from, g.top().to, g.top().weight);
			Union_.union_sets(a, b);
		}
	}

	std::ofstream fout("kruskal.out");
	fout << cost;
	fout.close();
	return 0;
}