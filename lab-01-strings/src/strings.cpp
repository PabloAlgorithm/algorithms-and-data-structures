#include "strings.h"
#include <cstring>
#include <filesystem>
#include <limits>
#ifdef _WIN32
#include <windows.h>
#include <io.h>
#endif

namespace lab {
namespace {
#ifdef _WIN32
bool isConsoleInput(FILE* input) {
    DWORD mode = 0;
    return GetConsoleMode(reinterpret_cast<HANDLE>(_get_osfhandle(_fileno(input))), &mode) != 0;
}

// Узкий ввод CRT при CP_UTF8 может заменять русские буквы нулями.
// Читаем UTF-16 по одному символу и сразу записываем UTF-8 в исходный массив.
// Дополнительных массивов нет: только скалярные переменные.
Status readConsoleLine(FILE* input, char* text, std::size_t capacity, std::size_t& length) {
    const HANDLE handle = reinterpret_cast<HANDLE>(_get_osfhandle(_fileno(input)));
    length = 0;
    bool tooLong = false;
    for (;;) {
        wchar_t unit = 0;
        DWORD count = 0;
        if (!ReadConsoleW(handle, &unit, 1, &count, nullptr) || count != 1)
            return Status::ReadError;
        if (unit == L'\n') return tooLong ? Status::TooLong : Status::Ok;
        if (unit == L'\r') continue; // Enter в Windows даёт CR LF.
        unsigned code = static_cast<unsigned>(unit);
        if (code >= 0xD800 && code <= 0xDBFF) {
            wchar_t low = 0;
            if (!ReadConsoleW(handle, &low, 1, &count, nullptr) || count != 1 ||
                low < 0xDC00 || low > 0xDFFF) return Status::ReadError;
            code = 0x10000 + ((code - 0xD800) << 10) + (static_cast<unsigned>(low) - 0xDC00);
        } else if (code >= 0xDC00 && code <= 0xDFFF) return Status::ReadError;
        const std::size_t width = code < 0x80 ? 1 : code < 0x800 ? 2 : code < 0x10000 ? 3 : 4;
        if (tooLong || width > capacity - length) { tooLong = true; continue; }
        if (width == 1) text[length++] = static_cast<char>(code);
        else {
            const unsigned prefix = width == 2 ? 0xC0 : width == 3 ? 0xE0 : 0xF0;
            text[length++] = static_cast<char>(prefix | (code >> (6 * (width - 1))));
            for (std::size_t remaining = width - 1; remaining > 0; --remaining)
                text[length++] = static_cast<char>(0x80 | ((code >> (6 * (remaining - 1))) & 0x3F));
        }
    }
}
#endif
struct Symbol { unsigned code; std::size_t bytes; };

// Декодируем один символ прямо в исходном массиве, без копирования.
Symbol decode(const char* p, std::size_t available) {
    const unsigned first = static_cast<unsigned char>(*p);
    if (first < 0x80) return {first, 1};
    const std::size_t width = first >= 0xC2 && first <= 0xDF ? 2 :
        first >= 0xE0 && first <= 0xEF ? 3 :
        first >= 0xF0 && first <= 0xF4 ? 4 : 0;
    if (width == 0 || width > available) return {0xFFFD, 1};
    unsigned code = first & (0x7F >> width);
    for (std::size_t i = 1; i < width; ++i) {
        const unsigned byte = static_cast<unsigned char>(p[i]);
        if ((byte & 0xC0) != 0x80) return {0xFFFD, 1};
        code = (code << 6) | (byte & 0x3F);
    }
    if ((width == 2 && code < 0x80) || (width == 3 && code < 0x800) ||
        (width == 4 && code < 0x10000) || code > 0x10FFFF ||
        (code >= 0xD800 && code <= 0xDFFF)) return {0xFFFD, 1};
    return {code, width};
}

Symbol decodeCString(const char* p) {
    std::size_t available = 0;
    while (available < 4 && p[available] != '\0') ++available;
    return decode(p, available);
}

bool isLetter(unsigned c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') ||
        (c >= 0x410 && c <= 0x44F) || c == 0x401 || c == 0x451;
}

bool isVowel(unsigned c) {
    if (c >= 'A' && c <= 'Z') c += 'a' - 'A';
    if (c >= 0x410 && c <= 0x42F) c += 0x20;
    if (c == 0x401) c = 0x451;
    switch (c) {
    case 'a': case 'e': case 'i': case 'o': case 'u':
    case 0x430: case 0x435: case 0x451: case 0x438: case 0x43E:
    case 0x443: case 0x44B: case 0x44D: case 0x44E: case 0x44F:
        return true;
    default: return false;
    }
}

// После полного буфера проверяем, закончилась ли строка, и дочитываем остаток.
Status finishLine(FILE* input) {
    int c = std::fgetc(input);
    if (c == '\r') c = std::fgetc(input);
    if (c == '\n' || c == EOF)
        return std::ferror(input) ? Status::ReadError : Status::Ok;
    while (c != '\n' && c != EOF) c = std::fgetc(input);
    return std::ferror(input) ? Status::ReadError : Status::TooLong;
}
}

bool removeLongestArray(char* text, std::size_t& length) {
    std::size_t bestStart = 0, bestEnd = 0, bestLetters = 0, pos = 0;
    unsigned bestFirst = 0;
    while (pos < length) {
        Symbol symbol = decode(text + pos, length - pos);
        if (!isLetter(symbol.code)) { pos += symbol.bytes; continue; }
        const std::size_t start = pos;
        const unsigned first = symbol.code;
        std::size_t letters = 0;
        do {
            ++letters;
            pos += symbol.bytes;
            if (pos == length) break;
            symbol = decode(text + pos, length - pos);
        } while (isLetter(symbol.code));
        // Строгое > оставляет ПЕРВОЕ слово при равенстве длин.
        if (letters > bestLetters) {
            bestStart = start; bestEnd = pos;
            bestLetters = letters; bestFirst = first;
        }
    }
    if (bestLetters <= 5 || isVowel(bestFirst)) return false;
    const std::size_t removed = bestEnd - bestStart;
    for (std::size_t i = bestEnd; i < length; ++i) text[i - removed] = text[i];
    length -= removed;
    return true;
}

bool removeLongestCString(char* text) {
    char* bestStart = text;
    char* bestEnd = text;
    char* pos = text;
    std::size_t bestLetters = 0;
    unsigned bestFirst = 0;
    while (*pos != '\0') {
        Symbol symbol = decodeCString(pos);
        if (!isLetter(symbol.code)) { pos += symbol.bytes; continue; }
        char* start = pos;
        const unsigned first = symbol.code;
        std::size_t letters = 0;
        do {
            ++letters;
            pos += symbol.bytes;
            if (*pos == '\0') break;
            symbol = decodeCString(pos);
        } while (isLetter(symbol.code));
        if (letters > bestLetters) {
            bestStart = start; bestEnd = pos;
            bestLetters = letters; bestFirst = first;
        }
    }
    if (bestLetters <= 5 || isVowel(bestFirst)) return false;
    // Сдвигаем хвост вместе с завершающим нулём.
    while ((*bestStart++ = *bestEnd++) != '\0') {}
    return true;
}

Status readArrayLine(FILE* input, char* text, std::size_t capacity, std::size_t& length) {
#ifdef _WIN32
    if (isConsoleInput(input)) return readConsoleLine(input, text, capacity, length);
#endif
    length = 0;
    int c = 0;
    while (length < capacity && (c = std::fgetc(input)) != EOF && c != '\n')
        text[length++] = static_cast<char>(c);
    Status status = std::ferror(input) ? Status::ReadError : Status::Ok;
    if (length == capacity && status == Status::Ok) status = finishLine(input);
    if (length > 0 && text[length - 1] == '\r') --length;
    return status;
}

Status readCStringLine(FILE* input, char* text, std::size_t capacity) {
    if (capacity < 2 || capacity > static_cast<std::size_t>((std::numeric_limits<int>::max)()))
        return Status::TooLong;
    text[0] = '\0';
#ifdef _WIN32
    if (isConsoleInput(input)) {
        std::size_t length = 0;
        const Status status = readConsoleLine(input, text, capacity - 1, length);
        text[length] = '\0';
        return status;
    }
#endif
    if (std::fgets(text, static_cast<int>(capacity), input) == nullptr)
        return std::ferror(input) ? Status::ReadError : Status::Ok;
    std::size_t n = std::strlen(text);
    Status status = Status::Ok;
    if (n > 0 && text[n - 1] == '\n') text[--n] = '\0';
    else if (!std::feof(input)) status = finishLine(input);
    if (n > 0 && text[n - 1] == '\r') text[n - 1] = '\0';
    return status;
}

Status processFile(const char* inputPath, const char* outputPath) {
    std::error_code error;
    if (std::filesystem::equivalent(inputPath, outputPath, error)) return Status::SameFile;
    FILE* input = std::fopen(inputPath, "rb");
    if (!input) return Status::OpenInputError;
    char text[Capacity]; // Единственный массив для хранения и обработки строки.
    Status status = readCStringLine(input, text, Capacity);
    if (std::fclose(input) != 0 && status == Status::Ok) status = Status::ReadError;
    if (status != Status::Ok) return status;
    removeLongestCString(text);
    FILE* output = std::fopen(outputPath, "wb");
    if (!output) return Status::OpenOutputError;
    if (std::fputs(text, output) == EOF) status = Status::WriteError;
    if (std::fclose(output) != 0) status = Status::WriteError;
    return status;
}

void setupConsole() {
#ifdef _WIN32
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
#endif
}
}
