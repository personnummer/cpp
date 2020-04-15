#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "personnummer.hpp"

struct TestDate {
  int year, month, day;
  bool valid;
  TestDate(int y, int m, int d, bool v) : year(y), month(m), day(d), valid(v) {}

  friend std::ostream &operator<<(std::ostream &out, const TestDate &fd) {
    return out << "Y=" << fd.year << ", M=" << fd.month << ", D=" << fd.day;
  }
};

TEST_CASE("Validate date", "[date]") {
  std::vector<TestDate> cases = {
      TestDate(1990, 1, 1, true),
      TestDate(1990, 1, 1, true),
      TestDate(1990, 1, 31, true),
      TestDate(1990, 2, 28, true),
      TestDate(2016, 2, 29, true), // 2016 was leap year
      TestDate(2020, 4, 30, true),
      TestDate(1990, 13, 1, false),
      TestDate(1990, 1, 32, false),
      TestDate(2017, 2, 29, false), // 2017 was not leap year
      TestDate(2020, 4, 31, false),
  };

  for (const auto &tc : cases) {
    std::stringstream case_title;
    case_title << "Testing " << tc;

    SECTION(case_title.str()) {
      REQUIRE(valid_date(tc.year, tc.month, tc.day) == tc.valid);
    }
  }
}

TEST_CASE("Validate social security number", "[pnr]") {
  std::map<std::string, bool> cases = {
      {"6403273813", true},    {"510818-9167", true},  {"19900101-0017", true},
      {"19130401+2931", true}, {"196408233234", true}, {"0001010107", true},
      {"000101-0107", true},   {"640327-381", false},  {"6403273814", false},
      {"640327-3814", false},
  };

  for (const auto &test_case : cases) {
    Personnummer pnr(test_case.first);

    SECTION("Testing " + test_case.first) {
      REQUIRE(pnr.valid() == test_case.second);
    }
  }
}

// vim: set ts=2 sw=2 et:
