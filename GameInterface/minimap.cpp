#include "minimap.h"
#include "Engine/mapcell.h"
#include "Engine/DataBase.h"

MiniMap::MiniMap(QWidget *parent, GameMap* battle_map)
    : QFrame{parent}
{
    battle_map_size.first = DataBase::get_DataBase()->get_width();
    battle_map_size.second = DataBase::get_DataBase()->get_height();

    moving_area = new MovingArea(this, battle_map);
    moving_area->setFrameStyle(QFrame::Panel | QFrame::Plain);
    moving_area->setFixedSize(battle_map->width() / 10, battle_map_size.first * battle_map->height()  / (10 * battle_map_size.second));
    moving_area->setLineWidth(2);
    setFrameStyle(QFrame::Panel | QFrame ::Plain);
    setLineWidth(3);
    this->battle_map = battle_map;

    connect(battle_map, &GameMap::area_was_changed, this, &MiniMap::sync_moving_area);
}

void MiniMap::paintEvent(QPaintEvent *event)
{
    QMap<std::string, QColor> type_to_color = {{"Лес", Qt::darkGreen}, {"Пустыня", Qt::darkYellow}, {"Болото", Qt::darkBlue}, {"Горы", Qt::darkGray}, {"non_moving_area", Qt::white}};
    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    painter.setRenderHint(QPainter::Antialiasing);
    MapCell **field = DataBase::get_DataBase()->get_map();

    for(int i = 0; i < battle_map_size.second; i++)
        for(int j = 0; j < battle_map_size.first; j++)
        {
            std::string type_of_terrain = field[i][j].get_type_of_terrain();
            painter.setBrush(QBrush(type_to_color[type_of_terrain], Qt::SolidPattern));
            painter.drawRect(QRectF((double)j * width() / battle_map_size.first, (double)i * height() / battle_map_size.second,
                                    (double)width() / battle_map_size.first, (double)height() / battle_map_size.second));
        }
    QFrame::paintEvent(event);
}

void MiniMap::sync_moving_area(QPoint position)
{
    moving_area->setGeometry(position.x() / 10, battle_map_size.first * position.y()  / (10 * battle_map_size.second), 0, 0);
}
