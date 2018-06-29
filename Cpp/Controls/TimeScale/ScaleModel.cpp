#include "ScaleModel.h"

namespace
{
const double ZOOM_RATIO = 3./2;

} // anonymous namespace

ScaleModel::ScaleModel(QObject* parent)
    : QAbstractListModel(parent)
{
    setInterval(TimeInterval(0, time_utils::duration_ms(time_utils::one_day)));
}

double ScaleModel::width() const
{
    return (m_factory.size.width() * m_factory.count);
}

double ScaleModel::height() const
{
    return m_factory.size.height();
}

void ScaleModel::setMinWidth(double value)
{
    m_minWidth = std::max(0., value);
    emit minWidthChanged();

    if (m_minWidth > width())
        fillWidth(m_minWidth);
}

void ScaleModel::setInterval(const TimeInterval& interval)
{
    beginResetModel();

    m_interval = interval;
    fillWidth(m_minWidth);

    endResetModel();
}

void ScaleModel::zoomIn()
{
    fillWidth(width() * ZOOM_RATIO);
}

void ScaleModel::zoomOut()
{
    fillWidth(width() / ZOOM_RATIO);
}

void ScaleModel::resetZoom()
{
    fillWidth(m_minWidth);
}

QVariant ScaleModel::data(const QModelIndex& index, int role) const
{
    switch (role)
    {
    case ImageRole:
        return m_factory.tile(index.row());

    default:
        return QVariant();
    }
}

int ScaleModel::rowCount(const QModelIndex&) const
{
    return m_factory.count;
}

QHash<int, QByteArray> ScaleModel::roleNames() const
{
    return { { ImageRole, "image" } };
}

void ScaleModel::fillWidth(double value)
{
//    beginResetModel();

//    m_tileInfo = ScaleTileInfo::create(value, m_interval);

//    if (width() < m_minWidth)
//        m_tileInfo = ScaleTileInfo::create(m_minWidth, m_interval);

//    endResetModel();

    const double prevWidth = width();
    const double prevCount = m_factory.count;

    m_factory = ScaleTilesFactory::create(value, m_interval);
    double curWidth = width();

    if (curWidth < m_minWidth)
    {
        m_factory = ScaleTilesFactory::create(m_minWidth, m_interval);
        curWidth = m_minWidth;
    }

    const QModelIndex& topIndex = index(0);

    if (prevWidth < curWidth)
    {
        emit dataChanged(topIndex, index(prevCount - 1));

        beginInsertRows(QModelIndex(), prevCount, (m_factory.count - 1));
        endInsertRows();
    }
    else if (prevWidth > curWidth)
    {
        emit dataChanged(topIndex, index(prevCount - 1));

        beginRemoveRows(QModelIndex(), m_factory.count, (prevCount - 1));
        endRemoveRows();
    }

    emit sizeChanged();
}
