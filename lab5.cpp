#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include <algorithm>
#include <type_traits>
#include <iomanip>

#include "flags.hh/Flags.hh"
#include "lab2_types.hh"

bool debug = false;

bool read_from_file(const std::string &filename, vv_float &matrix, size_t &s, size_t &t)
{
	std::ifstream fin(filename);

	if (!fin.is_open())
		return false;

	matrix.clear();

	size_t n;

	fin >> n;

	matrix.resize(n, v_float(n, 0));

	fin >> matrix >> s >> t;

	s--;
	t--;

	return true;
}

v dijkstra(const vv_float &matrix, size_t s, size_t t)
{
	const size_t inf = std::string::npos;
	const size_t n = matrix.size();

	v path;
	v_float d(n, fInf);
	v h(n, 0);
	v x(n, false);

	d[s] = 0;
	h[s] = 0;
	x[s] = true;

	size_t p = s;
	size_t iteration = 0;

	std::cout << iteration++ << " D " << v_inf(d) << std::endl;
	std::cout << "  H " << v_plus(h) << std::endl;
	std::cout << "  X " << x << std::endl;
	std::cout << std::endl;

	while (p != t)
	{
		for (size_t j = 0; j < n; ++j)
		{
			size_t v = matrix[p][j];

			if (d[v] > d[p] + matrix[p][v])
			{
				d[v] = d[p] + matrix[p][v];
				h[v] = p;
			}
		}
	
		size_t minV = inf;

		for (size_t i = 1; i < n; ++i)
		{
			if (x[i])
				continue;

			if (minV == inf || d[i] < d[minV])
				minV = i;

			x[minV] = true;
		}

		p = minV;

		
	std::cout << iteration++ << " D " << v_inf(d) << std::endl;
	std::cout << "  H " << v_plus(h) << std::endl;
	std::cout << "  X " << x << std::endl;
	std::cout << std::endl;

	}

	return path;
}

void task1(const vv_float &matrix, const size_t s, const size_t t)
{
	v path = dijkstra(matrix, s, t);
}

int main(int argc, char *argv[])
{
	setlocale(LC_CTYPE, "ru_RU.UTF8");

	std::string filename;
	bool help;
	size_t task;
	Flags flags;

	flags.Var(filename, 'f', "filename", std::string(), "Файл со списком рёбер графа");
	flags.Var(task, 't', "task", size_t(1), "Подзадача лабораторной работы");
	flags.Bool(debug, 'd', "debug", "Вывод отладки");
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

	vv_float matrix;
	size_t s, t;

	if (!read_from_file(filename, matrix, s, t))
	{
		std::cout << "Ошибка матрицы весов орграфа." << std::endl;
		return 2;
	}

	std::cout << "Прочитанная матрица весов: " << std::endl << matrix << std::endl;
	std::cout << "Исходная вершина: " << (s + 1) << std::endl;
	std::cout << "Конечная вершина: " << (t + 1) << std::endl;

	switch (task)
	{
		case 1:
			task1(matrix, s, t);
			break;
	}


/*
	std::cout << "Количество вершин в графе: " << edges.graph_size() << "." << std::endl;
	std::cout << "Прочитанный список рёбер графа: " << std::endl << edges << std::endl;

	if (rice)
		edges.negative_weight(true);

	auto spanning = spanning_tree_prima(edges);

	if (spanning.empty())
	{
		std::cout << "Граф не является связным, не удалось построить остовное дерево." << std::endl;
		return 3;
	}

	if (rice)
	{
		edges.negative_weight(false);
		spanning.negative_weight(false);
	}

	std::cout << "Вес рёбер остовного дерева: " << spanning.weight_sum() << "." << std::endl;
	std::cout << "Рёбра, входящие в остовное дерево: " << std::endl << spanning << std::endl;

	if (rice)
	{
		auto end = edges.end();

		for (auto e: spanning)
		{
			end = std::remove(edges.begin(), end, e);
			end = std::remove(edges.begin(), end, e.swapped());
		}

		end = std::remove_if(edges.begin(), edges.end(), &edge::is_not_rise);
		edges.erase(end, edges.end());
		std::cout << "Земляные валики, которые необходимо разрушить:" << std::endl << edges << std::endl;
	}*/



	return 0;
}
