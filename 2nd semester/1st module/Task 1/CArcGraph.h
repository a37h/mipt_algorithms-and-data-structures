#pragma once

#include "Graph.h"
#include "CPair.h"

class CArcGraph : public IGraph {
public:
	explicit CArcGraph(int _verticesCount);
	explicit CArcGraph(CArcGraph *input);

	virtual void AddEdge(int from, int to) override;

	virtual int VerticesCount() const override { return verticesCount; }
	int PairsAmount() { return pairsAmount; }

	virtual void GetNextVertices(int vertex, std::vector<int>& vertices) const override;
	virtual void GetPrevVertices(int vertex, std::vector<int>& vertices) const override;

private:
	std::vector<CPair> VerticesPairs;
	int pairsAmount;
	int verticesCount;
};