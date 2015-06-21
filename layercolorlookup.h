#ifndef LAYERCOLORLOOKUP_H
#define LAYERCOLORLOOKUP_H

#include "eagle.h"

#include <QColor>

class LayerColorLookup
{
public:
    LayerColorLookup();
    QColor layerColor(int layer);
    void setLayers(const Layers & layers);

private:
    Layers m_layers;
};

#endif // LAYERCOLORLOOKUP_H
