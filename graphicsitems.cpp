#include "graphicsitems.h"
#include <QPen>
#include <QPainter>
#include <QGraphicsScene>
#include <QPolygonF>
#include <QDebug>
#include <QPainterPath>
#include <math.h>

#define MM2PX(a) (a) * 32000

PlainItem::PlainItem(Plain plain, Palette & palette) :
    m_palette(palette)
{
    m_plain = plain;

    QPolygonF brectPolygon;
    foreach (Wire w, m_plain.wireList()) {
        brectPolygon.append(QPoint(MM2PX(w.x1()), MM2PX(w.y1())));
        brectPolygon.append(QPoint(MM2PX(w.x2()), MM2PX(w.y2())));
    }

    foreach (Hole h, m_plain.holeList()) {
        brectPolygon.append(QPointF(MM2PX(h.x() - h.drill()/2), MM2PX(h.y() - h.drill()/2)));
        brectPolygon.append(QPointF(MM2PX(h.x() + h.drill()/2), MM2PX(h.y() + h.drill()/2)));
    }
    m_boundingRect = brectPolygon.boundingRect();
}

void PlainItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    foreach (Wire w, m_plain.wireList()) {
        QPen pen;
        //pen.setCapStyle(w.cap());
        pen.setBrush(QBrush(m_palette.layerColor(w.layer())));
        pen.setWidth(MM2PX(w.width()));
        painter->setPen(pen);
        painter->drawLine(MM2PX(w.x1()), MM2PX(w.y1()), MM2PX(w.x2()), MM2PX(w.y2()));
    }

    foreach (Hole h, m_plain.holeList()) {
        QPen pen;
        //pen.setCapStyle(w.cap());
        pen.setBrush(QBrush(m_palette.layerColor(45))); // FIXME hardcoded layer
        pen.setWidth(0); // cosmetic width
        painter->setPen(pen);
        painter->drawEllipse(QPointF(MM2PX(h.x()), MM2PX(h.y())), MM2PX(h.drill()/2), MM2PX(h.drill()/2));

        // draw the "EAGLE" style rectangle around the hole
        QPolygonF polygon;
        polygon.append(QPointF(MM2PX(h.x()), MM2PX(h.y())));
        polygon.append(QPointF(MM2PX(h.x()), MM2PX(h.y() + h.drill() * 1.2)));
        polygon.append(QPointF(MM2PX(h.x()  - h.drill() * 1.2), MM2PX(h.y())));
        polygon.append(QPointF(MM2PX(h.x()), MM2PX(h.y() - h.drill() * 1.2)));
        polygon.append(QPointF(MM2PX(h.x()  + h.drill() * 1.2), MM2PX(h.y())));
        polygon.append(QPointF(MM2PX(h.x()), MM2PX(h.y() + h.drill() * 1.2)));
        pen.setWidth(1);
        painter->drawPolyline(polygon);
    }
}

QRectF PlainItem::boundingRect() const
{
    return m_boundingRect;
}

BoardItem::BoardItem(Board brd, Palette &palette)
{
    m_plainItem = new PlainItem(brd.plain(), palette);
    foreach (Signal s, brd.signalS().signalList()) {
        SignalItem *si = new SignalItem(s, palette);
        m_signalItems.append(si);
    }

    foreach (Element element, brd.elements().elementList()) {
        ElementItem *elementItem = new ElementItem(&element, &palette);
        m_elementItems.append(elementItem);
    }
}

void BoardItem::addToScene(QGraphicsScene *scene)
{
    scene->addItem(m_plainItem);

    foreach (SignalItem *s, m_signalItems) {
        s->addToScene(scene);
    }
}

PlainItem *BoardItem::plainItem() const
{
    return m_plainItem;
}

SignalItem::SignalItem(Signal signal, Palette &palette) :
    m_palette(palette)
{
    foreach (Wire wire, signal.wireList()) {
        WireItem *wireItem = new WireItem(wire, palette);
        m_wires.append(wireItem);
    }

    foreach (Via via, signal.viaList()) {
        ViaItem *viaItem = new ViaItem(via, palette);
        m_vias.append(viaItem);
    }
}

void SignalItem::addToScene(QGraphicsScene *scene)
{
    foreach (WireItem *wire, m_wires) {
        scene->addItem(wire);
    }

    foreach (ViaItem *via, m_vias) {
        scene->addItem(via);
    }
}

