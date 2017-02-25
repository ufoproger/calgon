#include <numeric>
#include <iostream>
#include <fstream>
#include <string>
#include <utility>
#include <cmath>

#include <ncurses.h>

#include "flags.hh/Flags.hh"
#include "lab2_types.hh"

typedef std::pair < int , int > point;
typedef std::vector < point > points;

std::ostream& operator<<(std::ostream &out, const point &p)
{
	out << "(" << p.first << ", " << p.second << ")";

	return out;
}

std::istream& operator>>(std::istream &in, point &p)
{
	in >> p.first >> p.second;

	return in;
}

vv algo_cnk(size_t n, size_t k)
{
	vv result;

	if (k > n)
		return result;

	v a;

	for (size_t i = 0; i < k; ++i)
		a.push_back(i);

	if (n == k)
		return vv(1, a);

	for (int p = (int)k - 1;;)
	{
		result.push_back(a);

		if (a.back() + 1 >= n)
			--p;
		else
			p = k - 1;

		if (p < 0)
			break;

		for (int i = (int)k - 1; i >= p; --i)
			a[i] = a[p] + i - p + 1;
	}

	return result;
}

inline bool isOnLine(const point &a, const point &b, const point &c)
{
	return (c.first - a.first) * (b.second - a.second) == (b.first - a.first) * (c.second - a.second);
}

double lengthBetweenPoints(const point &a, const point &b)
{
	return sqrt(pow((double)(b.first - a.first), 2.) + pow((double)(b.second - a.second), 2.));
}

double geronSquare(const point &a, const point &b, const point &c)
{
	double lA = lengthBetweenPoints(a, b);
	double lB = lengthBetweenPoints(b, c);
	double lC = lengthBetweenPoints(c, a);
	double p = (lA + lB + lC) * 0.5;

	return sqrt(p * (p - lA) * (p - lB) * (p - lC));
}

points readPointsFromFile(const std::string &filename)
{
	std::ifstream fin(filename);
	points points;

	if (!fin.is_open())
		return points;

	for (point p; fin >> p;)
		points.push_back(p);

	return points;
}

points generateRandomPoints(size_t n, size_t maxX, size_t maxY)
{
	points points;

	srand(time(NULL));

	for (size_t i = 0; i < n; ++i)
	{
		point point;

		while (true)
		{
			point.first = rand() % maxX + 1;
			point.second = rand() % maxY + 1;

			bool f = true;

			for (auto p: points)
				if (lengthBetweenPoints(point, p) < 2.)
				{
					f = false;
					break;
				}

			if (f)
				break;
		}

		points.push_back(point);
	}

	return points;
}

vv solve(const points &points)
{
	const double epsilon = 1e-5;
	vv solutions;
	double s = 0.;

	auto a = algo_cnk(points.size(), 3);

	for (auto sample: a)
	{
		point p1 = points[sample[0]];
		point p2 = points[sample[1]];
		point p3 = points[sample[2]];

		if (isOnLine(p1, p2, p3))
			continue;

		double currS = geronSquare(p1, p2, p3);

		if (s != 0. && currS > s)
			continue;

		if (fabs(currS - s) > epsilon)
			solutions.clear();

		s = currS;
		solutions.push_back(sample);
	}

	return solutions;
}

void displaySolutions(const points &points, const vv &solutions)
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

		const int gridY = row - 4;
		const int gridX = 4;

		clear();
		mvprintw(0, 0, "Вывод решения %d из %d:", index + 1, solutions.size());

		// Ось абсцисс
		mvhline(gridY, gridX, 0, column - gridX);
		mvaddch(2, gridX, ACS_UARROW);

		// Ось ординат
		mvvline(3, gridX, 0, gridY - 3);
		mvaddch(gridY, column - 1, ACS_RARROW);

		// Начало осей координат
		mvaddch(gridY, gridX, ACS_LLCORNER);
		mvaddch(gridY + 1, gridX - 2, '0');

		// Нумеруем ось ординат
		for (int i = 1; i < column - gridX - 1; ++i)
		{
			bool mark = false;

			if (i <= 10)
			{
				if (i % 2)
				{
					mvprintw(gridY + 1, gridX + i, "%d", i);
					mark = true;
				}
			}
			else
			{
				if (i % 5 == 0)
				{
					mvprintw(gridY + 1, gridX + i - 1, "%2d", i);
					mark = true;
				}
			}



			if (mark)
				mvaddch(gridY, gridX + i, ACS_TTEE);
		}

		// Нумеруем ось ординат
		for (int i = gridY - 1; i > 2; --i)
		{
			mvprintw(i, 0, "%3d", gridY - i);
			mvaddch(i, gridX, ACS_RTEE);
		}

		v solution = solutions[index];
		point p1 = points[solution[0]];
		point p2 = points[solution[1]];
		point p3 = points[solution[2]];

		mvprintw(1, 0, "Вершины треугольника: (%d, %d), (%d, %d), (%d, %d), площадь %.2f.",
			p1.first, p1.second,
			p2.first, p2.second,
			p3.first, p3.second,
			geronSquare(p1, p2, p3)
		);

		for (auto p: points)
		{
			if (p == p1 || p == p2 || p == p3)
			{
				attrset(A_BOLD);
				attron(COLOR_PAIR(1));
			}

			mvaddch(gridY - p.second, gridX + p.first, ACS_DIAMOND);
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
	bool random, help, output, algorithm;
 	size_t n, k, maxX, maxY;
	Flags flags;

	flags.Var(filename, 'f', "filename", std::string(), "Файл, содержащий список точек", "Список точек из файла");
	flags.Bool(random, 'r', "random", "Случайно расположить точки на плоскости", "Случайный список точек");
	flags.Var(n, 'n', "count", size_t(7), "Количество точек", "Случайный список точек");
	flags.Var(maxX, 'x', "maxX", size_t(50), "Максимальная координата по оси абсцисс", "Случайный список точек");
	flags.Var(maxY, 'y', "maxY", size_t(30), "Максимальная координата по оси ординат", "Случайный список точек");

	flags.Bool(algorithm, 'a', "algorithm", "Посчитать C_n^k", "Отладка");
	flags.Bool(output, 'o', "output", "Вывести все подмножества выборок", "Отладка");
	flags.Var(k, 'k', "k", size_t(5), "Количество элементов в выборке", "Отладка");
	flags.Bool(help, 'h', "help", "Показать помощь и выйти");

    if (!flags.Parse(argc, argv))
	{
		flags.PrintHelp(argv[0]);

		return 1;
    }

	if (algorithm)
	{
		std::cout << "Рассчёт c(n, k): " << std::endl;

		auto a = algo_cnk(n, k);

		std::cout << "c(" << n << ", " << k << ") = " << a.size() << "." << std::endl;

		if (output)
			std::cout << vv_plus(a) << std::endl;

		return 0;
	}

	if (help || (!random && filename.empty()))
	{
		flags.PrintHelp(argv[0]);
		return 0;
    }

	points points;

	if (!filename.empty())
		points = readPointsFromFile(filename);
	else if (random)
		points = generateRandomPoints(n, maxX, maxY);

	if (points.empty())
	{
		std::cout << "Ошибка получения точек." << std::endl;
		return 1;
	}

	auto solutions = solve(points);

	displaySolutions(points, solutions);

	return 0;
}
