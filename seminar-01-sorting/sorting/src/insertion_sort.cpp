#include "insertion_sort.h"

#include <string>
#include <utility>

// Сортировка вставками (Insertion Sort)
//
// Идея: левая часть массива считается отсортированной. Очередной элемент
// вынимается и вставляется на своё место в отсортированной части, при этом
// все элементы больше него сдвигаются на одну позицию вправо.
template <typename T>
void insertionSort(T* array, int size) {
    // Пустой массив и массив из одного элемента уже отсортированы
    if (array == nullptr || size < 2) {
        return;
    }

    for (int i = 1; i < size; i++) {
        T key = array[i];      // элемент, который вставляем
        int j = i - 1;

        // Сдвигаем вправо элементы отсортированной части, которые больше key
        while (j >= 0 && key < array[j]) {
            array[j + 1] = array[j];
            j--;
        }

        array[j + 1] = key;    // ставим элемент на освободившееся место
    }
}

// Явное инстанцирование шаблона: реализация находится в .cpp файле,
// поэтому здесь перечислены типы, для которых библиотека собирает функцию.
template void insertionSort<int>(int*, int);
template void insertionSort<long long>(long long*, int);
template void insertionSort<float>(float*, int);
template void insertionSort<double>(double*, int);
template void insertionSort<char>(char*, int);
template void insertionSort<std::string>(std::string*, int);
