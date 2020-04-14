#include <iostream>
#include <ctime>
#include <vector>

namespace Personnummer {
    int stoi_or_fallback(const std::string& maybe_digit, int fallback);

    bool valid_date(int year, int month, int day);

    int checksum(std::tm date, int number);

    void collect_digits(std::vector<int>& digits, int num);

    void collect_digits_pad_zero(std::vector<int>& digits, int num, int min_len);

    struct Personnummer {
        std::tm date;
        int number;
        int control;
        char divider[1];

        bool is_valid_date() {
            return valid_date(date.tm_year, date.tm_mon, date.tm_mday);
        };

        bool is_valid_luhn() {
            return checksum(date, number) == control;
        };

        bool valid() {
            return is_valid_date() && is_valid_luhn();
        };
    };

    bool from_string(const std::string& pnr, Personnummer& personnummer);
}
