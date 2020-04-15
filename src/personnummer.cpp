#include "personnummer.hpp"
#include <iomanip>
#include <regex>
#include <sstream>
#include <string>

void Personnummer::from_string(const std::string &pnr) {
  std::regex pnr_regex(
      "^(\\d{2})?(\\d{2})(\\d{2})(\\d{2})([-|+]?)?(\\d{3})(\\d?)$");
  std::smatch matches;

  if (!std::regex_search(pnr, matches, pnr_regex)) {
    return;
  }

  int century = stoi_or_fallback(matches.str(1), 19);
  int year = stoi_or_fallback(matches.str(2), 0);

  date.tm_year = century * 100 + year;
  date.tm_mon = stoi_or_fallback(matches.str(3), 0);
  date.tm_mday = stoi_or_fallback(matches.str(4), 0);
  number = stoi_or_fallback(matches.str(6), 0);
  control = stoi_or_fallback(matches.str(7), 0);

  divider = *matches.str(5).c_str();
}

int Personnummer::checksum() const {
  std::stringstream ss;
  ss.fill('0');

  ss << std::setw(2) << date.tm_year % 100 << std::setw(2) << date.tm_mon
     << std::setw(2) << date.tm_mday << std::setw(3) << number;

  auto str = ss.str();

  return 10 - luhn(str.begin(), str.end());
}

int stoi_or_fallback(const std::string &maybe_digit, int fallback) {
  try {
    return std::stoi(maybe_digit);
  } catch (...) {
    return fallback;
  }
}

bool valid_date(int year, int month, int day) {
  if (month < 1 || month > 12)
    return false;

  if (day < 1 || day > 31)
    return false;

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

int luhn(std::string::iterator begin, std::string::iterator end) {
  int sum = 0;

  for (bool even = true; begin != end; ++begin, even ^= true) {
    int digit = *begin - '0';

    if (even) {
      if ((digit *= 2) > 9)
        digit -= 9;
    }

    if ((sum += digit) > 9)
      sum -= 10;
  }

  return sum;
}

// vim: set ts=2 sw=2 et:
