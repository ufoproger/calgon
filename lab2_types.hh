#include <vector>
#include <ostream>
#include <queue>
#include <stack>

typedef std::vector < size_t > v;
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