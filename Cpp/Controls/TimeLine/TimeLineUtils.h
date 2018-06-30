#pragma once

#include <vector>

#include "Cpp/Controls/Common.h"

namespace timeline_utils
{
using IndexGroupBounds = std::pair<int, int>;
using GrouppedIndexes = std::vector<IndexGroupBounds>;

GrouppedIndexes group_items(const NamedIntervalList& values, std::time_t gap);

} // timeline_utils
