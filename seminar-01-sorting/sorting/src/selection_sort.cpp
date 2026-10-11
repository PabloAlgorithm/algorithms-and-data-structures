#include "selection_sort.h"

#include <string>
#include <utility>

// Сортировка выбором (Selection Sort)
//
// Идея: в неотсортированной части массива ищется минимальный элемент
// и меняется местами с первым элементом этой части. После каждого прохода
// отсортированная часть слева увеличивается на один элемент.
template <typename T>
void selectionSort(T* array, int size) {
    // Пустой массив и массив из одного элемента уже отсортированы
    if (array == nullptr || size < 2) {
        return;
    }

    for (int i = 0; i < size - 1; i++) {
        int minIndex = i;      // индекс минимального элемента в [i, size)

        for (int j = i + 1; j < size; j++) {
            if (array[j] < array[minIndex]) {
                minIndex = j;
            }
        }

        if (minIndex != i) {
            std::swap(array[i], array[minIndex]);
        }
    }
}

// Явное инстанцирование шаблона: реализация находится в .cpp файле,
// поэтому здесь перечислены типы, для которых библиотека собирает функцию.
template void selectionSort<int>(int*, int);
template void selectionSort<long long>(long long*, int);
template void selectionSort<float>(float*, int);
template void selectionSort<double>(double*, int);
template void selectionSort<char>(char*, int);
template void selectionSort<std::string>(std::string*, int);
