#include "minimap.h"
#include "Engine/DataBase.h"

MiniMap::MiniMap(QWidget *parent, HexMap* hex_map)
    : QFrame{parent}
{
    battle_map_size.first = DataBase::get_DataBase()->get_graph_map()->getWidth();
    battle_map_size.second = DataBase::get_DataBase()->get_graph_map()->getHeight();
    setFrameStyle(QFrame::Panel | QFrame ::Plain);
    setLineWidth(3);
    this->connected_hex_map = hex_map;
}

HexMap *MiniMap::get_connected_hex_map() const
{
    return connected_hex_map;
}

void MiniMap::create_moving_area()
{
    moving_area = new MovingArea(this);
    moving_area->setFrameStyle(QFrame::Panel | QFrame::Plain);

    int moving_area_width = connected_hex_map->width() * this->width() / connected_hex_map->get_source_pixmap_size().width();
    int moving_area_height = connected_hex_map->height() * this->height() / connected_hex_map->get_source_pixmap_size().height();

    moving_area->setFixedSize(moving_area_width, moving_area_height);
    moving_area->setLineWidth(2);

    connect(connected_hex_map, &HexMap::area_was_changed, this, &MiniMap::sync_moving_area);
}

void MiniMap::paintEvent(QPaintEvent *event)
{
    QFrame::paintEvent(event);
}

void MiniMap::sync_moving_area(QPoint position)
{
    int x = this->width() * position.x() / connected_hex_map->get_source_pixmap_size().width();
    int y = this->height() * position.y() / connected_hex_map->get_source_pixmap_size().height();
    moving_area->setGeometry(x, y, 0, 0);
}
