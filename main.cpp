#include "slog.h"
#include <iostream>
#include <map>
#include <string_view>

int main() {
    Slog slog = {Slog::Level::Info};

    slog.print_info("This is a test");

    std::cout << '\n';

    slog.print_error("New Test", std::map<std::string_view, Slog::map_values>{
        {"JOOO", "2"},
        {"TEST", true},
        {"Maria", 2.3}
    });

    return 0;
}
