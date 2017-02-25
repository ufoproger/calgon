#include <vector>

typedef std::vector < size_t > v;
typedef std::vector < v > vv;

std::ostream& operator<<(std::ostream &out, const v &v)
{
	for (auto value: v)
		out << (value + 1) << " ";

	return out;
}

std::ostream& operator<<(std::ostream &out, const vv &a)
{
	for (auto v: a)
		out << v << ::std::endl;

	return out;
}

