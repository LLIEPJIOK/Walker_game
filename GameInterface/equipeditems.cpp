
#include "equipeditems.h"


EquipedItems::EquipedItems(QWidget *parent) :
    QFrame(parent)
{
    setFixedSize(0.39*parent->width(),0.556*parent->height());
    setFrameStyle(QFrame::Panel | QFrame::Plain);
    setAutoFillBackground(true);


    main_hand = new DropItem(this, nullptr, "одноручное", "основная_рука", "sword");
    main_hand->setGeometry(0.06 * width(), 0.075 * height(), 0.19 * width(), 0.4375 * height());

    non_dominant_hand = new DropItem(this, nullptr, "одноручное", "неосновная_рука", "shield");
    non_dominant_hand->setGeometry(0.74 * width(), 0.075 * height(), 0.19 * width(), 0.4375 * height());

    first_ring = new DropItem(this, nullptr, "кольцо", "первое_кольцо", "first_ring");
    first_ring->setGeometry(0.27 * width(), 0.3625 * height(), 0.12 * width(), 0.15 * height());

    second_ring = new DropItem(this, nullptr, "кольцо", "второе_кольцо", "second_ring");
    second_ring->setGeometry(0.61 * width(), 0.3625 * height(), 0.12 * width(), 0.15 * height());

    necklace = new DropItem(this, nullptr, "ожерелье", "ожерелье", "necklace");
    necklace->setGeometry(0.61 * width(), 0.1875 * height(), 0.12 * width(), 0.15 * height());

    helmet = new DropItem(this, nullptr, "шлем", "шлем", "helmet");
    helmet->setGeometry(0.41 * width(), 0.025 * height(), 0.18 * width(), 0.225 * height());

    plate = new DropItem(this, nullptr, "нагрудник", "нагрудник", "plate");
    plate->setGeometry(0.41 * width(), 0.275 * height(), 0.18 * width(), 0.3375 * height());

    belt = new DropItem(this, nullptr, "пояс", "пояс", "belt");
    belt->setGeometry(0.41 * width(), 0.6375 * height(), 0.18 * width(), 0.0625 * height());

    leggings = new DropItem(this, nullptr, "поножи", "поножи", "pants");
    leggings->setGeometry(0.41 * width(), 0.725 * height(), 0.18 * width(), 0.25 * height());

    gloves = new DropItem(this, nullptr, "перчатки", "перчатки", "gloves");
    gloves->setGeometry(0.21 * width(), 0.5375 * height(), 0.18 * width(), 0.225 * height());

    boots = new DropItem(this, nullptr, "сапоги", "сапоги", "boots");
    boots->setGeometry(0.61 * width(), 0.5375 * height(), 0.18 * width(), 0.225 * height());

}

void EquipedItems::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPixmap back ("../Game/Resources/Pictures/inventory.png");
    back = back.scaled(this->width(), this->height());
    QPainter painter(this);
    //painter.setRenderHint(QPainter::Antialiasing);
    painter.drawPixmap(0, 0, back);
}
