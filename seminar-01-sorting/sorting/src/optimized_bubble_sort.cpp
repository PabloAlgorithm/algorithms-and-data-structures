#include "optimized_bubble_sort.h"

#include <string>
#include <utility>

// Сортировка пузырьком с улучшениями (Optimized Bubble Sort)
//
// Улучшения по сравнению с обычным пузырьком:
// 1) Флаг обмена: если за проход не было ни одного обмена, массив уже
//    отсортирован и работа завершается досрочно (на отсортированном
//    массиве выполняется всего один проход).
// 2) Запоминается позиция последнего обмена: все элементы правее неё уже
//    стоят на своих местах, поэтому следующий проход идёт только до неё.
template <typename T>
void optimizedBubbleSort(T* array, int size) {
    // Пустой массив и массив из одного элемента уже отсортированы
    if (array == nullptr || size < 2) {
        return;
    }

    int unsortedEnd = size - 1;    // правая граница неотсортированной части

    while (unsortedEnd > 0) {
        bool swapped = false;      // был ли обмен на этом проходе
        int lastSwap = 0;          // позиция последнего обмена

        for (int j = 0; j < unsortedEnd; j++) {
            if (array[j + 1] < array[j]) {
                std::swap(array[j], array[j + 1]);
                swapped = true;
                lastSwap = j;
            }
        }

        if (!swapped) {
            break;                 // обменов не было - массив отсортирован
        }

        unsortedEnd = lastSwap;    // правее lastSwap всё уже упорядочено
    }
}

// Явное инстанцирование шаблона: реализация находится в .cpp файле,
// поэтому здесь перечислены типы, для которых библиотека собирает функцию.
template void optimizedBubbleSort<int>(int*, int);
template void optimizedBubbleSort<long long>(long long*, int);
template void optimizedBubbleSort<float>(float*, int);
template void optimizedBubbleSort<double>(double*, int);
template void optimizedBubbleSort<char>(char*, int);
template void optimizedBubbleSort<std::string>(std::string*, int);
