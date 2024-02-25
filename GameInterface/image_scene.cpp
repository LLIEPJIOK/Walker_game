
#include "image_scene.h"
#include"Engine/translator.h"
#include <QPainter>
#include <mutex>
#include<thread>
#include<QImage>

std::mutex paint_mutex;

void Image_scene::paint(Item *item)
{
    QPainter painter;
    QImage info_canvas(info.size(), QImage::Format_RGB32);
    int h = info.height();
    double _px = h / 300;

    painter.begin(&info_canvas);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing, true);
    painter.setPen(QPen(Qt::black, 3));
    painter.drawPixmap(0, 0, info_canvas.width(), info_canvas.height(), background);

    painter.setFont(QFont("Arial", 20 * _px));
    painter.drawText(0, 5 * _px, info_canvas.width(), 20 * _px, Qt::AlignCenter, tr("General"));

    painter.setFont(QFont("Arial", 15 * _px));
    painter.drawText(7 * _px, 30 * _px, info_canvas.width() / 2, 23 * _px, Qt::AlignLeft, tr("Name"));
    painter.drawText(7 * _px + info_canvas.width() / 2, 30 * _px, info_canvas.width() / 2, 23 * _px, Qt::AlignCenter, Translator::translate(item->get_connected_item()->get_name().c_str()).c_str());

    painter.drawText(7 * _px, 55 * _px, info_canvas.width() / 2, 23 * _px, Qt::AlignLeft, tr("Class"));
    painter.drawText(info_canvas.width() / 2, 55 * _px, info_canvas.width() / 2, 23 * _px, Qt::AlignCenter, Translator::translate(item->get_connected_item()->get_class().c_str()).c_str());

    painter.drawText(7 * _px, 80 * _px, info_canvas.width() / 2, 23 * _px, Qt::AlignLeft, tr("Type"));
    painter.drawText(info_canvas.width() / 2, 80 * _px, info_canvas.width() / 2, 23 * _px, Qt::AlignCenter, Translator::translate(item->get_connected_item()->get_type().c_str()).c_str());

    painter.setFont(QFont("Arial", 20 * _px));
    painter.drawText(0, 110 * _px, info_canvas.width(), 20 * _px, Qt::AlignCenter, tr("Characteristics"));

    int i = 1;
    painter.setFont(QFont("Arial", 10 * _px));
    for (auto& ch : *item->get_connected_item()->get_item_characteristics()){
        painter.drawText(7 * _px, (110 + i * 25) * _px, info_canvas.width() / 2, 23 * _px, Qt::AlignLeft, ch.first.c_str());
        painter.drawText(info_canvas.width() / 2, (110 + i * 25) * _px, info_canvas.width() / 2, 23 * _px, Qt::AlignCenter, QString::number(ch.second));
        i++;
    }

    painter.end();

    paint_mutex.lock();
    this->info = QPixmap::fromImage(info_canvas);
    if (item->get_connected_item()->get_class() == "potion")
        this->current = QPixmap(":/equipment/Pictures/Equipment/" + QString::fromStdString(item->get_connected_item()->get_class()) + ".png");
    else
        this->current = QPixmap(":/equipment/Pictures/Equipment/" + QString::fromStdString(item->get_connected_item()->get_type()) + ".png");
    this->update();
    paint_mutex.unlock();
}

Image_scene::Image_scene() : QWidget()
{

}

Image_scene::Image_scene(QWidget *parent) : QWidget(parent)
{
    background = QPixmap(":/backgrounds/Pictures/widget_backgrounds/status.png");
    current = QPixmap(":/backgrounds/Pictures/widget_backgrounds/status.png");
    info = QPixmap(":/backgrounds/Pictures/widget_backgrounds/status.png");
}

void Image_scene::take_item(Item *item)
{
    std::thread t(&Image_scene::paint, this, item);
    t.detach();
}

void Image_scene::paintEvent(QPaintEvent *event)
{
    QPainter painter;
    painter.begin(this);
    painter.drawPixmap(0, 0, this->width(), this->height(), background);
    paint_mutex.lock();
    painter.drawPixmap(width() / 2 - 200, 20,  400, 600, current);
    painter.drawPixmap(20, 640, width() - 40, height() - 640, info);
    paint_mutex.unlock();
    QWidget::paintEvent(event);
}

