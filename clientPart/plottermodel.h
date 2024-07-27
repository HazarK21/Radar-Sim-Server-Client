#ifndef PLOTTERMODEL_H
#define PLOTTERMODEL_H

#include <QAbstractTableModel>
#include <QDateTime>
#include <QTimer>
#include <vector>
#include <QVXYModelMapper>
#include <QDateTime>

struct PlotItem {
    PlotItem(double az, double r) : azimuth(az), range(r) {
        timeStamp = QDateTime::currentMSecsSinceEpoch();
    }

    double azimuth;
    double range;
    uint64_t timeStamp;
};

class Plottermodel : public QAbstractTableModel {
    Q_OBJECT
public:
    Plottermodel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void control();
public slots:
    void addPlotItem(double azimuth, double range);

private:
    std::vector<PlotItem> m_data;
    QTimer timer;
};

#endif // PLOTTERMODEL_H
