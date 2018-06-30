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
    bool m_ready = false;

    int m_contentWidth = 0;
    int m_viewportWidth = 0;
    int m_viewportOffset = 0;

    QTimer m_resizeTimer;
    QTimer m_offsetTimer;

    TimeInterval m_interval;
    NamedIntervalList m_items;
    TimeLineItemList m_visibleItems;
    timeline_utils::GrouppedIndexes m_grouppedIndexes;

    QFutureSynchronizer<void> m_synchronizer;

    Q_PROPERTY(bool busy READ busy NOTIFY busyChanged)
    Q_PROPERTY(TimeInterval interval READ interval NOTIFY intervalChanged)
    Q_PROPERTY(TimeLineItemList visibleItems READ visibleItems NOTIFY visibleItemsChanged)
    Q_PROPERTY(int contentWidth READ contentWidth WRITE setContentWidth NOTIFY contentWidthChanged)
    Q_PROPERTY(int viewportWidth READ viewportWidth WRITE setViewportWidth NOTIFY viewportWidthChanged)
    Q_PROPERTY(int viewportOffset READ viewportOffset WRITE setViewportOffset NOTIFY viewportOffsetChanged)

public:
    TimeLine(QObject* parent = nullptr);

    void setup(const TimeInterval& interval, NamedIntervalsProviderPtr provider);

public:
    bool busy() const { return m_busy; }

    TimeInterval interval() const { return m_interval; }

    TimeLineItemList visibleItems() const { return m_visibleItems; }

    int contentWidth() const { return m_contentWidth; }
    void setContentWidth(int value);

    int viewportWidth() const { return m_viewportWidth; }
    void setViewportWidth(int value);

    int viewportOffset() const { return m_viewportOffset; }
    void setViewportOffset(int value);

public:
    Q_INVOKABLE void simulate(unsigned itemsCount);

private:
    void setBusy(bool value);

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
