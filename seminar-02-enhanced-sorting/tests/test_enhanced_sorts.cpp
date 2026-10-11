#define CATCH_CONFIG_MAIN  // Catch2 сам создаёт функцию main() - только в одном .cpp файле
#include <catch2/catch.hpp>

#include <algorithm>
#include <string>
#include <vector>

// Подключение библиотек сортировок из проекта EnhancedSorting
#include "binary_insertion_sort.h"
#include "shaker_sort.h"

// Создаёт массив из count псевдослучайных чисел в диапазоне [-500, 500].
// Используется линейный конгруэнтный генератор с фиксированным начальным
// значением, поэтому при каждом запуске тестов данные одни и те же.
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

// ============================================================================
// Сортировка вставкой с бинарным поиском (Binary Insertion Sort)
// ============================================================================

TEST_CASE("binaryInsertionSort: variant 13 array", "[binary_insertion]") {
    // Положительный случай: массив из варианта 13
    // Arrange
    std::vector<int> v = { 11, 7, 14, 20, 5, 1, 15, 9 };
    std::vector<int> expected = { 1, 5, 7, 9, 11, 14, 15, 20 };

    // Act
    binaryInsertionSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("binaryInsertionSort: empty array", "[binary_insertion]") {
    // Граничный случай: пустой массив
    // Arrange
    std::vector<int> v;

    // Act
    binaryInsertionSort(v.data(), 0);

    // Assert
    REQUIRE(v.empty());
}

TEST_CASE("binaryInsertionSort: null pointer", "[binary_insertion]") {
    // Граничный случай: нулевой указатель не должен приводить к падению
    // Arrange
    int* array = nullptr;

    // Act, Assert
    REQUIRE_NOTHROW(binaryInsertionSort(array, 0));
}

TEST_CASE("binaryInsertionSort: single element", "[binary_insertion]") {
    // Граничный случай: массив из одного элемента
    // Arrange
    std::vector<int> v = { 42 };
    std::vector<int> expected = { 42 };

    // Act
    binaryInsertionSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("binaryInsertionSort: two elements", "[binary_insertion]") {
    // Граничный случай: минимальный массив, в котором нужен обмен
    // Arrange
    std::vector<int> v = { 2, 1 };
    std::vector<int> expected = { 1, 2 };

    // Act
    binaryInsertionSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("binaryInsertionSort: already sorted array", "[binary_insertion]") {
    // Граничный случай: массив уже отсортирован
    // Arrange
    std::vector<int> v = { 1, 2, 3, 4, 5, 6, 7 };
    std::vector<int> expected = { 1, 2, 3, 4, 5, 6, 7 };

    // Act
    binaryInsertionSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("binaryInsertionSort: reverse ordered array", "[binary_insertion]") {
    // Граничный случай: массив в обратном порядке
    // Arrange
    std::vector<int> v = { 7, 6, 5, 4, 3, 2, 1 };
    std::vector<int> expected = { 1, 2, 3, 4, 5, 6, 7 };

    // Act
    binaryInsertionSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("binaryInsertionSort: duplicates and negative numbers", "[binary_insertion]") {
    // Положительный случай: повторяющиеся и отрицательные значения
    // Arrange
    std::vector<int> v = { 3, -1, 3, 0, -5, 2, -1, 0 };
    std::vector<int> expected = { -5, -1, -1, 0, 0, 2, 3, 3 };

    // Act
    binaryInsertionSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("binaryInsertionSort: all elements are equal", "[binary_insertion]") {
    // Граничный случай: все элементы одинаковые
    // Arrange
    std::vector<int> v = { 4, 4, 4, 4, 4 };
    std::vector<int> expected = { 4, 4, 4, 4, 4 };

    // Act
    binaryInsertionSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("binaryInsertionSort: double values", "[binary_insertion]") {
    // Проверка шаблона на типе double
    // Arrange
    std::vector<double> v = { 3.5, -1.25, 0.0, 2.75, -7.5, 2.5 };
    std::vector<double> expected = { -7.5, -1.25, 0.0, 2.5, 2.75, 3.5 };

    // Act
    binaryInsertionSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("binaryInsertionSort: string values", "[binary_insertion]") {
    // Проверка шаблона на типе std::string
    // Arrange
    std::vector<std::string> v = { "pear", "apple", "orange", "banana" };
    std::vector<std::string> expected = { "apple", "banana", "orange", "pear" };

    // Act
    binaryInsertionSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("binaryInsertionSort: large array matches std::sort", "[binary_insertion]") {
    // Положительный случай: большой массив сравнивается с эталоном std::sort
    // Arrange
    std::vector<int> v = makeRandomVector(1000);
    std::vector<int> expected = v;
    std::sort(expected.begin(), expected.end());

    // Act
    binaryInsertionSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("binaryInsertionSort: insertion at the edges and in the middle", "[binary_insertion]") {
    // Положительный случай: проверка границ бинарного поиска. После
    // отсортированного начала идут элементы, которые вставляются
    // в самое начало (5), в самый конец (45) и в середину (25)
    // Arrange
    std::vector<int> v = { 10, 20, 30, 40, 5, 45, 25 };
    std::vector<int> expected = { 5, 10, 20, 25, 30, 40, 45 };

    // Act
    binaryInsertionSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

// ============================================================================
// Сортировка шейкером (Shaker Sort, двунаправленная пузырьковая сортировка)
// ============================================================================

TEST_CASE("shakerSort: variant 13 array", "[shaker]") {
    // Положительный случай: массив из варианта 13
    // Arrange
    std::vector<int> v = { 11, 7, 14, 20, 5, 1, 15, 9 };
    std::vector<int> expected = { 1, 5, 7, 9, 11, 14, 15, 20 };

    // Act
    shakerSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("shakerSort: empty array", "[shaker]") {
    // Граничный случай: пустой массив
    // Arrange
    std::vector<int> v;

    // Act
    shakerSort(v.data(), 0);

    // Assert
    REQUIRE(v.empty());
}

TEST_CASE("shakerSort: null pointer", "[shaker]") {
    // Граничный случай: нулевой указатель не должен приводить к падению
    // Arrange
    int* array = nullptr;

    // Act, Assert
    REQUIRE_NOTHROW(shakerSort(array, 0));
}

TEST_CASE("shakerSort: single element", "[shaker]") {
    // Граничный случай: массив из одного элемента
    // Arrange
    std::vector<int> v = { 42 };
    std::vector<int> expected = { 42 };

    // Act
    shakerSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("shakerSort: two elements", "[shaker]") {
    // Граничный случай: минимальный массив, в котором нужен обмен
    // Arrange
    std::vector<int> v = { 2, 1 };
    std::vector<int> expected = { 1, 2 };

    // Act
    shakerSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("shakerSort: already sorted array", "[shaker]") {
    // Граничный случай: массив уже отсортирован
    // Arrange
    std::vector<int> v = { 1, 2, 3, 4, 5, 6, 7 };
    std::vector<int> expected = { 1, 2, 3, 4, 5, 6, 7 };

    // Act
    shakerSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("shakerSort: reverse ordered array", "[shaker]") {
    // Граничный случай: массив в обратном порядке
    // Arrange
    std::vector<int> v = { 7, 6, 5, 4, 3, 2, 1 };
    std::vector<int> expected = { 1, 2, 3, 4, 5, 6, 7 };

    // Act
    shakerSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("shakerSort: duplicates and negative numbers", "[shaker]") {
    // Положительный случай: повторяющиеся и отрицательные значения
    // Arrange
    std::vector<int> v = { 3, -1, 3, 0, -5, 2, -1, 0 };
    std::vector<int> expected = { -5, -1, -1, 0, 0, 2, 3, 3 };

    // Act
    shakerSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("shakerSort: all elements are equal", "[shaker]") {
    // Граничный случай: все элементы одинаковые
    // Arrange
    std::vector<int> v = { 4, 4, 4, 4, 4 };
    std::vector<int> expected = { 4, 4, 4, 4, 4 };

    // Act
    shakerSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("shakerSort: double values", "[shaker]") {
    // Проверка шаблона на типе double
    // Arrange
    std::vector<double> v = { 3.5, -1.25, 0.0, 2.75, -7.5, 2.5 };
    std::vector<double> expected = { -7.5, -1.25, 0.0, 2.5, 2.75, 3.5 };

    // Act
    shakerSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("shakerSort: string values", "[shaker]") {
    // Проверка шаблона на типе std::string
    // Arrange
    std::vector<std::string> v = { "pear", "apple", "orange", "banana" };
    std::vector<std::string> expected = { "apple", "banana", "orange", "pear" };

    // Act
    shakerSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("shakerSort: large array matches std::sort", "[shaker]") {
    // Положительный случай: большой массив сравнивается с эталоном std::sort
    // Arrange
    std::vector<int> v = makeRandomVector(1000);
    std::vector<int> expected = v;
    std::sort(expected.begin(), expected.end());

    // Act
    shakerSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("shakerSort: smallest element at the end", "[shaker]") {
    // Положительный случай: наименьший элемент стоит в конце массива -
    // именно этот случай шейкер обрабатывает быстрее обычного пузырька
    // Arrange
    std::vector<int> v = { 2, 3, 4, 5, 6, 7, 1 };
    std::vector<int> expected = { 1, 2, 3, 4, 5, 6, 7 };

    // Act
    shakerSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}
