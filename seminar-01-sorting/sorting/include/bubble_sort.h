#ifndef BUBBLE_SORT_H
#define BUBBLE_SORT_H

// Сортировка пузырьком (Bubble Sort)
//
// Сортирует массив по возрастанию.
//   array - указатель на первый элемент массива
//   size  - количество элементов в массиве
//
// Шаблон работает с любым типом, для которого определён оператор <.
// Реализация находится в файле bubble_sort.cpp.
template <typename T>
void bubbleSort(T* array, int size);

#endif // BUBBLE_SORT_H
