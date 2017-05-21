#include <fstream>
#include <vector>
#include <climits>
#include <string>
#include <set>
#include <algorithm>
#include <cmath>
#include <memory>

class Reshuffle {
public:
	Reshuffle(const std::vector<std::vector<short>> &p, int distance_ = INT_MAX, Reshuffle *parent_ = nullptr,
		bool enqueued_ = false);

	bool IsSolvable() const;
	std::string GetSolution();

	bool operator==(const Reshuffle &p) const;
	bool operator<(const Reshuffle &p) const;

	int GetHeuristic() const { return heuristic_; }
	int GetDistance() const { return distance_; }
	Reshuffle* GetParent() const { return parent_; }
	bool IsEnqueued() { return enqueued_; }

	void SetParent(Reshuffle *parent) { parent_ = parent; }
	void SetEnqueued() { enqueued_ = true; }
	void SetNotEnqueued() { enqueued_ = false; }
	void SetDistance(int distance) { distance_ = distance; }

private:
	int CountHeuristic() const;
	int ManhattanDistance(short element, int from_line, int from_column, int size) const;

	template <typename Comparator>
	std::string Restore(std::set<Reshuffle *, Comparator> &visited, Reshuffle *current, Reshuffle *start);

	std::vector<Reshuffle> GetNeighbours() const;
	std::vector<std::vector<short>> Reshuffle_;
	std::pair<int, int> GetPosition(short element) const;

	int heuristic_;
	int distance_;
	Reshuffle *parent_;
	bool enqueued_;
};

Reshuffle::Reshuffle(const std::vector<std::vector<short> > &p, int distance,
	Reshuffle *parent, bool enqueued) :
	Reshuffle_(p),
	heuristic_(CountHeuristic()),
	distance_(distance),
	parent_(parent),
	enqueued_(enqueued) {}


bool Reshuffle::operator<(const Reshuffle &p) const {
	return Reshuffle_ < p.Reshuffle_;
}

bool Reshuffle::operator==(const Reshuffle &p) const {
	return Reshuffle_ == p.Reshuffle_;
}

std::vector<Reshuffle> Reshuffle::GetNeighbours() const {
	std::vector<Reshuffle> neighbours;
	int size = Reshuffle_.size();
	int zero_line = GetPosition(0).first;
	int zero_column = GetPosition(0).second;

	std::vector<std::vector<short>> candidate(Reshuffle_);
	if (zero_line > 0) {
		std::swap(candidate[zero_line][zero_column], candidate[zero_line - 1][zero_column]);
		neighbours.push_back(Reshuffle(candidate));
		std::swap(candidate[zero_line][zero_column], candidate[zero_line - 1][zero_column]);
	}
	if (zero_line < size - 1) {
		std::swap(candidate[zero_line][zero_column], candidate[zero_line + 1][zero_column]);
		neighbours.push_back(Reshuffle(candidate));
		std::swap(candidate[zero_line][zero_column], candidate[zero_line + 1][zero_column]);
	}
	if (zero_column > 0) {
		std::swap(candidate[zero_line][zero_column], candidate[zero_line][zero_column - 1]);
		neighbours.push_back(Reshuffle(candidate));
		std::swap(candidate[zero_line][zero_column], candidate[zero_line][zero_column - 1]);
	}
	if (zero_column < size - 1) {
		std::swap(candidate[zero_line][zero_column], candidate[zero_line][zero_column + 1]);
		neighbours.push_back(Reshuffle(candidate));
		std::swap(candidate[zero_line][zero_column], candidate[zero_line][zero_column + 1]);
	}
	return neighbours;
}

std::pair<int, int> Reshuffle::GetPosition(short element) const {
	for (unsigned int line = 0; line < Reshuffle_.size(); ++line) {
		for (unsigned int column = 0; column < Reshuffle_[line].size(); ++column) {
			if (Reshuffle_[line][column] == element)
				return{ line, column };
		}
	}
}

bool Reshuffle::IsSolvable() const {
	std::vector<short> Reshuffle_list;
	for (unsigned int i = 0; i < Reshuffle_.size(); ++i) {
		for (unsigned int j = 0; j < Reshuffle_.size(); ++j) {
			Reshuffle_list.push_back(Reshuffle_[i][j]);
		}
	}

	int inv = 0;
	for (int i = 0; i<16; ++i)
		if (Reshuffle_list[i])
			for (int j = 0; j<i; ++j)
				if (Reshuffle_list[j] > Reshuffle_list[i])
					++inv;
	for (int i = 0; i<16; ++i)
		if (Reshuffle_list[i] == 0)
			inv += 1 + i / 4;

	return((inv & 1) ? 0 : 1);
}

int Reshuffle::ManhattanDistance(const short element, const int from_line, const int from_column, const int size) const {
	int to_line = element / size;
	int to_column = element % size;

	return (int)(std::abs(to_line - from_line) + std::abs(to_column - from_column));
}

