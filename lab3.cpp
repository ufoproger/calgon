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

vv readFromFile(const std::string &filename, bool skipOrientation)
{
	std::ifstream fin(filename);
	vv m;

	if (!fin.is_open())
		return m;

	size_t n;
	std::string s;

	fin >> n;
	m.resize(n);
	getline(fin, s);

	for (size_t i = 0; i < n; ++i)
	{

		// Если не удалось получить строку со списком смежных вершин, то дальше их тоже нет
		if (!getline(fin, s))
			break;

		std::istringstream sin(s);
		size_t vertex;

		while (sin >> vertex)
		{
			--vertex;
			m[i].push_back(vertex);

			// Если игнорируем направленность рёбер, то добавляем смежной вершине себя в список смежности, если этого уже не было сделано
			if (skipOrientation && i > vertex && find(m[vertex].begin(), m[vertex].end(), i) == m[vertex].end())
				m[vertex].push_back(i);
		}
	}

	return m;
}

class IStorage
{
public:
	virtual size_t get_next() = 0;
	virtual v get_v() const = 0;
};

class QueueStorage: public IStorage, public queue
{
public:
	size_t get_next()
	{
		return front();
	}

	v get_v() const
	{
		auto q = *this;
		v result;

		while (!q.empty())
		{
			result.push_back(q.get_next());
			q.pop();
		}

		return result;
	}
};

class StackStorage: public IStorage, public stack
{
public:
	size_t get_next()
	{
		return top();
	}

	v get_v() const
	{
		auto q = *this;
		v result;

		while (!q.empty())
		{
			result.push_back(q.get_next());
			q.pop();
		}

		std::reverse(result.begin(), result.end());

		return result;
	}
};

template < typename T > v algo_search(const vv &m, size_t start)
{
	v visited(m.size(), false);
	T storage;

	storage.push(start);
	visited[start] = true;

	if (debug)
		std::cout << "Вывод итераций: " << std::endl;

	while (!storage.empty())
	{

		size_t u = storage.get_next();

		if (debug)
			std::cout << (u + 1) << ", next " << v_plus(storage.get_v()) << std::endl;

		storage.pop();

		for (auto w: m[u])
			if (!visited[w])
			{
				storage.push(w);
				visited[w] = true;
			}
	}

	if (debug)
		std::cout << std::endl;

	return visited;
}

// Поиск в ширину
inline v bfs(const vv &m, size_t start)
{
	return algo_search<QueueStorage>(m, start);
}

// Поиск в глубину
inline v dfs(const vv &m, size_t start)
{
	return algo_search<StackStorage>(m, start);
}

bool is_all_visited(const v &visited)
{
	for (auto f: visited)
		if (!f)
			return false;

	return true;	
}

void task1(const vv &m)
{
	bool f = is_all_visited(bfs(m, 0));

	std::cout << "Граф" << (f ? " " : " не ") << "является связным." << std::endl;
}

void task2(const vv &m)
{
	const size_t n = m.size();

	v sources;

	for (size_t i = 0; i < n; ++i)
		if (is_all_visited(dfs(m, i)))
			sources.push_back(i);

	if (sources.empty())
		std::cout << "Источников орграфа нет" << std::endl;
	else
		std::cout << "Источники орграфа: " << v_plus(sources) << std::endl;
}

void task3(const vv &m)
{
}

int main(int argc, char *argv[])
{
	setlocale(LC_CTYPE, "ru_RU.UTF8");

	std::string filename;
	bool help, skipOrientation;
	size_t task;
	Flags flags;

	flags.Var(filename, 'f', "filename", std::string(), "Файл со списком смежности графа");
	flags.Bool(skipOrientation, 's', "skip-orientation", "Игнорировать ориентацию графа");
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

	vv m = readFromFile(filename, skipOrientation);

	if (m.empty())
	{
		std::cout << "Ошибка чтения матрицы смежности." << std::endl;
		return 2;
	}

	std::cout << "Прочитанная матрица смежности: " << std::endl << vv_plus(m, true) << std::endl;

	switch (task)
	{
		case 1:
			task1(m);
			break;

		case 2:
			task2(m);
			break;

		case 3:
			task3(m);
			break;
	}

	return 0;
}
