#pragma once

#include <QAbstractListModel>

#include "Cpp/Controls/Common.h"

class TimeLineItemModel : public QAbstractListModel
{
    Q_OBJECT

    enum
    {
        TitleRole = Qt::UserRole + 1,
        DescriptionRole,
        ColorRole,
        StartRole,
        WidthRole,
    };

    TimeLineItemList m_items;

    Q_PROPERTY(TimeLineItemList items READ items WRITE setItems NOTIFY itemsChanged)

public:
    TimeLineItemModel(QObject* parent = nullptr);

public:
    const TimeLineItemList& items() const { return m_items; }
    void setItems(const TimeLineItemList& items);

protected: // QAbstractItemModel
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QHash<int, QByteArray> roleNames() const override;

signals:
    void itemsChanged();

};
