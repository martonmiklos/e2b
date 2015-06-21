#include "zoomablegraphicsview.h"

#include <QWheelEvent>
#include <qmath.h>
#include <QDebug>
#include <QTimeLine>

ZoomableGraphicsView::ZoomableGraphicsView(QWidget *parent) :
    QGraphicsView(parent)
{
    zoomFactorBase = 1.0005;
    targetViewportPos = QPointF(0, 0);
}

void ZoomableGraphicsView::wheelEvent(QWheelEvent *event)
{
    if (event->orientation() == Qt::Vertical) {
        double angle = event->delta();
        double factor = qPow(zoomFactorBase, angle);
        gentleZoom(factor);
    }
}

void ZoomableGraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    targetViewportPos = event->pos();
    targetScenePos = mapToScene(event->pos());

    emit mousePositionChanged(mapToScene(event->pos()));

    /*qWarning() << ((((double)event->pos().x() / (double)this->width()) * 2) - 1) <<
                  ((((double)event->pos().y() / (double)this->height()) * 2) - 1);
                  */
}

void ZoomableGraphicsView::gentleZoom(double factor) {
    scale(factor, factor);
    centerOn(targetScenePos);
    /*QPointF delta_viewport_pos = targetViewportPos - QPointF(viewport()->width() / 2.0,
                                                             viewport()->height() / 2.0);

    QPointF viewport_center = mapFromScene(targetScenePos) - delta_viewport_pos;
    centerOn(mapToScene(viewport_center.toPoint()));*/
}


