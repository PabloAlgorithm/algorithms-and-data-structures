#define CATCH_CONFIG_MAIN  // Catch2 сам создаёт функцию main() - только в одном .cpp файле
#include <catch2/catch.hpp>

#include <algorithm>
#include <cstdio>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>

// Подключение библиотек сортировок из проекта ExternalSorting
#include "two_phase_merge_sort.h"
#include "one_phase_merge_sort.h"
#include "natural_merge_sort.h"

// Имя файла с данными, который создаётся в тестах и удаляется после проверки
static const std::string TEST_FILE = "test_external_sort.tmp";

// Создаёт массив из count псевдослучайных чисел в диапазоне [-500, 500].
static std::vector<int> makeRandomVector(int count) {
    std::vector<int> result;
    result.reserve(count);

    unsigned int state = 12345u;
    for (int i = 0; i < count; i++) {
        state = state * 1103515245u + 12345u;
        result.push_back(static_cast<int>((state / 65536u) % 1001u) - 500);
    }

    return result;
}

// Записывает значения в текстовый файл (по одному в строке).
template <typename T>
static void writeFile(const std::string& fileName, const std::vector<T>& values) {
    std::ofstream out(fileName, std::ios::trunc);
    out.precision(17);
    for (const T& value : values) {
        out << value << '\n';
    }
}

// Считывает все значения из текстового файла.
template <typename T>
static std::vector<T> readFile(const std::string& fileName) {
    std::vector<T> values;
    std::ifstream in(fileName);
    T value;
    while (in >> value) {
        values.push_back(value);
    }
    return values;
}

// Проверяет, существует ли файл.
static bool fileExists(const std::string& fileName) {
    std::ifstream in(fileName);
    return in.is_open();
}

// ============================================================================
// Двухфазная сортировка простым слиянием (Two-Phase Merge Sort)
// ============================================================================

