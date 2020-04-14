#include "personnummer.hpp"
#include <regex>

namespace Personnummer {
bool from_string(const std::string &pnr, Personnummer &personnummer) {
  std::regex pnr_regex(
      "^(\\d{2})?(\\d{2})(\\d{2})(\\d{2})([-|+]?)?(\\d{3})(\\d?)$");
  std::smatch matches;

  int century, year;

  if (std::regex_search(pnr, matches, pnr_regex)) {
    century = stoi_or_fallback(matches.str(1), 19);
    year = stoi_or_fallback(matches.str(2), 0);

    personnummer.date.tm_year = century * 100 + year;
    personnummer.date.tm_mon = stoi_or_fallback(matches.str(3), 0);
    personnummer.date.tm_mday = stoi_or_fallback(matches.str(4), 0);
    personnummer.number = stoi_or_fallback(matches.str(6), 0);
    personnummer.control = stoi_or_fallback(matches.str(7), 0);

    personnummer.divider[0] = *matches.str(5).c_str();
  } else {
    return false;
  }

  return true;
}

int stoi_or_fallback(const std::string &maybe_digit, int fallback) {
  try {
    return std::stoi(maybe_digit);
  } catch (...) {
    return fallback;
  }
}

bool valid_date(int year, int month, int day) {
  if (month < 1 || month > 12) {
    return false;
  }

  if (day < 1 || day > 31) {
    return false;
  }

  if (day > 30) {
    switch (month) {
    case 2:
    case 4:
    case 6:
    case 9:
    case 11:
      return false;
    }
  }

  if (month == 2 && day > 28) {
    bool is_leap_year = year % 400 == 0 || (year % 100 != 0 && year % 4 == 0);

    if (day != 29 || !is_leap_year) {
      return false;
    }
  }

  return true;
}

int checksum(std::tm date, int number) {
  std::vector<int> digits;

  collect_digits_pad_zero(digits, date.tm_year % 100, 2);
  collect_digits_pad_zero(digits, date.tm_mon, 2);
  collect_digits_pad_zero(digits, date.tm_mday, 2);
  collect_digits_pad_zero(digits, number, 3);

  int sum = 0;

  for (int i = 0; i < digits.size(); i++) {
    int temp = digits.at(i);

    if (i % 2 == 0) {
      temp *= 2;

      if (temp > 9)
        temp -= 9;
    }

    sum += temp;
  }

  return 10 - (sum % 10);
}

void collect_digits(std::vector<int> &digits, int num) {
  if (num > 9) {
    collect_digits(digits, num / 10);
  }

  digits.push_back(num % 10);
}

void collect_digits_pad_zero(std::vector<int> &digits, int num, int min_len) {
  // New vector for this section.
  std::vector<int> section_digits;

  // Collect the digits from given number.
  collect_digits(section_digits, num);

  // Add the potential padded seroes.
  int missing_digits = min_len - section_digits.size();
  for (int i = 0; i < missing_digits; i++) {
    section_digits.insert(section_digits.begin(), 0);
  }

  // Add the padded section to final vector.
  digits.insert(digits.end(), section_digits.begin(), section_digits.end());
}
} // namespace Personnummer
