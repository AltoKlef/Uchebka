#include "mainwindow.h"
#include "huesaturation.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    hueSaturationForm = new huesaturation();
    connect(hueSaturationForm, &huesaturation::saturationChanged, this, &MainWindow::on_valuesChangedSaturation);
    connect(hueSaturationForm, &huesaturation::hueChanged, this, &MainWindow::on_valuesChangedHue);
    connect(hueSaturationForm, &huesaturation::parametersAccepted, this, &MainWindow::on_MonochromeParametersChanged);
    connect(hueSaturationForm, &huesaturation::autoAccepted, this, &MainWindow::on_MonochromeAuto);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_Select_clicked()
{

    QString filename = QFileDialog::getOpenFileName(this, tr("Open File"), "C://", "Image Files (*.png *.jpg *.jpeg)");

    if (!filename.isEmpty()) {
        Custom_View *customView = ui->graphicsView;
        customView->loadImage(filename);
    }
    else{

    }
}


void MainWindow::on_Monochrome_clicked()
{

    if (ui->graphicsView->scene->items().isEmpty()) {
            QMessageBox::warning(this, tr("Error"), tr("No image loaded!"));
            return;
        }
    hueSaturationForm->show();

    if (ui->graphicsView->scene->items().isEmpty()) {
        QMessageBox::warning(this, tr("Error"), tr("No image loaded!"));
        return; // Пропускаем выполнение остальной части функции
    }

}



void MainWindow::on_MonochromeParametersChanged(int saturation, int hue)
{
    QGraphicsItem *item = ui->graphicsView->scene->items().first(); // Предполагается, что на сцене только один элемент
    if (item == nullptr) {
        QMessageBox::warning(this, tr("Error"), tr("No image loaded!"));
        return;
    }

    // Создаем копию изображения
    QGraphicsPixmapItem *pixmapItem = qgraphicsitem_cast<QGraphicsPixmapItem*>(item);
    if (!pixmapItem) {
        QMessageBox::warning(this, tr("Error"), tr("No image loaded!"));
        return;
    }

    QImage originalImage = pixmapItem->pixmap().toImage();
    QImage monochromeImage = originalImage.copy(); // Создаем копию оригинального изображения

    // Преобразуем копию в монохромное изображение
    for (int y = 0; y < monochromeImage.height(); ++y) {
        for (int x = 0; x < monochromeImage.width(); ++x) {
            QColor originalColor = monochromeImage.pixelColor(x, y);
            int gray = qGray(originalColor.rgb());
            QColor newColor = QColor::fromHsv((hue * 359) / 360, (saturation + 100) * 255 / 200, gray);
            monochromeImage.setPixelColor(x, y, newColor);
        }
    }

    // Сохраняем оригинальное и монохромное изображения для возможности отката
    originalPixmapItem = new QGraphicsPixmapItem(QPixmap::fromImage(originalImage));
    monochromePixmapItem = new QGraphicsPixmapItem(QPixmap::fromImage(monochromeImage));

    // Добавляем монохромное изображение на сцену и отображаем его
    ui->graphicsView->scene->addItem(monochromePixmapItem);
    ui->graphicsView->fitInView(monochromePixmapItem, Qt::KeepAspectRatio);
}


//малыши для теста того что связь между формами работает
void MainWindow::on_valuesChangedSaturation(int saturation)
{
    ui->saturationLabel->setText(QString::number(saturation));
}

void MainWindow::on_valuesChangedHue(int hue)
{
    ui->hueLabel->setText(QString::number(hue));
}
void MainWindow::on_MonochromeAuto(bool yes){
    // Получаем текущее изображение из сцены
    QGraphicsItem *item = ui->graphicsView->scene->items().first(); // Предполагается, что на сцене только один элемент
    if (item==NULL) {
        QMessageBox::warning(this, tr("Error"), tr("No image loaded!"));
        return;
    }

    // Создаем копию изображения
    QGraphicsPixmapItem *pixmapItem = qgraphicsitem_cast<QGraphicsPixmapItem*>(item);
    if (!pixmapItem) {
        QMessageBox::warning(this, tr("Error"), tr("No image loaded!"));
        return;
    }

    QImage originalImage = pixmapItem->pixmap().toImage();
    QImage monochromeImage = originalImage.copy(); // Создаем копию оригинального изображения

    // Преобразуем копию в монохромное изображение
    for (int y = 0; y < monochromeImage.height(); ++y) {
        for (int x = 0; x < monochromeImage.width(); ++x) {
            QColor color = monochromeImage.pixelColor(x, y);
            int gray = qGray(color.rgb());
            monochromeImage.setPixelColor(x, y, QColor(gray, gray, gray));
        }
    }

    // Сохраняем оригинальное и монохромное изображения для возможности отката
    originalPixmapItem=  new QGraphicsPixmapItem(QPixmap::fromImage(originalImage));
    monochromePixmapItem=  new QGraphicsPixmapItem(QPixmap::fromImage(monochromeImage));

    // Добавляем монохромное изображение на сцену и отображаем его
    ui->graphicsView->scene->addItem(monochromePixmapItem);
    ui->graphicsView->fitInView(monochromePixmapItem, Qt::KeepAspectRatio);
}
