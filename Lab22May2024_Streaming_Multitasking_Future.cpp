// Lab22May2024_Streaming_Multitasking_Future.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.

#include <iostream> // Включение заголовочного файла для стандартного ввода-вывода.
#include <future> // Включение заголовочного файла для работы с асинхронными операциями.
#include <random> // Включение заголовочного файла для генерации случайных чисел.
#include <mutex> // Включение заголовочного файла для мьютексов.
#include <condition_variable> // Включение заголовочного файла для работы с условными переменными.

// Глобальные переменные для синхронизации
std::mutex mtx; // Мьютекс для Thread 1
// Условная переменная для оповещения главного потока.
std::condition_variable cv;

void print_Multiplicate(int n, const long double* arr1, const long double* arr2, const std::string& threadName) {
    mtx.lock(); // Захватываем мьютекс
    for (int i = 0; i < n; ++i) {
        std::cout << threadName << " - Multiplicate: " << arr1[i] << " * " << arr2[i] << " = " << arr1[i] * arr2[i] << std::endl << std::endl;
    }
    mtx.unlock(); // Освобождаем мьютекс
    cv.notify_one(); // Оповещаем главный поток о завершении работы.
}

void print_Add(int n, const long double* arr3, const long double* arr4, const std::string& threadName) {
    mtx.lock(); // Захватываем мьютекс
    for (int i = 0; i < n; ++i) {
        std::cout << threadName << " - Add: " << arr3[i] << " + " << arr4[i] << " = " << arr3[i] + arr4[i] << std::endl << std::endl;
    }
    mtx.unlock(); // Освобождаем мьютекс
    cv.notify_one(); // Оповещаем главный поток о завершении работы.
}

void print_Random_num(int n, const std::string& threadName) {
    mtx.lock(); // Захватываем мьютекс
    for (int i = 0; i < n; ++i) {
        std::cout << threadName << " - Random num: " << (rand() % 25) / 3.1 << std::endl << std::endl;
    }
    mtx.unlock(); // Освобождаем мьютекс
    cv.notify_one(); // Оповещаем главный поток о завершении работы.
}

int main() {
    const int n1 = 20; // Константа для размера массивов 1,2 для операции умножение
    const int n2 = 21; // Константа для размера массивов 3,4 для операции сложение
    const int n3 = 22; // Константа для количества случайных чисел.

    long double arr1[n1], arr2[n1]; // Объявление массивов 1,2 для операции умножение (Thread_1)

    for (int i = 0; i < n1; ++i) {
        arr1[i] = (rand() % 25) / 3.2; // Заполнение массива 1.
        arr2[i] = (rand() % 25) / 3.2; // Заполнение массива 2.
    }

    std::cout << "Array 1:" << std::endl;
    for (int i = 0; i < n1; ++i) {
        std::cout << arr1[i] << " ";
    }
    std::cout << "\n\n";

    std::cout << "Array 2:" << std::endl;
    for (int i = 0; i < n1; ++i) {
        std::cout << arr2[i] << " ";
    }
    std::cout << "\n\n";



    long double arr3[n2], arr4[n2]; // Объявление массивов 3,4 для операции сложение (Thread_2)
    for (int i = 0; i < n2; ++i) {
        arr3[i] = (rand() % 25) / 3.2; // Заполнение массива 1.
        arr4[i] = (rand() % 25) / 3.2; // Заполнение массива 2.
    }

    std::cout << "Array 3:" << std::endl;
    for (int i = 0; i < n2; ++i) {
        std::cout << arr3[i] << " ";
    }
    std::cout << "\n\n";

    std::cout << "Array 4:" << std::endl;
    for (int i = 0; i < n2; ++i) {
        std::cout << arr4[i] << " ";
    }
    std::cout << "\n\n";

    // Запуск асинхронных потоков
    auto Thread_1 = std::async(std::launch::async, print_Multiplicate, n1, arr1, arr2, "Thread 1");
    auto Thread_2 = std::async(std::launch::async, print_Add, n2, arr3, arr4, "Thread 2");
    auto Thread_3 = std::async(std::launch::async, print_Random_num, n3, "Thread 3");

    {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [&]() { return true; });
    }

    // Ожидание завершения потоков и вывод сообщение об окончании
    Thread_1.get();
    Thread_2.get();
    Thread_3.get();

    std::cout << "\nMain thread finished execution." << std::endl;

    return 0;
}
