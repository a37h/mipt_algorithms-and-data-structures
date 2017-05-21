#include "CListGraph.h"

#include <algorithm>
#include <assert.h>

CListGraph::CListGraph(int _verticesCount) :
	verticesCount(_verticesCount),
	in(verticesCount),
	out(verticesCount)
{
}

CListGraph::CListGraph(CListGraph *input) :
	verticesCount(input->verticesCount),
	in(input->in),
	out(input->out)
{
}

void CListGraph::AddEdge(int from, int to)
{
	in[to].push_back(from);
	out[from].push_back(to);
}

void CListGraph::GetNextVertices(int vertex, std::vector<int>& vertices) const
{
	if (vertices.empty() != 1)
		vertices.clear();

	vertices = out[vertex];
}

void CListGraph::GetPrevVertices(int vertex, std::vector<int>& vertices) const
{
	if (vertices.empty() != 1)
		vertices.clear();

	vertices = in[vertex];
}