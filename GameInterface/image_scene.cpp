
#include "image_scene.h"
#include"Engine/translator.h"

std::mutex paint_mutex;

void Image_scene::paint_item(Item item)
{
    if (!item.get_connected_item())
        return;


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
    painter.drawText(7 * _px + info_canvas.width() / 2, 30 * _px, info_canvas.width() / 2, 23 * _px, Qt::AlignLeft, Translator::translate(item.get_connected_item()->get_name().c_str()).c_str());

    painter.drawText(7 * _px, 55 * _px, info_canvas.width() / 2, 23 * _px, Qt::AlignLeft, tr("Class"));
    painter.drawText(info_canvas.width() / 2, 55 * _px, info_canvas.width() / 2, 23 * _px, Qt::AlignLeft, Translator::translate(item.get_connected_item()->get_class().c_str()).c_str());

    painter.drawText(7 * _px, 80 * _px, info_canvas.width() / 2, 23 * _px, Qt::AlignLeft, tr("Type"));
    painter.drawText(info_canvas.width() / 2, 80 * _px, info_canvas.width() / 2, 23 * _px, Qt::AlignLeft, Translator::translate(item.get_connected_item()->get_type().c_str()).c_str());


    if (item.get_connected_item()->get_class() != "potion") {
        painter.setFont(QFont("Arial", 20 * _px));
        painter.drawText(0, 110 * _px, info_canvas.width(), 20 * _px, Qt::AlignCenter, tr("Characteristics"));

        int i = 1;
        painter.setFont(QFont("Arial", 13 * _px));
        for (auto& ch : *item.get_connected_item()->get_item_characteristics()){
            painter.drawText(7 * _px, (110 + i * 25) * _px, info_canvas.width() / 2, 23 * _px, Qt::AlignLeft, ch.first.c_str());
            painter.drawText(info_canvas.width() / 2, (110 + i * 25) * _px, info_canvas.width() / 2, 23 * _px, Qt::AlignLeft, QString::number(ch.second));
            i++;
        }
    }
    else{
        painter.setFont(QFont("Arial", 20 * _px));
        painter.drawText(0, 110 * _px, info_canvas.width(), 23 * _px, Qt::AlignCenter, tr("Potion info"));

        painter.setFont(QFont("Arial", 15 * _px));
        Potion* pot = dynamic_cast<Potion*>(item.get_connected_item());

        painter.drawText(7 * _px, 135 * _px, info_canvas.width() / 2, 23 * _px, Qt::AlignLeft, tr("Effect name"));
        painter.drawText(info_canvas.width() / 2, 135 * _px, info_canvas.width() / 2, 23 * _px, Qt::AlignLeft, Translator::translate(pot->get_effect_name().c_str()).c_str());

        painter.drawText(7 * _px, 160 * _px, info_canvas.width() / 2, 23 * _px, Qt::AlignLeft, tr("Effect duration"));
        painter.drawText(info_canvas.width() / 2, 160 * _px, info_canvas.width() / 2, 23 * _px, Qt::AlignLeft, QString::number(pot->get_duration()));

        if (!item.get_connected_item()->get_item_characteristics()->empty()){
            painter.setFont(QFont("Arial", 20 * _px));
            painter.drawText(0, 190 * _px, info_canvas.width(), 30 * _px, Qt::AlignCenter, tr("Instant gains"));

            int i = 1;
            painter.setFont(QFont("Arial", 13 * _px));
            for (auto& ch : *item.get_connected_item()->get_item_characteristics()){
                painter.drawText(7 * _px, (195 + i * 25) * _px, info_canvas.width() / 2, 23 * _px, Qt::AlignLeft, ch.first.c_str());
                painter.drawText(info_canvas.width() / 2, (195 + i * 25) * _px, info_canvas.width() / 2, 23 * _px, Qt::AlignLeft, QString::number(ch.second));
                i++;
            }
        }
    }



    painter.end();
    //item->being_painted = false;

    paint_mutex.lock();
    this->info = QPixmap::fromImage(info_canvas);
    if (item.get_connected_item()->get_class() == "potion")
        this->current = QPixmap(":/equipment/Pictures/Equipment/" + QString::fromStdString(item.get_connected_item()->get_class()) + ".png");
    else
        this->current = QPixmap(":/equipment/Pictures/Equipment/" + QString::fromStdString(item.get_connected_item()->get_type()) + ".png");
    this->update();
    paint_mutex.unlock();

}

