#include <numeric>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include <ncurses.h>

#include "flags.hh/Flags.hh"

typedef std::vector < std::vector < bool > > matrix;
typedef std::vector < std::vector < uint32_t > > solutions;

std::ostream& operator<<(std::ostream &out, const std::vector < bool > &x)
{
	for (auto value: x)
		out << value << " ";

	return out;
}

std::ostream& operator<<(std::ostream &out, const matrix &a)
{
	for (auto row: a)
		out << row << std::endl;

	return out;
}

inline bool checkInputMatrixDimension(uint32_t n, uint32_t m)
{
	return (n > 0 && n < 25 && m > 0 && m < 666);
}

matrix readMatrixFromFile(const std::string &filename)
{
	std::ifstream fin(filename);
	matrix a;
	uint32_t n, m;

	if (!fin.is_open())
		return a;

	fin >> n >> m;

	if (!checkInputMatrixDimension(n, m))
		return a;

	a.resize(n);

	for (size_t i = 0; i < n; ++i)
		for (size_t j = 0; j < m; ++j)
		{
			uint32_t t;

			if (!(fin >> t))
				return matrix();

			a[i].push_back((bool)t);
		}

	return a;
}

matrix generateRandomMatrix(uint32_t n, uint32_t m)
{
	matrix a;

	if (!checkInputMatrixDimension(n, m))
		return a;

	a.resize(n);
	srand(time(0));

	for (size_t i = 0; i < n; ++i)
	{
		uint32_t sum = 0;

		for (size_t j = 0; j < m; ++j)
		{
			// Увеличиваем вероятность того, что проекту не нужен конкретный ресурс
			uint32_t t = (bool)!(rand() % 4);

			a[i].push_back(t);
			sum += t;
		}

		// Если проекту не требуются ресурсы
		if (!sum)
			// Принуждаем ему к какому-нибудь одному ресурсу
			a[i][rand() % m] = true;
	}

	return a;
}

void gray(matrix &codes, size_t len)
{
	if (!len)
		return;

	if (codes.empty())
	{
		codes.resize(2);
		codes[0].push_back(0);
		codes[1].push_back(1);
	}
	else
	{
		for (auto& code: codes)
			code.push_back(0);

		matrix reverseCodes;

		for (auto it = codes.rbegin(); it != codes.rend(); it++)
		{
			auto x = *it;

			*x.rbegin() = 1;
			reverseCodes.push_back(x);
		}

		codes.insert(codes.end(), reverseCodes.begin(), reverseCodes.end());
	}

	gray(codes, len - 1);
}

matrix solveMatrix(const matrix &a)
{
	// Количество проектов
	const uint32_t n = a.size();
	// Количество ресурсов
	const uint32_t m = a.front().size();

	matrix codes;
	matrix s;

	gray(codes, n);

	uint32_t maxX = 0;

	for (auto code: codes)
	{
		std::vector < bool > x(m, false);
		bool isSuitable = true;

		for (size_t i = 0; (i < n) && isSuitable; ++i)
		{
			for (size_t j = 0; (j < m) && isSuitable; ++j)
				if (code[i] && a[i][j])
				{
					if (x[j])
						isSuitable = false;
					else
						x[j] = true;
				}
		}

		if (!isSuitable)
			continue;

		uint32_t currX = std::accumulate(code.begin(), code.end(), (uint32_t)0);

		if (currX < maxX)
			continue;

		if (currX > maxX)
		{
			maxX = currX;
			s.clear();
		}

		s.push_back(code);
	}

	return s;
}

