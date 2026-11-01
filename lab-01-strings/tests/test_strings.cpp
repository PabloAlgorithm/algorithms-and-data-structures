#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include "strings.h"
#include <cstring>
#include <string>
#include <fstream>
#include <filesystem>
#include <chrono>
#include <memory>
#ifdef _WIN32
#include <windows.h>
#endif

// Массивы с ожиданиями допустимы в ТЕСТАХ: рабочие функции их не используют.
struct Example { const char* input; const char* expected; bool removed; };
const Example examples[] = {
    {"", "", false}, {"   \t", "   \t", false}, {"123!?", "123!?", false},
    {"cat", "cat", false}, {"brick", "brick", false}, {"planet", "", true},
    {"planet cat", " cat", true}, {"cat planet dog", "cat  dog", true},
    {"cat planet", "cat ", true}, {"orange planet", "orange planet", false},
    {"planet orange", " orange", true}, {"elephant planet", "elephant planet", false},
    {"Planet", "", true}, {"ORANGE", "ORANGE", false},
    {"a,planet!b", "a,!b", true}, {"planet\tcat", "\tcat", true},
    {"123planet456", "123456", true}, {"planet-rocket", "-rocket", true},
    {"кот", "кот", false}, {"книга", "книга", false}, {"машина", "", true},
    {"МАШИНА", "", true}, {"моя машина тут", "моя  тут", true},
    {"облако машина", "облако машина", false}, {"Облако", "Облако", false},
    {"Ёлочка", "Ёлочка", false}, {"ёлочка", "ёлочка", false},
    {"яблоко", "яблоко", false}, {"йогурт", "", true},
    {"съезды", "", true}, {"вьюнок", "", true}, {"жёлтый", "", true},
    {"машина привет", " привет", true}, {"машина,кот!", ",кот!", true},
    {"книга planet", "книга ", true}, {"alphabet машина", "alphabet машина", false},
    {"  машина  ", "    ", true}, {"😀машина😀", "😀😀", true},
    {"猫planet猫", "猫猫", true}, {"\xEF\xBB\xBFмашина", "\xEF\xBB\xBF", true}
};

TEST_CASE("Both representations follow variant 13", "[algorithm]") {
    for (const auto& example : examples) {
        DYNAMIC_SECTION(example.input) {
            std::size_t n = std::strlen(example.input);
            // Ровно n байтов: даже физически нет места для нуля.
            auto array = std::make_unique<char[]>(n == 0 ? 1 : n);
            std::memcpy(array.get(), example.input, n);
            CHECK(lab::removeLongestArray(array.get(), n) == example.removed);
            CHECK(std::string(array.get(), n) == example.expected);
            char text[lab::Capacity];
            std::strcpy(text, example.input);
            CHECK(lab::removeLongestCString(text) == example.removed);
            CHECK(std::string(text) == example.expected);
        }
    }
}

TEST_CASE("All vowels in both cases preserve the longest word", "[vowels]") {
    const char* vowels[] = {"a","e","i","o","u","A","E","I","O","U",
        "а","е","ё","и","о","у","ы","э","ю","я","А","Е","Ё","И","О","У","Ы","Э","Ю","Я"};
    for (auto vowel : vowels) {
        const std::string original = std::string(vowel) + "bcdef";
        char text[lab::Capacity]; std::strcpy(text, original.c_str());
        std::size_t n = original.size();
        CHECK_FALSE(lab::removeLongestArray(text, n));
        CHECK(std::string(text, n) == original);
        CHECK_FALSE(lab::removeLongestCString(text));
        CHECK(std::string(text) == original);
    }
}

TEST_CASE("No writes outside logical array or C terminator", "[bounds]") {
    char array[] = {'p','l','a','n','e','t','X'};
    std::size_t n = 6;
    REQUIRE(lab::removeLongestArray(array, n)); CHECK(n == 0); CHECK(array[6] == 'X');
    char text[] = {'p','l','a','n','e','t','\0','X'};
    REQUIRE(lab::removeLongestCString(text)); CHECK(text[0] == '\0'); CHECK(text[7] == 'X');
}

// Отдельный временный каталог на тест; исходные пользовательские файлы не затрагиваются.
struct Files {
    std::filesystem::path dir;
    Files() {
        const auto id = std::chrono::high_resolution_clock::now().time_since_epoch().count();
        dir = std::filesystem::temp_directory_path() / ("lab01_tests_" + std::to_string(id));
        if (!std::filesystem::create_directory(dir)) throw std::runtime_error("test directory collision");
    }
    ~Files() { std::error_code ec; std::filesystem::remove_all(dir, ec); }
    std::string path(const char* name) const { return (dir / name).string(); }
    void write(const char* name, const std::string& text) const {
        std::ofstream out(path(name), std::ios::binary); out << text;
        if (!out) throw std::runtime_error("test fixture write failed");
    }
    std::string read(const char* name) const {
        std::ifstream in(path(name), std::ios::binary);
        return {std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>()};
    }
};

TEST_CASE("File variant matches every example and preserves input", "[files]") {
    Files f;
    for (const auto& e : examples) {
        f.write("input.txt", e.input);
        REQUIRE(lab::processFile(f.path("input.txt").c_str(), f.path("output.txt").c_str()) == lab::Status::Ok);
        CHECK(f.read("output.txt") == e.expected);
        CHECK(f.read("input.txt") == e.input);
    }
}

