#pragma once

#include <vector>

struct IGraph {
	// Дестуктор пустой, т.к. он не нужен
	virtual ~IGraph() {}

	// Добавление направленной грани
	virtual void AddEdge(int from, int to) = 0;

	// Получение количества вершин
	virtual int VerticesCount() const = 0;

	// Получение списка вершин которые достижимы за 1 шаг из данной
	virtual void GetNextVertices(int vertex, std::vector<int>& vertices) const = 0;
	// Получение списка вершин которые достигают данную за 1 шаг
	virtual void GetPrevVertices(int vertex, std::vector<int>& vertices) const = 0;
};