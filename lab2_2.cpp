#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <algorithm>

#include "flags.hh/Flags.hh"
#include "lab2_types.hh"

vv algoPermutation(size_t n)
{
	vv result;

	v a;

	for (size_t i = 0; i < n; ++i)
		a.push_back(i);

	while (true)
	{
		result.push_back(a);

		int i;

		for (i = (int)n - 2; i >= 0; --i)
			if (a[i] < a[i + 1])
				break;

		if (i < 0)
			break;

		int minJ = -1;

		for (int j = i + 1; j < (int)n; ++j)
			if (minJ == -1 || (a[i] < a[j] && a[j] < a[minJ]))
				minJ = j;

		std::swap(a[i], a[minJ]);
		std::reverse(a.begin() + i + 1, a.end());
	}

	return result;
}

bool readFromFile(const std::string &filename, vv &C, vv &V, vv &A)
{
	std::ifstream fin(filename);
	
	if (!fin.is_open())
		return false;

	size_t n;

	fin >> n;

	for (size_t k = 0; k < 3; ++k)
	{
		vv *t;

		switch (k)
		{
			case 0:
				t = &C;
				break;

			case 1:
				t = &V;
				break;

			case 2:
				t = &A;
				break;
		};

		t->clear();

		for (size_t i = 0; i < n; ++i)
			t->push_back(v(n, 0));

		for (size_t i = 0; i < n; ++i)
			for (size_t j = 0; j < n; ++j)
				if (!(fin >> (*t)[i][j]))
					return false;
	}

	return true;
}

size_t calcS(const vv &C, const vv &V, const v &p)
{
	const size_t n = C.size();

	size_t s = 0;

	for (size_t i = 0; i < n; ++i)
		for (size_t j = 0; j < n; ++j)
			s += C[i][j] * V[p[i]][p[j]];

	return s;
}

vv solve(const vv &C, const vv &V, const vv &A)
{
	const size_t n = C.size();

	vv solutions;
	vv permutations = algoPermutation(n);
	size_t minS = 0;

	for (auto p: permutations)
	{
		bool f = false;

		// Проверяем возможность постройки p[i] завода в городе i
		for (size_t i = 0; i < n; ++i)
			if (A[p[i]][i])
			{
				f = true;
				break;
			}

		// Если какой-то завод нельзя строить, то даже и не пытаемся
		if (f)
			continue;

		size_t s = calcS(C, V, p);

		if (solutions.empty())
			minS = s;

		if (minS < s)
			continue;

		if (s < minS)
			solutions.clear();

		minS = s;
		solutions.push_back(p);
	}

	return solutions;
}

int main(int argc, char *argv[])
{
	setlocale(LC_CTYPE, "ru_RU.UTF8");

	std::string filename;
	bool help, output, algorithm;
	size_t n;
	Flags flags;

	flags.Var(filename, 'f', "filename", std::string(), "Файл, содержащий матрицы C, V, A", "");
	flags.Bool(algorithm, 'a', "algorithm", "Посчитать перестановки", "Отладка");
	flags.Bool(output, 'o', "output", "Вывести все перестановки", "Отладка");
	flags.Var(n, 'n', "count", size_t(5), "Количество элементов", "Отладка");
	flags.Bool(help, 'h', "help", "Показать помощь и выйти");

    if (!flags.Parse(argc, argv))
	{
		flags.PrintHelp(argv[0]);

		return 1;
    }

	if (algorithm)
	{
		std::cout << "Рассчёт перестановок: " << std::endl;

		auto permutations = algoPermutation(n);

		std::cout << n << "! = " << permutations.size() << std::endl;

		if (output)
			std::cout << vv_plus(permutations) << std::endl;

		return 0;
	}

	if (help || filename.empty())
	{
		flags.PrintHelp(argv[0]);
		return 0;
    }

    vv C, V, A;

    if (!readFromFile(filename, C, V, A))
    {
    	std::cout << "Ошибка чтения исходных данных." << std::endl;
    }

    std::cout << "Матрица C (стоимость переводки грузов между городами): " << std::endl << C << std::endl;
    std::cout << "Матрица V (необходимые объемы перевозок между заводами): " << std::endl << V << std::endl;
    std::cout << "Матрица A (возможность размещения заводов в городах): " << std::endl << A << std::endl;

	auto solutions = solve(C, V, A);
	
	std::cout << "Количество решений: " << solutions.size() << std::endl;
		
	for (auto sample: solutions)
	{
		std::cout << v_plus(sample);
		std::cout << ": s = " << calcS(C, V, sample) << std::endl;
	}

	return 0;
}
