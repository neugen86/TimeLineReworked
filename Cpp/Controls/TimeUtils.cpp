#include "TimeUtils.h"

#include <iomanip>
#include <sstream>
#include <algorithm>

namespace
{
const std::time_t ZERO_TIME(0);

} // anonymous namespace

template <>
std::time_t time_utils::duration_ms(DurationGroup group)
{
    switch (group)
    {
    case DurationGroup::Days:
        return duration_ms(one_day);

    case DurationGroup::HalfDays:
        return duration_ms(half_day);

    case DurationGroup::QuarterDays:
        return duration_ms(quarter_day);

    case DurationGroup::TwoHours:
        return duration_ms(two_hours);

    case DurationGroup::Hours:
        return duration_ms(one_hour);

    case DurationGroup::HalfHours:
        return duration_ms(half_hour);

    case DurationGroup::QuarterHours:
        return duration_ms(quarter_hour);

    case DurationGroup::FiveMinutes:
        return duration_ms(five_min);

    case DurationGroup::Minutes:
        return duration_ms(one_min);

    case DurationGroup::HalfMinutes:
        return duration_ms(half_min);

    case DurationGroup::QuarterMinutes:
        return duration_ms(quarter_min);

    case DurationGroup::FiveSeconds:
        return duration_ms(five_sec);

    case DurationGroup::Seconds:
        return duration_ms(one_sec);

    default:
        return 0;
    }
}

time_utils::DurationGroup time_utils::group(std::time_t duration)
{
    static const int first = (int(DurationGroup::Top) + 1);
    static const int last = (int(DurationGroup::Last) - 2);

    for (int i = last; i >= first; --i)
    {
        const DurationGroup group = DurationGroup(i);

        if (duration < duration_ms(group))
            return subgroup(group);
    }

    return DurationGroup(first);
}

time_utils::DurationGroup time_utils::subgroup(DurationGroup group)
{
    if (group < DurationGroup::Last)
        return time_utils::DurationGroup(int(group) + 1);

    return DurationGroup::Last;
}

time_utils::DurationGroupSize time_utils::group_count(std::time_t duration, DurationGroup group)
{
    const auto value = std::chrono::milliseconds(duration);

    switch (group)
    {
    case DurationGroup::TwoHours:
    case DurationGroup::HalfDays:
    case DurationGroup::HalfHours:
    case DurationGroup::HalfMinutes:
    case DurationGroup::QuarterDays:
    case DurationGroup::QuarterHours:
    case DurationGroup::QuarterMinutes:
    case DurationGroup::FiveMinutes:
    case DurationGroup::FiveSeconds:
    {
        const DurationGroup sub = subgroup(group);
        return (group_count(duration, sub) / subgroup_count(group));
    }

    case DurationGroup::Days:
        typedef std::chrono::duration<int64_t, std::ratio<86400>> std_chrono_days;
        return std::chrono::duration_cast<std_chrono_days>(value).count();

    case DurationGroup::Hours:
        return std::chrono::duration_cast<std::chrono::hours>(value).count();

    case DurationGroup::Minutes:
        return std::chrono::duration_cast<std::chrono::minutes>(value).count();

    case DurationGroup::Seconds:
        return std::chrono::duration_cast<std::chrono::seconds>(value).count();

    default:
        break;
    }

    return std::chrono::duration_cast<std::chrono::milliseconds>(value).count();
}

time_utils::DurationGroupSize time_utils::subgroup_count(DurationGroup group)
{
    switch (group)
    {
    case DurationGroup::Days:
    case DurationGroup::Hours:
    case DurationGroup::TwoHours:
    case DurationGroup::Minutes:
    case DurationGroup::Seconds:
    case DurationGroup::HalfDays:
    case DurationGroup::HalfHours:
    case DurationGroup::HalfMinutes:
        return 2;

    case DurationGroup::QuarterDays:
    case DurationGroup::QuarterHours:
    case DurationGroup::QuarterMinutes:
        return 3;

    case DurationGroup::FiveMinutes:
    case DurationGroup::FiveSeconds:
        return 5;

    default:
        return 0;
    }
}

std::string time_utils::to_string(std::time_t value, DurationGroup group)
{
    value /= duration_ms(one_sec); // no idea why it works so

    std::ostringstream stream;
    std::tm* const tm = std::gmtime(&value);

    switch (group)
    {
    case DurationGroup::Days:
        stream << std::put_time(tm, "%d-%m-%Y");
        break;

    case DurationGroup::Hours:
    case DurationGroup::TwoHours:
    case DurationGroup::HalfDays:
    case DurationGroup::QuarterDays:
        stream << std::put_time(tm, "%d-%m-%Y - %H");
        break;

    case DurationGroup::Minutes:
    case DurationGroup::FiveMinutes:
    case DurationGroup::HalfHours:
    case DurationGroup::QuarterHours:
        stream << std::put_time(tm, "%d-%m-%Y - %H:%M");
        break;

    case DurationGroup::Seconds:
    case DurationGroup::FiveSeconds:
    case DurationGroup::HalfMinutes:
    case DurationGroup::QuarterMinutes:
        stream << std::put_time(tm, "%d-%m-%Y - %H:%M:%S");
        break;

    default:
        stream << int(group);
        break;
    }

    return stream.str();
}

std::time_t time_utils::before(std::time_t value, DurationGroup group)
{
    std::tm* const tm = std::gmtime(&value);

    switch (group)
    {
    case DurationGroup::Days:
    case DurationGroup::HalfDays:
    case DurationGroup::QuarterDays:
    case DurationGroup::TwoHours:
        tm->tm_hour = 0;

    case DurationGroup::Hours:
    case DurationGroup::HalfHours:
    case DurationGroup::QuarterHours:
    case DurationGroup::FiveMinutes:
        tm->tm_min = 0;

    case DurationGroup::Minutes:
    case DurationGroup::HalfMinutes:
    case DurationGroup::QuarterMinutes:
    case DurationGroup::FiveSeconds:
        tm->tm_sec = 0;

    default:
        break;
    }

//    return std::max(ZERO_TIME, std::mktime(tm));
    return value;
}

std::time_t time_utils::after(std::time_t value, DurationGroup group)
{
    std::tm* const tm = std::gmtime(&value);

    switch (group)
    {
    case DurationGroup::Days:
    case DurationGroup::HalfDays:
    case DurationGroup::QuarterDays:
    case DurationGroup::TwoHours:
        tm->tm_sec = 0;
        tm->tm_min = 0;
        tm->tm_hour = 0;
        ++tm->tm_mday;
        break;

    case DurationGroup::Hours:
    case DurationGroup::HalfHours:
    case DurationGroup::QuarterHours:
    case DurationGroup::FiveMinutes:
        tm->tm_sec = 0;
        tm->tm_min = 0;
        ++tm->tm_hour;
        break;

    case DurationGroup::Minutes:
    case DurationGroup::HalfMinutes:
    case DurationGroup::QuarterMinutes:
    case DurationGroup::FiveSeconds:
        tm->tm_sec = 0;
        ++tm->tm_min;
        break;

    case DurationGroup::Seconds:
        ++tm->tm_sec;
        break;

    default:
        break;
    }

//    return std::max(ZERO_TIME, std::mktime(tm));
    return value;
}
