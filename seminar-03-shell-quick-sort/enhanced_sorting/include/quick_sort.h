#ifndef QUICK_SORT_H
#define QUICK_SORT_H

// Быстрая сортировка (Quick Sort)
//
// Сортирует массив по возрастанию.
//   array - указатель на первый элемент массива
//   size  - количество элементов в массиве
//
// Шаблон работает с любым типом, для которого определён оператор <.
// Реализация находится в файле quick_sort.cpp.
template <typename T>
void quickSort(T* array, int size);

#endif // QUICK_SORT_H
