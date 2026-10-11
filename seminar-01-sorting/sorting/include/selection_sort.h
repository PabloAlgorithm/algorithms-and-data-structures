#ifndef SELECTION_SORT_H
#define SELECTION_SORT_H

// Сортировка выбором (Selection Sort)
//
// Сортирует массив по возрастанию.
//   array - указатель на первый элемент массива
//   size  - количество элементов в массиве
//
// Шаблон работает с любым типом, для которого определён оператор <.
// Реализация находится в файле selection_sort.cpp.
template <typename T>
void selectionSort(T* array, int size);

#endif // SELECTION_SORT_H
