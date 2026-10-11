#include "one_phase_merge_sort.h"

#include <cstdio>
#include <fstream>
#include <stdexcept>
#include <string>

// Однофазная сортировка простым слиянием
//
// В двухфазной сортировке каждый проход состоит из распределения и слияния,
// причём распределение само по себе ничего не упорядочивает - это лишняя
// перезапись всех данных. В однофазной сортировке эти две фазы объединены:
// результат слияния сразу распределяется по двум выходным файлам.
// Для этого нужны четыре вспомогательных файла: два входных и два выходных.
//
// Порядок работы:
//   1) Один раз исходный файл распределяется по файлам A и B сериями длины 1.
//   2) Проход: пары серий из входных файлов (A и B) сливаются, а полученные
//      серии удвоенной длины записываются по очереди в выходные файлы (C и D).
//   3) Входные и выходные файлы меняются ролями, длина серии удваивается.
//   4) Когда длина серии становится не меньше количества элементов, все данные
//      лежат одной серией в первом входном файле - он копируется в исходный.
// По сравнению с двухфазной сортировкой число перезаписей данных
// уменьшается примерно в два раза ценой двух дополнительных файлов.

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
    return "one_phase_merge_sort_" + std::to_string(counter) + ".tmp";
}

// ----------------------------------------------------------------------------
// Начальное распределение
// ----------------------------------------------------------------------------

// Распределяет элементы исходного файла по двум файлам сериями длины 1:
// элементы с чётными номерами попадают в первый файл, с нечётными - во второй.
template <typename T>
static void distributeInitial(const std::string& source,
                              const std::string& first, const std::string& second) {
    std::ifstream in;
    std::ofstream out1;
    std::ofstream out2;
    openInput(in, source);
    openOutput(out1, first);
    openOutput(out2, second);

    T value;
    bool toFirst = true;
    while (readValue(in, value)) {
        if (toFirst) {
            writeValue(out1, value);
        } else {
            writeValue(out2, value);
        }
        toFirst = !toFirst;
    }
}

// ----------------------------------------------------------------------------
// Слияние, объединённое с распределением (единственная фаза прохода)
// ----------------------------------------------------------------------------

// Сливает серии длины seriesLength из файлов in1 и in2. Каждая пара серий
// превращается в серию длины 2 * seriesLength; полученные серии записываются
// по очереди в файлы out1 и out2 (первая в out1, вторая в out2 и так далее).
template <typename T>
static void mergeAndDistribute(const std::string& in1Name, const std::string& in2Name,
                               const std::string& out1Name, const std::string& out2Name,
                               long long seriesLength) {
    std::ifstream in1;
    std::ifstream in2;
    std::ofstream out1;
    std::ofstream out2;
    openInput(in1, in1Name);
    openInput(in2, in2Name);
    openOutput(out1, out1Name);
    openOutput(out2, out2Name);

    T value1;
    T value2;
    bool has1 = readValue(in1, value1);    // есть ли текущий элемент в in1
    bool has2 = readValue(in2, value2);    // есть ли текущий элемент в in2
    bool toFirst = true;                   // в какой выходной файл пишем серию

    while (has1 || has2) {
        std::ofstream& out = toFirst ? out1 : out2;
        long long taken1 = 0;      // сколько элементов текущей серии взято из in1
        long long taken2 = 0;      // сколько элементов текущей серии взято из in2

        // Пока в обеих сериях есть элементы, записываем меньший из двух.
        // При равенстве берём элемент из in1, поэтому сортировка устойчива
        while (has1 && taken1 < seriesLength && has2 && taken2 < seriesLength) {
            if (value2 < value1) {
                writeValue(out, value2);
                taken2++;
                has2 = readValue(in2, value2);
            } else {
                writeValue(out, value1);
                taken1++;
                has1 = readValue(in1, value1);
            }
        }

        // Одна из серий закончилась - дописываем остаток другой
        while (has1 && taken1 < seriesLength) {
            writeValue(out, value1);
            taken1++;
            has1 = readValue(in1, value1);
        }
        while (has2 && taken2 < seriesLength) {
            writeValue(out, value2);
            taken2++;
            has2 = readValue(in2, value2);
        }

        toFirst = !toFirst;        // следующая серия пойдёт в другой файл
    }
}

// Копирует все значения из файла source в файл target.
template <typename T>
static void copyFile(const std::string& source, const std::string& target) {
    std::ifstream in;
    std::ofstream out;
    openInput(in, source);
    openOutput(out, target);

    T value;
    while (readValue(in, value)) {
        writeValue(out, value);
    }
}

// ----------------------------------------------------------------------------
// Сортировка файла
// ----------------------------------------------------------------------------

template <typename T>
void onePhaseMergeSortFile(const std::string& fileName) {
    // Четыре вспомогательных файла: в каждый момент два из них входные,
    // два - выходные
    const std::string files[4] = {
        fileName + ".a.tmp", fileName + ".b.tmp",
        fileName + ".c.tmp", fileName + ".d.tmp"
    };

    const long long count = countElements<T>(fileName);

    if (count > 1) {
        // Начальное распределение исходного файла сериями длины 1
        distributeInitial<T>(fileName, files[0], files[1]);

        int in = 0;        // индекс первого входного файла (второй: in + 1)
        int out = 2;       // индекс первого выходного файла (второй: out + 1)

        for (long long seriesLength = 1; seriesLength < count; seriesLength *= 2) {
            mergeAndDistribute<T>(files[in], files[in + 1],
                                  files[out], files[out + 1], seriesLength);

            // Выходные файлы становятся входными для следующего прохода
            int temp = in;
            in = out;
            out = temp;
        }

        // Все данные лежат одной серией в первом входном файле
        copyFile<T>(files[in], fileName);
    }

    // Удаляем вспомогательные файлы
    for (int i = 0; i < 4; i++) {
        std::remove(files[i].c_str());
    }
}

// ----------------------------------------------------------------------------
// Сортировка массива через временный файл
// ----------------------------------------------------------------------------

template <typename T>
void onePhaseMergeSort(T* array, int size) {
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
    onePhaseMergeSortFile<T>(fileName);

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
template void onePhaseMergeSortFile<int>(const std::string&);
template void onePhaseMergeSortFile<long long>(const std::string&);
template void onePhaseMergeSortFile<float>(const std::string&);
template void onePhaseMergeSortFile<double>(const std::string&);
template void onePhaseMergeSortFile<std::string>(const std::string&);

template void onePhaseMergeSort<int>(int*, int);
template void onePhaseMergeSort<long long>(long long*, int);
template void onePhaseMergeSort<float>(float*, int);
template void onePhaseMergeSort<double>(double*, int);
template void onePhaseMergeSort<std::string>(std::string*, int);
