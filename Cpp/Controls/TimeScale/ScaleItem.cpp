#include "ScaleItem.h"

#include <QPainter>

ScaleItem::ScaleItem(QQuickItem* parent)
    : QQuickPaintedItem(parent)
{
}

void ScaleItem::setData(const QImage& value)
{
    m_data = value;
    emit dataChanged();

    setSize(value.size());
}

void ScaleItem::paint(QPainter* painter)
{
    painter->drawImage(QPoint(), m_data);
}
