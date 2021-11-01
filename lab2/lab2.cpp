#include <iostream>
#include <fstream>
#include <string.h>
#include <omp.h>

// Определение типа для функций тестирования (имеют общуу структуру но вызывают соответствующие функции)
typedef double(*TestFunctTempl)(long&);
// Функция рассчета среднеарифметического значения в доверительном интервале
double AvgTrustedInterval(double& avg, double*& times, int& cnt);
// Функция для замера времени Функции работы с данными 
double TestIter(void* Funct, long size, int iterations);

/// Функции работы с данными

// Вычисление Pi последовательно
double Pi_Single(long num_steps);
// Статическое распределение
double Pi_SheduleStatic(long num_steps);
// Динамическое распределение
double Pi_SheduleDynamic(long num_steps);
// Управляемое распределение
double Pi_SheduleGuided(long num_steps);
// Sections распределение
double Pi_Section(long num_steps);
// Sections распределение
double Pi_ParSer(long num_steps);

/// Функции тестирования 

// Вычисление Pi последовательно
double TestPi_Single(long& num_steps)
{
	return Pi_Single(num_steps);
}
// Статическое распределение
double TestPi_SheduleStatic(long& num_steps)
{
	return Pi_SheduleStatic(num_steps);
}
// Динамическое распределение
double TestPi_SheduleDynamic(long& num_steps)
{
	return Pi_SheduleDynamic(num_steps);
}
// Управляемое распределение
double TestPi_SheduleGuided(long& num_steps)
{
	return Pi_SheduleGuided(num_steps);
}
// Секционное распределение
double TestPi_SheduleSection(long& num_steps)
{
	return Pi_Section(num_steps);
}
// Параллельно-последовательное распределение
double TestPi_ParSer(long& num_steps)
{
	return Pi_ParSer(num_steps);
}

// Реализация функций
double Pi_Single(long num_steps)
{
	double t_start = omp_get_wtime();

	double step, pi, x, sum = 0.0;
	int i;
	step = 1.0 / (double)num_steps;
#pragma omp single
	{
		for (i = 0; i < num_steps; i++)
		{
			x = (i + 0.5) * step;
			sum = sum + 4.0 / (1.0 + x * x);
		}
	}
	pi = step * sum;
	//printf("Pi = %f\n", pi);

	double t_end = omp_get_wtime();
	return t_end - t_start;
}

double Pi_SheduleStatic(long num_steps)
{
	double t_start = omp_get_wtime();

	double step, pi, x, sum = 0.0;
	int i;
	step = 1.0 / (double)num_steps;

#pragma omp parallel for schedule(static, omp_get_num_threads())
		for (i = 0; i < num_steps; i++)
		{
			x = (i + 0.5) * step;
			sum = sum + 4.0 / (1.0 + x * x);
		}
	pi = step * sum;
	//printf("Pi = %f\n", pi);

	double t_end = omp_get_wtime();
	return t_end - t_start;
}

double Pi_SheduleDynamic(long num_steps)
{
	double t_start = omp_get_wtime();

	double step, pi, x, sum = 0.0;
	int i;
	step = 1.0 / (double)num_steps;

#pragma omp parallel for schedule(dynamic)
	for (i = 0; i < num_steps; i++)
	{
		x = (i + 0.5) * step;
		sum = sum + 4.0 / (1.0 + x * x);
	}
	pi = step * sum;
	//printf("Pi = %f\n", pi);

	double t_end = omp_get_wtime();
	return t_end - t_start;
}

double Pi_SheduleGuided(long num_steps)
{
	double t_start = omp_get_wtime();

	double step, pi, x, sum = 0.0;
	int i;
	step = 1.0 / (double)num_steps;

#pragma omp parallel for schedule(guided)
	for (i = 0; i < num_steps; i++)
	{
		x = (i + 0.5) * step;
		sum = sum + 4.0 / (1.0 + x * x);
	}
	pi = step * sum;
	//printf("Pi = %f\n", pi);

	double t_end = omp_get_wtime();
	return t_end - t_start;
}

double Pi_Section(long num_steps)
{
	double t_start = omp_get_wtime();

	double step, pi, x, sum = 0.0;
	int i;
	step = 1.0 / (double)num_steps;

#pragma omp parallel sections
	{
#pragma omp section
		for (i = 0; i < num_steps / omp_get_num_threads(); i++)
		{
			x = (i + 0.5) * step;
			sum = sum + 4.0 / (1.0 + x * x);
		}
#pragma omp section
		if (omp_get_num_threads() > 1)
			for (i = num_steps / omp_get_num_threads(); i < num_steps * 2 / omp_get_num_threads(); i++)
			{
				x = (i + 0.5) * step;
				sum = sum + 4.0 / (1.0 + x * x);
			}
#pragma omp section
		if (omp_get_num_threads() > 2)
			for (i = num_steps * 2 / omp_get_num_threads(); i < num_steps * 3 / omp_get_num_threads(); i++)
			{
				x = (i + 0.5) * step;
				sum = sum + 4.0 / (1.0 + x * x);
			}
#pragma omp section
		if (omp_get_num_threads() > 3)
			for (i = num_steps * 3 / omp_get_num_threads(); i < num_steps * 4 / omp_get_num_threads(); i++)
			{
				x = (i + 0.5) * step;
				sum = sum + 4.0 / (1.0 + x * x);
			}
	}

	pi = step * sum;
	//printf("Pi = %f\n", pi);

	double t_end = omp_get_wtime();
	return t_end - t_start;
}

