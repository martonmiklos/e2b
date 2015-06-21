#ifndef EAGLERCLOADER_H
#define EAGLERCLOADER_H

#include <QObject>
#include <QSettings>
#include <QColor>
#include <QMultiMap>

#include "eagle.h"

class Palette
{
public:
    Palette(int index) : m_paletteIndex(index) {}
    void parse(const QSettings *settings);
    QColor layerColor(int layerIndex);

    Layers layers() const;
    void setLayers(const Layers &layers);

private:
    int m_paletteIndex;
    QMultiMap<int, QColor> m_colors;
    Layers m_layers;
};

class EagleRCLoader : public QObject
{
    Q_OBJECT
public:
    explicit EagleRCLoader(QObject *parent = 0);
    bool load(const QString & path);
    Palette *schPalette();
    Palette *brdPalette();



    QString error() const;

    void setLayers(const Layers &layers);

private:
    QList<Palette*> m_palettes;
    int m_schPaletteIndex, m_brdPaletteIndex;
    QString m_error;
    Layers m_layers;

signals:

public slots:

};

#endif // EAGLERCLOADER_H
