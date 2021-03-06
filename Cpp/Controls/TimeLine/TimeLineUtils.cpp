#include "TimeLineUtils.h"

#include <algorithm>

namespace
{
const int MAX_GROUP_ITEMS = 5;

TimeLineItem::Segment CreateSegment(const TimeLineInfo& timeline, const TimeInterval& interval)
{
    TimeLineItem::Segment segment;

    segment.start = (timeline.fromTime(interval.from) - timeline.viewportOffset);
    segment.width = timeline.fromTime((interval.till() - interval.from), false);

    if (segment.start < 0)
    {
        segment.width += segment.start;
        segment.start = 0;
    }

    if ((segment.start + segment.width) > timeline.viewportWidth)
        segment.width = (timeline.viewportWidth - segment.start);

    return segment;
}

TimeLineItem CreateItem(const TimeLineInfo& timeline, const NamedInterval& interval)
{
    return TimeLineItem::single(interval.title, CreateSegment(timeline, interval));
}

TimeLineItem CreateItem(const TimeLineInfo& timeline, const NamedIntervalList& items,
                        const timeline_utils::IndexGroupBounds& bounds)
{
    const int count = (bounds.second - bounds.first + 1);

    QStringList description;
    {
        const int visibleCount = std::min(count, MAX_GROUP_ITEMS);

        for (int index = 0; index < visibleCount; ++index)
        {
            description.append(items[(bounds.first + index)].title);
        }

        if (count > visibleCount)
            description.append(QString("\n+%1 more").arg(count - visibleCount));
    }

    const std::time_t from = items[bounds.first].from;
    const std::time_t till = items[bounds.second].till();

    const TimeInterval interval(from, (till - from));

    return TimeLineItem::group(count, description.join("\n"), CreateSegment(timeline, interval));
}

} // anonymous namespace

std::time_t TimeLineInfo::fromDistance(double distance, bool relative) const
{
    const std::time_t add = (relative ? interval.from : 0);
    return (add + (distance * interval.duration/ contentWidth));
}

double TimeLineInfo::fromTime(std::time_t time, bool relative) const
{
    const std::time_t add = (relative ? interval.from : 0);
    return (contentWidth * (time - add) / interval.duration);
}

void timeline_utils::sort_and_filter(const TimeInterval& interval, NamedIntervalList& items)
{
    auto it = std::remove_if(items.begin(), items.end(), [&interval](auto& item)
    {
        return !interval.contains(item);
    });
    items.erase(it, items.end());

    std::sort(items.begin(), items.end());

    if (const int size = items.size())
    {
        TimeInterval& item = items[size-1];

        if (item.till() > interval.till())
            item.duration = (item.till() - interval.till());
    }
}

timeline_utils::GrouppedIndexes timeline_utils::group_items(const NamedIntervalList& items,
                                                            std::time_t treshold)
{
    if (items.empty())
        return GrouppedIndexes();

    GrouppedIndexes result;

    const int size = items.size();

    int groupStartIndex = 0;
    std::time_t groupStartFrom = items[0].from;

    for (int index = 0; index < size; ++index)
    {
        const std::time_t cur = items[index].from;

        if ((cur - groupStartFrom) > treshold)
        {
            result.emplace_back(std::make_pair(groupStartIndex, index - 1));
            groupStartIndex = index;
            groupStartFrom = cur;
        }
    }

    result.emplace_back(std::make_pair(groupStartIndex, size - 1));

    return result;
}

TimeLineItemList timeline_utils::get_visible_items(const TimeLineInfo& timeline,
                                                   const NamedIntervalList& items,
                                                   const GrouppedIndexes& indexes)
{
    if (items.empty() || indexes.empty())
        return TimeLineItemList();

    TimeLineItemList result;

    IndexGroupBounds lastOverlapped = std::make_pair(-1, -1);

    const std::time_t viewportFrom = timeline.fromDistance(timeline.viewportOffset);
    const std::time_t viewportTill = timeline.fromDistance(timeline.viewportOffset + timeline.viewportWidth);

    for (auto& bounds : indexes)
    {
        const NamedInterval& first = items[bounds.first];

        if (first.from >= viewportTill)
            break;

        const NamedInterval& last = items[bounds.second];

        if (last.till() <= viewportFrom)
            continue;

        if ((first.from <= viewportFrom) && (last.till() >= viewportTill))
        {
            lastOverlapped = bounds;
            continue;
        }

        result.push_back((bounds.first != bounds.second)
                         ? CreateItem(timeline, items, bounds)
                         : CreateItem(timeline, first));
    }

    if (lastOverlapped.first >= 0)
    {
        result.push_front((lastOverlapped.first != lastOverlapped.second)
                          ? CreateItem(timeline, items, lastOverlapped)
                          : CreateItem(timeline, items[lastOverlapped.first]));
    }

    return result;
}
