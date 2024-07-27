#include "chartview.h"
#include <QtGui/QMouseEvent>
#include <QtCore/QDebug>
#include <QtCharts/QAbstractAxis>
#include <QtCharts/QValueAxis>
#include <QtCharts/QScatterSeries>

ChartView::ChartView(QWidget *parent)
    : QChartView(parent){}

void ChartView::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Plus:
        chart()->zoomIn();
        break;
    case Qt::Key_Minus:
        chart()->zoomOut();
        break;
    }

    QValueAxis *angularAxis = qobject_cast<QValueAxis*>(chart()->axisX());
    if (angularAxis) {
        angularAxis->setRange(0, 360);
    }
    QValueAxis *radialAxis = qobject_cast<QValueAxis*>(chart()->axisY());
    if (radialAxis) {
        qreal currentMax = radialAxis->max();
        qreal delta = currentMax;

        radialAxis->setRange(0, delta);
    }
}
