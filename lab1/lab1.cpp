#include <iostream>
#include <fstream>
#include <string.h>
#include <omp.h>

void lab1_1()
{
#pragma omp single
	{
		printf("Всего потоков: %d\n\n", omp_get_max_threads());
	}

	int a = 0;
	std::cout << "От скольки потоков вывести Hello World?: ";
	std::cin >> a;
	std::cout << std::endl;
	omp_set_num_threads(a);

	// Обработка в параллельной области
#pragma omp parallel
	{
		printf("Hello World! От потока %d\n", omp_get_thread_num());
	}
}

// Определение типа для функций тестирования (имеют общуу структуру но вызывают соответствующие функции)
typedef double(*TestFunctTempl)(double*&, double*&, double*&, int&);
// Функция рассчета среднеарифметического значения в доверительном интервале
double AvgTrustedInterval(double& avg, double*& times, int& cnt);
// Функция для замера времени Функции работы с данными 
double TestIter(void* Funct, double*& arr_in1, double*& arr_in2, double*& arr_res, int size, int iterations);

/// Функции работы с данными
// заполнение массивов два варианта
// последовательно
double Fillarr1(double* arr, int size);
double Fillarr2(double* arr, int size);
// параллельно
double Fillarr1Par(double* arr, int size);
double Fillarr2Par(double* arr, int size);
// Сложение массивов 
double SumArrays(double*& arr_in1, double*& arr_in2, double*& arr_res, int& size);
double SumArraysParFor(double*& arr_in1, double*& arr_in2, double*& arr_res, int& size);
double SumArraysParSec(double*& arr_in1, double*& arr_in2, double*& arr_res, int& size);
// Получение суммы элементов массива 
double ElArrSum(double*& arr, int& size, double& sum);
double ElArrSumParRed(double*& arr, int& size, double& sum);
double ElArrSumParCritical(double*& arr, int& size, double& sum);

/// Функции тестирования 

// Заполнение массива 1 последовательно
double TestFillarr1(double*& arr, double*& empty1, double*& empty2, int& size)
{
	return Fillarr1(arr, size);
}

// Заполнение массива 2 последовательно
double TestFillarr2(double*& empty1, double*& arr, double*& empty2, int& size)
{
	return Fillarr2(arr, size);
}

// Заполнение массива 1 параллельно
double TestFillarr1Par(double*& arr, double*& empty1, double*& empty2, int& size)
{
	return Fillarr1Par(arr, size);
}

// Заполнение массива 2 параллельно
double TestFillarr2Par(double*& empty1, double*& arr, double*& empty2, int& size)
{
	return Fillarr2Par(arr, size);
}

// Сложение массивов
double TestSum(double*& arr_in1, double*& arr_in2, double*& arr_res, int& size)
{
	return SumArrays(arr_in1, arr_in2, arr_res, size);
}

// Сложение массивов (параллельно)
double TestSumParFor(double*& arr_in1, double*& arr_in2, double*& arr_res, int& size)
{
	return SumArraysParFor(arr_in1, arr_in2, arr_res, size);
}

// Сложение массивов (параллельно с исп. sections)
double TestSumParSec(double*& arr_in1, double*& arr_in2, double*& arr_res, int& size)
{
	return SumArraysParSec(arr_in1, arr_in2, arr_res, size);
}

double TestElArrSum(double*& arr, double*& empty1, double*& empty2, int& size)
{
	double summel = 0;
	return ElArrSum(arr, size, summel);
}

double TestElArrSumParRed(double*& arr, double*& empty1, double*& empty2, int& size)
{
	double summel = 0;
	return ElArrSumParRed(arr, size, summel);
}

double TestElArrSumParCritical(double*& arr, double*& empty1, double*& empty2, int& size)
{
	double summel = 0;
	return ElArrSumParCritical(arr, size, summel);
}

