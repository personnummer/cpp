#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "personnummer.hpp"

struct TestDate {
  int year, month, day;
  std::string pnr;
  bool valid;

  TestDate(int y, int m, int d, bool v) : year(y), month(m), day(d), valid(v) {}
  TestDate(std::string p, bool v) : pnr(p), valid(v) {}

  friend std::ostream &operator<<(std::ostream &out, const TestDate &fd) {
    if (fd.pnr != "") {
      return out << "pnr=" << fd.pnr;
    }

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
  std::vector<TestDate> cases = {
      TestDate("6403273813", true),    TestDate("510818-9167", true),
      TestDate("19900101-0017", true), TestDate("19130401+2931", true),
      TestDate("196408233234", true),  TestDate("0001010107", true),
      TestDate("000101-0107", true),   TestDate("640327-381", false),
      TestDate("6403273814", false),   TestDate("640327-3814", false),
  };

  for (const auto &tc : cases) {
    std::stringstream case_title;
    case_title << "Testing " << tc;

    SECTION(case_title.str()) {
      Personnummer pnr(tc.pnr);
      REQUIRE(pnr.valid() == tc.valid);
    }
  }
}

TEST_CASE("Validate luhn", "[luhn]") {
  std::map<std::string, int> cases = {
      {"900101001", 7}, {"640327381", 3}, {"640823323", 4},
      {"000101010", 7}, {"510818916", 7}, {"130401293", 1},
  };

  for (const auto &tc : cases) {
    std::string pnr = tc.first;
    int control = tc.second;

    std::stringstream case_title;
    case_title << "Testing " << pnr;

    SECTION(case_title.str()) {
      REQUIRE(luhn(pnr.begin(), pnr.end()) == control);
    }
  }
}

// vim: set ts=2 sw=2 et:
