#include "shaker_sort.h"

#include <string>
#include <utility>

// Сортировка шейкером (Shaker Sort, двунаправленная пузырьковая сортировка)
//
// Идея: улучшение пузырьковой сортировки. Проходы по массиву чередуются:
// слева направо (самый большой элемент уходит в конец) и справа налево
// (самый маленький элемент уходит в начало). Поэтому маленькие элементы
// из конца массива попадают на своё место за один проход, а не за много.
// Дополнительно запоминается позиция последнего обмена: за ней элементы уже
// упорядочены, и границы неотсортированной части сдвигаются сразу до неё.
// Если за проход не было обменов, границы сходятся и сортировка завершается.
template <typename T>
void shakerSort(T* array, int size) {
    // Пустой массив и массив из одного элемента уже отсортированы
    if (array == nullptr || size < 2) {
        return;
    }

    int left = 0;              // левая граница неотсортированной части
    int right = size - 1;      // правая граница неотсортированной части

    while (left < right) {
        // Проход слева направо: наибольший элемент "всплывает" вправо
        int lastSwap = left;
        for (int j = left; j < right; j++) {
            if (array[j + 1] < array[j]) {
                std::swap(array[j], array[j + 1]);
                lastSwap = j;
            }
        }
        right = lastSwap;      // правее последнего обмена всё упорядочено

        // Проход справа налево: наименьший элемент "тонет" влево
        lastSwap = right;
        for (int j = right; j > left; j--) {
            if (array[j] < array[j - 1]) {
                std::swap(array[j - 1], array[j]);
                lastSwap = j;
            }
        }
        left = lastSwap;       // левее последнего обмена всё упорядочено
    }
}

// Здесь перечислены типы, для которых библиотека собирает функцию.
template void shakerSort<int>(int*, int);
template void shakerSort<long long>(long long*, int);
template void shakerSort<float>(float*, int);
template void shakerSort<double>(double*, int);
template void shakerSort<char>(char*, int);
template void shakerSort<std::string>(std::string*, int);
