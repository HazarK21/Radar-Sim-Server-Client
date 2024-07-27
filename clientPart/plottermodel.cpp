#include "plottermodel.h"
#include <QApplication>
#include <QtCharts/QVXYModelMapper>
#include <QRandomGenerator64>
#include <QTimer>
#include <QVXYModelMapper>

Plottermodel::Plottermodel(QObject *parent) : QAbstractTableModel(parent)
{
    timer.start(200);
    connect(&timer, &QTimer::timeout, this, &Plottermodel::control);
}

void Plottermodel::addPlotItem(double azimuth, double range)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_data.emplace_back(azimuth, range);
    endInsertRows();
}

int Plottermodel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_data.size();
}

int Plottermodel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 2;
}

QVariant Plottermodel::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_UNUSED(role)
    if (orientation == Qt::Horizontal) {
        if (section == 0)
            return "Azimuth";
        else if (section == 1)
            return "Range";
    }
    return QVariant();
}

QVariant Plottermodel::data(const QModelIndex &index, int role) const
{
    Q_UNUSED(role)
    if (!index.isValid())
        return QVariant();

    if (index.column() == 0)
        return m_data[index.row()].azimuth;
    else if (index.column() == 1)
        return m_data[index.row()].range;
    return QVariant();
}

void Plottermodel::control()
{
    static std::vector<int> deletedPlots;
    deletedPlots.clear();
    static int counter = 0;
    counter = 0;

    for(const auto plot  : m_data)
    {
        if( QDateTime::currentMSecsSinceEpoch() - plot.timeStamp > 3000)
        {
            deletedPlots.push_back(counter);
        }
        ++counter;
    }

    if(deletedPlots.empty())
        return;

    auto min = std::min_element(begin(deletedPlots),end(deletedPlots));
    auto max = std::max_element(begin(deletedPlots),end(deletedPlots));
    beginRemoveRows(QModelIndex(), *min,*max);
    auto f = [](PlotItem &item) {return QDateTime::currentMSecsSinceEpoch() - item.timeStamp > 3000;};
    m_data.erase(std::remove_if(m_data.begin(), m_data.end(), f), m_data.end());
    endRemoveRows();
}
