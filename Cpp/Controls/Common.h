#pragma once

#include <QList>
#include <QColor>

#include "TimeUtils.h"

const double MIN_ITEM_WIDTH = 3;

const QColor SINGLE_ITEM_COLOR(Qt::cyan);
const QColor GROUP_ITEM_COLOR(Qt::green);

template <typename T>
class IDataProvider
{
public:
    virtual ~IDataProvider() {}
    virtual T getData() = 0;

};

struct TimeInterval
{
    std::time_t from = 0;
    std::time_t duration = 0;

    TimeInterval() = default;

    TimeInterval(std::time_t from, std::time_t duration)
        : from(std::max(std::time_t(0), from))
        , duration(std::max(std::time_t(500),
                            std::min(std::time_t(3153600000),
                                     duration)))
    {}

///Note: disabled until no dynamic types
//    virtual ~TimeInterval() {}

    bool contains(const TimeInterval& other) const
    {
        return (other.duration > 0)
                && (other.from >= from)
                && (other.from < till());
    }

    std::time_t till() const { return (from + duration); }

};

struct NamedInterval : public TimeInterval
{
    QString title;

    NamedInterval(const QString& title, std::time_t from, std::time_t duration)
        : TimeInterval(from, duration)
        , title(title)
    {}

};
using NamedIntervalList = QList<NamedInterval>;

struct TimeLineItem
{
    QRectF rect;
    QColor color;
    QString title;
    QString description;

    TimeLineItem() = default;

    TimeLineItem(const QString& title, const QRectF& rect, const QColor& color)
        : rect(rect)
        , color(color)
        , title(title)
        , description(title)
    {
        this->rect.setWidth(std::max(rect.width(), MIN_ITEM_WIDTH));
    }

    TimeLineItem(const QString& title, const QString& description, const QRectF& rect, const QColor& color)
        : TimeLineItem(title, rect, color)
    {
        this->description = description;
    }

    static TimeLineItem single(const QString& title, const QRectF& rect)
    {
        return TimeLineItem(title, rect, SINGLE_ITEM_COLOR);
    }

    static TimeLineItem group(int count, const QString& description, const QRectF& rect)
    {
        return TimeLineItem(QString::number(count), description, rect, GROUP_ITEM_COLOR);
    }

};
using TimeLineItemList = QList<TimeLineItem>;

inline bool operator <(const TimeInterval& lhv, const TimeInterval& rhv)
{
    return lhv.from < rhv.from;
}
