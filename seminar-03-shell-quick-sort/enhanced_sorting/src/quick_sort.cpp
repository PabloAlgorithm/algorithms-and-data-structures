#include "quick_sort.h"

#include <string>
#include <utility>

// Быстрая сортировка (Quick Sort)
//
// Идея ("разделяй и властвуй"): выбирается опорный элемент, после чего массив
// разделяется на две части: слева оказываются элементы не больше опорного,
// справа - не меньше опорного. Затем каждая часть сортируется тем же способом
// рекурсивно. Части длиной 0 или 1 уже отсортированы.
// Опорным берётся элемент из середины участка: при таком выборе уже
// отсортированный массив и массив в обратном порядке делятся ровно пополам
// и не приводят к худшему случаю.
// Сложность в среднем O(n log n), в худшем случае O(n^2).

// Вспомогательная функция: сортирует участок массива [left, right]
// (обе границы включительно).
template <typename T>
static void quickSortRange(T* array, int left, int right) {
    int i = left;
    int j = right;
    T pivot = array[left + (right - left) / 2];    // опорный элемент

    // Разделение: i идёт слева направо, j - справа налево
    while (i <= j) {
        while (array[i] < pivot) {     // пропускаем элементы меньше опорного
            i++;
        }
        while (pivot < array[j]) {     // пропускаем элементы больше опорного
            j--;
        }

        if (i <= j) {
            // array[i] >= pivot и array[j] <= pivot: меняем их местами
            std::swap(array[i], array[j]);
            i++;
            j--;
        }
    }

    // Теперь [left, j] - элементы не больше опорного,
    // [i, right] - элементы не меньше опорного. Сортируем обе части
    if (left < j) {
        quickSortRange(array, left, j);
    }
    if (i < right) {
        quickSortRange(array, i, right);
    }
}

template <typename T>
void quickSort(T* array, int size) {
    // Пустой массив и массив из одного элемента уже отсортированы
    if (array == nullptr || size < 2) {
        return;
    }

    quickSortRange(array, 0, size - 1);
}

// Явное инстанцирование шаблона: реализация находится в .cpp файле,
// поэтому здесь перечислены типы, для которых библиотека собирает функцию.
template void quickSort<int>(int*, int);
template void quickSort<long long>(long long*, int);
template void quickSort<float>(float*, int);
template void quickSort<double>(double*, int);
template void quickSort<char>(char*, int);
template void quickSort<std::string>(std::string*, int);
