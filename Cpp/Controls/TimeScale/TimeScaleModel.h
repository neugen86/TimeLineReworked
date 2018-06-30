#pragma once

#include <QAbstractListModel>

#include "ScaleTilesFactory.h"

class TimeScaleModel : public QAbstractListModel
{
    Q_OBJECT

    double m_minWidth = 0;
    TimeInterval m_interval;
    ScaleTilesFactory m_factory;

    enum { ImageRole = Qt::UserRole + 1 };

    Q_PROPERTY(double width READ width NOTIFY sizeChanged)
    Q_PROPERTY(double height READ height NOTIFY sizeChanged)
    Q_PROPERTY(double minWidth READ minWidth WRITE setMinWidth NOTIFY minWidthChanged)

public:
    TimeScaleModel(QObject* parent = nullptr);

public: // properties
    double width() const;
    double height() const;

    double minWidth() const { return m_minWidth; }
    void setMinWidth(double value);

public: // methods
    Q_INVOKABLE void setInterval(const TimeInterval& interval);

    Q_INVOKABLE void zoomIn();
    Q_INVOKABLE void zoomOut();
    Q_INVOKABLE void resetZoom();

protected: // QAbstractItemModel
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QHash<int, QByteArray> roleNames() const override;

private:
    void fillWidth(double value);

signals:
    void sizeChanged();
    void minWidthChanged();

};
