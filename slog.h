#include <iomanip>
#include <iostream>
#include <map>
#include <optional>
#include <sstream>
#include <string_view>

class Slog {
public:
    enum Level {
        LevelInfo,
        LevelError,
        LevelFatal,
        LevelOff
    };

private:
    Level m_min_level;

    std::string getCurrentTimeInRFC3339() const {
        // Get the current time in UTC
        auto now = std::chrono::system_clock::now();
        std::time_t now_c = std::chrono::system_clock::to_time_t(now);
        std::tm now_tm = *std::gmtime(&now_c);

        // Format the time in RFC3339 format
        std::ostringstream oss;
        oss << std::put_time(&now_tm, "%FT%TZ");

        return oss.str();
    }

public:
    Slog(Level min_level) :
        m_min_level{min_level}
    {}

    std::string_view get_level_string(Level level) const {
        switch (level) {
        case Level::LevelInfo: return "INFO";
        case Level::LevelError: return "ERROR";
        case Level::LevelFatal: return "FATAL";
        default: return "";
        }
    }

    void print(Level level,
               std::string_view message,
               std::optional<std::map<std::string_view, std::string_view>> properties = std::nullopt) const {
        if (level < m_min_level) return;
        std::cout << "{\"level\":\"" << get_level_string(level) << "\",\"time\":\"" << getCurrentTimeInRFC3339() <<
        "\",\"message\":\"" << message << '\"';
        if (properties != std::nullopt) {
            std::cout << ",{";
            const auto& last_key = properties->rbegin()->first;
            for (const auto& pair : *properties) {
                std::cout << '\"' << pair.first << "\":\"" << pair.second << '\"';
                if (pair.first != last_key) std::cout << ',';
            }
            std::cout << '}';
        }
        std::cout << "}\n";
    }
};
