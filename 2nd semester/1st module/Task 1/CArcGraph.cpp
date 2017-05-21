#include "CArcGraph.h"

#include <iostream>

CArcGraph::CArcGraph(int _verticesCount) :
	verticesCount(_verticesCount),
	pairsAmount(0)
{
}

CArcGraph::CArcGraph(CArcGraph *input) :
	verticesCount(input->VerticesCount()),
	pairsAmount(input->PairsAmount()),
	VerticesPairs(input->VerticesPairs)
{
}

void CArcGraph::AddEdge(int from, int to)
{
	CPair AddedPair(from, to);
	VerticesPairs.push_back(AddedPair);
	pairsAmount++;
}

void CArcGraph::GetNextVertices(int vertex, std::vector<int>& vertices) const
{
	if (vertices.empty() != 1)
		vertices.clear();

	for (int i = 0; i < pairsAmount; i++)
		if (VerticesPairs[i].from == vertex) vertices.push_back(VerticesPairs[i].to);
}

void CArcGraph::GetPrevVertices(int vertex, std::vector<int>& vertices) const
{
	if (vertices.empty() != 1)
		vertices.clear();

	for (int i = 0; i < pairsAmount; i++)
		if (VerticesPairs[i].to == vertex) vertices.push_back(VerticesPairs[i].from);
}