void displaySolutions(const matrix &a, const matrix& solutions)
{
	initscr();

	if (!has_colors() || start_color() != OK)
	{
		std::cout << "Терминал не поддерживает цвета, поэтому будет выведен сырой результат кодов Грея." << std::endl;
		std::cout << solutions << std::endl;
		return;
	}

	start_color();
	init_pair(1, COLOR_RED, COLOR_BLACK);

	const int tableY = 5;
	const int tableX = 5;
	const size_t n = a.size();
	const size_t m = a.front().size();

	int index = 0;

	keypad(stdscr, TRUE);
	noecho();

	int ch = 0;

	while (true)
	{
		refresh();

		bool isExit = false;

		switch(ch)
		{
			case KEY_UP:
			case KEY_LEFT:
			case 'a':
				--index;
				break;

			case KEY_DOWN:
			case KEY_RIGHT:
			case 'd':
				++index;
				break;

			case 'q':
			case 'Q':
				isExit = true;
				break;
		}

		if (isExit)
			break;

		index = (index + solutions.size()) % solutions.size();

		int row, column;

		getmaxyx(stdscr, row, column);
		clear();
		mvprintw(0, 0, "Вывод решения %d из %d:", index + 1, solutions.size());
		mvaddstr(1, 0, "Множество проектов: {");

		bool isFirstProject = true;

		for (size_t i = 0; i < n; ++i)
			if (solutions[index][i])
			{
				if (!isFirstProject)
					addstr(", ");

				isFirstProject = false;

				printw("s%d", (i + 1));
			}

		addstr("}\n");

		int resourcesCount = 0;

		for (size_t i = 0; i < n; ++i)
		{
			if (!solutions[index][i])
				continue;

			for (size_t j = 0; j < m; ++j)
				resourcesCount += (int)a[i][j];
		}

		printw("Количество задействованных ресурсов: %d из %d.", resourcesCount, m);
		mvaddstr(tableY - 1, 0, "S \\ R");

		for (size_t j = 0; j < m; ++j)
			mvprintw(tableY - 1, tableX + 3 * j, "%3d", j + 1);

		for (size_t i = 0; i < n; ++i)
		{
			if (solutions[index][i])
			{
				attrset(A_BOLD);
				attron(COLOR_PAIR(1));
			}

			mvprintw(tableY + i, 0, "%2d", i + 1);
			attrset(A_NORMAL);
		}

		for (size_t i = 0; i < n; ++i)
			for (size_t j = 0; j < m; ++j)
				if (a[i][j])
				{
					if (solutions[index][i])
					{
						attrset(A_BOLD);
						attron(COLOR_PAIR(1));
					}

					mvprintw(tableY + i, tableX + 3 * j, "%3c", '*');
					attrset(A_NORMAL);
				}

		mvaddstr(row - 1, 0, "Для перехода между решениями используйте кнопки со стрелками; для выхода нажмите Q.");
		ch = getch();
	}

	endwin();
}

int main(int argc, char *argv[])
{
	setlocale(LC_CTYPE, "ru_RU.UTF8");

	std::string filename;
	bool isRandom;
	bool help;
	bool isOnlyOutput, isOnlyRaw;
	uint32_t n, m;
	Flags flags;

	flags.Var(filename, 'f', "filename", std::string(), "Имя файла с матрицей A, описывающей потребность проектов в ресурсах", "Матрица из файла");
	flags.Bool(isRandom, 'r', "random", "Режим генерации случайной матрицы A", "Случайная матрица");
	flags.Var(n, 'n', "projects", uint32_t(5), "Количество проектов", "Случайная матрица");
	flags.Var(m, 'm', "resources", uint32_t(7), "Количество ресурсов", "Случайная матрица");

	flags.Bool(isOnlyOutput, 'o', "output", "Вывести подготовленную матрицу и выйти", "Отладка");
	flags.Bool(isOnlyRaw, 'R', "raw", "Вывести сырой результат и выйти", "Отладка");
	flags.Bool(help, 'h', "help", "Показать помощь и выйти");

    if (!flags.Parse(argc, argv))
	{
		flags.PrintHelp(argv[0]);
		return 1;
    }

	if (help || (!isRandom && filename.empty()))
	{
		flags.PrintHelp(argv[0]);
		return 0;
    }

	matrix a;

	if (!filename.empty())
		a = readMatrixFromFile(filename);
	else if (isRandom)
		a = generateRandomMatrix(n, m);

	if (a.empty())
	{
		std::cout << "Ошибка подготовки матрицы!" << std::endl;
		return 2;
	}

	if (isOnlyOutput)
	{
		std::cout << a.size() << " " << a.front().size() << std::endl << a << std::endl;
		return 0;
	}

	auto solutions = solveMatrix(a);

	if (solutions.empty())
	{
		std::cout << "Не удалось найти решения!" << std::endl;
		return 3;
	}

	if (isOnlyRaw)
	{
		std::cout << solutions << std::endl;
	}
	else
		displaySolutions(a, solutions);

	return 0;
}
