#include "strings.h"
int main() {
    lab::setupConsole();
    char text[lab::Capacity];
    std::size_t length = 0;
    std::printf("Введите строку (до 1023 байтов UTF-8):\n");
    const auto status = lab::readArrayLine(stdin, text, lab::Capacity - 1, length);
    if (status != lab::Status::Ok) { std::fprintf(stderr, "Ошибка ввода: %d\n", static_cast<int>(status)); return 1; }
    lab::removeLongestArray(text, length);
    std::printf("Результат: [");
    // Печатаем ровно length байтов, без требования завершающего нуля.
    if (std::fwrite(text, 1, length, stdout) != length) return 1;
    std::puts("]");
    return std::ferror(stdout) ? 1 : 0;
}