TEST_CASE("twoPhaseMergeSort: variant 13 array", "[two_phase]") {
    // Положительный случай: массив из варианта 13
    // Arrange
    std::vector<int> v = { 11, 7, 14, 20, 5, 1, 15, 9 };
    std::vector<int> expected = { 1, 5, 7, 9, 11, 14, 15, 20 };

    // Act
    twoPhaseMergeSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("twoPhaseMergeSort: empty array", "[two_phase]") {
    // Граничный случай: пустой массив
    // Arrange
    std::vector<int> v;

    // Act
    twoPhaseMergeSort(v.data(), 0);

    // Assert
    REQUIRE(v.empty());
}

TEST_CASE("twoPhaseMergeSort: null pointer", "[two_phase]") {
    // Граничный случай: нулевой указатель не должен приводить к падению
    // Arrange
    int* array = nullptr;

    // Act, Assert
    REQUIRE_NOTHROW(twoPhaseMergeSort(array, 0));
}

TEST_CASE("twoPhaseMergeSort: single element", "[two_phase]") {
    // Граничный случай: массив из одного элемента
    // Arrange
    std::vector<int> v = { 42 };
    std::vector<int> expected = { 42 };

    // Act
    twoPhaseMergeSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("twoPhaseMergeSort: two elements", "[two_phase]") {
    // Граничный случай: минимальный массив, в котором нужен обмен
    // Arrange
    std::vector<int> v = { 2, 1 };
    std::vector<int> expected = { 1, 2 };

    // Act
    twoPhaseMergeSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("twoPhaseMergeSort: already sorted array", "[two_phase]") {
    // Граничный случай: массив уже отсортирован
    // Arrange
    std::vector<int> v = { 1, 2, 3, 4, 5, 6, 7 };
    std::vector<int> expected = { 1, 2, 3, 4, 5, 6, 7 };

    // Act
    twoPhaseMergeSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("twoPhaseMergeSort: reverse ordered array", "[two_phase]") {
    // Граничный случай: массив в обратном порядке
    // Arrange
    std::vector<int> v = { 7, 6, 5, 4, 3, 2, 1 };
    std::vector<int> expected = { 1, 2, 3, 4, 5, 6, 7 };

    // Act
    twoPhaseMergeSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("twoPhaseMergeSort: duplicates and negative numbers", "[two_phase]") {
    // Положительный случай: повторяющиеся и отрицательные значения
    // Arrange
    std::vector<int> v = { 3, -1, 3, 0, -5, 2, -1, 0 };
    std::vector<int> expected = { -5, -1, -1, 0, 0, 2, 3, 3 };

    // Act
    twoPhaseMergeSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("twoPhaseMergeSort: all elements are equal", "[two_phase]") {
    // Граничный случай: все элементы одинаковые
    // Arrange
    std::vector<int> v = { 4, 4, 4, 4, 4 };
    std::vector<int> expected = { 4, 4, 4, 4, 4 };

    // Act
    twoPhaseMergeSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("twoPhaseMergeSort: double values", "[two_phase]") {
    // Проверка шаблона на типе double
    // Arrange
    std::vector<double> v = { 3.5, -1.25, 0.0, 2.75, -7.5, 2.5 };
    std::vector<double> expected = { -7.5, -1.25, 0.0, 2.5, 2.75, 3.5 };

    // Act
    twoPhaseMergeSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("twoPhaseMergeSort: string values", "[two_phase]") {
    // Проверка шаблона на типе std::string
    // Arrange
    std::vector<std::string> v = { "pear", "apple", "orange", "banana" };
    std::vector<std::string> expected = { "apple", "banana", "orange", "pear" };

    // Act
    twoPhaseMergeSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("twoPhaseMergeSort: large array matches std::sort", "[two_phase]") {
    // Положительный случай: большой массив сравнивается с эталоном std::sort
    // Arrange
    std::vector<int> v = makeRandomVector(1000);
    std::vector<int> expected = v;
    std::sort(expected.begin(), expected.end());

    // Act
    twoPhaseMergeSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("twoPhaseMergeSort: size is not a power of two", "[two_phase]") {
    // Граничный случай: количество элементов не степень двойки - последние серии неполные
    // Arrange
    std::vector<int> v = { 9, 4, 7, 1, 8, 2, 6 };
    std::vector<int> expected = { 1, 2, 4, 6, 7, 8, 9 };

    // Act
    twoPhaseMergeSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("twoPhaseMergeSort: array with sorted runs", "[two_phase]") {
    // Положительный случай: массив состоит из уже упорядоченных участков (серий)
    // Arrange
    std::vector<int> v = { 1, 5, 9, 2, 6, 3, 4, 8, 0, 7 };
    std::vector<int> expected = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    // Act
    twoPhaseMergeSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

// ---- Сортировка данных в файле и работа с временными файлами ----

TEST_CASE("twoPhaseMergeSortFile: sorts numbers stored in a file", "[two_phase][file]") {
    // Положительный случай: файл с массивом из варианта 13
    // Arrange
    writeFile<int>(TEST_FILE, { 11, 7, 14, 20, 5, 1, 15, 9 });
    std::vector<int> expected = { 1, 5, 7, 9, 11, 14, 15, 20 };

    // Act
    twoPhaseMergeSortFile<int>(TEST_FILE);

    // Assert
    std::vector<int> actual = readFile<int>(TEST_FILE);
    std::remove(TEST_FILE.c_str());
    REQUIRE(actual == expected);
}

TEST_CASE("twoPhaseMergeSortFile: empty file", "[two_phase][file]") {
    // Граничный случай: пустой файл остаётся пустым
    // Arrange
    writeFile<int>(TEST_FILE, {});

    // Act
    twoPhaseMergeSortFile<int>(TEST_FILE);

    // Assert
    std::vector<int> actual = readFile<int>(TEST_FILE);
    std::remove(TEST_FILE.c_str());
    REQUIRE(actual.empty());
}

TEST_CASE("twoPhaseMergeSortFile: file with a single value", "[two_phase][file]") {
    // Граничный случай: в файле одно значение
    // Arrange
    writeFile<int>(TEST_FILE, { 42 });
    std::vector<int> expected = { 42 };

    // Act
    twoPhaseMergeSortFile<int>(TEST_FILE);

    // Assert
    std::vector<int> actual = readFile<int>(TEST_FILE);
    std::remove(TEST_FILE.c_str());
    REQUIRE(actual == expected);
}

TEST_CASE("twoPhaseMergeSortFile: reverse ordered file", "[two_phase][file]") {
    // Граничный случай: значения в файле идут в обратном порядке
    // Arrange
    writeFile<int>(TEST_FILE, { 9, 8, 7, 6, 5, 4, 3, 2, 1 });
    std::vector<int> expected = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    // Act
    twoPhaseMergeSortFile<int>(TEST_FILE);

    // Assert
    std::vector<int> actual = readFile<int>(TEST_FILE);
    std::remove(TEST_FILE.c_str());
    REQUIRE(actual == expected);
}

TEST_CASE("twoPhaseMergeSortFile: double values keep their precision", "[two_phase][file]") {
    // Проверка шаблона на типе double: значения не теряют точность при записи в файл
    // Arrange
    writeFile<double>(TEST_FILE, { 3.141592653589793, -0.1, 2.5e-9, 1e10, 0.30000000000000004 });
    std::vector<double> expected = { -0.1, 2.5e-9, 0.30000000000000004, 3.141592653589793, 1e10 };

    // Act
    twoPhaseMergeSortFile<double>(TEST_FILE);

    // Assert
    std::vector<double> actual = readFile<double>(TEST_FILE);
    std::remove(TEST_FILE.c_str());
    REQUIRE(actual == expected);
}

TEST_CASE("twoPhaseMergeSortFile: large file matches std::sort", "[two_phase][file]") {
    // Положительный случай: большой файл (много проходов слияния)
    // сравнивается с эталоном std::sort
    // Arrange
    std::vector<int> values = makeRandomVector(1000);
    writeFile<int>(TEST_FILE, values);
    std::vector<int> expected = values;
    std::sort(expected.begin(), expected.end());

    // Act
    twoPhaseMergeSortFile<int>(TEST_FILE);

    // Assert
    std::vector<int> actual = readFile<int>(TEST_FILE);
    std::remove(TEST_FILE.c_str());
    REQUIRE(actual == expected);
}

TEST_CASE("twoPhaseMergeSortFile: temporary files are removed", "[two_phase][file]") {
    // Вспомогательные файлы после сортировки должны быть удалены
    // Arrange
    writeFile<int>(TEST_FILE, { 5, 3, 8, 1, 9, 2 });

    // Act
    twoPhaseMergeSortFile<int>(TEST_FILE);

    // Assert
    bool anyTempFileLeft = false;
    anyTempFileLeft = anyTempFileLeft || fileExists(TEST_FILE + ".b.tmp");
    anyTempFileLeft = anyTempFileLeft || fileExists(TEST_FILE + ".c.tmp");
    std::remove(TEST_FILE.c_str());
    REQUIRE_FALSE(anyTempFileLeft);
}

TEST_CASE("twoPhaseMergeSortFile: missing file throws an exception", "[two_phase][file]") {
    // Граничный случай: файла не существует - ожидается исключение
    // Arrange
    const std::string missingFile = "file_that_does_not_exist.tmp";

    // Act, Assert
    REQUIRE_THROWS_AS(twoPhaseMergeSortFile<int>(missingFile), std::runtime_error);
}

// ============================================================================
// Однофазная сортировка простым слиянием (One-Phase Merge Sort)
// ============================================================================

TEST_CASE("onePhaseMergeSort: variant 13 array", "[one_phase]") {
    // Положительный случай: массив из варианта 13
    // Arrange
    std::vector<int> v = { 11, 7, 14, 20, 5, 1, 15, 9 };
    std::vector<int> expected = { 1, 5, 7, 9, 11, 14, 15, 20 };

    // Act
    onePhaseMergeSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("onePhaseMergeSort: empty array", "[one_phase]") {
    // Граничный случай: пустой массив
    // Arrange
    std::vector<int> v;

    // Act
    onePhaseMergeSort(v.data(), 0);

    // Assert
    REQUIRE(v.empty());
}

TEST_CASE("onePhaseMergeSort: null pointer", "[one_phase]") {
    // Граничный случай: нулевой указатель не должен приводить к падению
    // Arrange
    int* array = nullptr;

    // Act, Assert
    REQUIRE_NOTHROW(onePhaseMergeSort(array, 0));
}

TEST_CASE("onePhaseMergeSort: single element", "[one_phase]") {
    // Граничный случай: массив из одного элемента
    // Arrange
    std::vector<int> v = { 42 };
    std::vector<int> expected = { 42 };

    // Act
    onePhaseMergeSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("onePhaseMergeSort: two elements", "[one_phase]") {
    // Граничный случай: минимальный массив, в котором нужен обмен
    // Arrange
    std::vector<int> v = { 2, 1 };
    std::vector<int> expected = { 1, 2 };

    // Act
    onePhaseMergeSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("onePhaseMergeSort: already sorted array", "[one_phase]") {
    // Граничный случай: массив уже отсортирован
    // Arrange
    std::vector<int> v = { 1, 2, 3, 4, 5, 6, 7 };
    std::vector<int> expected = { 1, 2, 3, 4, 5, 6, 7 };

    // Act
    onePhaseMergeSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("onePhaseMergeSort: reverse ordered array", "[one_phase]") {
    // Граничный случай: массив в обратном порядке
    // Arrange
    std::vector<int> v = { 7, 6, 5, 4, 3, 2, 1 };
    std::vector<int> expected = { 1, 2, 3, 4, 5, 6, 7 };

    // Act
    onePhaseMergeSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("onePhaseMergeSort: duplicates and negative numbers", "[one_phase]") {
    // Положительный случай: повторяющиеся и отрицательные значения
    // Arrange
    std::vector<int> v = { 3, -1, 3, 0, -5, 2, -1, 0 };
    std::vector<int> expected = { -5, -1, -1, 0, 0, 2, 3, 3 };

    // Act
    onePhaseMergeSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("onePhaseMergeSort: all elements are equal", "[one_phase]") {
    // Граничный случай: все элементы одинаковые
    // Arrange
    std::vector<int> v = { 4, 4, 4, 4, 4 };
    std::vector<int> expected = { 4, 4, 4, 4, 4 };

    // Act
    onePhaseMergeSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("onePhaseMergeSort: double values", "[one_phase]") {
    // Проверка шаблона на типе double
    // Arrange
    std::vector<double> v = { 3.5, -1.25, 0.0, 2.75, -7.5, 2.5 };
    std::vector<double> expected = { -7.5, -1.25, 0.0, 2.5, 2.75, 3.5 };

    // Act
    onePhaseMergeSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("onePhaseMergeSort: string values", "[one_phase]") {
    // Проверка шаблона на типе std::string
    // Arrange
    std::vector<std::string> v = { "pear", "apple", "orange", "banana" };
    std::vector<std::string> expected = { "apple", "banana", "orange", "pear" };

    // Act
    onePhaseMergeSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("onePhaseMergeSort: large array matches std::sort", "[one_phase]") {
    // Положительный случай: большой массив сравнивается с эталоном std::sort
    // Arrange
    std::vector<int> v = makeRandomVector(1000);
    std::vector<int> expected = v;
    std::sort(expected.begin(), expected.end());

    // Act
    onePhaseMergeSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("onePhaseMergeSort: size is not a power of two", "[one_phase]") {
    // Граничный случай: количество элементов не степень двойки - последние серии неполные
    // Arrange
    std::vector<int> v = { 9, 4, 7, 1, 8, 2, 6 };
    std::vector<int> expected = { 1, 2, 4, 6, 7, 8, 9 };

    // Act
    onePhaseMergeSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("onePhaseMergeSort: array with sorted runs", "[one_phase]") {
    // Положительный случай: массив состоит из уже упорядоченных участков (серий)
    // Arrange
    std::vector<int> v = { 1, 5, 9, 2, 6, 3, 4, 8, 0, 7 };
    std::vector<int> expected = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    // Act
    onePhaseMergeSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

// ---- Сортировка данных в файле и работа с временными файлами ----

TEST_CASE("onePhaseMergeSortFile: sorts numbers stored in a file", "[one_phase][file]") {
    // Положительный случай: файл с массивом из варианта 13
    // Arrange
    writeFile<int>(TEST_FILE, { 11, 7, 14, 20, 5, 1, 15, 9 });
    std::vector<int> expected = { 1, 5, 7, 9, 11, 14, 15, 20 };

    // Act
    onePhaseMergeSortFile<int>(TEST_FILE);

    // Assert
    std::vector<int> actual = readFile<int>(TEST_FILE);
    std::remove(TEST_FILE.c_str());
    REQUIRE(actual == expected);
}

TEST_CASE("onePhaseMergeSortFile: empty file", "[one_phase][file]") {
    // Граничный случай: пустой файл остаётся пустым
    // Arrange
    writeFile<int>(TEST_FILE, {});

    // Act
    onePhaseMergeSortFile<int>(TEST_FILE);

    // Assert
    std::vector<int> actual = readFile<int>(TEST_FILE);
    std::remove(TEST_FILE.c_str());
    REQUIRE(actual.empty());
}

TEST_CASE("onePhaseMergeSortFile: file with a single value", "[one_phase][file]") {
    // Граничный случай: в файле одно значение
    // Arrange
    writeFile<int>(TEST_FILE, { 42 });
    std::vector<int> expected = { 42 };

    // Act
    onePhaseMergeSortFile<int>(TEST_FILE);

    // Assert
    std::vector<int> actual = readFile<int>(TEST_FILE);
    std::remove(TEST_FILE.c_str());
    REQUIRE(actual == expected);
}

TEST_CASE("onePhaseMergeSortFile: reverse ordered file", "[one_phase][file]") {
    // Граничный случай: значения в файле идут в обратном порядке
    // Arrange
    writeFile<int>(TEST_FILE, { 9, 8, 7, 6, 5, 4, 3, 2, 1 });
    std::vector<int> expected = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    // Act
    onePhaseMergeSortFile<int>(TEST_FILE);

    // Assert
    std::vector<int> actual = readFile<int>(TEST_FILE);
    std::remove(TEST_FILE.c_str());
    REQUIRE(actual == expected);
}

TEST_CASE("onePhaseMergeSortFile: double values keep their precision", "[one_phase][file]") {
    // Проверка шаблона на типе double: значения не теряют точность при записи в файл
    // Arrange
    writeFile<double>(TEST_FILE, { 3.141592653589793, -0.1, 2.5e-9, 1e10, 0.30000000000000004 });
    std::vector<double> expected = { -0.1, 2.5e-9, 0.30000000000000004, 3.141592653589793, 1e10 };

    // Act
    onePhaseMergeSortFile<double>(TEST_FILE);

    // Assert
    std::vector<double> actual = readFile<double>(TEST_FILE);
    std::remove(TEST_FILE.c_str());
    REQUIRE(actual == expected);
}

TEST_CASE("onePhaseMergeSortFile: large file matches std::sort", "[one_phase][file]") {
    // Положительный случай: большой файл (много проходов слияния)
    // сравнивается с эталоном std::sort
    // Arrange
    std::vector<int> values = makeRandomVector(1000);
    writeFile<int>(TEST_FILE, values);
    std::vector<int> expected = values;
    std::sort(expected.begin(), expected.end());

    // Act
    onePhaseMergeSortFile<int>(TEST_FILE);

    // Assert
    std::vector<int> actual = readFile<int>(TEST_FILE);
    std::remove(TEST_FILE.c_str());
    REQUIRE(actual == expected);
}

TEST_CASE("onePhaseMergeSortFile: temporary files are removed", "[one_phase][file]") {
    // Вспомогательные файлы после сортировки должны быть удалены
    // Arrange
    writeFile<int>(TEST_FILE, { 5, 3, 8, 1, 9, 2 });

    // Act
    onePhaseMergeSortFile<int>(TEST_FILE);

    // Assert
    bool anyTempFileLeft = false;
    anyTempFileLeft = anyTempFileLeft || fileExists(TEST_FILE + ".a.tmp");
    anyTempFileLeft = anyTempFileLeft || fileExists(TEST_FILE + ".b.tmp");
    anyTempFileLeft = anyTempFileLeft || fileExists(TEST_FILE + ".c.tmp");
    anyTempFileLeft = anyTempFileLeft || fileExists(TEST_FILE + ".d.tmp");
    std::remove(TEST_FILE.c_str());
    REQUIRE_FALSE(anyTempFileLeft);
}

TEST_CASE("onePhaseMergeSortFile: missing file throws an exception", "[one_phase][file]") {
    // Граничный случай: файла не существует - ожидается исключение
    // Arrange
    const std::string missingFile = "file_that_does_not_exist.tmp";

    // Act, Assert
    REQUIRE_THROWS_AS(onePhaseMergeSortFile<int>(missingFile), std::runtime_error);
}

// ============================================================================
// Естественная сортировка слиянием (Natural Merge Sort)
// ============================================================================

TEST_CASE("naturalMergeSort: variant 13 array", "[natural]") {
    // Положительный случай: массив из варианта 13
    // Arrange
    std::vector<int> v = { 11, 7, 14, 20, 5, 1, 15, 9 };
    std::vector<int> expected = { 1, 5, 7, 9, 11, 14, 15, 20 };

    // Act
    naturalMergeSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("naturalMergeSort: empty array", "[natural]") {
    // Граничный случай: пустой массив
    // Arrange
    std::vector<int> v;

    // Act
    naturalMergeSort(v.data(), 0);

    // Assert
    REQUIRE(v.empty());
}

TEST_CASE("naturalMergeSort: null pointer", "[natural]") {
    // Граничный случай: нулевой указатель не должен приводить к падению
    // Arrange
    int* array = nullptr;

    // Act, Assert
    REQUIRE_NOTHROW(naturalMergeSort(array, 0));
}

TEST_CASE("naturalMergeSort: single element", "[natural]") {
    // Граничный случай: массив из одного элемента
    // Arrange
    std::vector<int> v = { 42 };
    std::vector<int> expected = { 42 };

    // Act
    naturalMergeSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("naturalMergeSort: two elements", "[natural]") {
    // Граничный случай: минимальный массив, в котором нужен обмен
    // Arrange
    std::vector<int> v = { 2, 1 };
    std::vector<int> expected = { 1, 2 };

    // Act
    naturalMergeSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("naturalMergeSort: already sorted array", "[natural]") {
    // Граничный случай: массив уже отсортирован
    // Arrange
    std::vector<int> v = { 1, 2, 3, 4, 5, 6, 7 };
    std::vector<int> expected = { 1, 2, 3, 4, 5, 6, 7 };

    // Act
    naturalMergeSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("naturalMergeSort: reverse ordered array", "[natural]") {
    // Граничный случай: массив в обратном порядке
    // Arrange
    std::vector<int> v = { 7, 6, 5, 4, 3, 2, 1 };
    std::vector<int> expected = { 1, 2, 3, 4, 5, 6, 7 };

    // Act
    naturalMergeSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("naturalMergeSort: duplicates and negative numbers", "[natural]") {
    // Положительный случай: повторяющиеся и отрицательные значения
    // Arrange
    std::vector<int> v = { 3, -1, 3, 0, -5, 2, -1, 0 };
    std::vector<int> expected = { -5, -1, -1, 0, 0, 2, 3, 3 };

    // Act
    naturalMergeSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("naturalMergeSort: all elements are equal", "[natural]") {
    // Граничный случай: все элементы одинаковые
    // Arrange
    std::vector<int> v = { 4, 4, 4, 4, 4 };
    std::vector<int> expected = { 4, 4, 4, 4, 4 };

    // Act
    naturalMergeSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("naturalMergeSort: double values", "[natural]") {
    // Проверка шаблона на типе double
    // Arrange
    std::vector<double> v = { 3.5, -1.25, 0.0, 2.75, -7.5, 2.5 };
    std::vector<double> expected = { -7.5, -1.25, 0.0, 2.5, 2.75, 3.5 };

    // Act
    naturalMergeSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("naturalMergeSort: string values", "[natural]") {
    // Проверка шаблона на типе std::string
    // Arrange
    std::vector<std::string> v = { "pear", "apple", "orange", "banana" };
    std::vector<std::string> expected = { "apple", "banana", "orange", "pear" };

    // Act
    naturalMergeSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("naturalMergeSort: large array matches std::sort", "[natural]") {
    // Положительный случай: большой массив сравнивается с эталоном std::sort
    // Arrange
    std::vector<int> v = makeRandomVector(1000);
    std::vector<int> expected = v;
    std::sort(expected.begin(), expected.end());

    // Act
    naturalMergeSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("naturalMergeSort: size is not a power of two", "[natural]") {
    // Граничный случай: количество элементов не степень двойки - последние серии неполные
    // Arrange
    std::vector<int> v = { 9, 4, 7, 1, 8, 2, 6 };
    std::vector<int> expected = { 1, 2, 4, 6, 7, 8, 9 };

    // Act
    naturalMergeSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("naturalMergeSort: array with sorted runs", "[natural]") {
    // Положительный случай: массив состоит из уже упорядоченных участков (серий)
    // Arrange
    std::vector<int> v = { 1, 5, 9, 2, 6, 3, 4, 8, 0, 7 };
    std::vector<int> expected = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    // Act
    naturalMergeSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

// ---- Сортировка данных в файле и работа с временными файлами ----

TEST_CASE("naturalMergeSortFile: sorts numbers stored in a file", "[natural][file]") {
    // Положительный случай: файл с массивом из варианта 13
    // Arrange
    writeFile<int>(TEST_FILE, { 11, 7, 14, 20, 5, 1, 15, 9 });
    std::vector<int> expected = { 1, 5, 7, 9, 11, 14, 15, 20 };

    // Act
    naturalMergeSortFile<int>(TEST_FILE);

    // Assert
    std::vector<int> actual = readFile<int>(TEST_FILE);
    std::remove(TEST_FILE.c_str());
    REQUIRE(actual == expected);
}

TEST_CASE("naturalMergeSortFile: empty file", "[natural][file]") {
    // Граничный случай: пустой файл остаётся пустым
    // Arrange
    writeFile<int>(TEST_FILE, {});

    // Act
    naturalMergeSortFile<int>(TEST_FILE);

    // Assert
    std::vector<int> actual = readFile<int>(TEST_FILE);
    std::remove(TEST_FILE.c_str());
    REQUIRE(actual.empty());
}

TEST_CASE("naturalMergeSortFile: file with a single value", "[natural][file]") {
    // Граничный случай: в файле одно значение
    // Arrange
    writeFile<int>(TEST_FILE, { 42 });
    std::vector<int> expected = { 42 };

    // Act
    naturalMergeSortFile<int>(TEST_FILE);

    // Assert
    std::vector<int> actual = readFile<int>(TEST_FILE);
    std::remove(TEST_FILE.c_str());
    REQUIRE(actual == expected);
}

TEST_CASE("naturalMergeSortFile: reverse ordered file", "[natural][file]") {
    // Граничный случай: значения в файле идут в обратном порядке
    // Arrange
    writeFile<int>(TEST_FILE, { 9, 8, 7, 6, 5, 4, 3, 2, 1 });
    std::vector<int> expected = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    // Act
    naturalMergeSortFile<int>(TEST_FILE);

    // Assert
    std::vector<int> actual = readFile<int>(TEST_FILE);
    std::remove(TEST_FILE.c_str());
    REQUIRE(actual == expected);
}

TEST_CASE("naturalMergeSortFile: double values keep their precision", "[natural][file]") {
    // Проверка шаблона на типе double: значения не теряют точность при записи в файл
    // Arrange
    writeFile<double>(TEST_FILE, { 3.141592653589793, -0.1, 2.5e-9, 1e10, 0.30000000000000004 });
    std::vector<double> expected = { -0.1, 2.5e-9, 0.30000000000000004, 3.141592653589793, 1e10 };

    // Act
    naturalMergeSortFile<double>(TEST_FILE);

    // Assert
    std::vector<double> actual = readFile<double>(TEST_FILE);
    std::remove(TEST_FILE.c_str());
    REQUIRE(actual == expected);
}

TEST_CASE("naturalMergeSortFile: large file matches std::sort", "[natural][file]") {
    // Положительный случай: большой файл (много проходов слияния)
    // сравнивается с эталоном std::sort
    // Arrange
    std::vector<int> values = makeRandomVector(1000);
    writeFile<int>(TEST_FILE, values);
    std::vector<int> expected = values;
    std::sort(expected.begin(), expected.end());

    // Act
    naturalMergeSortFile<int>(TEST_FILE);

    // Assert
    std::vector<int> actual = readFile<int>(TEST_FILE);
    std::remove(TEST_FILE.c_str());
    REQUIRE(actual == expected);
}

TEST_CASE("naturalMergeSortFile: temporary files are removed", "[natural][file]") {
    // Вспомогательные файлы после сортировки должны быть удалены
    // Arrange
    writeFile<int>(TEST_FILE, { 5, 3, 8, 1, 9, 2 });

    // Act
    naturalMergeSortFile<int>(TEST_FILE);

    // Assert
    bool anyTempFileLeft = false;
    anyTempFileLeft = anyTempFileLeft || fileExists(TEST_FILE + ".b.tmp");
    anyTempFileLeft = anyTempFileLeft || fileExists(TEST_FILE + ".c.tmp");
    std::remove(TEST_FILE.c_str());
    REQUIRE_FALSE(anyTempFileLeft);
}

TEST_CASE("naturalMergeSortFile: missing file throws an exception", "[natural][file]") {
    // Граничный случай: файла не существует - ожидается исключение
    // Arrange
    const std::string missingFile = "file_that_does_not_exist.tmp";

    // Act, Assert
    REQUIRE_THROWS_AS(naturalMergeSortFile<int>(missingFile), std::runtime_error);
}
