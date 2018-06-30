#include "Backend.h"

#include <random>

backend::Simulator::Simulator(unsigned count)
    : IDataProvider<NamedIntervalList>()
    , m_count(count)
    , m_interval(0, time_utils::duration_ms(time_utils::one_day))
{
}

NamedIntervalList backend::Simulator::getData()
{
    NamedIntervalList result;
    result.reserve(m_count);

    if (false)
    {
        const std::time_t interval = time_utils::duration_ms(time_utils::half_sec);

        for (int i = 0; i < 1000000; ++i)
        {
            result.push_back(NamedInterval(QString("Bookmark %1").arg(i),
                                           (2 * i + 1) * interval, interval));
        }
    }

//    if (false)
    {
        std::random_device rd;
        std::mt19937 mt(rd());

        const std::time_t durationMin = time_utils::duration_ms(time_utils::one_min);
        const std::time_t durationMax = time_utils::duration_ms(time_utils::one_hour);

        std::uniform_real_distribution<double> startDist(m_interval.from, m_interval.till());
        std::uniform_real_distribution<double> durationDist(durationMin, durationMax);

        for (int i = 0; i < m_count; ++i)
        {
            result.push_back(NamedInterval(QString("Bookmark %1").arg(i), startDist(mt), durationDist(mt)));
        }
    }

    return result;
}
