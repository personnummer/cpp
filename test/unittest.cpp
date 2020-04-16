#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "personnummer.hpp"
#include <ctime>

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

struct TestFormat {
  std::string input;
  std::string expected_short;
  std::string expected_long;

  TestFormat(std::string i, std::string s, std::string l)
      : input(i), expected_short(s), expected_long(l) {}
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

TEST_CASE("Format number", "[format]") {
  std::vector<TestFormat> cases = {
      TestFormat("9001018080", "900101-8080", "19900101-8080"),
      TestFormat("900101-8080", "900101-8080", "19900101-8080"),
      TestFormat("900101+8080", "900101-8080", "19900101-8080"),
      TestFormat("19900101-8080", "900101-8080", "19900101-8080"),
      TestFormat("19900101+8080", "900101-8080", "19900101-8080"),
      TestFormat("18900101-8080", "900101-8080", "18900101-8080"),
  };

  for (const auto &tc : cases) {
    Personnummer pnr(tc.input);

    REQUIRE(pnr.format() == tc.expected_short);
    REQUIRE(pnr.format(false) == tc.expected_short);
    REQUIRE(pnr.format(true) == tc.expected_long);
  }
}

TEST_CASE("Check age", "[age]") {
  time_t now_time = time(NULL);
  tm *now = localtime(&now_time);

  std::stringstream twenty_years_yesterday;
  twenty_years_yesterday.fill('0');
  twenty_years_yesterday << now->tm_year + 1900 - 20 << std::setw(2)
                         << now->tm_mon + 1 << std::setw(2) << now->tm_mday - 1
                         << "-1111";

  std::stringstream twenty_years_tomorrow;
  twenty_years_tomorrow.fill('0');
  twenty_years_tomorrow << now->tm_year + 1900 - 20 << std::setw(2)
                        << now->tm_mon + 1 << std::setw(2) << now->tm_mday + 1
                        << "-2222";

  std::stringstream one_hundred_one_jan;
  one_hundred_one_jan.fill('0');
  one_hundred_one_jan << now->tm_year + 1799 << std::setw(2) << 1
                      << std::setw(2) << 1 << "-3333";

  std::map<std::string, int> cases = {
      {twenty_years_yesterday.str(), 20},
      {twenty_years_tomorrow.str(), 19},
      {one_hundred_one_jan.str(), 101},
  };

  for (const auto &tc : cases) {
    Personnummer pnr(tc.first);
    int age = tc.second;

    std::stringstream case_title;
    case_title << "Testing " << pnr.format(true);

    SECTION(case_title.str()) { REQUIRE(pnr.get_age() == age); }
  }
}

TEST_CASE("Check gender", "[gender]") {
  std::map<std::string, bool> cases = {
      {"800101-3294", false},
      {"000903-6603", true},
      {"19090903-6603", true},
      {"800101+3294", false},
  };

  for (const auto &tc : cases) {
    Personnummer pnr(tc.first);
    int is_female = tc.second;

    std::stringstream case_title;
    case_title << "Testing " << pnr.format(true);

    SECTION(case_title.str()) {
      REQUIRE(pnr.is_female() == is_female);
      REQUIRE(pnr.is_male() == !is_female);
    }
  }
}

TEST_CASE("Check coordination", "[coordination]") {
  std::map<std::string, bool> cases = {
      {"800161-3294", true},
      {"800101-3294", false},
  };

  for (const auto &tc : cases) {
    Personnummer pnr(tc.first);
    int is_coordination_number = tc.second;

    std::stringstream case_title;
    case_title << "Testing " << pnr.format(true);

    SECTION(case_title.str()) {
      REQUIRE(pnr.valid());
      REQUIRE(pnr.is_coordination_number() == is_coordination_number);
    }
  }
}

TEST_CASE("Parse strign", "[parse]") {
  std::string pnr_str = "19900101-0017";

  Personnummer pnr = Personnummer::parse(pnr_str);

  REQUIRE(pnr.valid());
  REQUIRE(pnr.is_male());
}

// vim: set ts=2 sw=2 et:
