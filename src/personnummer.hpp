#include <ctime>
#include <string>
#include <vector>

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

  bool valid() const;
};

// vim: set ts=2 sw=2 et:
