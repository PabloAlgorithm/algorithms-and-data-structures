#ifndef OPTIMIZED_BUBBLE_SORT_H
#define OPTIMIZED_BUBBLE_SORT_H

// Сортировка пузырьком с улучшениями (Optimized Bubble Sort)
//
// Сортирует массив по возрастанию.
//   array - указатель на первый элемент массива
//   size  - количество элементов в массиве
//
// Шаблон работает с любым типом, для которого определён оператор <.
// Реализация находится в файле optimized_bubble_sort.cpp.
template <typename T>
void optimizedBubbleSort(T* array, int size);

#endif // OPTIMIZED_BUBBLE_SORT_H
