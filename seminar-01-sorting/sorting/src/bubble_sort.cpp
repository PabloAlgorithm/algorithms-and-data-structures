#include "bubble_sort.h"

#include <string>
#include <utility>

// Сортировка пузырьком (Bubble Sort)
//
// Идея: массив проходится несколько раз, сравниваются соседние элементы.
// Если левый больше правого, они меняются местами. За один проход самый
// большой элемент "всплывает" в конец массива.
// Классический вариант: всегда выполняется size - 1 проход.
template <typename T>
void bubbleSort(T* array, int size) {
    // Пустой массив и массив из одного элемента уже отсортированы
    if (array == nullptr || size < 2) {
        return;
    }

    for (int i = 0; i < size - 1; i++) {
        // Последние i элементов уже стоят на своих местах
        for (int j = 0; j < size - 1 - i; j++) {
            if (array[j + 1] < array[j]) {
                std::swap(array[j], array[j + 1]);
            }
        }
    }
}

// Явное инстанцирование шаблона: реализация находится в .cpp файле,
// поэтому здесь перечислены типы, для которых библиотека собирает функцию.
template void bubbleSort<int>(int*, int);
template void bubbleSort<long long>(long long*, int);
template void bubbleSort<float>(float*, int);
template void bubbleSort<double>(double*, int);
template void bubbleSort<char>(char*, int);
template void bubbleSort<std::string>(std::string*, int);
