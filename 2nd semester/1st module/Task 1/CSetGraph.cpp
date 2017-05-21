#include "CSetGraph.h"

CSetGraph::CSetGraph(int _verticesCount) :
	verticesCount(_verticesCount),
	vertices(verticesCount)
{
}

CSetGraph::CSetGraph(CSetGraph *input) :
	verticesCount(input->verticesCount),
	vertices(input->vertices)
{
}

void CSetGraph::AddEdge(int from, int to)
{
	vertices[from].insert(to);
}

void CSetGraph::GetNextVertices(int vertex, std::vector<int>& _vertices) const
{
	if (_vertices.empty()!=1)
	_vertices.clear();

	std::list<int>::const_iterator i;

	for (i = vertices[vertex].begin(); i != vertices[vertex].end(); i++)
	{
		_vertices.push_back(*i);
	}
}

void CSetGraph::GetPrevVertices(int vertex, std::vector<int>& _vertices) const
{
	if (_vertices.empty() != 1)
		_vertices.clear();

	for (unsigned int i = 1; i < verticesCount; i++)
	{
		std::list<int>::const_iterator j;

		for (j = vertices[i].begin(); j != vertices[i].end(); j++) {
			if (*j == vertex) {
				_vertices.push_back(i);
				break;
			}
		}
	}
}