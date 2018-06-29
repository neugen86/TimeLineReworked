#pragma once

#include <QImage>

#include "Cpp/Controls/Common.h"
#include "Cpp/Controls/TimeUtils.h"

struct ScaleTilesFactory
{
    QSize size;
    std::time_t from = 0;
    std::time_t step = 0;
    unsigned spanCount = 1;
    time_utils::DurationGroupSize count = 0;
    time_utils::DurationGroup group = time_utils::DurationGroup::Days;

    ScaleTilesFactory() = default;

    ScaleTilesFactory(double width, const TimeInterval& interval, time_utils::DurationGroup group);

    static ScaleTilesFactory create(double width, const TimeInterval& interval);

    QImage tile(int index) const;

};