// Реализация функций
double Fillarr1(double* arr, int size)
{
	double t_start = omp_get_wtime();

	for (int i = 0; i < size; i++)
		if (i % 2 == 0) // чёт
			arr[i] = (i * sin(i));
		else // не чёт
			arr[i] = (i * cos(i));

	double t_end = omp_get_wtime();
	return t_end - t_start;
}

double Fillarr2(double* arr, int size)
{
	double t_start = omp_get_wtime();

	for (int i = 0; i < size; i++)
		if (i % 2 == 0) // чёт
			arr[i] = (i * sin(i));
		else // не чёт
			arr[i] = (i * cos(i));

	double t_end = omp_get_wtime();
	return t_end - t_start;
}

double Fillarr1Par(double* arr, int size)
{
	double t_start = omp_get_wtime();

#pragma omp parallel for
	for (int i = 0; i < size; i++)
		if (i % 2 == 0) // чёт
			arr[i] = (i * sin(i));
		else // не чёт
			arr[i] = (i * cos(i));

	double t_end = omp_get_wtime();
	return t_end - t_start;
}

double Fillarr2Par(double* arr, int size)
{
	double t_start = omp_get_wtime();

#pragma omp parallel for
	for (int i = 0; i < size; i++)
		if (i % 2 == 0) // чёт
			arr[i] = (i * sin(i));
		else // не чёт
			arr[i] = (i * cos(i));

	double t_end = omp_get_wtime();
	return t_end - t_start;
}

// Сложение массивов (последовательно)
double SumArrays(double*& arr_in1, double*& arr_in2, double*& arr_res, int& size)
{
	double t_start = omp_get_wtime();

	for (int i = 0; i < size; i++)
		arr_res[i] = arr_in1[i] + arr_in2[i];

	double t_end = omp_get_wtime();
	return t_end - t_start;
}

// Сложение массивов (omp for)
double SumArraysParFor(double*& arr_in1, double*& arr_in2, double*& arr_res, int& size)
{
	double t_start = omp_get_wtime();

#pragma omp parallel for
	for (int i = 0; i < size; i++)
		arr_res[i] = arr_in1[i] + arr_in2[i];

	double t_end = omp_get_wtime();
	return t_end - t_start;
}

// Сложение массивов (Section)
double SumArraysParSec(double*& arr_in1, double*& arr_in2, double*& arr_res, int& size)
{
	double t_start = omp_get_wtime();
	
#pragma omp parallel sections
	{
#pragma omp section
		for (int i = 0; i < size / omp_get_num_threads(); i++)
			arr_res[i] = arr_in1[i] + arr_in2[i];

#pragma omp section
		if (omp_get_num_threads() > 1)
			for (int i = size / omp_get_num_threads(); i < size*2/ omp_get_num_threads(); i++)
				arr_res[i] = arr_in1[i] + arr_in2[i];

#pragma omp section
		if (omp_get_num_threads() > 2)
			for (int i = size*2/ omp_get_num_threads(); i < size*3/ omp_get_num_threads(); i++)
				arr_res[i] = arr_in1[i] + arr_in2[i];

#pragma omp section
		if (omp_get_num_threads() > 3)
			for (int i = size*3/ omp_get_num_threads(); i < size*4/ omp_get_num_threads(); i++)
				arr_res[i] = arr_in1[i] + arr_in2[i];
	}

	double t_end = omp_get_wtime();
	return t_end - t_start;
}

// Подсчёт суммы всех элементов массива (последовательно)
double ElArrSum(double*& arr, int& size, double& sum)
{
	double t_start = omp_get_wtime();
	double temp_summ = 0;

	for (int i = 0; i < size; i++)
		temp_summ = temp_summ + arr[i];

	double t_end = omp_get_wtime();
	sum = temp_summ;
	return t_end - t_start;
}

// Подсчёт суммы всех элементов массива (параллельно с исп. редуктора)
double ElArrSumParRed(double*& arr, int& size, double& sum)
{
	double t_start = omp_get_wtime();
	double temp_summ = 0;

#pragma omp parallel for reduction(+:temp_summ)
	for (int i = 0; i < size; i++)
		temp_summ = temp_summ + arr[i];

	double t_end = omp_get_wtime();
	sum = temp_summ;
	return t_end - t_start;
}

