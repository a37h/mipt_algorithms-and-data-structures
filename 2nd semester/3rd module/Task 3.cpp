#include <fstream>
#include <ostream>
#include <algorithm>
#include <vector>
#include <climits>

class DinicGraph {
public:
	explicit DinicGraph(int _verticesCount) : sizes(_verticesCount), flows(_verticesCount) {
		for (int i = 0; i < _verticesCount; i++) {
			sizes[i].resize(_verticesCount);
			flows[i].resize(_verticesCount);
		}
	}
	void SetSize(int from, int to, int size) { sizes[from][to] = size; };
	void SetFlow(int from, int to, int flow) { flows[from][to] = flow; };
	int GetSize(int from, int to) { return sizes[from][to]; };
	int GetFlow(int from, int to) { return flows[from][to]; };
private:
	std::vector<std::vector<int>> sizes;
	std::vector<std::vector<int>> flows;
};

bool bfs(int n, DinicGraph& graph, int s, int t, std::vector<int>& d) {
	std::vector<int> q(n);
	int QueueBot = 0, QueueTop = 0;
	q[QueueTop++] = s;
	for (int i = 0; i < n; i++) d[i] = -1;
	d[s] = 0;
	while (QueueBot < QueueTop) {
		int v = q[QueueBot++];
		for (int to = 0; to< n; ++to)
			if (d[to] == -1 && graph.GetFlow(v, to) < graph.GetSize(v, to)) {
				q[QueueTop++] = to;
				d[to] = d[v] + 1;
			}
	}
	return d[t] != -1;
}

int dfs(int n, int INF, DinicGraph& graph, int s, int t, std::vector<int>& d, std::vector<int>& ptr, int v, int flow) {
	if (!flow)  return 0;
	if (v == t)  return flow;
	for (int & to = ptr[v]; to<n; ++to) {
		if (d[to] != d[v] + 1)  continue;
		int pushed = dfs(n, INF, graph, s, t, d, ptr, to, std::min(flow, graph.GetSize(v, to) - graph.GetFlow(v, to)));
		if (pushed) {
			graph.SetFlow(v, to, graph.GetFlow(v, to) + pushed);
			graph.SetFlow(to, v, graph.GetFlow(to, v) - pushed);
			return pushed;
		}
	}
	return 0;
}

int solve(int n, int INF, DinicGraph& graph, int s, int t) {
	int flow = 0;
	std::vector<int> ptr(n);
	std::vector<int> d(n);

	for (;;) {
		if (!bfs(n, graph, s, t, d))  break;
		for (int i = 0; i < n; i++) ptr[i] = 0;
		while (int pushed = dfs(n, INF, graph, s, t, d, ptr, s, INF))
			flow += pushed;
	}
	return flow;
}

int main(void)
{
	std::ifstream fin("maxflow.in");
	int n = 0, m = 0, infinity = INT_MAX;
	fin >> n >> m;
	int s = 0, t = n - 1;
	DinicGraph graph(n);
	for (int i = 0; i < m; ++i) {
		int temp1, temp2, temp3;
		fin >> temp1 >> temp2 >> temp3;
		graph.SetSize(temp1 - 1, temp2 - 1, temp3);
		graph.SetFlow(temp1 - 1, temp2 - 1, 0);
	}
	fin.close();

	int flow = solve(n, 100000, graph, s, t);

	std::ofstream fout("maxflow.out");
	fout << flow;
	fout.close(); 
}