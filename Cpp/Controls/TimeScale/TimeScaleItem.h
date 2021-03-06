#pragma once

#include <QImage>
#include <QQuickPaintedItem>

class TimeScaleItem : public QQuickPaintedItem
{
    Q_OBJECT

    QImage m_data;

    Q_PROPERTY(QImage data READ data WRITE setData NOTIFY dataChanged)

public:
    TimeScaleItem(QQuickItem* parent = nullptr);

public: // properties
    const QImage& data() const { return m_data; }
    void setData(const QImage& value);

protected: // QQuickItem
    void paint(QPainter* painter) override;

signals:
    void dataChanged();

};
