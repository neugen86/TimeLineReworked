#include "TimeLineItemModel.h"

TimeLineItemModel::TimeLineItemModel(QObject* parent)
    : QAbstractListModel(parent)
{
}

void TimeLineItemModel::setItems(const TimeLineItemList& items)
{
    beginResetModel();
    m_items = items;
    endResetModel();

    emit itemsChanged();
}

QVariant TimeLineItemModel::data(const QModelIndex& index, int role) const
{
    const TimeLineItem& item = m_items.at(index.row());

    switch (role)
    {
    case TitleRole:
        return item.title;

    case DescriptionRole:
        return item.description;

    case ColorRole:
        return item.color;

    case StartRole:
        return index.row() * 20;

    case WidthRole:
        return 15;

    default:
        return QVariant();
    }
}

int TimeLineItemModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return m_items.size();
}

QHash<int, QByteArray> TimeLineItemModel::roleNames() const
{
    return {
        { TitleRole, "title" },
        { DescriptionRole, "description" },
        { ColorRole, "color" },
        { StartRole, "start" },
        { WidthRole, "width" },
    };
}