int Reshuffle::CountHeuristic() const {
	int heuristicA = 0;
	int heuristicB = 0;
	for (unsigned int i = 0; i < Reshuffle_.size(); ++i) {
		for (unsigned int j = 0; j < Reshuffle_.size(); ++j) {
			if (Reshuffle_[i][j] == 0 || Reshuffle_[i][j] == i + j)
				continue;
			heuristicA += ManhattanDistance(Reshuffle_[i][j] - 1, i, j, Reshuffle_.size());
			if (Reshuffle_[i][j] == 15 - (i + 1) * 4 + (4 - j))
				heuristicB += 1;
		}
	}
	int heuristic = heuristicA * 5 - heuristicB * 3;
	return heuristic;
}

std::string Reshuffle::GetSolution() {
	std::vector<std::vector<short>> solved;
	solved.resize(Reshuffle_.size());
	for (auto &x : solved)
		x.resize(Reshuffle_.size());

	for (unsigned short i = 0; i < Reshuffle_.size(); ++i) {
		for (unsigned short j = 0; j < Reshuffle_.size(); ++j)
			solved[i][j] = (short)(i * Reshuffle_.size() + j + 1);
	}
	solved[Reshuffle_.size() - 1][Reshuffle_.size() - 1] = 0;

	Reshuffle finish(solved);

	const int step_cost = 1;

	std::vector<Reshuffle*> all_Reshuffles;

	auto visited_comparator = [](const Reshuffle *a, const Reshuffle *b) {
		return *a < *b;
	};
	std::set<Reshuffle *, decltype(visited_comparator)> visited(visited_comparator);

	std::set<std::pair<int, Reshuffle *>> queue;

	SetDistance(0);
	visited.insert(this);
	queue.insert({ GetHeuristic() + GetDistance(), this });

	Reshuffle *current;
	current = queue.begin()->second;
	while (queue.size() != 0) {
		current = queue.begin()->second;
		int current_f = queue.begin()->first;

		if (*current == finish)
			break;
		queue.erase({ current_f, current });
		current->SetNotEnqueued();
		for (auto &neighbour : current->GetNeighbours()) {
			int distance = current->GetDistance() + step_cost;
			auto it = visited.find(&neighbour);
			if (it == visited.end()) {
				all_Reshuffles.push_back(new Reshuffle(neighbour));
				auto neighbour_ptr = all_Reshuffles.back();
				neighbour_ptr->SetDistance(distance);
				neighbour_ptr->SetParent(current);
				neighbour_ptr->SetEnqueued();

				visited.insert(neighbour_ptr);
				queue.insert({ neighbour_ptr->GetHeuristic() + distance, neighbour_ptr });
			}
			else {
				if (distance < (*it)->GetDistance()) {
					Reshuffle *neighbour_ptr = *it;
					if (neighbour.IsEnqueued())
						queue.erase({ neighbour_ptr->GetHeuristic() + neighbour_ptr->GetDistance(), neighbour_ptr });
					neighbour_ptr->SetParent(current);
					neighbour_ptr->SetDistance(distance);
					queue.insert({ distance + neighbour_ptr->GetHeuristic(), neighbour_ptr });
					neighbour_ptr->SetEnqueued();
				}
			}
		}
	}

	return Restore(visited, current, this);
}

template <typename Comparator>

std::string Reshuffle::Restore(std::set<Reshuffle *, Comparator> &visited,
	Reshuffle *current, Reshuffle *start) {
	std::string reversed_answer;
	while (!(*current == *start)) {
		std::pair<int, int> current_pos = current->GetPosition(0);
		std::pair<int, int> parent_pos = current->GetParent()->GetPosition(0);
		if (current_pos.first == parent_pos.first) {
			if (current_pos.second == parent_pos.second + 1) {
				reversed_answer.append("L");
			}
			else if (current_pos.second == parent_pos.second - 1) {
				reversed_answer.append("R");
			}
		}
		else {
			if (current_pos.first == parent_pos.first + 1) {
				reversed_answer.append("U");
			}
			else if (current_pos.first == parent_pos.first - 1) {
				reversed_answer.append("D");
			}
		}
		current = current->GetParent();
	}
	std::reverse(reversed_answer.begin(), reversed_answer.end());
	return reversed_answer;
}

int main() {
	int size = 4;

	std::ifstream in("input.txt");
	std::ofstream out("output.txt");

	std::vector<std::vector<short>> input;
	input.resize(size);
	for (int i = 0; i < size; ++i)
		input[i].resize(size);
	for (int line = 0; line < size; ++line) {
		for (int column = 0; column < size; ++column)
			in >> input[line][column];
	}
	Reshuffle start(input);

	if (!start.IsSolvable()) {
		out << "-1\n";
		return 0;
	}

	auto answer = start.GetSolution();
	out << answer.size() << std::endl << answer;

	return 0;
}