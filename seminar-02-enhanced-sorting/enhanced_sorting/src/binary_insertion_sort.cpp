#include "binary_insertion_sort.h"

#include <string>

// Сортировка вставкой с бинарным поиском (Binary Insertion Sort)
//
// Идея: как и в обычной сортировке вставками, левая часть массива считается
// отсортированной, и очередной элемент вставляется в неё на своё место.
// Улучшение: место вставки ищется не последовательным просмотром, а бинарным
// поиском, так как левая часть уже упорядочена. Число сравнений на один
// элемент уменьшается с O(n) до O(log n). Количество сдвигов остаётся прежним.
template <typename T>
void binaryInsertionSort(T* array, int size) {
    // Пустой массив и массив из одного элемента уже отсортированы
    if (array == nullptr || size < 2) {
        return;
    }

    for (int i = 1; i < size; i++) {
        T key = array[i];      // элемент, который вставляем

        // Бинарный поиск места вставки в отсортированной части [0, i).
        // Ищем первую позицию, элемент на которой больше key. Равные элементы
        // остаются слева от key, поэтому сортировка устойчива.
        int left = 0;
        int right = i;
        while (left < right) {
            int middle = left + (right - left) / 2;
            if (key < array[middle]) {
                right = middle;        // место вставки левее или в middle
            } else {
                left = middle + 1;     // место вставки правее middle
            }
        }

        // Сдвигаем элементы [left, i) на одну позицию вправо
        for (int j = i; j > left; j--) {
            array[j] = array[j - 1];
        }

        array[left] = key;     // ставим элемент на найденное место
    }
}

// Здесь перечислены типы, для которых библиотека собирает функцию.
template void binaryInsertionSort<int>(int*, int);
template void binaryInsertionSort<long long>(long long*, int);
template void binaryInsertionSort<float>(float*, int);
template void binaryInsertionSort<double>(double*, int);
template void binaryInsertionSort<char>(char*, int);
template void binaryInsertionSort<std::string>(std::string*, int);
