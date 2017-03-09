#include <vector>
#include <ostream>
#include <queue>
#include <stack>
#include <cmath>
#include <string>

const size_t inf = std::string::npos;
const float fInf = 1e10;

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

typedef std::vector < std::string > v_string;
typedef std::vector < float > v_float;
typedef std::vector < v_float > vv_float;
typedef std::vector < size_t > v;
typedef std::vector < v > vv;
typedef std::queue < size_t > queue;
typedef std::stack < size_t > stack;

class v_plus: public v
{
	public:
		v_plus(v _v): v(_v) {}
};

class v_inf: public v_float
{
	public:
		v_inf(v_float _v): v_float(_v) {}
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

std::ostream& operator<<(std::ostream &out, const v_string &v)
{
	for (auto value: v)
		out << value << " ";

	return out;
}

std::ostream& operator<<(std::ostream &out, const v_float &v)
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

std::ostream& operator<<(std::ostream &out, const vv_float &a)
{
	for (auto v: a)
		out << v << ::std::endl;

	return out;
}

std::istream& operator>>(std::istream &in, vv_float &a)
{
	for (auto &row: a)
		for (auto &element: row)
			in >> element;

	return in;
}

std::ostream& operator<<(std::ostream &out, const v_plus &v)
{
	for (auto value: v)
		out << value + 1 << " ";

	return out;
}

std::ostream& operator<<(std::ostream &out, const v_inf &v)
{
	for (auto value: v)
	{
		if (value == fInf)
			out << "oo";
		else
			out << value;

		out << " ";
	}

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
