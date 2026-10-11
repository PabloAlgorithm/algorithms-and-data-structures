#ifndef SHAKER_SORT_H
#define SHAKER_SORT_H

// Сортировка шейкером (Shaker Sort, двунаправленная пузырьковая сортировка)
//
// Сортирует массив по возрастанию.
//   array - указатель на первый элемент массива
//   size  - количество элементов в массиве
//
// Шаблон работает с любым типом, для которого определён оператор <.
// Реализация находится в файле shaker_sort.cpp.
template <typename T>
void shakerSort(T* array, int size);

#endif // SHAKER_SORT_H
