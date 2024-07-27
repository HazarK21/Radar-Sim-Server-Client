#ifndef RADARPPISCOPE_H
#define RADARPPISCOPE_H

#include "chartview.h"
#include "plottermodel.h"
#include <QTimer>
#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QBrush>
#include <QPolygon>

class RadarPPIScope : public QWidget
{
    Q_OBJECT

public:
    RadarPPIScope(QWidget *parent= nullptr);

    ChartView* getChartView() const;
    double distance;
    double degree;
    QGraphicsLineItem *stick = new QGraphicsLineItem();
    double stickAngle;

public slots:
    void getNewPlot(double distance, double degree);

private:
    ChartView* chartView;
    QTimer timer;
    Plottermodel *model;
};
#endif // RADARPPISCOPE_H
