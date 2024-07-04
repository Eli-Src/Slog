#ifndef SLOG_H
#define SLOG_H

#include <iomanip>
#include <iostream>
#include <map>
#include <optional>
#include <sstream>
#include <string_view>
#include <variant>

class Slog {
public:
    enum Level {
        Info,
        Error,
        Fatal,
        Off
    };

    using map_values = std::variant<bool, int, float, double, std::string_view>;

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
    using Properties = std::map<std::string_view, Slog::map_values>;

    Slog(Level min_level) :
        m_min_level{min_level}
    {}

    std::string_view get_level_string(Level level) const {
        switch (level) {
        case Level::Info: return "INFO";
        case Level::Error: return "ERROR";
        case Level::Fatal: return "FATAL";
        default: return "";
        }
    }

    void print(Level level,
               std::string_view message,
               std::optional<Properties> properties = std::nullopt) const {
        if (level < m_min_level) return;
        std::ostream& out = (level >= Slog::Level::Error) ? std::cerr : std::cout;
        out << "{\"level\":\"" << get_level_string(level) << "\",\"time\":\"" << getCurrentTimeInRFC3339() <<
        "\",\"message\":\"" << message << '\"';
        if (properties != std::nullopt && (*properties).size() != 0) {
            out << ",\"properties\":{";
            const auto& last_key = properties->rbegin()->first;
            for (const auto& pair : *properties) {
                if (std::holds_alternative<std::string_view>(pair.second))
                    out << '\"' << pair.first << "\":\"" << std::get<std::string_view>(pair.second) << '\"';
                else if (std::holds_alternative<bool>(pair.second))
                    out << '\"' << pair.first << "\":" << std::boolalpha << std::get<bool>(pair.second);
                else if (std::holds_alternative<int>(pair.second))
                    out << '\"' << pair.first << "\":" << std::get<int>(pair.second);
                else if (std::holds_alternative<float>(pair.second))
                    out << '\"' << pair.first << "\":" << std::get<float>(pair.second);
                else if (std::holds_alternative<double>(pair.second))
                    out << '\"' << pair.first << "\":" << std::get<double>(pair.second);
                if (pair.first != last_key) std::cout << ',';
            }
            out << '}';
        }
        out << "}\n";
        if (level == Level::Fatal) std::exit(1);
    }

    void print_info(std::string_view message,
                    std::optional<Properties> properties = std::nullopt) const {
        print(Level::Info, message, properties);
    }

    void print_error(std::string_view message,
                    std::optional<Properties> properties = std::nullopt) const {
        print(Level::Error, message, properties);
    }

    void print_fatal(std::string_view message,
                    std::optional<Properties> properties = std::nullopt) const {
        print(Level::Fatal, message, properties);
    }
};

#endif // SLOG_H
