#ifndef EXCHANGE_SORT_H
#define EXCHANGE_SORT_H

// Сортировка обменом (Exchange Sort)
//
// Сортирует массив по возрастанию.
//   array - указатель на первый элемент массива
//   size  - количество элементов в массиве
//
// Шаблон работает с любым типом, для которого определён оператор <.
// Реализация находится в файле exchange_sort.cpp.
template <typename T>
void exchangeSort(T* array, int size);

#endif // EXCHANGE_SORT_H
