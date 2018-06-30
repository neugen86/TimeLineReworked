#include "TimeLine.h"

#include <QFutureWatcher>
#include <QtConcurrent/QtConcurrent>

#include "Cpp/Backend.h"
#include "TimeLineUtils.h"

namespace
{
const int TIMER_INTERVAL = 100;
const double COLLAPSE_DISTANCE = 100;

TimeInterval Adjust(const TimeInterval& interval)
{
    const auto group = time_utils::group(interval.duration);

    const std::time_t from = time_utils::before(interval.from, group);
    const std::time_t to = time_utils::after(interval.till(), group);

    return TimeInterval(from, to - from);
}

} // anonymous namespace

using Watcher = QFutureWatcher<void>;

TimeLine::TimeLine(QObject* parent)
    : QObject(parent)
{
    QObject::connect(&m_contentTimer, &QTimer::timeout, this, &TimeLine::arrangeItems);
    QObject::connect(&m_viewportTimer, &QTimer::timeout, this, &TimeLine::updateVisibleItems);
}

void TimeLine::setup(const TimeInterval& interval, NamedIntervalsProviderPtr provider)
{
    resetVisibleItems();

    m_synchronizer.waitForFinished();
    m_synchronizer.clearFutures();

    m_data.interval = Adjust(interval);
    emit intervalChanged();

    loadItems(provider);
}

void TimeLine::setContentWidth(double value)
{
    if (!qFuzzyCompare(m_data.contentWidth, value))
    {
        resetVisibleItems();

        m_data.contentWidth = value;
        emit contentWidthChanged();

        m_contentTimer.start(TIMER_INTERVAL);
    }
}

void TimeLine::setViewportWidth(double value)
{
    if (!qFuzzyCompare(m_data.viewportWidth, value))
    {
        resetVisibleItems();

        m_data.viewportWidth = value;
        emit viewportWidthChanged();

        m_viewportTimer.start(TIMER_INTERVAL);
    }
}

void TimeLine::setViewportOffset(double value)
{
    if (!qFuzzyCompare(m_data.viewportOffset, value))
    {
        resetVisibleItems();

        m_data.viewportOffset = value;
        emit viewportOffsetChanged();

        m_viewportTimer.start(TIMER_INTERVAL);
    }
}

void TimeLine::simulate(unsigned itemsCount)
{
    if (!itemsCount)
        return;

    auto simulator = std::make_shared<backend::Simulator>(itemsCount);

    setup(simulator->interval(), simulator);
}

void TimeLine::setBusy(bool value)
{
    m_busy = value;
    emit busyChanged();
}

void TimeLine::resetVisibleItems()
{
    m_visibleItems = TimeLineItemList();
    emit visibleItemsChanged();
}

void TimeLine::loadItems(NamedIntervalsProviderPtr provider)
{
    if (m_busy)
        return;

    setBusy(true);

    auto watcher = new Watcher();
    QObject::connect(watcher, &Watcher::finished, this, [this, watcher]
    {
        watcher->deleteLater();

        setBusy(false);
        arrangeItems();
    });

    auto routine = [this](NamedIntervalsProviderPtr provider)
    {
        m_items = provider->getData();
        timeline_utils::sort_and_filter(m_data.interval, m_items);
    };

    auto future = QtConcurrent::run(routine, provider);

    m_synchronizer.addFuture(future);
    watcher->setFuture(future);
}

void TimeLine::arrangeItems()
{
    if (m_busy)
        return;

    m_contentTimer.stop();

    auto watcher = new Watcher();
    QObject::connect(watcher, &Watcher::finished, this, [this, watcher]
    {
        watcher->deleteLater();
        updateVisibleItems();
    });

    auto routine = [this](std::time_t treshold)
    {
        m_indexes = timeline_utils::group_items(m_items, treshold);
    };

    const std::time_t treshold = m_data.fromDistance(COLLAPSE_DISTANCE, false);
    auto future = QtConcurrent::run(routine, treshold);

    m_synchronizer.addFuture(future);
    watcher->setFuture(future);
}

void TimeLine::updateVisibleItems()
{
    if (m_busy)
        return;

    m_viewportTimer.stop();

    auto watcher = new Watcher();
    QObject::connect(watcher, &Watcher::finished, this, [this, watcher]
    {
        watcher->deleteLater();

        m_synchronizer.clearFutures();
        emit visibleItemsChanged();
    });

    auto routine = [this]()
    {
        m_visibleItems = std::move(timeline_utils::get_visible(m_data, m_items, m_indexes));
    };

    auto future = QtConcurrent::run(routine);

    m_synchronizer.addFuture(future);
    watcher->setFuture(future);
}
