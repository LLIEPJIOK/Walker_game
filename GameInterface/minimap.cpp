#include "minimap.h"
#include "Engine/mapcell.h"
#include "Engine/DataBase.h"

MiniMap::MiniMap(QWidget *parent, GameMap* battle_map)
    : QFrame{parent}
{
    QSize s = QApplication::screens().at(0)->size();
    moving_area = new MovingArea(this, battle_map);
    moving_area->setFrameStyle(QFrame::Panel | QFrame::Plain);
    moving_area->setFixedSize(0.079 * s.width(), 0.094 * s.height());
    moving_area->setLineWidth(2);
    setFrameStyle(QFrame::Panel | QFrame ::Plain);
    setLineWidth(3);
    this->battle_map = battle_map;
}

void MiniMap::paintEvent(QPaintEvent *event)
{
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
            if(type_of_terrain=="Лес")
                painter.setBrush(QBrush(Qt::darkGreen, Qt::SolidPattern));
            else if(type_of_terrain=="Пустыня")
                painter.setBrush(QBrush(Qt::darkYellow, Qt::SolidPattern));
            else if (type_of_terrain=="Болото")
                painter.setBrush(QBrush(Qt::darkBlue, Qt::SolidPattern));
            else if (type_of_terrain=="Горы")
                painter.setBrush(QBrush(Qt::darkGray, Qt::SolidPattern));
            else
                painter.setBrush(QBrush(Qt::white, Qt::SolidPattern));
            painter.drawRect(QRectF((double)j*width()/battle_map_width, (double)i*height()/battle_map_height, (double)width()/battle_map_width, (double)height()/battle_map_height));
        }
    QFrame::paintEvent(event);
}
