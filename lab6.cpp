#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include <algorithm>
#include <type_traits>

#include "lab2_types.hh"
#include "flags.hh/Flags.hh"

bool debug = false;
/*
v_edges read_from_file(const std::string &filename, bool skipOrientation)
{
	std::ifstream fin(filename);
	v_edges edges;

	if (!fin.is_open())
		return edges;

	std::string s;

	while (getline(fin, s))
	{
		std::istringstream sin(s);
		edge e;

		sin >> e;

		edges.push_back(e);

		if (skipOrientation)
		{
			std::swap(e.from, e.to);
			edges.push_back(e);
		}
	}

	return edges;
}
*/

v_string haffman(v_float p)
{
	std::sort(p.begin(), p.end());
	v pos;

	while (p.size() > 2)
	{
		float sum = p[0] + p[1];

		p.erase(p.begin(), p.begin() + 2);
		
		auto it = std::lower_bound(p.begin(), p.end(), sum);
		
		pos.push_back(p.size() - (it - p.begin()));
		p.insert(it, sum);
	}

	std::vector < std::string > codes({"0", "1"});

	for (auto it = pos.rbegin(); it != pos.rend(); ++it)
	{
		std::string code = codes[*it];

		codes.erase(codes.begin() + *it);
		codes.push_back(code + "0");
		codes.push_back(code + "1");
	}

	return codes;
}

void task1(const std::string &filename)
{
	std::ifstream fin(filename);

	if (!fin.is_open())
		return;

	v_float p;
	
	for (float t; fin >> t;)
		p.push_back(t);

	std::cout << "Прочитанный массив вероятностей (n = " << p.size() << "): " << p << std::endl;

	auto codes = haffman(p);
	std::cout << codes << std::endl;
}

void task2(const std::string &filename)
{

}

int main(int argc, char *argv[])
{
	setlocale(LC_CTYPE, "ru_RU.UTF8");

	std::string filename;
	bool help;
	size_t task;
	Flags flags;

	flags.Var(filename, 'f', "filename", std::string(), "Файл со входными данными");
	flags.Bool(debug, 'd', "debug", "Вывод отладки");
	flags.Var(task, 't', "task", size_t(0), "Номер подзадания");
	flags.Bool(help, 'h', "help", "Показать помощь и выйти");

	if (!flags.Parse(argc, argv))
	{
		flags.PrintHelp(argv[0]);
		return 1;
	}

	if (help || filename.empty())
	{
		flags.PrintHelp(argv[0]);
		return 0;
	}

	switch (task)
	{
		case 1:
			task1(filename);
			break;

		case 2:
			task2(filename);
			break;

		default:
			std::cout << "Неопознанный номер задачи" << std::endl;
	}

	return 0;
}
