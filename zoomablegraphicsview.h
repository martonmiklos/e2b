#ifndef ZOOMABLEGRAPHICSVIEW_H
#define ZOOMABLEGRAPHICSVIEW_H

#include <QGraphicsView>

class ZoomableGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit ZoomableGraphicsView(QWidget *parent = 0);
    void wheelEvent(QWheelEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

signals:
    void mousePositionChanged(QPointF position);

private:
    void gentleZoom(double factor);
    double zoomFactorBase;
    QPointF targetScenePos, targetViewportPos;
};

#endif // ZOOMABLEGRAPHICSVIEW_H
