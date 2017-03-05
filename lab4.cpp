#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include <algorithm>
#include <type_traits>

#include "flags.hh/Flags.hh"
#include "lab2_types.hh"

bool debug = false;

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

bool is_all_visited(const v &visited)
{
	for (auto f: visited)
		if (!f)
			return false;

	return true;
}

v_edges spanning_tree_prima(const v_edges &edges)
{
	size_t n = edges.graph_size();
	v_edges result;
	v vertexes;
	edge minEdge = *min_element(edges.begin(), edges.end());

	result.push_back(minEdge);
	vertexes.push_back(minEdge.from);
	vertexes.push_back(minEdge.to);

	for (size_t i = 2; i < n; ++i)
	{
		minEdge = edge();

		for (auto e: edges)
		{
			// Нет вершины для этого ребра
			if (find(vertexes.begin(), vertexes.end(), e.from) == vertexes.end())
				continue;

			// Образуется цикл
			if (find(vertexes.begin(), vertexes.end(), e.to) != vertexes.end())
				continue;

			if (minEdge.empty() || e < minEdge)
				minEdge = e;
		}

		// Граф не связный
		if (minEdge.empty())
			return v_edges();

		result.push_back(minEdge);
		vertexes.push_back(minEdge.to);

		if (debug)
		{
			std::cout << "Вершины остовного дерева: " << v_plus(vertexes) << std::endl;
			std::cout << "Рёбра остовного дерева T_" << (i) << std::endl;
			std::cout << result << std::endl;
		}
	}

	return result;
}

int main(int argc, char *argv[])
{
	setlocale(LC_CTYPE, "ru_RU.UTF8");

	std::string filename;
	bool help, skipOrientation = false, rice;
	Flags flags;

	flags.Var(filename, 'f', "filename", std::string(), "Файл со списком рёбер графа");
	flags.Bool(debug, 'd', "debug", "Вывод отладки");
	flags.Bool(skipOrientation, 's', "skip-orientation", "Игнорировать направление рёбер");
	flags.Bool(rice, 'r', "rice", "Режим задачи о мелиорации рисовых полей");
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

	v_edges edges = read_from_file(filename, skipOrientation);

	if (edges.empty())
	{
		std::cout << "Ошибка чтения рёбер графа." << std::endl;
		return 2;
	}

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
	}

	return 0;
}
