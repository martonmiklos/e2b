#ifndef GRAPHICSITEMS_H
#define GRAPHICSITEMS_H

#include <QGraphicsItem>
#include <QPolygonF>
#include <QPen>

#include "eagle.h"
#include "eaglercloader.h"

class PlainItem : public QGraphicsItem
{
public:
    PlainItem(Plain plain, Palette &palette);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;
private:
    QPolygonF m_polygon;
    Plain m_plain;
    Palette & m_palette;
    QRectF m_boundingRect;
};

class WireItem : public QGraphicsLineItem
{
public:
    WireItem(Wire wire, Palette &palette);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
private:
    static Qt::PenCapStyle EAGLECapStyle2QtPenCapStyle(Wire::CapEnum capStyle);
};

class ViaItem : public QGraphicsItem
{
public:
    ViaItem(const Via &via, Palette &palette);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;

private:
    Via m_via;
    Palette m_palette;
};

class SignalItem
{
public:
    SignalItem(Signal signal, Palette & palette);
    void addToScene(QGraphicsScene *scene);

private:
    Signal m_signal;
    Palette & m_palette;
    QList<WireItem*> m_wires;
    QList<ViaItem*> m_vias;
};

class ElementItem : public QGraphicsItem
{
public:
    ElementItem(Element *part, Palette *palette);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};

class BoardItem
{
public:
    BoardItem(Board brd, Palette & palette);
    void addToScene(QGraphicsScene *scene);

    PlainItem *plainItem() const;

private:
    PlainItem *m_plainItem;
    QList <SignalItem *> m_signalItems;
    QList <ElementItem *> m_elementItems;
};



#endif // GRAPHICSITEMS_H
