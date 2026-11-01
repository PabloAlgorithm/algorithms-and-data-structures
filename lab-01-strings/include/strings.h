#pragma once
#include <cstddef>
#include <cstdio>

namespace lab {
constexpr std::size_t Capacity = 1024;
enum class Status { Ok, TooLong, ReadError, OpenInputError, OpenOutputError,
                    WriteError, SameFile };
// Корректный UTF-8. Слово — последовательность русских/английских букв.
// При равенстве длин выбираем первое слово. Разделители сохраняем.
// Для массива length — число байтов; завершающий ноль НЕ требуется.
bool removeLongestArray(char* text, std::size_t& length);
// Для C-строки обязателен завершающий ноль.
bool removeLongestCString(char* text);
Status readArrayLine(FILE* input, char* text, std::size_t capacity, std::size_t& length);
Status readCStringLine(FILE* input, char* text, std::size_t capacity);
Status processFile(const char* inputPath, const char* outputPath);
void setupConsole();
}
