#include "personnummer.hpp"
#include <cmath>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>

/*
 * Convert a string to integer and if the conversion fails, return the fallback
 * integer instead.
 * */
int stoi_or_fallback(const std::string &maybe_digit, int fallback)
{
  try
  {
    return std::stoi(maybe_digit);
  }
  catch (...)
  {
    return fallback;
  }
}

/*
 * Check if a date is valid or not. This implementation is here isntead of using
 * something from newer chrono libraries to support C++11.
 */
bool valid_date(int year, int month, int day)
{
  if (month < 1 || month > 12)
    return false;

  if (day < 1 || day > 31)
    return false;

  if (day > 30)
  {
    switch (month)
    {
    case 2:
    case 4:
    case 6:
    case 9:
    case 11:
      return false;
    }
  }

  if (month == 2 && day > 28)
  {
    bool is_leap_year = year % 400 == 0 || (year % 100 != 0 && year % 4 == 0);

    if (day != 29 || !is_leap_year)
    {
      return false;
    }
  }

  return true;
}

/*
 * Return the result of applying luhn algoritm on the passed string iterator.
 * See more at https://en.wikipedia.org/wiki/Luhn_algorithm
 */
int luhn(std::string::iterator begin, std::string::iterator end)
{
  int sum = 0;

  // Iterate from begin to end of string representation of the personal identity
  // number. XOR the even bit every iteration to keep track of odd or even
  // position in the sequence.
  for (bool even = true; begin != end; ++begin, even ^= true)
  {
    // Conver the string value of the digit to an integer.
    int digit = *begin - '0';

    if (even)
    {
      if ((digit *= 2) > 9)
        digit -= 9;
    }

    if ((sum += digit) > 9)
      sum -= 10;
  }

  int checksum = 10 - (sum % 10);

  return checksum == 10 ? 0 : checksum;
}

/*
 * Create a new instance of the Personnummer class by calling `parse()` on a
 * static method. This is essentially the same as `Personnummer pnr(nr)` but is
 * added to fulfil v3 of `personnummer`, see
 * https://github.com/personnummer/meta#package-specification-v3
 */
Personnummer Personnummer::parse(const std::string &pnr)
{
  Personnummer pnr_instance(pnr);

  return pnr_instance;
}

/*
 * Receive a personal identity number string and set each part at appropreate
 * place on the date field of the Personnummer class. If the string format
 * isnt't valid nothing will be set.
 */
void Personnummer::from_string(const std::string &pnr)
{
  std::regex pnr_regex("^(\\d{2})?(\\d{2})(\\d{2})(\\d{2})([-+]?)?(\\d{3})(\\d?)$");
  std::smatch matches;

  if (!std::regex_search(pnr, matches, pnr_regex))
  {
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

/*
 * Format the personal identity number with a fixed divider (-). Defaults to
 * short format (omits the century) but can output long format if `true` is
 * passed as argument.
 */
std::string Personnummer::format(bool long_format) const
{
  std::stringstream ss;
  ss.fill('0');

  if (long_format)
  {
    ss << std::setw(2) << date.tm_year / 100;
  }

  ss << std::setw(2) << date.tm_year % 100 << std::setw(2) << date.tm_mon
     << std::setw(2) << date.tm_mday << "-" << std::setw(3) << number
     << control;

  return ss.str();
}

/*
 * Return the age of the person by calculating the diff between now and the day
 * the person was born. This is calculated in seconds, assuming each year has 6
 * extra hours due to leap year, without considering time zones.
 */
int Personnummer::get_age() const
{
  std::time_t t = std::time(0);
  std::tm *now = std::localtime(&t);
  int current_year = now->tm_year + 1900;
  int current_mon = now->tm_mon + 1;

  if (date.tm_mon > current_mon)
  {
    return current_year - date.tm_year - 1;
  }
  else
  {
    if (date.tm_mon == current_mon && date.tm_mday > now->tm_mday)
    {
      return current_year - date.tm_year - 1;
    }
    else
    {
      return current_year - date.tm_year;
    }
  }
}

/*
 * Calculate the checksum for a given personal identity number by using the luhn
 * algoritm. Ensures that each section is zero padded to get the correct control
 * digit.
 */
int Personnummer::checksum() const
{
  std::stringstream ss;
  ss.fill('0');

  ss << std::setw(2) << date.tm_year % 100 << std::setw(2) << date.tm_mon
     << std::setw(2) << date.tm_mday % coordination_extra << std::setw(3)
     << number;

  auto str = ss.str();

  return luhn(str.begin(), str.end());
}

bool Personnummer::valid() const
{
  return valid_date(date.tm_year, date.tm_mon,
                    date.tm_mday % coordination_extra) &&
         number > 0 && checksum() == control;
}

// vim: set ts=2 sw=2 et:
