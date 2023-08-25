#include "minimap.h"
#include "Engine/mapcell.h"
#include "Engine/DataBase.h"

MiniMap::MiniMap(QWidget *parent, GameMap* battle_map)
    : QFrame{parent}
{
    int height = DataBase::get_DataBase()->get_height();
    int width = DataBase::get_DataBase()->get_width();
    moving_area = new MovingArea(this, battle_map);
    moving_area->setFrameStyle(QFrame::Panel | QFrame::Plain);
    moving_area->setFixedSize(battle_map->width() / 10, width * battle_map->height()  / (10 * height));
    moving_area->setLineWidth(2);
    setFrameStyle(QFrame::Panel | QFrame ::Plain);
    setLineWidth(3);
    this->battle_map = battle_map;
}

void MiniMap::paintEvent(QPaintEvent *event)
{
    QMap<std::string, QColor> type_to_color = {{"Лес", Qt::darkGreen}, {"Пустыня", Qt::darkYellow}, {"Болото", Qt::darkBlue}, {"Горы", Qt::darkGray}, {"non_moving_area", Qt::white}};
    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    painter.setRenderHint(QPainter::Antialiasing);
    MapCell **field = DataBase::get_DataBase()->get_map();
    int battle_map_height = DataBase::get_DataBase()->get_height();
    int battle_map_width = DataBase::get_DataBase()->get_width();

    for(int i = 0; i < battle_map_height; i++)
        for(int j = 0; j < battle_map_width; j++)
        {
            std::string type_of_terrain = field[i][j].get_type_of_terrain();
            painter.setBrush(QBrush(type_to_color[type_of_terrain], Qt::SolidPattern));
            painter.drawRect(QRectF((double)j*width()/battle_map_width, (double)i*height()/battle_map_height, (double)width()/battle_map_width, (double)height()/battle_map_height));
        }
    QFrame::paintEvent(event);
}
