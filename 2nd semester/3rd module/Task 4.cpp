#include <climits>
#include <iostream>
#include <vector>
#include <iostream>
#include <ostream>
#include <fstream>
#include <algorithm>
#include <string>

class Graph {
public: // graph data access only via methods
	explicit Graph(std::string istring_, std::string pattern_, int istring_size, int pattern_size);

	// function works FINE
	int GetGraphSize(void) {
		//std::cout << std::endl << std::endl << "GetGraphSize() was called" << std::endl;
		//std::cout << "istring_questions: " << istring_questions << std::endl;
		//std::cout << "pattern_questions: " << pattern_questions << std::endl;
		//std::cout << "GetGraphSize(): " << istring_questions + pattern_questions + 2 << std::endl << std::endl;
		return istring_questions + pattern_questions + 2;
	};

	int GetIstringQuestions(void) {
		//std::cout << std::endl << std::endl << "GetIstringQuestions(void) was called" << std::endl;
		//std::cout << "istring_questions: " << istring_questions << std::endl << std::endl;
		return istring_questions;
	};
	int GetPatternQuestions(void) {
		//std::cout << std::endl << std::endl << "GetPatternQuestions(void) was called" << std::endl;
		//std::cout << "pattern_questions: " << pattern_questions << std::endl << std::endl;
		return pattern_questions;
	};

	void IncrementSize(int from, int to) { sizes[from][to]++; };
	void SetSize(int from, int to, int size) { sizes[from][to] = size; };
	void SetFlow(int from, int to, int flow) { flows[from][to] = flow; };
	int GetSize(int from, int to) { return sizes[from][to]; };
	int GetFlow(int from, int to) { return flows[from][to]; };

	std::string GetIString(void) { return istring; };
	std::string GetPattern(void) { return pattern; };
	void PutIString(std::string NewIString);
	void PutPattern(std::string NewPattern);

private: // all graph data is safe and protected
	std::vector<std::vector<int>> sizes;
	std::vector<std::vector<int>> flows;
	int istring_questions, pattern_questions;
	std::string istring;
	std::string pattern;
};

int buildgraph(Graph& graph, std::string& istring, std::string& pattern)
{
	// works FINE
	// graph will contain of 0, 1 vertices + istring.length amount of ? from istring, same for pattern.
	int graphSize = graph.GetGraphSize();

	// works FINE
	// we'll count amount of conflicts between constant characters (0 and 1 but not ? and 1 or ? and 0)
	int constConfAm = 0;

	// works FINE
	int istring_questions = graph.GetIstringQuestions();
	int pattern_questions = graph.GetPatternQuestions();

	// works FINE
	// a simple log for tracking needed logs

	// moving across the istring istring.length()-pattern.length()+1 times comparing pattern to the current part of the istring
	for (unsigned int offset = 0; offset <= istring.length() - pattern.length(); ++offset)
	{
		// works FINE
		//std::cout << "offset: " << offset << std::endl;

		int istringIteratorInGraph = 1;

		for (int i = 0; i < offset; i++)
		{
			if (istring[i] == '?')
				istringIteratorInGraph++;

		}

		int patternIteratorInGraph = istring_questions + 1;

		for (unsigned int patternIterator = 0; patternIterator < pattern.length(); ++patternIterator)
		{
			// works FINE
			// patternIterator = patternIterator + 0;
			int istringIterator = patternIterator + offset;
			char istrSymbol = istring[istringIterator];
			char pattSymbol = pattern[patternIterator];

			// if symbols didn't match
			if (istrSymbol != pattSymbol)
			{
				// if we met constant conflict
				if ((istrSymbol == '1' && pattSymbol == '0') || (istrSymbol == '0' && pattSymbol == '1'))
				{
					constConfAm++;
					continue;
				}
				// if str has 0 and pattern ? then build edge from 0 to pattSymbol
				if (istrSymbol == '0' && pattSymbol == '?')
				{
					graph.IncrementSize(0, patternIteratorInGraph);
					patternIteratorInGraph++;
					continue;
				}
				// if str has 1 and pattern ? then build edge from pattSymbol to 1
				if (istrSymbol == '1' && pattSymbol == '?')
				{
					graph.IncrementSize(patternIteratorInGraph, graphSize - 1);
					patternIteratorInGraph++;
					continue;
				}
				// if str has ? and pattern 0 then build edge from 0 to istrSymbol
				if (istrSymbol == '?' && pattSymbol == '0')
				{
					graph.IncrementSize(0, istringIteratorInGraph);
					istringIteratorInGraph++;
					continue;
				}
				// if str has ? and pattern 1 then build edge from istrSymbol to 1
				if (istrSymbol == '?' && pattSymbol == '1')
				{
					graph.IncrementSize(istringIteratorInGraph, graphSize - 1);
					istringIteratorInGraph++;
					continue;
				}
			}
			// and if they did
			{
				// if str has ? and pattern ? then we build both edges from istrSymbol to pattSymbol
				if (istrSymbol == '?' && pattSymbol == '?')
				{
					graph.IncrementSize(istringIteratorInGraph, patternIteratorInGraph);
					graph.IncrementSize(patternIteratorInGraph, istringIteratorInGraph);
					patternIteratorInGraph++;
					istringIteratorInGraph++;
					continue;
				}
				else
				{
				}
			}
		}
	}

	return constConfAm;
}