double Pi_ParSer(long num_steps)
{
	double t_start = omp_get_wtime();

	double step, pi, x, sum = 0.0;
	int i;
	step = 1.0 / (double)num_steps;
#pragma omp parallel for
		for (i = 0; i < num_steps; i++)
		{
#pragma omp critical
			x = (i + 0.5) * step;
			sum = sum + 4.0 / (1.0 + x * x);
		}
	pi = step * sum;
	//printf("Pi = %f\n", pi);

	double t_end = omp_get_wtime();
	return t_end - t_start;
}

double TestIter(void* Funct, long size, int iterations)
{
	double curtime = 0, avgTime = 0, avgTimeT = 0, correctAVG = 0;;
	double* Times = new double[iterations];
	for (int i = 0; i < iterations; i++)
	{
		// Запуск функции и получение времени в миллисекундах
		curtime = ((*(TestFunctTempl)Funct)(size)) * 1000;
		// запись времени в массив для определения среднеарифметического значения в доверительном интервале
		Times[i] = curtime;
		//       std::cout << curtime << std::endl;
	}
	//std::cout << "AvgTime:" << avgTime << std::endl;
	// Определения среднеарифметического значения в доверительном интервале по всем итерациям и вывод значения на экран
	avgTimeT = AvgTrustedInterval(avgTime, Times, iterations);
	//std::cout << "AvgTimeTrusted:" << avgTimeT << std::endl;
	return avgTimeT;
}

// Функция рассчета среднеарифметического значения в доверительном интервале
double AvgTrustedInterval(double& avg, double*& times, int& cnt)
{
	double sd = 0, newAVg = 0;
	int newCnt = 0;
	for (int i = 0; i < cnt; i++)
	{
		sd += (times[i] - avg) * (times[i] - avg);
	}
	sd /= (cnt - 1.0);
	sd = sqrt(sd);
	for (int i = 0; i < cnt; i++)
	{
		if (avg - sd <= times[i] && times[i] <= avg + sd)
		{
			newAVg += times[i];
			newCnt++;
		}
	}
	if (newCnt == 0) newCnt = 1;
	return newAVg / newCnt;
}

void test_functions(void** Functions, std::string(&function_names)[6], long size, int iterations)
{
	double Functions_time_ms[6];
	// проведение замеров времени работы по каждой функции
	for (int i = 0; i < 6; i++)
		Functions_time_ms[i] = TestIter(Functions[i], size, iterations);

	std::ofstream out;          // поток для записи
	out.open("result.txt", std::ios::app); // окрываем файл для записи

	if (out.is_open())
		// Вывод результатов замера (можно организовать вывод в файл)
		for (int i = 0; i < 6; i++)
			out << function_names[i] << "\t\t\t" << Functions_time_ms[i] << " ms." << std::endl;

	out << std::endl;
	out.close();
}

int main()
{
	void** Functions = new void* [6]{ TestPi_Single, Pi_SheduleStatic, TestPi_SheduleDynamic, TestPi_SheduleGuided, TestPi_SheduleSection, TestPi_ParSer };
	std::string  function_names[6]{ "Single", "Static", "Dynamic", "Guided", "Sections", "ParSer" };
	setlocale(LC_ALL, "Russian");

	std::ofstream out;
	out.open("result.txt", std::ios::app);
	if (out.is_open())
		out << "|||||-Замеры времени начаты: " << __TIMESTAMP__ << "-|||||" << std::endl;
	out.close();

	for (int i = 1; i < 5; i++)
	{
		out.open("result.txt", std::ios::app);
		if (out.is_open())
			out << "\t\tПОТОКОВ: " << i << std::endl << std::endl;
		out.close();

		std::cout << "\t\tПОТОКОВ: " << i << std::endl;
		omp_set_num_threads(i);

		for (long j = 125000; j <= 500000; j += 125000)
		{
			out.open("result.txt", std::ios::app);
			if (out.is_open())
				out << "\tИТЕРАЦИЙ: " << j << std::endl << std::endl;
			out.close();

			test_functions(Functions, function_names, j, 30);
		}
	}
	std::cout << "Замеры времени выполнения функций выполнены, все данные записаны в файл result.txt." << std::endl;
	system("result.txt");
}