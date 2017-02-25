#include <vector>
#include <ostream>

typedef std::vector < size_t > v;
typedef std::vector < v > vv;

class v_plus: public v
{
	public:
		v_plus(v& _v): v(_v) {}
};

class vv_plus: public vv
{
	public:
		vv_plus(vv& _vv): vv(_vv) {}
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
	for (auto v: a)
		out << v_plus(v) << ::std::endl;

	return out;
}

