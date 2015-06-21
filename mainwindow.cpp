#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QDebug>
#include <QElapsedTimer>
#include "eagle.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(QApplication::applicationName() + " - " + QApplication::applicationVersion());

    eagleRC = new EagleRCLoader(this);
    eagleRC->load(QDir::homePath()+QDir::separator() + ".eaglerc"); // fixme multiplatform

    m_scene = new QGraphicsScene(this);

    if (eagleRC->brdPalette() != NULL) {
        ui->graphicsView->setBackgroundBrush(QBrush(eagleRC->brdPalette()->layerColor(0), Qt::SolidPattern));
    }
    ui->graphicsView->setScene(m_scene);
    ui->graphicsView->scale(1, -1);

    positionLabel = new QLabel(this);
    ui->statusBar->addWidget(positionLabel);

    connect(ui->graphicsView, SIGNAL(mousePositionChanged(QPointF)), this, SLOT(mousePositionChanged(QPointF)));

    m_layerColorLUT = new LayerColorLookup();

    loadFile("/home/mm/Projektek/eagle/e2b_test/e2b_test.brd");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpen_triggered()
{
    m_settings.beginGroup("directories");
    QString filePath = QFileDialog::getOpenFileName(
                this,
                tr("Open board file"),
                m_settings.value("lastOpenPath", QDir::homePath()).toString(),
                tr("Board files (*.brd)")
                );
    if (!filePath.isEmpty()) {
        loadFile(filePath);
    }
    m_settings.endGroup();
}

void MainWindow::mousePositionChanged(QPointF pos)
{
    positionLabel->setText(QString("(%1,%2)").arg(pos.x() / 32000).arg(pos.y() / 32000));
}

void MainWindow::loadFile(const QString path)
{
    QFileInfo fi(path);
    m_settings.setValue("lastOpenPath", fi.dir().path());

    Eagle eagle;

    QDomDocument doc("eagle");
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
        return;
    if (!doc.setContent(&file)) {
        file.close();
        return;
    }
    file.close();

    // print out the element names of all elements that are direct children
    // of the outermost element.
    QElapsedTimer timer;
    timer.start();
    bool ok = false;
    eagle = Eagle::parseElement(doc.documentElement(), &ok);
    qDebug() << "The slow operation took" << timer.elapsed() << "milliseconds" << ok;
    if (ok) {
        setWindowTitle(QApplication::applicationName() + " - " + fi.fileName());
    }

    ui->comboBoxVariants->clear();
    foreach (Variantdef vd, eagle.drawing().board().variantdefs().variantdefList()) {
        ui->comboBoxVariants->addItem(vd.name());
    }

    if (eagleRC->brdPalette() != NULL) {
        eagleRC->brdPalette()->setLayers(eagle.drawing().layers());
        ui->graphicsView->setBackgroundBrush(QBrush(eagleRC->brdPalette()->layerColor(0), Qt::SolidPattern));
        m_boardItem = new BoardItem(eagle.drawing().board(), *eagleRC->brdPalette());
        m_boardItem->addToScene(m_scene);
        ui->graphicsView->fitInView(m_boardItem->plainItem(), Qt::KeepAspectRatio);
    }
}
