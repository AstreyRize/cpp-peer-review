#include <stdexcept>
#include <string>
#include <array>

using namespace std;

struct DateTime {
    int year, month, day, hour, minute, second;
};

void CheckYear(int year) {
    const int min_year = 1;
    const int max_year = 9999;

    if (year < min_year) {
        throw domain_error("year is too small"s);
    }
    if (year > max_year) {
        throw domain_error("year is too big"s);
    }
}

void CheckMonth(int month) {
    const int min_month = 1;
    const int max_month = 12;

    if (month < min_month) {
        throw domain_error("month is too small"s);
    }
    if (month > max_month) {
        throw domain_error("month is too big"s);
    }
}

void CheckDay(int year, int month, int day) {
    const bool is_leap_year = (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
    const array<int, 12> month_lengths = { 31, 28 + is_leap_year, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    if (day < 1 || day > month_lengths[month - 1]) {
        throw domain_error("day is invalid"s);
    }
}

void CheckHour(int hour) {
    const int max_hour = 23;

    if (hour < 0 || hour > max_hour) {
        throw domain_error("hour is invalid"s);
    }
}

void CheckMinuteSecond(int value, const string& type) {
    const int max_value = 59;

    if (value < 0 || value > max_value) {
        throw domain_error(type + " is invalid"s);
    }
}

void CheckDateTimeValidity(const DateTime& dt) {
    CheckYear(dt.year);
    CheckMonth(dt.month);
    CheckDay(dt.year, dt.month, dt.day);
    CheckHour(dt.hour);
    CheckMinuteSecond(dt.minute, "minute");
    CheckMinuteSecond(dt.second, "second");
}
