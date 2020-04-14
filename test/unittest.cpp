#define CATCH_CONFIG_MAIN
#include "../src/personnummer.hpp"
#include "catch.hpp"

TEST_CASE("Valid date", "[date]") {
  std::vector<std::vector<int>> valid_dates = {
      {1990, 1, 1},  {1990, 1, 31},
      {1990, 2, 28}, {2016, 2, 29}, // 2016 is leap year
      {2020, 4, 30},
  };

  std::vector<std::vector<int>> invalid_dates = {
      {1990, 13, 1},
      {1990, 1, 32},
      {2017, 2, 29}, // 2017 is not leap year
      {2020, 4, 31},
  };

  for (int i = 0; i < valid_dates.size(); i++) {
    std::vector<int> test_case = valid_dates[i];

    std::stringstream case_title;
    case_title << "Testinv VALID: Y=" << test_case[0] << ", M=" << test_case[1]
               << ", D=" << test_case[2];

    SECTION(case_title.str()) {
      REQUIRE(
          Personnummer::valid_date(test_case[0], test_case[1], test_case[2]));
    }
  }

  for (int i = 0; i < invalid_dates.size(); i++) {
    std::vector<int> test_case = invalid_dates[i];

    std::stringstream case_title;
    case_title << "Testinv INVALID: Y=" << test_case[0]
               << ", M=" << test_case[1] << ", D=" << test_case[2];

    SECTION(case_title.str()) {
      REQUIRE(
          !Personnummer::valid_date(test_case[0], test_case[1], test_case[2]));
    }
  }
}

TEST_CASE("Valid personal number", "[pnr]") {
  Personnummer::Personnummer p;

  std::vector<std::string> valid = {
      "6403273813",   "510818-9167", "19900101-0017", "19130401+2931",
      "196408233234", "0001010107",  "000101-0107",
  };

  std::vector<std::string> invalid = {
      "640327-381",
      "6403273814",
      "640327-3814",
  };

  for (int i = 0; i < valid.size(); i++) {
    std::stringstream case_title;
    case_title << "Testing VALID: " << valid[i];

    SECTION(case_title.str()) {
      REQUIRE(Personnummer::from_string(valid[i], p));
      REQUIRE(p.valid());
    }
  }

  for (int i = 0; i < invalid.size(); i++) {
    std::stringstream case_title;
    case_title << "Testing INVALID: " << invalid[i];

    SECTION(case_title.str()) {
      REQUIRE(Personnummer::from_string(invalid[i], p));
      REQUIRE(!p.valid());
    }
  }
}
