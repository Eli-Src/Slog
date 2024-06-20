#include "slog.h"
#include <iostream>
#include <map>
#include <string_view>

int main() {
    SLog slog = {SLog::Level::LevelInfo};

    slog.print(SLog::Level::LevelInfo, "This is a test");

    std::cout << '\n';

    slog.print(SLog::Level::LevelError, "New Test", std::map<std::string_view, std::string_view>{
        {"JOOO", "2"},
        {"TEST", "KASDK"}
    });

    return 0;
}
