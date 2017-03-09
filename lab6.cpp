#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include <numeric>
#include <algorithm>
#include <type_traits>
#include <utility>
#include <iomanip>

#include "iconv.h"
#include "lab2_types.hh"
#include "flags.hh/Flags.hh"

const size_t BUF_SIZE = 1024;

bool debug = false;

std::string _iconv(const std::string &text, const std::string &from, const std::string &to)
{
	std::string result;
    iconv_t cd;

    cd = iconv_open(to.c_str(), from.c_str());

    if (cd == (iconv_t)(-1))
    	throw std::runtime_error("Не удалось открыть дескриптор iconv");

    size_t in_size = text.size();
    const char *in = text.c_str();

    char out_buf[BUF_SIZE];
    char *out;
    size_t out_size;

    size_t  k;

    while(in_size > 0)
    {
        out = out_buf;
        out_size = BUF_SIZE;

        errno = 0;
        k = iconv(cd, &in,  &in_size, &out, &out_size );

        if (k == (size_t)-1 && errno)
			throw std::runtime_error("Ошибка");

        result.append(out_buf, BUF_SIZE - out_size);
    }

     if (iconv_close(cd) != 0)
	 {
        std::runtime_error("Не удается закрыть дескриптор iconv");
	}

	return result;
}

std::string encode(const std::string &text)
{
	return _iconv(text, "UTF8", "CP1251");
}

std::string decode(const std::string &text)
{
	return _iconv(text, "CP1251", "UTF8");
}

std::string decode(char ch)
{
	std::string text;
	text += ch;
	return decode(text);
}

float codes_l_average(const v_float &p, const v_string &codes)
{
	float average = 0.;

	for (size_t i = 0; i < codes.size(); ++i)
		average += p[i] * codes[i].length();

	return average;
}

v_string haffman(v_float p)
{
	std::sort(p.begin(), p.end());
	v_float pOriginal(p);
	v pos;

	while (p.size() > 2)
	{
		float sum = p[0] + p[1];

		p.erase(p.begin(), p.begin() + 2);

		auto it = std::lower_bound(p.begin(), p.end(), sum);

		pos.push_back(p.size() - (it - p.begin()));
		p.insert(it, sum);
	}

	if (debug)
		std::cout << "Массив значений j: " << v_plus(pos) << std::endl;

	v_string codes({"0", "1"});

	for (auto it = pos.rbegin(); it != pos.rend(); ++it)
	{
		std::string code = codes[*it];

		codes.erase(codes.begin() + *it);
		codes.push_back(code + "0");
		codes.push_back(code + "1");
	}

	std::sort(pOriginal.rbegin(), pOriginal.rend());

	if (debug)
	{
		float avg = codes_l_average(pOriginal, codes);
		float mid = (1. - avg / 8.) * 100.;
		std::cout << "Построенные коды: " << codes << std::endl;
		std::cout << "l_ср = " << avg << "." << std::endl;
		std::cout << "К_сж = " << std::fixed << std::setprecision(1) << mid << "%." << std::endl;
	}

	return codes;
}

void task1(const std::string &filename)
{
	std::ifstream fin(filename);

	if (!fin.is_open())
	{
		std::cout << "Ошибка открытия файла \"" << filename << "\"." << std::endl;
		return;
	}

	v_float p;

	for (float t; fin >> t;)
		p.push_back(t);

	std::cout << "Прочитанный массив вероятностей (n = " << p.size() << "): " << p << std::endl;

	float pSum =std::accumulate(p.begin(), p.end(), 0.);

	if (fabs(pSum - 1.) > 1e5)
		std::cout << "Сумма вероятностей не равна 1. Её значение: " << pSum << "." << std::endl;

	auto codes = haffman(p);
}

void task2(const std::string &filename)
{
	std::ifstream fin(filename);

	if (!fin.is_open())
	{
		std::cout << "Ошибка открытия файла \"" << filename << "\"." << std::endl;
		return;
	}

	std::string text, line;

	while (getline(fin, line))
	{
		if (!text.empty())
			text += "\n";

		text += line;
	}

	std::cout << "Прочитанный текст: " << std::endl << text << std::endl << std::endl;

	text = encode(text);
	std::string textOriginal(text);

	std::sort(text.begin(), text.end());

	typedef std::pair < char , size_t > pair;

	std::vector < pair > pairs;

	for (size_t i = 1, seq = 1, len = text.size(); i < len; ++i, ++seq)
	{
		if (text[i] == text[i - 1] && i + 1 != len)
			continue;

		pairs.push_back(pair(text[i - 1], seq));
		seq = 0;
	}

	std::sort(pairs.begin(), pairs.end(), [=](const pair &a, const pair &b) -> bool
	{
		return a.second >= b.second;
	});

	v_float p;

	for (auto value: pairs)
		p.push_back((float)value.second / (float)text.size());

	if (debug)
	{
		std::cout << "Частота встречаемости символов в тексте: " << std::endl;

		size_t i = 0;

		for (auto value: pairs)
		{
			if (value.first == 10)
				std::cout << "[Enter]";
			else
				std::cout << "    \"" << decode(value.first) << "\"";

			std::cout << " - " << value.second << "(" << p[i++] << ")" << std::endl;
		}
	}

	auto codes = haffman(p);

	if (debug)
	{
		std::cout << "Построенные коды: " << std::endl;

		for (size_t i = 0; i < pairs.size(); ++i)
		{
			if (pairs[i].first == 10)
				std::cout << "[Enter]";
			else
				std::cout << "    \"" << decode(pairs[i].first) << "\"";

			std::cout << " - " <<  codes[i] << std::endl;
		}

		std::cout << "l_ср = " << codes_l_average(p, codes) << "." << std::endl;
	}

	std::string textCodes;

	for (auto ch: textOriginal)
		for (size_t i = 0; i < pairs.size(); ++i)
			if (pairs[i].first == ch)
			{
				textCodes.append(codes[i]);
				break;
			}

	std::cout << "Закодированный текст: " << std::endl << textCodes << std::endl;
	std::cout << "Исходный размер текста: " << (text.size() * 8) << " бит." << std::endl;
	std::cout << "Размер закодированного текста: " << textCodes.size() << " бит." << std::endl;
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
