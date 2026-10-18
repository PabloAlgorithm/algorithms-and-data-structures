#define CATCH_CONFIG_MAIN  // Catch2 сам создаёт функцию main() - только в одном .cpp файле
#include <catch2/catch.hpp>

#include <algorithm>
#include <string>
#include <vector>

// Подключение библиотек сортировок из проекта Sorting
#include "insertion_sort.h"
#include "selection_sort.h"
#include "exchange_sort.h"
#include "bubble_sort.h"
#include "optimized_bubble_sort.h"

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

// ============================================================================
// Сортировка вставками (Insertion Sort)
// ============================================================================

TEST_CASE("insertionSort: variant 13 array", "[insertion]") {
    // Положительный случай: массив из варианта 13
    // Arrange
    std::vector<int> v = { 11, 7, 14, 20, 5, 1, 15, 9 };
    std::vector<int> expected = { 1, 5, 7, 9, 11, 14, 15, 20 };

    // Act
    insertionSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("insertionSort: empty array", "[insertion]") {
    // Граничный случай: пустой массив
    // Arrange
    std::vector<int> v;

    // Act
    insertionSort(v.data(), 0);

    // Assert
    REQUIRE(v.empty());
}

TEST_CASE("insertionSort: null pointer", "[insertion]") {
    // Граничный случай: нулевой указатель не должен приводить к падению
    // Arrange
    int* array = nullptr;

    // Act, Assert
    REQUIRE_NOTHROW(insertionSort(array, 0));
}

TEST_CASE("insertionSort: single element", "[insertion]") {
    // Граничный случай: массив из одного элемента
    // Arrange
    std::vector<int> v = { 42 };
    std::vector<int> expected = { 42 };

    // Act
    insertionSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("insertionSort: two elements", "[insertion]") {
    // Граничный случай: минимальный массив, в котором нужен обмен
    // Arrange
    std::vector<int> v = { 2, 1 };
    std::vector<int> expected = { 1, 2 };

    // Act
    insertionSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("insertionSort: already sorted array", "[insertion]") {
    // Граничный случай: массив уже отсортирован
    // Arrange
    std::vector<int> v = { 1, 2, 3, 4, 5, 6, 7 };
    std::vector<int> expected = { 1, 2, 3, 4, 5, 6, 7 };

    // Act
    insertionSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("insertionSort: reverse ordered array", "[insertion]") {
    // Граничный случай: массив в обратном порядке
    // Arrange
    std::vector<int> v = { 7, 6, 5, 4, 3, 2, 1 };
    std::vector<int> expected = { 1, 2, 3, 4, 5, 6, 7 };

    // Act
    insertionSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("insertionSort: duplicates and negative numbers", "[insertion]") {
    // Положительный случай: повторяющиеся и отрицательные значения
    // Arrange
    std::vector<int> v = { 3, -1, 3, 0, -5, 2, -1, 0 };
    std::vector<int> expected = { -5, -1, -1, 0, 0, 2, 3, 3 };

    // Act
    insertionSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("insertionSort: all elements are equal", "[insertion]") {
    // Граничный случай: все элементы одинаковые
    // Arrange
    std::vector<int> v = { 4, 4, 4, 4, 4 };
    std::vector<int> expected = { 4, 4, 4, 4, 4 };

    // Act
    insertionSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("insertionSort: double values", "[insertion]") {
    // Проверка шаблона на типе double
    // Arrange
    std::vector<double> v = { 3.5, -1.25, 0.0, 2.75, -7.5, 2.5 };
    std::vector<double> expected = { -7.5, -1.25, 0.0, 2.5, 2.75, 3.5 };

    // Act
    insertionSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("insertionSort: string values", "[insertion]") {
    // Проверка шаблона на типе std::string
    // Arrange
    std::vector<std::string> v = { "pear", "apple", "orange", "banana" };
    std::vector<std::string> expected = { "apple", "banana", "orange", "pear" };

    // Act
    insertionSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("insertionSort: large array matches std::sort", "[insertion]") {
    // Положительный случай: большой массив сравнивается с эталоном std::sort
    // Arrange
    std::vector<int> v = makeRandomVector(1000);
    std::vector<int> expected = v;
    std::sort(expected.begin(), expected.end());

    // Act
    insertionSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

// ============================================================================
// Сортировка выбором (Selection Sort)
// ============================================================================

TEST_CASE("selectionSort: variant 13 array", "[selection]") {
    // Положительный случай: массив из варианта 13
    // Arrange
    std::vector<int> v = { 11, 7, 14, 20, 5, 1, 15, 9 };
    std::vector<int> expected = { 1, 5, 7, 9, 11, 14, 15, 20 };

    // Act
    selectionSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("selectionSort: empty array", "[selection]") {
    // Граничный случай: пустой массив
    // Arrange
    std::vector<int> v;

    // Act
    selectionSort(v.data(), 0);

    // Assert
    REQUIRE(v.empty());
}

TEST_CASE("selectionSort: null pointer", "[selection]") {
    // Граничный случай: нулевой указатель не должен приводить к падению
    // Arrange
    int* array = nullptr;

    // Act, Assert
    REQUIRE_NOTHROW(selectionSort(array, 0));
}

TEST_CASE("selectionSort: single element", "[selection]") {
    // Граничный случай: массив из одного элемента
    // Arrange
    std::vector<int> v = { 42 };
    std::vector<int> expected = { 42 };

    // Act
    selectionSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("selectionSort: two elements", "[selection]") {
    // Граничный случай: минимальный массив, в котором нужен обмен
    // Arrange
    std::vector<int> v = { 2, 1 };
    std::vector<int> expected = { 1, 2 };

    // Act
    selectionSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("selectionSort: already sorted array", "[selection]") {
    // Граничный случай: массив уже отсортирован
    // Arrange
    std::vector<int> v = { 1, 2, 3, 4, 5, 6, 7 };
    std::vector<int> expected = { 1, 2, 3, 4, 5, 6, 7 };

    // Act
    selectionSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("selectionSort: reverse ordered array", "[selection]") {
    // Граничный случай: массив в обратном порядке
    // Arrange
    std::vector<int> v = { 7, 6, 5, 4, 3, 2, 1 };
    std::vector<int> expected = { 1, 2, 3, 4, 5, 6, 7 };

    // Act
    selectionSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("selectionSort: duplicates and negative numbers", "[selection]") {
    // Положительный случай: повторяющиеся и отрицательные значения
    // Arrange
    std::vector<int> v = { 3, -1, 3, 0, -5, 2, -1, 0 };
    std::vector<int> expected = { -5, -1, -1, 0, 0, 2, 3, 3 };

    // Act
    selectionSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("selectionSort: all elements are equal", "[selection]") {
    // Граничный случай: все элементы одинаковые
    // Arrange
    std::vector<int> v = { 4, 4, 4, 4, 4 };
    std::vector<int> expected = { 4, 4, 4, 4, 4 };

    // Act
    selectionSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("selectionSort: double values", "[selection]") {
    // Проверка шаблона на типе double
    // Arrange
    std::vector<double> v = { 3.5, -1.25, 0.0, 2.75, -7.5, 2.5 };
    std::vector<double> expected = { -7.5, -1.25, 0.0, 2.5, 2.75, 3.5 };

    // Act
    selectionSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("selectionSort: string values", "[selection]") {
    // Проверка шаблона на типе std::string
    // Arrange
    std::vector<std::string> v = { "pear", "apple", "orange", "banana" };
    std::vector<std::string> expected = { "apple", "banana", "orange", "pear" };

    // Act
    selectionSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("selectionSort: large array matches std::sort", "[selection]") {
    // Положительный случай: большой массив сравнивается с эталоном std::sort
    // Arrange
    std::vector<int> v = makeRandomVector(1000);
    std::vector<int> expected = v;
    std::sort(expected.begin(), expected.end());

    // Act
    selectionSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

// ============================================================================
// Сортировка обменом (Exchange Sort)
// ============================================================================

TEST_CASE("exchangeSort: variant 13 array", "[exchange]") {
    // Положительный случай: массив из варианта 13
    // Arrange
    std::vector<int> v = { 11, 7, 14, 20, 5, 1, 15, 9 };
    std::vector<int> expected = { 1, 5, 7, 9, 11, 14, 15, 20 };

    // Act
    exchangeSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("exchangeSort: empty array", "[exchange]") {
    // Граничный случай: пустой массив
    // Arrange
    std::vector<int> v;

    // Act
    exchangeSort(v.data(), 0);

    // Assert
    REQUIRE(v.empty());
}

TEST_CASE("exchangeSort: null pointer", "[exchange]") {
    // Граничный случай: нулевой указатель не должен приводить к падению
    // Arrange
    int* array = nullptr;

    // Act, Assert
    REQUIRE_NOTHROW(exchangeSort(array, 0));
}

TEST_CASE("exchangeSort: single element", "[exchange]") {
    // Граничный случай: массив из одного элемента
    // Arrange
    std::vector<int> v = { 42 };
    std::vector<int> expected = { 42 };

    // Act
    exchangeSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("exchangeSort: two elements", "[exchange]") {
    // Граничный случай: минимальный массив, в котором нужен обмен
    // Arrange
    std::vector<int> v = { 2, 1 };
    std::vector<int> expected = { 1, 2 };

    // Act
    exchangeSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("exchangeSort: already sorted array", "[exchange]") {
    // Граничный случай: массив уже отсортирован
    // Arrange
    std::vector<int> v = { 1, 2, 3, 4, 5, 6, 7 };
    std::vector<int> expected = { 1, 2, 3, 4, 5, 6, 7 };

    // Act
    exchangeSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("exchangeSort: reverse ordered array", "[exchange]") {
    // Граничный случай: массив в обратном порядке
    // Arrange
    std::vector<int> v = { 7, 6, 5, 4, 3, 2, 1 };
    std::vector<int> expected = { 1, 2, 3, 4, 5, 6, 7 };

    // Act
    exchangeSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("exchangeSort: duplicates and negative numbers", "[exchange]") {
    // Положительный случай: повторяющиеся и отрицательные значения
    // Arrange
    std::vector<int> v = { 3, -1, 3, 0, -5, 2, -1, 0 };
    std::vector<int> expected = { -5, -1, -1, 0, 0, 2, 3, 3 };

    // Act
    exchangeSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("exchangeSort: all elements are equal", "[exchange]") {
    // Граничный случай: все элементы одинаковые
    // Arrange
    std::vector<int> v = { 4, 4, 4, 4, 4 };
    std::vector<int> expected = { 4, 4, 4, 4, 4 };

    // Act
    exchangeSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("exchangeSort: double values", "[exchange]") {
    // Проверка шаблона на типе double
    // Arrange
    std::vector<double> v = { 3.5, -1.25, 0.0, 2.75, -7.5, 2.5 };
    std::vector<double> expected = { -7.5, -1.25, 0.0, 2.5, 2.75, 3.5 };

    // Act
    exchangeSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("exchangeSort: string values", "[exchange]") {
    // Проверка шаблона на типе std::string
    // Arrange
    std::vector<std::string> v = { "pear", "apple", "orange", "banana" };
    std::vector<std::string> expected = { "apple", "banana", "orange", "pear" };

    // Act
    exchangeSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("exchangeSort: large array matches std::sort", "[exchange]") {
    // Положительный случай: большой массив сравнивается с эталоном std::sort
    // Arrange
    std::vector<int> v = makeRandomVector(1000);
    std::vector<int> expected = v;
    std::sort(expected.begin(), expected.end());

    // Act
    exchangeSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

// ============================================================================
// Сортировка пузырьком (Bubble Sort)
// ============================================================================

TEST_CASE("bubbleSort: variant 13 array", "[bubble]") {
    // Положительный случай: массив из варианта 13
    // Arrange
    std::vector<int> v = { 11, 7, 14, 20, 5, 1, 15, 9 };
    std::vector<int> expected = { 1, 5, 7, 9, 11, 14, 15, 20 };

    // Act
    bubbleSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("bubbleSort: empty array", "[bubble]") {
    // Граничный случай: пустой массив
    // Arrange
    std::vector<int> v;

    // Act
    bubbleSort(v.data(), 0);

    // Assert
    REQUIRE(v.empty());
}

TEST_CASE("bubbleSort: null pointer", "[bubble]") {
    // Граничный случай: нулевой указатель не должен приводить к падению
    // Arrange
    int* array = nullptr;

    // Act, Assert
    REQUIRE_NOTHROW(bubbleSort(array, 0));
}

TEST_CASE("bubbleSort: single element", "[bubble]") {
    // Граничный случай: массив из одного элемента
    // Arrange
    std::vector<int> v = { 42 };
    std::vector<int> expected = { 42 };

    // Act
    bubbleSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("bubbleSort: two elements", "[bubble]") {
    // Граничный случай: минимальный массив, в котором нужен обмен
    // Arrange
    std::vector<int> v = { 2, 1 };
    std::vector<int> expected = { 1, 2 };

    // Act
    bubbleSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("bubbleSort: already sorted array", "[bubble]") {
    // Граничный случай: массив уже отсортирован
    // Arrange
    std::vector<int> v = { 1, 2, 3, 4, 5, 6, 7 };
    std::vector<int> expected = { 1, 2, 3, 4, 5, 6, 7 };

    // Act
    bubbleSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("bubbleSort: reverse ordered array", "[bubble]") {
    // Граничный случай: массив в обратном порядке
    // Arrange
    std::vector<int> v = { 7, 6, 5, 4, 3, 2, 1 };
    std::vector<int> expected = { 1, 2, 3, 4, 5, 6, 7 };

    // Act
    bubbleSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("bubbleSort: duplicates and negative numbers", "[bubble]") {
    // Положительный случай: повторяющиеся и отрицательные значения
    // Arrange
    std::vector<int> v = { 3, -1, 3, 0, -5, 2, -1, 0 };
    std::vector<int> expected = { -5, -1, -1, 0, 0, 2, 3, 3 };

    // Act
    bubbleSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("bubbleSort: all elements are equal", "[bubble]") {
    // Граничный случай: все элементы одинаковые
    // Arrange
    std::vector<int> v = { 4, 4, 4, 4, 4 };
    std::vector<int> expected = { 4, 4, 4, 4, 4 };

    // Act
    bubbleSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("bubbleSort: double values", "[bubble]") {
    // Проверка шаблона на типе double
    // Arrange
    std::vector<double> v = { 3.5, -1.25, 0.0, 2.75, -7.5, 2.5 };
    std::vector<double> expected = { -7.5, -1.25, 0.0, 2.5, 2.75, 3.5 };

    // Act
    bubbleSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("bubbleSort: string values", "[bubble]") {
    // Проверка шаблона на типе std::string
    // Arrange
    std::vector<std::string> v = { "pear", "apple", "orange", "banana" };
    std::vector<std::string> expected = { "apple", "banana", "orange", "pear" };

    // Act
    bubbleSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("bubbleSort: large array matches std::sort", "[bubble]") {
    // Положительный случай: большой массив сравнивается с эталоном std::sort
    // Arrange
    std::vector<int> v = makeRandomVector(1000);
    std::vector<int> expected = v;
    std::sort(expected.begin(), expected.end());

    // Act
    bubbleSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

// ============================================================================
// Сортировка пузырьком с улучшениями (Optimized Bubble Sort)
// ============================================================================

TEST_CASE("optimizedBubbleSort: variant 13 array", "[optimized_bubble]") {
    // Положительный случай: массив из варианта 13
    // Arrange
    std::vector<int> v = { 11, 7, 14, 20, 5, 1, 15, 9 };
    std::vector<int> expected = { 1, 5, 7, 9, 11, 14, 15, 20 };

    // Act
    optimizedBubbleSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("optimizedBubbleSort: empty array", "[optimized_bubble]") {
    // Граничный случай: пустой массив
    // Arrange
    std::vector<int> v;

    // Act
    optimizedBubbleSort(v.data(), 0);

    // Assert
    REQUIRE(v.empty());
}

TEST_CASE("optimizedBubbleSort: null pointer", "[optimized_bubble]") {
    // Граничный случай: нулевой указатель не должен приводить к падению
    // Arrange
    int* array = nullptr;

    // Act, Assert
    REQUIRE_NOTHROW(optimizedBubbleSort(array, 0));
}

TEST_CASE("optimizedBubbleSort: single element", "[optimized_bubble]") {
    // Граничный случай: массив из одного элемента
    // Arrange
    std::vector<int> v = { 42 };
    std::vector<int> expected = { 42 };

    // Act
    optimizedBubbleSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("optimizedBubbleSort: two elements", "[optimized_bubble]") {
    // Граничный случай: минимальный массив, в котором нужен обмен
    // Arrange
    std::vector<int> v = { 2, 1 };
    std::vector<int> expected = { 1, 2 };

    // Act
    optimizedBubbleSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("optimizedBubbleSort: already sorted array", "[optimized_bubble]") {
    // Граничный случай: массив уже отсортирован
    // Arrange
    std::vector<int> v = { 1, 2, 3, 4, 5, 6, 7 };
    std::vector<int> expected = { 1, 2, 3, 4, 5, 6, 7 };

    // Act
    optimizedBubbleSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("optimizedBubbleSort: reverse ordered array", "[optimized_bubble]") {
    // Граничный случай: массив в обратном порядке
    // Arrange
    std::vector<int> v = { 7, 6, 5, 4, 3, 2, 1 };
    std::vector<int> expected = { 1, 2, 3, 4, 5, 6, 7 };

    // Act
    optimizedBubbleSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("optimizedBubbleSort: duplicates and negative numbers", "[optimized_bubble]") {
    // Положительный случай: повторяющиеся и отрицательные значения
    // Arrange
    std::vector<int> v = { 3, -1, 3, 0, -5, 2, -1, 0 };
    std::vector<int> expected = { -5, -1, -1, 0, 0, 2, 3, 3 };

    // Act
    optimizedBubbleSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("optimizedBubbleSort: all elements are equal", "[optimized_bubble]") {
    // Граничный случай: все элементы одинаковые
    // Arrange
    std::vector<int> v = { 4, 4, 4, 4, 4 };
    std::vector<int> expected = { 4, 4, 4, 4, 4 };

    // Act
    optimizedBubbleSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("optimizedBubbleSort: double values", "[optimized_bubble]") {
    // Проверка шаблона на типе double
    // Arrange
    std::vector<double> v = { 3.5, -1.25, 0.0, 2.75, -7.5, 2.5 };
    std::vector<double> expected = { -7.5, -1.25, 0.0, 2.5, 2.75, 3.5 };

    // Act
    optimizedBubbleSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("optimizedBubbleSort: string values", "[optimized_bubble]") {
    // Проверка шаблона на типе std::string
    // Arrange
    std::vector<std::string> v = { "pear", "apple", "orange", "banana" };
    std::vector<std::string> expected = { "apple", "banana", "orange", "pear" };

    // Act
    optimizedBubbleSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("optimizedBubbleSort: large array matches std::sort", "[optimized_bubble]") {
    // Положительный случай: большой массив сравнивается с эталоном std::sort
    // Arrange
    std::vector<int> v = makeRandomVector(1000);
    std::vector<int> expected = v;
    std::sort(expected.begin(), expected.end());

    // Act
    optimizedBubbleSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}
