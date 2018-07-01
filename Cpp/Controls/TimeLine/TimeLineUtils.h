#pragma once

#include <vector>

#include "Cpp/Controls/Common.h"

struct TimeLineInfo
{
    TimeInterval interval;

    double contentWidth = 0;
    double viewportWidth = 0;
    double viewportOffset = 0;

    TimeLineInfo() = default;

    std::time_t fromDistance(double distance, bool relative = true) const;
    double fromTime(std::time_t time, bool relative = true) const;

};

namespace timeline_utils
{
using IndexGroupBounds = std::pair<int, int>;
using GrouppedIndexes = std::vector<IndexGroupBounds>;

void sort_and_filter(const TimeInterval& interval, NamedIntervalList& items);

GrouppedIndexes group_items(const NamedIntervalList& items, std::time_t treshold);

TimeLineItemList get_visible_items(const TimeLineInfo& timeline,
                                   const NamedIntervalList& items,
                                   const GrouppedIndexes& groups);

} // timeline_utils
