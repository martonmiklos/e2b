#include "eaglercloader.h"
#include <QDebug>



EagleRCLoader::EagleRCLoader(QObject *parent) :
    QObject(parent)
{
}

bool EagleRCLoader::load(const QString &path)
{
    QSettings eagleRC(path, QSettings::IniFormat);
    int paletteIndex = 0;
    while (1) {
        if (eagleRC.value(QString("Palette.%1.0").arg(paletteIndex), "").toString().isEmpty())
            break;

        Palette *palette = new Palette(paletteIndex);
        palette->parse(&eagleRC);
        m_palettes.append(palette);
        paletteIndex++;
    }

    if (paletteIndex == 0) {
        m_error = tr("Unable to load palettes");
        return false;
    }

    m_schPaletteIndex = eagleRC.value("Sch.Palette", "0").toInt();
    m_brdPaletteIndex = eagleRC.value("Brd.Palette", "0").toInt();

    if (m_schPaletteIndex >= m_palettes.size()) {
        m_error = tr("Sch.Palette index does not points to a valid palette");
        return false;
    }

    if (m_brdPaletteIndex >= m_palettes.size()) {
        m_error = tr("Brd.Palette index does not points to a valid palette");
        return false;
    }
    return true;
}

Palette *EagleRCLoader::schPalette()
{
    if (m_schPaletteIndex < m_palettes.size())
        return m_palettes.at(m_schPaletteIndex);
    return NULL;
}

Palette *EagleRCLoader::brdPalette()
{
    if (m_brdPaletteIndex < m_palettes.size())
        return m_palettes.at(m_brdPaletteIndex);
    return NULL;
}
QString EagleRCLoader::error() const
{
    return m_error;
}


void EagleRCLoader::setLayers(const Layers &layers)
{
    m_layers = layers;
}


void Palette::parse(const QSettings *settings)
{
    int layerIndex = 0;
    while (1) {
        QString colorString = settings->value(QString("Palette.%1.%2").arg(m_paletteIndex).arg(layerIndex), "").toString();
        if (colorString.length() == 10) {
            QColor color;
            color.setNamedColor("#" + colorString.mid(2, colorString.length() - 2));
            m_colors.insert(layerIndex, color);
        } else {
            break;
        }
        layerIndex++;
    }
}

QColor Palette::layerColor(int layerIndex)
{
    foreach (Layer layer, m_layers.layerList()) {
        if (layer.number() == layerIndex) {
            return m_colors.value(layer.color());
        }
    }

    return m_colors.value(layerIndex);
}

Layers Palette::layers() const
{
    return m_layers;
}

void Palette::setLayers(const Layers &layers)
{
    m_layers = layers;
}

