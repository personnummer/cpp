#include <ctime>
#include <string>
#include <vector>

// See https://bit.ly/34ICqic abotut "Samordningsnummer"
const int coordination_extra = 60;

bool valid_date(int year, int month, int day);

int luhn(std::string::iterator begin, std::string::iterator end);

class Personnummer {
  std::tm date;
  int number;
  int control;
  char divider;

  void from_string(const std::string &pnr);
  int checksum() const;

public:
  Personnummer(const std::string &pnr) { from_string(pnr); }

  static Personnummer parse(const std::string &pnr);

  std::string format(bool long_format = false) const;
  int get_age() const;

  bool valid() const;
  bool is_female() const { return (number % 10) % 2 == 0; }
  bool is_male() const { return !is_female(); };
  bool is_coordination_number() const { return date.tm_mday > 12; }
};

// vim: set ts=2 sw=2 et:
