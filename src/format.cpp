#include <string>
#include "format.h"

using std::string;

#define NEXT_TIME_UNIT 60
#define NUM_DIGITS 2 // Only relevant for seconds and minutes. Hours can grow unbounded

// DID: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long secs) {
    int seconds = secs % NEXT_TIME_UNIT;
    int minutes = (secs / NEXT_TIME_UNIT) % 60; // integer division
    int hours = secs / NEXT_TIME_UNIT / NEXT_TIME_UNIT;

    string seconds_ = std::to_string(seconds);
    string minutes_ = std::to_string(minutes);
    string hours_ = std::to_string(hours);

    seconds_ = string(NUM_DIGITS - seconds_.length(), '0') + seconds_;
    minutes_ = string(NUM_DIGITS - minutes_.length(), '0') + minutes_;

    return hours_ + ":" + minutes_ + ":" + seconds_;
}