void Graph::PutIString(std::string NewIString) {
	istring = NewIString;
};
void Graph::PutPattern(std::string NewPattern) {
	pattern = NewPattern;
};

Graph::Graph(std::string istring_, std::string pattern_, int istring_size, int pattern_size) :
	istring_questions(0),
	pattern_questions(0),
	istring(istring_),
	pattern(pattern_)
{
	for (unsigned int i = 0; i < istring_.size(); i++)
		if (istring_[i] == '?')
		{
			istring_questions++;
		}
	for (unsigned int i = 0; i < pattern_.size(); i++)
		if (pattern_[i] == '?')
		{
			pattern_questions++;
		}

	sizes.resize(istring_questions + pattern_questions + 2);
	flows.resize(istring_questions + pattern_questions + 2);

	for (unsigned int i = 0; i < sizes.size(); i++) {
		sizes[i].resize(sizes.size());
	}
	for (unsigned int i = 0; i < flows.size(); i++) {
		flows[i].resize(flows.size());
	}
}

bool bfs(int n, Graph& graph, int s, int t, std::vector<int>& d) {
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

int dfs(int n, int INF, Graph& graph, int s, int t, std::vector<int>& d, std::vector<int>& ptr, int v, int flow) {
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


int dinic(int n, int INF, Graph& graph, int s, int t) {
	int flow = 0;
	std::vector<int> ptr(n);
	std::vector<int> d(n);

	for (;;) {
		if (!bfs(n, graph, s, t, d))  break;
		for (int i = 0; i < n; i++) ptr[i] = 0;
		while (int pushed = dfs(n, INF, graph, s, t, d, ptr, s, INF))
			flow += pushed;
	}


	std::string istring;
	istring = graph.GetIString();
	std::string newistring;

	int iterator = 0;

	for (unsigned int i = 0; i < istring.size(); ++i)
	{
		if (istring[i] == '?')
		{
			iterator++;
			if (d[iterator] == -1)
			{
				newistring.append(1, '1');
			}
			else
			{
				newistring.append(1, '0');
			}
		}
		else
		{
			newistring.append(1, istring[i]);
		}
	}

	std::string pattern;
	pattern = graph.GetPattern();
	std::string newpattern;

	for (unsigned int i = 0; i < pattern.size(); ++i)
	{
		if (pattern[i] == '?')
		{
			iterator++;
			if (d[iterator] == -1)
			{
				newpattern.append(1, '1');
			}
			else
			{
				newpattern.append(1, '0');
			}
		}
		else
		{
			newpattern.append(1, pattern[i]);
		}
	}

	graph.PutIString(newistring);
	graph.PutPattern(newpattern);

	return flow;
}


void getInput(std::string& first, std::string& second)
{
	std::ifstream fin("input.txt");
	fin >> first;
	fin >> second;
	fin.close();
}

void putOutput(int result, std::string& first, std::string& second)
{
	std::ofstream fout("output.txt");
	fout << result << std::endl;
	fout << first << std::endl;
	fout << second;
	fout.close();
}

int main(void)
{
	std::string istring;
	std::string pattern;

	getInput(istring, pattern);

	// graph will contain of 0, 1 vertices + istring.length amount of ? from istring, same for pattern.
	Graph graph(istring, pattern, istring.size(), pattern.size());

	// build the graph from those 2 strings + put logs of building it
	int constConf = buildgraph(graph, istring, pattern);

	// call dinic
	int flow = dinic(graph.GetGraphSize(), 100000, graph, 0, graph.GetGraphSize() - 1);

	int result = constConf + flow;

	istring = graph.GetIString();
	pattern = graph.GetPattern();

	putOutput(result, istring, pattern);
	return 0;
}