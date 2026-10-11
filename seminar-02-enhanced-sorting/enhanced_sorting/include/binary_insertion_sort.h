#ifndef BINARY_INSERTION_SORT_H
#define BINARY_INSERTION_SORT_H

// Сортировка вставкой с бинарным поиском (Binary Insertion Sort)
//
// Сортирует массив по возрастанию.
//   array - указатель на первый элемент массива
//   size  - количество элементов в массиве
//
// Шаблон работает с любым типом, для которого определён оператор <.
// Реализация находится в файле binary_insertion_sort.cpp.
template <typename T>
void binaryInsertionSort(T* array, int size);

#endif // BINARY_INSERTION_SORT_H
