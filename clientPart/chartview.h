#ifndef CHARTVIEW_H
#define CHARTVIEW_H

#include <QtCharts/QChartView>
#include <QtCharts/QPolarChart>
#include <QScatterSeries>

class ChartView : public QChartView
{
public:
    ChartView(QWidget *parent=nullptr);
    ChartView(const ChartView &) = default;
    ChartView(ChartView &&) = default;
    ChartView &operator=(const ChartView &) = default;
    ChartView &operator=(ChartView &&) = default;
public slots:

protected:
    void keyPressEvent(QKeyEvent *event);

private:
    void switchChartType();
};
#endif
