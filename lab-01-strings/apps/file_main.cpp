#include "strings.h"
int main(int argc, char* argv[]) {
    lab::setupConsole();
    if (argc != 1 && argc != 3) { std::fputs("Usage: FileProgram [input.txt output.txt]\n", stderr); return 1; }
    const auto status = lab::processFile(argc == 3 ? argv[1] : "input.txt", argc == 3 ? argv[2] : "output.txt");
    if (status != lab::Status::Ok) { std::fprintf(stderr, "Ошибка работы с файлами: %d\n", static_cast<int>(status)); return 1; }
    std::puts("Результат записан в выходной файл.");
    return 0;
}
