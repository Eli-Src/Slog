#include "slog.h"

int main() {
    Slog slog = {Slog::Level::Info};

    slog.print_info("This is a test");

    slog.print_error("New Test", Slog::Properties{
        {"JOOO", "2"},
        {"TEST", true},
        {"Maria", 2.3}
    });

    slog.print_error("New Test", Slog::Properties{
    });

    return 0;
}
