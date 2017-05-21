#include "CMatrixGraph.h"
#include <iostream>

MatrixGraph::MatrixGraph(int _verticesCount):
	verticesCount(_verticesCount),
	edges(_verticesCount)
{
	for (int i = 0; i < verticesCount; i++)
	{
		edges[i].resize(verticesCount);
	}
}

MatrixGraph::MatrixGraph(MatrixGraph *input) :
	verticesCount(input->verticesCount),
	edges(input->edges)
{
	
}

void MatrixGraph::AddEdge(int from, int to)
{
	edges[from][to] = 1;
}

void MatrixGraph::GetNextVertices(int vertex, std::vector<int>& vertices) const
{
	if (vertices.empty() != 1)
		vertices.clear();
	for (int i = 0; i < verticesCount; i++)
		if (edges[vertex][i] == 1) vertices.push_back(i);
}

void MatrixGraph::GetPrevVertices(int vertex, std::vector<int>& vertices) const
{
	if (vertices.empty() != 1)
		vertices.clear();
	for (int i = 0; i < verticesCount; i++)
		if (edges[i][vertex] == 1) vertices.push_back(i);
}