#include "TimeScaleItem.h"

#include <QPainter>

TimeScaleItem::TimeScaleItem(QQuickItem* parent)
    : QQuickPaintedItem(parent)
{
}

void TimeScaleItem::setData(const QImage& value)
{
    m_data = value;
    emit dataChanged();

    setSize(value.size());
}

void TimeScaleItem::paint(QPainter* painter)
{
    painter->drawImage(QPoint(), m_data);
}
