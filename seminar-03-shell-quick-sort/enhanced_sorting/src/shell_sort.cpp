#include "shell_sort.h"

#include <string>

// Сортировка методом Шелла (Shell Sort)
//
// Идея: улучшение сортировки вставками. Обычная вставка сдвигает элемент
// только на одну позицию за шаг, поэтому маленький элемент из конца массива
// добирается до начала очень долго. В методе Шелла сначала сортируются
// вставками элементы, стоящие друг от друга на большом расстоянии gap (шаг).
// Затем шаг уменьшается, и на последнем проходе с шагом 1 выполняется
// обычная сортировка вставками, но массив к этому моменту уже почти
// упорядочен, поэтому сдвигов требуется мало.
// Последовательность шагов (предложена Шеллом): size/2, size/4, ..., 1.
template <typename T>
void shellSort(T* array, int size) {
    // Пустой массив и массив из одного элемента уже отсортированы
    if (array == nullptr || size < 2) {
        return;
    }

    for (int gap = size / 2; gap > 0; gap /= 2) {
        // Сортировка вставками для элементов, отстоящих друг от друга на gap
        for (int i = gap; i < size; i++) {
            T key = array[i];      // элемент, который вставляем
            int j = i;

            // Сдвигаем вправо на gap элементы своей подпоследовательности,
            // которые больше key
            while (j >= gap && key < array[j - gap]) {
                array[j] = array[j - gap];
                j -= gap;
            }

            array[j] = key;        // ставим элемент на освободившееся место
        }
    }
}

// Явное инстанцирование шаблона: реализация находится в .cpp файле,
// поэтому здесь перечислены типы, для которых библиотека собирает функцию.
template void shellSort<int>(int*, int);
template void shellSort<long long>(long long*, int);
template void shellSort<float>(float*, int);
template void shellSort<double>(double*, int);
template void shellSort<char>(char*, int);
template void shellSort<std::string>(std::string*, int);
