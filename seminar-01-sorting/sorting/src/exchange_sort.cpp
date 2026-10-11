#include "exchange_sort.h"

#include <string>
#include <utility>

// Сортировка обменом (Exchange Sort)
//
// Идея: элемент на позиции i сравнивается со всеми элементами правее него.
// Если правый элемент меньше, они сразу меняются местами. После прохода
// на позиции i оказывается минимальный элемент оставшейся части.
// В отличие от пузырька сравниваются не соседние элементы, а пара (i, j).
template <typename T>
void exchangeSort(T* array, int size) {
    // Пустой массив и массив из одного элемента уже отсортированы
    if (array == nullptr || size < 2) {
        return;
    }

    for (int i = 0; i < size - 1; i++) {
        for (int j = i + 1; j < size; j++) {
            if (array[j] < array[i]) {
                std::swap(array[i], array[j]);
            }
        }
    }
}

// Явное инстанцирование шаблона: реализация находится в .cpp файле,
// поэтому здесь перечислены типы, для которых библиотека собирает функцию.
template void exchangeSort<int>(int*, int);
template void exchangeSort<long long>(long long*, int);
template void exchangeSort<float>(float*, int);
template void exchangeSort<double>(double*, int);
template void exchangeSort<char>(char*, int);
template void exchangeSort<std::string>(std::string*, int);
