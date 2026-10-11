#include "natural_merge_sort.h"

#include <cstdio>
#include <fstream>
#include <stdexcept>
#include <string>

// Естественная сортировка слиянием
//
// В простом слиянии длина серий фиксирована (1, 2, 4, ...) и никак не
// учитывает, что в данных уже могут быть упорядоченные участки. Естественная
// сортировка использует серии, которые уже есть в файле. Естественная серия -
// максимальная неубывающая последовательность подряд идущих элементов;
// серия заканчивается, когда следующий элемент меньше предыдущего.
//
// Используются три файла: исходный A и два вспомогательных B и C.
// Каждый проход состоит из двух фаз:
//   1) Распределение: естественные серии из A по очереди записываются в B и C.
//   2) Слияние: очередная серия из B сливается с очередной серией из C,
//      результат записывается в A.
// Проходы повторяются, пока в файле A не останется одна серия.
// Чем больше данные упорядочены изначально, тем меньше проходов: уже
// отсортированный файл - это одна серия, и слияние не выполняется ни разу.

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

// Создаёт имя временного файла для сортировки массива.
static std::string makeTempFileName() {
    static int counter = 0;
    counter++;
    return "natural_merge_sort_" + std::to_string(counter) + ".tmp";
}

// ----------------------------------------------------------------------------
// Фаза 1. Распределение естественных серий
// ----------------------------------------------------------------------------

// Распределяет естественные серии из файла A по файлам B и C по очереди.
// Возвращает количество серий, найденных в файле A.
template <typename T>
static long long distribute(const std::string& fileA, const std::string& fileB,
                            const std::string& fileC) {
    std::ifstream a;
    std::ofstream b;
    std::ofstream c;
    openInput(a, fileA);
    openOutput(b, fileB);
    openOutput(c, fileC);

    long long seriesCount = 0;
    bool toB = true;           // в какой файл записывается текущая серия

    T previous;
    T current;
    if (readValue(a, current)) {
        seriesCount = 1;
        writeValue(b, current);
        previous = current;

        while (readValue(a, current)) {
            if (current < previous) {
                // Порядок нарушен: началась новая серия, меняем файл
                seriesCount++;
                toB = !toB;
            }

            if (toB) {
                writeValue(b, current);
            } else {
                writeValue(c, current);
            }
            previous = current;
        }
    }

    return seriesCount;
}

// ----------------------------------------------------------------------------
// Фаза 2. Слияние естественных серий
// ----------------------------------------------------------------------------

// Записывает текущий элемент current в выходной файл и считывает следующий
// элемент из входного файла. Возвращает true, если серия во входном файле
// на этом закончилась (файл закончился или следующий элемент меньше
// записанного).
template <typename T>
static bool copyElement(std::ifstream& in, std::ofstream& out, T& current, bool& has) {
    writeValue(out, current);

    T next;
    has = readValue(in, next);
    if (!has) {
        return true;               // файл закончился - серия закончилась
    }

    bool seriesEnded = next < current;
    current = next;
    return seriesEnded;
}

// Сливает естественные серии из файлов B и C в файл A: очередная серия из B
// сливается с очередной серией из C в одну упорядоченную серию.
template <typename T>
static void merge(const std::string& fileB, const std::string& fileC,
                  const std::string& fileA) {
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

    // Пока в обоих файлах есть серии, сливаем их попарно
    while (hasB && hasC) {
        bool endB = false;         // закончилась ли текущая серия в B
        bool endC = false;         // закончилась ли текущая серия в C

        // Пока обе серии не закончились, записываем меньший из двух элементов.
        // При равенстве берём элемент из B
        while (!endB && !endC) {
            if (valueC < valueB) {
                endC = copyElement(c, a, valueC, hasC);
            } else {
                endB = copyElement(b, a, valueB, hasB);
            }
        }

        // Одна из серий закончилась - дописываем остаток другой
        while (!endB) {
            endB = copyElement(b, a, valueB, hasB);
        }
        while (!endC) {
            endC = copyElement(c, a, valueC, hasC);
        }
    }

    // В одном из файлов серии закончились - дописываем остаток другого
    while (hasB) {
        copyElement(b, a, valueB, hasB);
    }
    while (hasC) {
        copyElement(c, a, valueC, hasC);
    }
}

// ----------------------------------------------------------------------------
// Сортировка файла
// ----------------------------------------------------------------------------

template <typename T>
void naturalMergeSortFile(const std::string& fileName) {
    const std::string fileB = fileName + ".b.tmp";
    const std::string fileC = fileName + ".c.tmp";

    while (true) {
        // Фаза 1: распределяем серии и узнаём, сколько их в файле
        long long seriesCount = distribute<T>(fileName, fileB, fileC);

        // Нет серий (пустой файл) или одна серия - файл отсортирован
        if (seriesCount <= 1) {
            break;
        }

        // Фаза 2: сливаем серии обратно в исходный файл
        merge<T>(fileB, fileC, fileName);
    }

    // Удаляем вспомогательные файлы
    std::remove(fileB.c_str());
    std::remove(fileC.c_str());
}

// ----------------------------------------------------------------------------
// Сортировка массива через временный файл
// ----------------------------------------------------------------------------

template <typename T>
void naturalMergeSort(T* array, int size) {
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
    naturalMergeSortFile<T>(fileName);

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
template void naturalMergeSortFile<int>(const std::string&);
template void naturalMergeSortFile<long long>(const std::string&);
template void naturalMergeSortFile<float>(const std::string&);
template void naturalMergeSortFile<double>(const std::string&);
template void naturalMergeSortFile<std::string>(const std::string&);

template void naturalMergeSort<int>(int*, int);
template void naturalMergeSort<long long>(long long*, int);
template void naturalMergeSort<float>(float*, int);
template void naturalMergeSort<double>(double*, int);
template void naturalMergeSort<std::string>(std::string*, int);
