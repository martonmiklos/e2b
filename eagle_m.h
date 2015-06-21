#ifndef EAGLE_M_H
#define EAGLE_M_H
#include "eagle.h"
#include <QGraphicsScene>

class ElementsItem : public Elements
{
public:
    //void setLibraries(Libraries *libs) {mLibraries = libs;}
    void addToScene(QGraphicsScene *scene);
};

class BoardItem : public Board
{
public:
    void parseElement(const QDomElement &element, bool *ok);
    void addToScene(QGraphicsScene *scene);
};


#endif // EAGLE_M_H
