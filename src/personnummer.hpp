#include <ctime>
#include <string>
#include <vector>

int stoi_or_fallback(const std::string &maybe_digit, int fallback);

bool valid_date(int year, int month, int day);

int luhn(std::string::iterator begin, std::string::iterator end);

class Personnummer {
  std::tm date;
  int number;
  int control;
  char divider;

  void from_string(const std::string &pnr);

  int checksum() const;

  bool is_valid_date() const {
    return valid_date(date.tm_year, date.tm_mon, date.tm_mday);
  }

  bool is_valid_luhn() const { return checksum() == control; }

public:
  Personnummer(const std::string &pnr) { from_string(pnr); }

  bool valid() const { return is_valid_date() && is_valid_luhn(); };
};

// vim: set ts=2 sw=2 et:
