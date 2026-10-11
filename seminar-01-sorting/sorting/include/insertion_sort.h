#ifndef INSERTION_SORT_H
#define INSERTION_SORT_H

// Сортировка вставками (Insertion Sort)
//
// Сортирует массив по возрастанию.
//   array - указатель на первый элемент массива
//   size  - количество элементов в массиве
//
// Шаблон работает с любым типом, для которого определён оператор <.
// Реализация находится в файле insertion_sort.cpp.
template <typename T>
void insertionSort(T* array, int size);

#endif // INSERTION_SORT_H
