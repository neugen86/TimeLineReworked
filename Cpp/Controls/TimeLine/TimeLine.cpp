#include "TimeLine.h"

#include <QFutureWatcher>
#include <QtConcurrent/QtConcurrent>

#include "Cpp/Backend.h"
#include "TimeLineUtils.h"

namespace
{
const int TIMER_INTERVAL = 200;

} // anonymous namespace

using Watcher = QFutureWatcher<void>;

TimeLine::TimeLine(QObject* parent)
    : QObject(parent)
{
    QObject::connect(&m_resizeTimer, &QTimer::timeout, this, &TimeLine::arrangeItems);
    QObject::connect(&m_offsetTimer, &QTimer::timeout, this, &TimeLine::updateVisibleItems);
}

void TimeLine::setup(const TimeInterval& interval, NamedIntervalsProviderPtr provider)
{
    m_synchronizer.waitForFinished();

    m_interval = interval;
    emit intervalChanged();

    loadItems(provider);
}

void TimeLine::setContentWidth(int value)
{
    m_contentWidth = value;
    m_resizeTimer.start(TIMER_INTERVAL);
}

void TimeLine::setViewportWidth(int value)
{
    m_viewportWidth = value;
    m_resizeTimer.start(TIMER_INTERVAL);
}

void TimeLine::setViewportOffset(int value)
{
    m_viewportOffset = value;
    m_offsetTimer.start(TIMER_INTERVAL);
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

void TimeLine::loadItems(NamedIntervalsProviderPtr provider)
{
    if (m_busy)
        return;

    setBusy(true);
    m_ready = false;

    auto watcher = new Watcher();
    QObject::connect(watcher, &Watcher::finished, this, [this, watcher]
    {
        watcher->deleteLater();

        m_ready = true;

        setBusy(false);
        arrangeItems();
    });

    auto func = [this](NamedIntervalsProviderPtr provider)
    {
        m_items = provider->getData();
    };

    auto future = QtConcurrent::run(func, provider);

    m_synchronizer.addFuture(future);
    watcher->setFuture(future);
}

void TimeLine::arrangeItems()
{
    if (!m_ready || m_busy)
        return;

    setBusy(true);

    auto watcher = new Watcher();
    QObject::connect(watcher, &Watcher::finished, this, [this, watcher]
    {
        watcher->deleteLater();

        setBusy(false);
        updateVisibleItems();
    });

    auto func = [this](std::time_t gap)
    {
        m_grouppedIndexes = timeline_utils::group_items(m_items, gap);
    };

    std::time_t gap = 1.;
    auto future = QtConcurrent::run(func, gap);

    m_synchronizer.addFuture(future);
    watcher->setFuture(future);
}

void TimeLine::updateVisibleItems()
{
    if (!m_ready || m_busy)
        return;

    setBusy(true);

    auto watcher = new Watcher();
    QObject::connect(watcher, &Watcher::finished, this, [this, watcher]
    {
        watcher->deleteLater();

        setBusy(false);
    });

    auto func = [this]()
    {
        m_visibleItems.clear();
    };

    auto future = QtConcurrent::run(func);

    m_synchronizer.addFuture(future);
    watcher->setFuture(future);
}
