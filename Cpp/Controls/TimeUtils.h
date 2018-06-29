#pragma once

#include <ctime>
#include <chrono>
#include <string>

namespace time_utils
{
enum class DurationGroup
{
    Top,
    Days,
    HalfDays,
    QuarterDays,
    TwoHours,
    Hours,
    HalfHours,
    QuarterHours,
    FiveMinutes,
    Minutes,
    HalfMinutes,
    QuarterMinutes,
    FiveSeconds,
    Seconds,
    Last
};

using DurationGroupSize = long long;
using DurationGroupIndex = unsigned;

const auto one_day = std::chrono::hours(24);
const auto one_hour = std::chrono::hours(1);
const auto one_min = std::chrono::minutes(1);
const auto one_sec = std::chrono::seconds(1);
const auto one_msec = std::chrono::milliseconds(1);

const auto two_hours = std::chrono::hours(2);
const auto five_min = std::chrono::minutes(5);
const auto five_sec = std::chrono::seconds(5);

const auto half_day = std::chrono::hours(12);
const auto half_hour = std::chrono::minutes(30);
const auto half_min = std::chrono::seconds(30);
const auto half_sec = std::chrono::milliseconds(500);

const auto quarter_day = std::chrono::hours(6);
const auto quarter_hour = std::chrono::minutes(15);
const auto quarter_min = std::chrono::seconds(15);

template <typename DurationType>
std::time_t duration_ms(DurationType type)
{
    return std::time_t(std::chrono::duration_cast<std::chrono::milliseconds>(type).count());
}

template <>
std::time_t duration_ms(DurationGroup group);

DurationGroup group(std::time_t duration);
time_utils::DurationGroup subgroup(DurationGroup group);

DurationGroupSize group_count(std::time_t duration, DurationGroup group);
DurationGroupSize subgroup_count(DurationGroup group);

std::string to_string(std::time_t value, DurationGroup group);

std::time_t before(std::time_t value, DurationGroup group);
std::time_t after(std::time_t value, DurationGroup group);

} // namespace time_utils
