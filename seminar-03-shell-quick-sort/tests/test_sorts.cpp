#define CATCH_CONFIG_MAIN  // Catch2 сам создаёт функцию main() - только в одном .cpp файле
#include <catch2/catch.hpp>

#include <algorithm>
#include <string>
#include <vector>

// Подключение библиотек сортировок из проекта EnhancedSorting
#include "shell_sort.h"
#include "quick_sort.h"

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
// Сортировка методом Шелла (Shell Sort)
// ============================================================================

TEST_CASE("shellSort: variant 13 array", "[shell]") {
    // Положительный случай: массив из варианта 13
    // Arrange
    std::vector<int> v = { 11, 7, 14, 20, 5, 1, 15, 9 };
    std::vector<int> expected = { 1, 5, 7, 9, 11, 14, 15, 20 };

    // Act
    shellSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("shellSort: empty array", "[shell]") {
    // Граничный случай: пустой массив
    // Arrange
    std::vector<int> v;

    // Act
    shellSort(v.data(), 0);

    // Assert
    REQUIRE(v.empty());
}

TEST_CASE("shellSort: null pointer", "[shell]") {
    // Граничный случай: нулевой указатель не должен приводить к падению
    // Arrange
    int* array = nullptr;

    // Act, Assert
    REQUIRE_NOTHROW(shellSort(array, 0));
}

TEST_CASE("shellSort: single element", "[shell]") {
    // Граничный случай: массив из одного элемента
    // Arrange
    std::vector<int> v = { 42 };
    std::vector<int> expected = { 42 };

    // Act
    shellSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("shellSort: two elements", "[shell]") {
    // Граничный случай: минимальный массив, в котором нужен обмен
    // Arrange
    std::vector<int> v = { 2, 1 };
    std::vector<int> expected = { 1, 2 };

    // Act
    shellSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("shellSort: already sorted array", "[shell]") {
    // Граничный случай: массив уже отсортирован
    // Arrange
    std::vector<int> v = { 1, 2, 3, 4, 5, 6, 7 };
    std::vector<int> expected = { 1, 2, 3, 4, 5, 6, 7 };

    // Act
    shellSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("shellSort: reverse ordered array", "[shell]") {
    // Граничный случай: массив в обратном порядке
    // Arrange
    std::vector<int> v = { 7, 6, 5, 4, 3, 2, 1 };
    std::vector<int> expected = { 1, 2, 3, 4, 5, 6, 7 };

    // Act
    shellSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("shellSort: duplicates and negative numbers", "[shell]") {
    // Положительный случай: повторяющиеся и отрицательные значения
    // Arrange
    std::vector<int> v = { 3, -1, 3, 0, -5, 2, -1, 0 };
    std::vector<int> expected = { -5, -1, -1, 0, 0, 2, 3, 3 };

    // Act
    shellSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("shellSort: all elements are equal", "[shell]") {
    // Граничный случай: все элементы одинаковые
    // Arrange
    std::vector<int> v = { 4, 4, 4, 4, 4 };
    std::vector<int> expected = { 4, 4, 4, 4, 4 };

    // Act
    shellSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("shellSort: double values", "[shell]") {
    // Проверка шаблона на типе double
    // Arrange
    std::vector<double> v = { 3.5, -1.25, 0.0, 2.75, -7.5, 2.5 };
    std::vector<double> expected = { -7.5, -1.25, 0.0, 2.5, 2.75, 3.5 };

    // Act
    shellSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("shellSort: string values", "[shell]") {
    // Проверка шаблона на типе std::string
    // Arrange
    std::vector<std::string> v = { "pear", "apple", "orange", "banana" };
    std::vector<std::string> expected = { "apple", "banana", "orange", "pear" };

    // Act
    shellSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("shellSort: large array matches std::sort", "[shell]") {
    // Положительный случай: большой массив сравнивается с эталоном std::sort
    // Arrange
    std::vector<int> v = makeRandomVector(1000);
    std::vector<int> expected = v;
    std::sort(expected.begin(), expected.end());

    // Act
    shellSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("shellSort: small elements at the end", "[shell]") {
    // Положительный случай: маленькие элементы стоят в конце массива -
    // за счёт больших шагов они быстро переносятся в начало
    // Arrange
    std::vector<int> v = { 9, 10, 11, 12, 13, 14, 15, 16, 1, 2, 3, 4, 5, 6, 7, 8 };
    std::vector<int> expected = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };

    // Act
    shellSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

// ============================================================================
// Быстрая сортировка (Quick Sort)
// ============================================================================

TEST_CASE("quickSort: variant 13 array", "[quick]") {
    // Положительный случай: массив из варианта 13
    // Arrange
    std::vector<int> v = { 11, 7, 14, 20, 5, 1, 15, 9 };
    std::vector<int> expected = { 1, 5, 7, 9, 11, 14, 15, 20 };

    // Act
    quickSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("quickSort: empty array", "[quick]") {
    // Граничный случай: пустой массив
    // Arrange
    std::vector<int> v;

    // Act
    quickSort(v.data(), 0);

    // Assert
    REQUIRE(v.empty());
}

TEST_CASE("quickSort: null pointer", "[quick]") {
    // Граничный случай: нулевой указатель не должен приводить к падению
    // Arrange
    int* array = nullptr;

    // Act, Assert
    REQUIRE_NOTHROW(quickSort(array, 0));
}

TEST_CASE("quickSort: single element", "[quick]") {
    // Граничный случай: массив из одного элемента
    // Arrange
    std::vector<int> v = { 42 };
    std::vector<int> expected = { 42 };

    // Act
    quickSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("quickSort: two elements", "[quick]") {
    // Граничный случай: минимальный массив, в котором нужен обмен
    // Arrange
    std::vector<int> v = { 2, 1 };
    std::vector<int> expected = { 1, 2 };

    // Act
    quickSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("quickSort: already sorted array", "[quick]") {
    // Граничный случай: массив уже отсортирован
    // Arrange
    std::vector<int> v = { 1, 2, 3, 4, 5, 6, 7 };
    std::vector<int> expected = { 1, 2, 3, 4, 5, 6, 7 };

    // Act
    quickSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("quickSort: reverse ordered array", "[quick]") {
    // Граничный случай: массив в обратном порядке
    // Arrange
    std::vector<int> v = { 7, 6, 5, 4, 3, 2, 1 };
    std::vector<int> expected = { 1, 2, 3, 4, 5, 6, 7 };

    // Act
    quickSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("quickSort: duplicates and negative numbers", "[quick]") {
    // Положительный случай: повторяющиеся и отрицательные значения
    // Arrange
    std::vector<int> v = { 3, -1, 3, 0, -5, 2, -1, 0 };
    std::vector<int> expected = { -5, -1, -1, 0, 0, 2, 3, 3 };

    // Act
    quickSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("quickSort: all elements are equal", "[quick]") {
    // Граничный случай: все элементы одинаковые
    // Arrange
    std::vector<int> v = { 4, 4, 4, 4, 4 };
    std::vector<int> expected = { 4, 4, 4, 4, 4 };

    // Act
    quickSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("quickSort: double values", "[quick]") {
    // Проверка шаблона на типе double
    // Arrange
    std::vector<double> v = { 3.5, -1.25, 0.0, 2.75, -7.5, 2.5 };
    std::vector<double> expected = { -7.5, -1.25, 0.0, 2.5, 2.75, 3.5 };

    // Act
    quickSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("quickSort: string values", "[quick]") {
    // Проверка шаблона на типе std::string
    // Arrange
    std::vector<std::string> v = { "pear", "apple", "orange", "banana" };
    std::vector<std::string> expected = { "apple", "banana", "orange", "pear" };

    // Act
    quickSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("quickSort: large array matches std::sort", "[quick]") {
    // Положительный случай: большой массив сравнивается с эталоном std::sort
    // Arrange
    std::vector<int> v = makeRandomVector(1000);
    std::vector<int> expected = v;
    std::sort(expected.begin(), expected.end());

    // Act
    quickSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}

TEST_CASE("quickSort: many elements equal to the pivot", "[quick]") {
    // Положительный случай: значение опорного элемента (5) повторяется
    // много раз - проверка корректности разделения массива
    // Arrange
    std::vector<int> v = { 5, 5, 3, 5, 1, 5, 5, 2, 5 };
    std::vector<int> expected = { 1, 2, 3, 5, 5, 5, 5, 5, 5 };

    // Act
    quickSort(v.data(), static_cast<int>(v.size()));

    // Assert
    REQUIRE(v == expected);
}