// Подсчёт суммы всех элементов массива (параллельно с исп. критической опции)
double ElArrSumParCritical(double*& arr, int& size, double& sum)
{
	double t_start = omp_get_wtime();
	double temp_summ = 0;

#pragma omp parallel for
		for (int i = 0; i < size; i++)
#pragma omp critical
			temp_summ = temp_summ + arr[i];

	double t_end = omp_get_wtime();
	sum = temp_summ;
	return t_end - t_start;
}

double TestIter(void* Funct, double*& arr_in1, double*& arr_in2, double*& arr_res, int size, int iterations)
{
	double curtime = 0, avgTime = 0, avgTimeT = 0, correctAVG = 0;;
	double* Times = new double[iterations];
	for (int i = 0; i < iterations; i++)
	{
		// Запуск функции и получение времени в миллисекундах
		curtime = ((*(TestFunctTempl)Funct)(arr_in1, arr_in2, arr_res, size)) * 1000;
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


void test_functions(void** Functions, std::string(&function_names)[10], int size, int iterations)
{
	double* a, * b, * c;
	a = new double[size];
	b = new double[size];
	c = new double[size];
	double Functions_time_ms[10];
	// проведение замеров времени работы по каждой функции
	for (int i = 0; i < 10; i++)
		Functions_time_ms[i] = TestIter(Functions[i], a, b, c, size, iterations);

	std::ofstream out;          // поток для записи
	out.open("result.txt", std::ios::app); // окрываем файл для записи
	if (out.is_open())
	{
		// Вывод результатов замера (можно организовать вывод в файл)
		for (int i = 0; i < 10; i++)
		{
			if (i == 4 || i == 5 || i == 6 || i == 7 || i == 8 || i == 9)
				out << function_names[i] << "\t\t" << Functions_time_ms[i] << " ms." << std::endl;
			else
				out << function_names[i] << "\t\t\t" << Functions_time_ms[i] << " ms." << std::endl;
		}
	}
	out << std::endl;
	out.close();
		

	delete[]a;
	delete[]b;
	delete[]c;
}

int main()
{
	void** Functions = new void* [10]{ TestFillarr1 ,TestFillarr2, TestFillarr1Par, TestFillarr2Par, TestSum, TestSumParFor, TestSumParSec,  TestElArrSum, TestElArrSumParRed, TestElArrSumParCritical };
	std::string  function_names[10]{ "Заполнение Пос 1","Заполнение Пос 2", "Заполнение Пар 1", "Заполнение Пар 2", "Сумма массивов A и B вар 1", "Сумма массивов A и B вар 2", "Сумма массивов A и B вар 3", "Сумма элементов массива С вар 1", "Сумма элементов массива С вар 2", "Сумма элементов массива С вар 3" };
	setlocale(LC_ALL, "Russian");

	lab1_1();
	std::cout << std::endl;

	std::ofstream out;
	out.open("result.txt", std::ios::app);
	if (out.is_open())
		out << "Замеры времени начаты: " << __TIMESTAMP__ << std::endl;
	out.close();

	for (int i = 1; i < 5; i++)
	{
		out.open("result.txt", std::ios::app);
		if (out.is_open())
			out << "Работа с " << i << " потоком" << std::endl << std::endl;
		out.close();

		std::cout << "Работа с " << i << " потоком" << std::endl;
		omp_set_num_threads(i);

		for (int j = 75000; j <= 150000; j += 25000)
		{
			out.open("result.txt", std::ios::app);
			if (out.is_open())
				out << "Количество элементов: " << j << std::endl << std::endl;
			out.close();

			test_functions(Functions, function_names, j, 2000);
		}
	}
	std::cout << "Замеры времени выполнения функций выполнены, все данные записаны в файл result.txt." << std::endl;
	system("result.txt");
}