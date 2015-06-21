#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QGraphicsScene>
#include <QLabel>

#include "eaglercloader.h"
#include "graphicsitems.h"
#include "layercolorlookup.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionOpen_triggered();
    void mousePositionChanged(QPointF pos);

private:
    void loadFile(const QString path);

    Ui::MainWindow *ui;
    QSettings m_settings;
    QGraphicsScene *m_scene;
    EagleRCLoader *eagleRC;
    BoardItem *m_boardItem;
    QLabel *positionLabel;
    LayerColorLookup *m_layerColorLUT;
};

#endif // MAINWINDOW_H
