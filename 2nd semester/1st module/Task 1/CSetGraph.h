#pragma once

#include "Graph.h"
#include <vector>
#include <unordered_set>

class CSetGraph : public IGraph {
public:
	explicit CSetGraph(int _verticesCount);
	explicit CSetGraph(CSetGraph *input);

	virtual void AddEdge(int from, int to) override;

	virtual int VerticesCount() const override { return verticesCount; }

	virtual void GetNextVertices(int vertex, std::vector<int>& _vertices) const override;
	virtual void GetPrevVertices(int vertex, std::vector<int>& _vertices) const override;

private:
	int verticesCount;
	std::vector<std::unordered_set<int>> vertices;
};