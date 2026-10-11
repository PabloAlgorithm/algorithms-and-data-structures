#include "two_phase_merge_sort.h"

#include <cstdio>
#include <fstream>
#include <stdexcept>
#include <string>

// Двухфазная сортировка простым слиянием
//
// Используются три файла: исходный A и два вспомогательных B и C.
// Серия - упорядоченная последовательность элементов. Сначала длина серии
// равна 1 (один элемент всегда упорядочен). Каждый проход состоит из двух фаз:
//   1) Фаза распределения: серии из файла A по очереди записываются
//      в файлы B и C (первая серия в B, вторая в C, третья в B и так далее).
//   2) Фаза слияния: очередная серия из B сливается с очередной серией из C
//      в одну упорядоченную серию удвоенной длины, которая записывается в A.
// После каждого прохода длина серии удваивается: 1, 2, 4, 8, ...
// Когда длина серии становится не меньше количества элементов, файл
// отсортирован. Число проходов - log2(n), сложность O(n log n).

// ----------------------------------------------------------------------------
// Вспомогательные функции (используются только внутри этого файла)
// ----------------------------------------------------------------------------

// Открывает файл для чтения. Если файл открыть не удалось - исключение.
static void openInput(std::ifstream& in, const std::string& fileName) {
    in.open(fileName);
    if (!in.is_open()) {
        throw std::runtime_error("Cannot open file for reading: " + fileName);
    }
}

// Открывает файл для записи (старое содержимое стирается).
static void openOutput(std::ofstream& out, const std::string& fileName) {
    out.open(fileName, std::ios::trunc);
    if (!out.is_open()) {
        throw std::runtime_error("Cannot open file for writing: " + fileName);
    }
    out.precision(17);     // чтобы вещественные числа записывались без потери точности
}

// Считывает очередное значение из файла.
// Возвращает true, если значение считано, и false, если файл закончился.
template <typename T>
static bool readValue(std::ifstream& in, T& value) {
    in >> value;
    return !in.fail();
}

// Записывает значение в файл.
template <typename T>
static void writeValue(std::ofstream& out, const T& value) {
    out << value << '\n';
}

// Подсчитывает количество значений в файле.
template <typename T>
static long long countElements(const std::string& fileName) {
    std::ifstream in;
    openInput(in, fileName);

    long long count = 0;
    T value;
    while (readValue(in, value)) {
        count++;
    }
    return count;
}

// Создаёт имя временного файла для сортировки массива.
static std::string makeTempFileName() {
    static int counter = 0;
    counter++;
    return "two_phase_merge_sort_" + std::to_string(counter) + ".tmp";
}

// ----------------------------------------------------------------------------
// Фаза 1. Распределение
// ----------------------------------------------------------------------------

// Распределяет серии длины seriesLength из файла A по файлам B и C:
// серии с чётными номерами (0, 2, 4, ...) попадают в B, с нечётными - в C.
template <typename T>
static void distribute(const std::string& fileA, const std::string& fileB,
                       const std::string& fileC, long long seriesLength) {
    std::ifstream a;
    std::ofstream b;
    std::ofstream c;
    openInput(a, fileA);
    openOutput(b, fileB);
    openOutput(c, fileC);

    T value;
    long long index = 0;       // номер текущего элемента в файле A
    while (readValue(a, value)) {
        long long seriesNumber = index / seriesLength;
        if (seriesNumber % 2 == 0) {
            writeValue(b, value);
        } else {
            writeValue(c, value);
        }
        index++;
    }
}

// ----------------------------------------------------------------------------
// Фаза 2. Слияние
// ----------------------------------------------------------------------------

