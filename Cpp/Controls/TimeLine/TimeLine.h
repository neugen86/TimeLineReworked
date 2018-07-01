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

    bool m_loading = false;
    bool m_processing = false;

    double m_contentWidth = 0;
    double m_viewportWidth = 0;
    double m_viewportOffset = 0;

    QTimer m_contentTimer;
    QTimer m_viewportTimer;

    TimeLineInfo m_timeline;

    NamedIntervalList m_items;
    TimeLineItemList m_visibleItems;
    timeline_utils::GrouppedIndexes m_indexes;

    QFutureSynchronizer<void> m_synchronizer;

    Q_PROPERTY(bool loading READ loading NOTIFY loadingChanged)
    Q_PROPERTY(TimeInterval interval READ interval NOTIFY intervalChanged)
    Q_PROPERTY(TimeLineItemList visibleItems READ visibleItems NOTIFY visibleItemsChanged)
    Q_PROPERTY(double contentWidth READ contentWidth WRITE setContentWidth NOTIFY contentWidthChanged)
    Q_PROPERTY(double viewportWidth READ viewportWidth WRITE setViewportWidth NOTIFY viewportWidthChanged)
    Q_PROPERTY(double viewportOffset READ viewportOffset WRITE setViewportOffset NOTIFY viewportOffsetChanged)

public:
    TimeLine(QObject* parent = nullptr);

    void setup(const TimeInterval& interval, NamedIntervalsProviderPtr provider);

public:
    bool loading() const { return m_loading; }

    TimeInterval interval() const { return m_timeline.interval; }

    TimeLineItemList visibleItems() const { return m_visibleItems; }

    double contentWidth() const { return m_contentWidth; }
    void setContentWidth(double value);

    double viewportWidth() const { return m_viewportWidth; }
    void setViewportWidth(double value);

    double viewportOffset() const { return m_viewportOffset; }
    void setViewportOffset(double value);

public:
    Q_INVOKABLE void simulate(unsigned itemsCount);

private:
    void setLoading(bool value);
    void resetVisibleItems();

    void loadItems(NamedIntervalsProviderPtr provider);
    void arrangeItems();
    void updateVisibleItems();

signals:
    void loadingChanged();
    void intervalChanged();
    void visibleItemsChanged();
    void contentWidthChanged();
    void viewportWidthChanged();
    void viewportOffsetChanged();

};