void Image_scene::paint_effect(Effect_item *item)
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
    painter.drawText(7 * _px + info_canvas.width() / 2, 30 * _px, info_canvas.width() / 2, 23 * _px, Qt::AlignLeft, Translator::translate(item->get_connected_effect()->get_effect_name().c_str()).c_str());

    painter.drawText(7 * _px, 55 * _px, info_canvas.width() / 2, 23 * _px, Qt::AlignLeft, tr("Type"));
    painter.drawText(info_canvas.width() / 2, 55 * _px, info_canvas.width() / 2, 23 * _px, Qt::AlignLeft, Translator::translate(item->get_connected_effect()->get_effect_type().c_str()).c_str());

    painter.drawText(7 * _px, 80 * _px, info_canvas.width() / 2, 23 * _px, Qt::AlignLeft, tr("Duration"));
    painter.drawText(info_canvas.width() / 2, 80 * _px, info_canvas.width() / 2, 23 * _px, Qt::AlignLeft, QString::number(item->get_connected_effect()->get_effect_duration()));


    if (!item->get_connected_effect()->get_sp_chs().empty()) {
        painter.setFont(QFont("Arial", 20 * _px));
        painter.drawText(0, 110 * _px, info_canvas.width(), 30 * _px, Qt::AlignCenter, tr("Temporary buffs"));

        int i = 1;
        painter.setFont(QFont("Arial", 10 * _px));
        for (auto& ch : item->get_connected_effect()->get_sp_chs()){
            painter.drawText(7 * _px, (115 + i * 25) * _px, info_canvas.width() / 2, 23 * _px, Qt::AlignLeft, ch.first.c_str());
            painter.drawText(info_canvas.width() / 2, (115 + i * 25) * _px, info_canvas.width() / 2, 23 * _px, Qt::AlignLeft, QString::number(ch.second));
            i++;
        }
    }



    painter.end();

    paint_mutex.lock();
    this->info = QPixmap::fromImage(info_canvas);
    //this->current = QPixmap(":/equipment/Pictures/Equipment/" + QString::fromStdString(item->get_connected_effect()->get_effect_name()) + ".png");

    this->current = QPixmap(":/events/Pictures/Events/missingContentError.png");
    this->update();
    paint_mutex.unlock();
}

Image_scene::Image_scene() : QWidget()
{

}

Image_scene::Image_scene(QWidget *parent) : QWidget(parent)
{
    prev_chosen = nullptr;
    background = QPixmap(":/backgrounds/Pictures/widget_backgrounds/status.png");
    current = QPixmap(":/backgrounds/Pictures/widget_backgrounds/status.png");
    info = QPixmap(":/backgrounds/Pictures/widget_backgrounds/status.png");
}

void Image_scene::reset()
{
    paint_mutex.lock();
    prev_chosen = nullptr;
    current = QPixmap(":/backgrounds/Pictures/widget_backgrounds/status.png");
    info = QPixmap(":/backgrounds/Pictures/widget_backgrounds/status.png");
    update();
    paint_mutex.unlock();
}

void Image_scene::take_item(Item *item)
{
    if (!item)
        return;

    if (prev_chosen)
        prev_chosen->setSelected(false);

    item->setSelected(true);
    prev_chosen = item;
    std::thread t(&Image_scene::paint_item, this, *item);
    t.detach();
}

void Image_scene::take_effect(QListWidgetItem *item)
{
    if (!item)
        return;

    if (prev_chosen)
        prev_chosen->setSelected(false);

    item->setSelected(true);
    prev_chosen = item;

    std::thread t(&Image_scene::paint_effect, this, dynamic_cast<Effect_item*>(item));
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

