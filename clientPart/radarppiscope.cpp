#include "radarppiscope.h"
#include "plottermodel.h"
#include <QtCharts/QScatterSeries>
#include <QtCharts/QPolarChart>
#include <QtCharts/QValueAxis>
#include <QChartView>
#include <QVXYModelMapper>
#include <QAbstractTableModel>
#include <QTableView>
#include <QGraphicsLineItem>
#include <QGraphicsScene>
#include <QPropertyAnimation>
#include <QGraphicsTransform>

RadarPPIScope::RadarPPIScope(QWidget  *parent) : QWidget (parent) {
    const qreal angularMin = 0;
    const qreal angularMax = 360;
    const qreal radialMin = 0;
    const qreal radialMax = 10000;
    model = new Plottermodel();

    auto tableView  = new QTableView;
    tableView->setModel(model);
    QPolarChart *chart = new QPolarChart();
    model = new Plottermodel();
    chart->setAnimationOptions(QChart::NoAnimation);

    QScatterSeries *series1 = new QScatterSeries();
    series1->setName("scatter");

    auto *mapper = new QVXYModelMapper();
    mapper->setModel(model);
    mapper->setXColumn(0);
    mapper->setYColumn(1);

    chart->addSeries(series1);
    mapper->setSeries(series1);
    chart->setTitle("Use +/- to zoom.");

    QValueAxis *angularAxis = new QValueAxis();
    angularAxis->setTickCount(9);
    angularAxis->setLabelFormat("%.1f");
    angularAxis->setShadesVisible(true);
    angularAxis->setShadesBrush(QBrush(QColor(249, 249, 255)));
    chart->addAxis(angularAxis, QPolarChart::PolarOrientationAngular);

    QValueAxis *radialAxis = new QValueAxis();
    radialAxis->setTickCount(9);
    radialAxis->setLabelFormat("%d");
    chart->addAxis(radialAxis, QPolarChart::PolarOrientationRadial);

    series1->attachAxis(radialAxis);
    series1->attachAxis(angularAxis);

    radialAxis->setRange(radialMin, radialMax);
    angularAxis->setRange(angularMin, angularMax);

    chartView = new ChartView();
    chartView->setChart(chart);
    chartView->setFixedSize(800,800);
    chartView->setRenderHint(QPainter::Antialiasing);

    stick->setLine(0, 0, 320, 0);
    stick->setPos(400, 430);
    QPen pen(Qt::black);
    pen.setWidth(5);
    stick->setPen(pen);
    chart->scene()->addItem(stick);
    QGraphicsRotation *rotation = new QGraphicsRotation();
    stick->setRotation(stickAngle-90);

    QPropertyAnimation *animation = new QPropertyAnimation(rotation, "angle");
    animation->setDuration(1000);
    animation->setStartValue(0);
    animation->setEndValue(360);
    animation->setLoopCount(-1);

    animation->start();

}
void RadarPPIScope::getNewPlot(double distance, double degree){
    model->addPlotItem(degree, distance);
}

ChartView* RadarPPIScope::getChartView() const {
    return chartView;
}