WireItem::WireItem(Wire wire, Palette &palette)
{
    this->setLine(QLineF(MM2PX(wire.x1()), MM2PX(wire.y1()), MM2PX(wire.x2()), MM2PX(wire.y2())));
    QPen pen;
    pen.setWidth(MM2PX(wire.width()));
    pen.setColor(palette.layerColor(wire.layer()));
    pen.setCapStyle(EAGLECapStyle2QtPenCapStyle(wire.cap()));
    this->setPen(pen);
    this->setZValue(-wire.layer());
}

void WireItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setCompositionMode(QPainter::CompositionMode_SourceOver);
    QGraphicsLineItem::paint(painter, option, widget);
}

Qt::PenCapStyle WireItem::EAGLECapStyle2QtPenCapStyle(Wire::CapEnum capStyle)
{
    // XSD uses round cap as default:
    // <attribute name="cap" use="default" value="round">
    if (capStyle == Wire::Cap_flat)
        return Qt::FlatCap;
    else
        return Qt::RoundCap;
}


ViaItem::ViaItem(const Via &via, Palette &palette) :
    m_via(via),
    m_palette(palette)
{
}

void ViaItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPen pen;
    switch (m_via.shape()) {
    case Via::Shape_round: {
        double width = (m_via.diameter() - m_via.drill())/2;
        pen.setWidth(MM2PX(width));
        pen.setColor(Qt::green);
        painter->setPen(pen);
        painter->drawEllipse(MM2PX(m_via.x() - m_via.drill()+width/2),
                             MM2PX(m_via.y() - m_via.drill()+width/2),
                             MM2PX(m_via.drill() + width),
                             MM2PX(m_via.drill() + width));
    } break;
    case Via::Shape_square: {
        QPainterPath outerSquarePath;
        outerSquarePath.setFillRule(Qt::WindingFill);
        outerSquarePath.addRect(boundingRect());

        QPainterPath drillPath;
        drillPath.addEllipse(MM2PX(m_via.x() - m_via.drill()/2),
                             MM2PX(m_via.y() - m_via.drill()/2),
                             MM2PX(m_via.drill()),
                             MM2PX(m_via.drill()));

        QPainterPath FillPath = outerSquarePath.subtracted(drillPath);
        painter->setRenderHint(QPainter::Antialiasing);
        painter->fillPath(FillPath, Qt::green);
    } break;
    case Via::Shape_octagon: {
        QPolygonF octagon;
        double radius = MM2PX(m_via.diameter()/2);
        double x = MM2PX(m_via.diameter() * sin(0.392699082f));
        double left = MM2PX(m_via.x() - m_via.diameter() /2);
        double bottom = MM2PX(m_via.y() - m_via.diameter() /2);
        double diameter = MM2PX(m_via.diameter());

        octagon << QPointF(left + radius - x/2, bottom);
        octagon << QPointF(left + radius + x/2, bottom);
        octagon << QPointF(left + diameter, bottom + radius - x/2);
        octagon << QPointF(left + diameter, bottom + radius + x/2);
        octagon << QPointF(left + radius + x/2, bottom + diameter);
        octagon << QPointF(left + radius - x/2, bottom + diameter);
        octagon << QPointF(left, bottom + radius + x/2);
        octagon << QPointF(left, bottom + radius - x/2);
        octagon << QPointF(left + radius - x/2, bottom);

        QPainterPath ocatagonPath;
        ocatagonPath.setFillRule(Qt::OddEvenFill);
        ocatagonPath.addPolygon(octagon);
        ocatagonPath.closeSubpath();

        QPainterPath drillPath;
        drillPath.addEllipse(MM2PX(m_via.x() - m_via.drill()/2),
                             MM2PX(m_via.y() - m_via.drill()/2),
                             MM2PX(m_via.drill()),
                             MM2PX(m_via.drill()));

        QPainterPath FillPath = ocatagonPath.subtracted(drillPath);
        painter->setRenderHint(QPainter::Antialiasing);
        painter->fillPath(FillPath, Qt::green);
    } break;
    }
}

QRectF ViaItem::boundingRect() const
{
    return QRectF(MM2PX(m_via.x() - m_via.diameter()/2),
                  MM2PX(m_via.y() - m_via.diameter()/2),
                  MM2PX(m_via.diameter()),
                  MM2PX(m_via.diameter()));
}


ElementItem::ElementItem(Element *part, Palette *palette)
{

}

QRectF ElementItem::boundingRect() const
{

}

void ElementItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

}
