#include "layercolorlookup.h"

LayerColorLookup::LayerColorLookup()
{
}

QColor LayerColorLookup::layerColor(int layer)
{
    /*foreach (Layer layer, m_layers) {
        if (layer.number() == layer) {
            return QColor();
        }
    }*/
}

void LayerColorLookup::setLayers(const Layers &layers)
{
    m_layers = layers;
}

