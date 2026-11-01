#include "strings.h"
int main() {
    lab::setupConsole();
    char text[lab::Capacity];
    std::puts("Введите строку (до 1023 байтов UTF-8):");
    const auto status = lab::readCStringLine(stdin, text, lab::Capacity);
    if (status != lab::Status::Ok) { std::fprintf(stderr, "Ошибка ввода: %d\n", static_cast<int>(status)); return 1; }
    lab::removeLongestCString(text);
    std::printf("Результат: [%s]\n", text);
    return std::ferror(stdout) ? 1 : 0;
}
