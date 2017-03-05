#include <vector>
#include <ostream>
#include <queue>
#include <stack>
#include <cmath>

struct edge
{
	size_t from, to;
	float weight;

	edge():
		from(0),
		to(0),
		weight(0.)
	{}

	edge(size_t from_, size_t to_, float weight_):
		from(from_),
		to(to_),
		weight(weight_)
	{}

	bool static is_not_rise(const edge &e)
	{
		return (e.to < e.from);
	}

	bool operator<(const edge &e) const
	{
		return (weight < e.weight);
	}

	bool operator==(const edge &e) const
	{
		return (from == e.from && to == e.to && weight == e.weight);
	}

	bool empty() const
	{
		return !(from || to || weight);
	}

	edge swapped() const
	{
		return edge(to, from, weight);
	}
};

class v_edges: public std::vector < edge >
{
	public:
		float weight_sum() const
		{
			float sum = 0.;

			for (auto e: *this)
				sum += e.weight;

			return sum;
		}

		size_t graph_size() const
		{
			size_t n = 0;

			for (auto e: *this)
				n = std::max(n, std::max(e.from, e.to));

			return (n + 1);
		}

		void negative_weight(bool f)
		{
			for (auto &e: *this)
				e.weight = fabs(e.weight) * (f ? -1. : 1.);
		}
};

typedef std::vector < size_t > v;
// typedef std::vector < edge > v_edges;
typedef std::vector < v > vv;
typedef std::queue < size_t > queue;
typedef std::stack < size_t > stack;

class v_plus: public v
{
	public:
		v_plus(v _v): v(_v) {}
};

class vv_plus: public vv
{
	public:
		bool showRowIndex;

		vv_plus(vv& _vv, bool _showRowIndex = false):
			vv(_vv),
			showRowIndex(_showRowIndex)
		{}
};

std::ostream& operator<<(std::ostream &out, const edge &e)
{
	return out << "(" << (e.from + 1) << ", " << (e.to + 1) << ", w=" << e.weight << ")";
}

std::istream& operator>>(std::istream &in, edge &e)
{
	in >> e.from >> e.to >> e.weight;
	--e.from;
	--e.to;

	return in;
}

std::ostream& operator<<(std::ostream &out, const v_edges &v)
{
	for (auto e: v)
		out << e << std::endl;

	return out;
}

std::ostream& operator<<(std::ostream &out, const v &v)
{
	for (auto value: v)
		out << value << " ";

	return out;
}

std::ostream& operator<<(std::ostream &out, const vv &a)
{
	for (auto v: a)
		out << v << ::std::endl;

	return out;
}

std::ostream& operator<<(std::ostream &out, const v_plus &v)
{
	for (auto value: v)
		out << value + 1 << " ";

	return out;
}

std::ostream& operator<<(std::ostream &out, const vv_plus &a)
{
	size_t row = 0;

	for (auto v: a)
	{
		++row;

		if (a.showRowIndex)
			out << row << ": ";

		out << v_plus(v) << ::std::endl;
	}

	return out;
}
