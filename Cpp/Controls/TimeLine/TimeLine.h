#pragma once

#include <memory>

#include <QTimer>
#include <QFutureSynchronizer>

#include "TimeLineUtils.h"

using NamedIntervalsProvider = IDataProvider<NamedIntervalList>;
using NamedIntervalsProviderPtr = std::shared_ptr<NamedIntervalsProvider>;

class TimeLine : public QObject
{
    Q_OBJECT

    bool m_busy = false;

    TimeLineData m_data;

    QTimer m_contentTimer;
    QTimer m_viewportTimer;

    NamedIntervalList m_items;
    TimeLineItemList m_visibleItems;
    timeline_utils::GrouppedIndexes m_indexes;

    QFutureSynchronizer<void> m_synchronizer;

    Q_PROPERTY(bool busy READ busy NOTIFY busyChanged)
    Q_PROPERTY(TimeInterval interval READ interval NOTIFY intervalChanged)
    Q_PROPERTY(TimeLineItemList visibleItems READ visibleItems NOTIFY visibleItemsChanged)
    Q_PROPERTY(double contentWidth READ contentWidth WRITE setContentWidth NOTIFY contentWidthChanged)
    Q_PROPERTY(double viewportWidth READ viewportWidth WRITE setViewportWidth NOTIFY viewportWidthChanged)
    Q_PROPERTY(double viewportOffset READ viewportOffset WRITE setViewportOffset NOTIFY viewportOffsetChanged)

public:
    TimeLine(QObject* parent = nullptr);

    void setup(const TimeInterval& interval, NamedIntervalsProviderPtr provider);

public:
    bool busy() const { return m_busy; }

    TimeInterval interval() const { return m_data.interval; }

    TimeLineItemList visibleItems() const { return m_visibleItems; }

    double contentWidth() const { return m_data.contentWidth; }
    void setContentWidth(double value);

    double viewportWidth() const { return m_data.viewportWidth; }
    void setViewportWidth(double value);

    double viewportOffset() const { return m_data.viewportOffset; }
    void setViewportOffset(double value);

public:
    Q_INVOKABLE void simulate(unsigned itemsCount);

private:
    void setBusy(bool value);
    void resetVisibleItems();

    void loadItems(NamedIntervalsProviderPtr provider);
    void arrangeItems();
    void updateVisibleItems();

signals:
    void busyChanged();
    void intervalChanged();
    void visibleItemsChanged();
    void contentWidthChanged();
    void viewportWidthChanged();
    void viewportOffsetChanged();

};
