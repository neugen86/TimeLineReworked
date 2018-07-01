#include "TimeScaleModel.h"

namespace
{
const double ZOOM_RATIO = 3./2;

} // anonymous namespace

TimeScaleModel::TimeScaleModel(QObject* parent)
    : QAbstractListModel(parent)
{
    setInterval(TimeInterval(0, time_utils::duration_ms(time_utils::one_day)));
}

double TimeScaleModel::width() const
{
    return (m_factory.size.width() * m_factory.count);
}

double TimeScaleModel::height() const
{
    return m_factory.size.height();
}

void TimeScaleModel::setMinWidth(double value)
{
    m_minWidth = std::max(0., value);
    emit minWidthChanged();

    if (m_minWidth > width())
        fillWidth(m_minWidth);
}

void TimeScaleModel::setInterval(const TimeInterval& interval)
{
    beginResetModel();

    m_interval = interval;
    fillWidth(m_minWidth);

    endResetModel();
}

void TimeScaleModel::zoomIn()
{
    fillWidth(width() * ZOOM_RATIO);
}

void TimeScaleModel::zoomOut()
{
    fillWidth(width() / ZOOM_RATIO);
}

void TimeScaleModel::resetZoom()
{
    fillWidth(0);
}

QVariant TimeScaleModel::data(const QModelIndex& index, int role) const
{
    switch (role)
    {
    case ImageRole:
        return m_factory.tile(index.row());

    default:
        return QVariant();
    }
}

int TimeScaleModel::rowCount(const QModelIndex&) const
{
    return m_factory.count;
}

QHash<int, QByteArray> TimeScaleModel::roleNames() const
{
    return { { ImageRole, "image" } };
}

void TimeScaleModel::fillWidth(double value)
{
    beginResetModel();

    m_factory = ScaleTilesFactory::create(value, m_interval);

    if (width() < m_minWidth)
        m_factory = ScaleTilesFactory::create(m_minWidth, m_interval);

    endResetModel();

    emit sizeChanged();
}