TEST_CASE("Line readers handle EOF, CRLF, limits and next line", "[input]") {
    Files f;
    const std::string lines[] = {"", "\n", "\r\n", "машина", "машина\n", "машина\r\n",
        std::string(1023, 'b'), std::string(1023, 'b')+"\n", std::string(1023, 'b')+"\r\n",
        std::string(1024, 'b'), std::string(1024, 'b')+"\n", std::string(1024, 'b')+"\r\n"};
    for (const auto& line : lines) {
        f.write("input.txt", line);
        const auto expectedStatus = line.find(std::string(1024, 'b')) == 0 ? lab::Status::TooLong : lab::Status::Ok;
        for (bool cstring : {false, true}) {
            FILE* stream = std::fopen(f.path("input.txt").c_str(), "rb"); REQUIRE(stream != nullptr);
            char text[lab::Capacity]; std::size_t n = 0;
            const auto status = cstring ? lab::readCStringLine(stream, text, lab::Capacity)
                : lab::readArrayLine(stream, text, lab::Capacity-1, n);
            std::fclose(stream);
            CHECK(status == expectedStatus);
            if (status == lab::Status::Ok) {
                auto expected = line;
                if (!expected.empty() && expected.back() == '\n') expected.pop_back();
                if (!expected.empty() && expected.back() == '\r') expected.pop_back();
                CHECK((cstring ? std::string(text) : std::string(text, n)) == expected);
            }
        }
    }
    for (bool cstring : {false, true}) {
        f.write("input.txt", std::string(1100, 'b') + "\ncat\n");
        FILE* stream = std::fopen(f.path("input.txt").c_str(), "rb"); REQUIRE(stream != nullptr);
        char text[lab::Capacity]; std::size_t n = 0;
        auto read = [&] { return cstring ? lab::readCStringLine(stream,text,lab::Capacity) : lab::readArrayLine(stream,text,lab::Capacity-1,n); };
        CHECK(read() == lab::Status::TooLong);
        CHECK(read() == lab::Status::Ok);
        CHECK((cstring ? std::string(text) : std::string(text,n)) == "cat");
        std::fclose(stream);
    }
}

TEST_CASE("File errors do not overwrite a result before successful input", "[errors]") {
    Files f; f.write("output.txt", "KEEP");
    CHECK(lab::processFile(f.path("missing").c_str(),f.path("output.txt").c_str()) == lab::Status::OpenInputError);
    CHECK(f.read("output.txt") == "KEEP");
    f.write("input.txt",std::string(1024,'b'));
    CHECK(lab::processFile(f.path("input.txt").c_str(),f.path("output.txt").c_str()) == lab::Status::TooLong);
    CHECK(f.read("output.txt") == "KEEP");
    CHECK(lab::processFile(f.path("input.txt").c_str(),f.path("input.txt").c_str()) == lab::Status::SameFile);
    CHECK(f.read("input.txt") == std::string(1024,'b'));
    f.write("input.txt","planet\nsecond line");
    CHECK(lab::processFile(f.path("input.txt").c_str(),f.path("output.txt").c_str()) == lab::Status::Ok);
    CHECK(f.read("output.txt").empty());
    CHECK(lab::processFile(f.path("input.txt").c_str(),f.path("missing/output.txt").c_str()) == lab::Status::OpenOutputError);
}

#ifdef _WIN32
// Скрытый тест: запускается отдельно как Tests.exe "[console]" в консоли,
// поскольку окно вывода сборки Visual Studio перенаправляет стандартный ввод.
TEST_CASE("Windows console preserves Unicode input", "[.][console]") {
    DWORD mode = 0;
    const HANDLE input = GetStdHandle(STD_INPUT_HANDLE);
    REQUIRE(GetConsoleMode(input, &mode) != 0);
    struct ConsoleExample { std::wstring keys; std::string expected; std::size_t capacity; lab::Status status; };
    const ConsoleExample cases[] = {
        {L"Моя машина тут", "Моя машина тут", 1023, lab::Status::Ok},
        {L"English text", "English text", 1023, lab::Status::Ok},
        {L"Ёлочка йогурт", "Ёлочка йогурт", 1023, lab::Status::Ok},
        {L"猫😀", "猫😀", 1023, lab::Status::Ok},
        {L"", "", 1023, lab::Status::Ok},
        {L"яя", "яя", 4, lab::Status::Ok},
        {L"яя", "я", 3, lab::Status::TooLong},
        {L"😀", "😀", 4, lab::Status::Ok},
        {L"😀", "", 3, lab::Status::TooLong}
    };
    for (const auto& example : cases) {
        for (bool cstring : {false,true}) {
            for (wchar_t unit : example.keys + L"\r") {
                INPUT_RECORD event{};
                event.EventType = KEY_EVENT;
                event.Event.KeyEvent.bKeyDown = TRUE;
                event.Event.KeyEvent.wRepeatCount = 1;
                event.Event.KeyEvent.wVirtualKeyCode = unit == L'\r' ? VK_RETURN : 0;
                event.Event.KeyEvent.uChar.UnicodeChar = unit;
                DWORD written = 0;
                REQUIRE(WriteConsoleInputW(input, &event, 1, &written) != 0);
                REQUIRE(written == 1);
            }
            char text[lab::Capacity];
            std::size_t n = 0;
            const auto status = cstring ? lab::readCStringLine(stdin,text,example.capacity+1)
                : lab::readArrayLine(stdin,text,example.capacity,n);
            REQUIRE(status == example.status);
            CHECK((cstring ? std::string(text) : std::string(text,n)) == example.expected);
        }
    }
}
#endif