// Сливает серии длины seriesLength из файлов B и C в файл A.
// Каждая пара серий (одна из B, одна из C) превращается в одну
// упорядоченную серию длины 2 * seriesLength.
template <typename T>
static void merge(const std::string& fileB, const std::string& fileC,
                  const std::string& fileA, long long seriesLength) {
    std::ifstream b;
    std::ifstream c;
    std::ofstream a;
    openInput(b, fileB);
    openInput(c, fileC);
    openOutput(a, fileA);

    T valueB;
    T valueC;
    bool hasB = readValue(b, valueB);      // есть ли текущий элемент в B
    bool hasC = readValue(c, valueC);      // есть ли текущий элемент в C

    while (hasB || hasC) {
        long long takenB = 0;      // сколько элементов текущей серии взято из B
        long long takenC = 0;      // сколько элементов текущей серии взято из C

        // Пока в обеих сериях есть элементы, записываем меньший из двух.
        // При равенстве берём элемент из B, поэтому сортировка устойчива
        while (hasB && takenB < seriesLength && hasC && takenC < seriesLength) {
            if (valueC < valueB) {
                writeValue(a, valueC);
                takenC++;
                hasC = readValue(c, valueC);
            } else {
                writeValue(a, valueB);
                takenB++;
                hasB = readValue(b, valueB);
            }
        }

        // Одна из серий закончилась - дописываем остаток другой
        while (hasB && takenB < seriesLength) {
            writeValue(a, valueB);
            takenB++;
            hasB = readValue(b, valueB);
        }
        while (hasC && takenC < seriesLength) {
            writeValue(a, valueC);
            takenC++;
            hasC = readValue(c, valueC);
        }
    }
}

// ----------------------------------------------------------------------------
// Сортировка файла
// ----------------------------------------------------------------------------

template <typename T>
void twoPhaseMergeSortFile(const std::string& fileName) {
    const std::string fileB = fileName + ".b.tmp";
    const std::string fileC = fileName + ".c.tmp";

    const long long count = countElements<T>(fileName);

    // Длина серии удваивается после каждого прохода
    for (long long seriesLength = 1; seriesLength < count; seriesLength *= 2) {
        distribute<T>(fileName, fileB, fileC, seriesLength);   // фаза 1
        merge<T>(fileB, fileC, fileName, seriesLength);        // фаза 2
    }

    // Удаляем вспомогательные файлы
    std::remove(fileB.c_str());
    std::remove(fileC.c_str());
}

// ----------------------------------------------------------------------------
// Сортировка массива через временный файл
// ----------------------------------------------------------------------------

template <typename T>
void twoPhaseMergeSort(T* array, int size) {
    // Пустой массив и массив из одного элемента уже отсортированы
    if (array == nullptr || size < 2) {
        return;
    }

    const std::string fileName = makeTempFileName();

    // Записываем массив во временный файл
    {
        std::ofstream out;
        openOutput(out, fileName);
        for (int i = 0; i < size; i++) {
            writeValue(out, array[i]);
        }
    }

    // Сортируем файл внешней сортировкой
    twoPhaseMergeSortFile<T>(fileName);

    // Считываем отсортированные данные обратно в массив
    {
        std::ifstream in;
        openInput(in, fileName);
        for (int i = 0; i < size; i++) {
            readValue(in, array[i]);
        }
    }

    std::remove(fileName.c_str());
}

// Явное инстанцирование шаблонов: реализация находится в .cpp файле,
// поэтому здесь перечислены типы, для которых библиотека собирает функции.
template void twoPhaseMergeSortFile<int>(const std::string&);
template void twoPhaseMergeSortFile<long long>(const std::string&);
template void twoPhaseMergeSortFile<float>(const std::string&);
template void twoPhaseMergeSortFile<double>(const std::string&);
template void twoPhaseMergeSortFile<std::string>(const std::string&);

template void twoPhaseMergeSort<int>(int*, int);
template void twoPhaseMergeSort<long long>(long long*, int);
template void twoPhaseMergeSort<float>(float*, int);
template void twoPhaseMergeSort<double>(double*, int);
template void twoPhaseMergeSort<std::string>(std::string*, int);
