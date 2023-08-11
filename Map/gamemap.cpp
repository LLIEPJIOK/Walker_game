#include "gamemap.h"
#include "Events/event_window.h"
#include "Engine/DataBase.h"
#include "Engine/Turn.h"

#include <QApplication>
#include <QScreen>

#define seq DataBase::get_DataBase()->get_sequence()

GameMap::GameMap(QWidget *parent)
    : QGraphicsView{parent}
{
    icons.push_back("knight");
    icons.push_back("wizzard");
    icons.push_back("dwarf");
    icons.push_back("elf");

    screen_size = QApplication::screens().at(0)->size();
    int h = DataBase::get_DataBase()->get_height();
    int w = DataBase::get_DataBase()->get_width();
    scalex = 1.95 * screen_size.width() / w;
    scaley  = 3.47 * screen_size.height() / h;
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &GameMap::initialize);
    info = new InfoCell(this);
    info->setVisible(false);
    battle_map = new QGraphicsScene(this);

    setRenderHint(QPainter::Antialiasing, true);

    battle_map->setSceneRect(0, 0, this->width(), this->height());
    setScene(battle_map);

    delta = QPoint(0, 0);
    timer->start(50);
    chosen_way = QPoint(-1, -1);

    continue_moving = false;
}

GameMap::~GameMap()
{
    delete battle_map;
    delete timer;
    delete info;
}

void GameMap::update_current_area(QPoint point)
{
    setSceneRect(QRect(point, QSize(this->width(), this->height())));
}

void GameMap::set_delta(QPoint point)
{
    delta = point;
}

void GameMap::highlight_possible_ways(std::vector<std::pair<int, int>> ways)
{
    if(ways.size() == 1 && chosen_way != QPoint(-1, -1))
    {
        chosen_way = QPoint(ways[0].first, ways[0].second);
        want_to_move();
        return;
    }
    for(const auto& i : ways)
    {
        cells[i.second][i.first]->highlight();
        way_cells.insert(cells[i.second][i.first]);
    }
}

void GameMap::want_to_move()
{
    Turn* turn = Turn::get_Turn();
    if(chosen_way != QPoint(-1 , -1))
    {
        turn->set_chosen_direction(chosen_way.x(), chosen_way.y());
    }
    ways = turn->move_player();
    timer->start(10);
}

QPoint GameMap::get_chosen_way()
{
    return chosen_way;
}

void GameMap::clear_chosen_way()
{
    chosen_way = QPoint(-1, -1);
}

// обновляет координаты игрока
void GameMap::update_player_coords(Player *player)
{
    auto i = player->get_id() - 1;
    players_on_map[i]->setPos(player->get_x() * scalex + positions[i].x() * 0.6 * scalex,
                              player->get_y() * scaley + positions[i].y() * 0.6 * scaley);
}

// обновляет координаты игрока
void GameMap::update_coords()
{
    for (int i = 0; i < seq->size(); i++)
    {
        update_player_coords(seq->at(i));
    }
}

void GameMap::end_movement()
{
    emit update_roll();
    Turn* turn = Turn::get_Turn();

    if (turn->get_already_moved())
    {
        emit event_triggered(); // запуск цепочки обработок всех действий, происходящих после конца движения
    }
}

void GameMap::show_cell_info()
{
    Cell* cell = qobject_cast<Cell*>(sender());
    MapCell ** map = DataBase::get_DataBase()->get_map();
    if(cell->get_hovered())
    {
        QPointF pos = cell->pos();
        int info_x = pos.x() - delta.x() + scalex;
        int info_y = pos.y() - delta.y();
        if(info_x + info->width() > width())
           info_x -= 320;
        if(info_y + info->height() > height())
           info_y -= 90;
        else if(info_y < 0)
           info_y = info_y + cell->boundingRect().height() + 10;
        info->setGeometry(info_x, info_y, info->width() , info->height());
        info->set_terrain_type(QString::fromStdString(map[(int)pos.y()/scaley][(int)pos.x()/scalex].get_type_of_terrain()));
        info->set_item_name(QString::fromStdString(map[(int)pos.y()/scaley][(int)pos.x()/scalex].get_item()));
        info->set_event_name(QString::fromStdString(map[(int)pos.y()/scaley][(int)pos.x()/scalex].get_event_name()));
        info->setVisible(true);
    }
    else
        info->setVisible(false);
}

void GameMap::player_move()
{
    Turn* turn = Turn::get_Turn();
    auto map_player = players_on_map[(turn->get_turn_number()-1) % players_on_map.size()];
    QPoint pos = QPoint(map_player->pos().x(),map_player->pos().y()) ;
    Player* pl = turn->get_player();
    QPoint to_move;
    int shx = positions[(turn->get_turn_number()-1) % players_on_map.size()].x() * 0.6 * scalex;
    int shy = positions[(turn->get_turn_number()-1) % players_on_map.size()].y() * 0.6 * scaley;

    if(pos.x() - shx != pl->get_x() * scalex || pos.y() - shy != pl->get_y() * scaley)
    {
        if(pl->get_x() * scalex < pos.x() - shx)
        {
           to_move.setX(-1);
           map_player->set_right_direction();
        }
        else if(pl->get_y()* scaley < pos.y() - shy )
           to_move.setY(-1);
        else if (pl->get_x() * scalex > pos.x() - shx)
        {
           to_move.setX(1);
           map_player->set_left_direction();
        }
        else if (pl->get_y() * scaley > pos.y() - shy)
           to_move.setY(1);
        map_player->move(to_move);
    }
    else
    {
        timer->stop();
        end_movement();
        highlight_possible_ways(ways);   
    }
}

void GameMap::process_attack()
{
    Turn* turn = Turn::get_Turn();
    auto s = turn->check_players_in_range();
    Player* player = qobject_cast<PlayersModel*>(sender())->get_connected_player();

    if(turn->get_moving())
    {
        emit action("Вы не можете атаковать в движении!");
        return;
    }
    if(turn->get_has_attacked() /*&& s.find(player)!=s.end()*/)
        emit action("Вы уже атаковали на этом ходу!");
    else
    {
        if(s.find(player)!=s.end())
        {
            turn->get_player()->attack(player);
            if(turn->get_player()->get_killed_player()!= -1)
            {
                process_killed_player(turn->get_player()->get_killed_player());
                turn->set_has_attacked(true);
                return;
            }
            emit action("Вы ударили " + QString::fromStdString(player->get_name()));
            turn->set_has_attacked(true);

        }
        else
            emit action("Вы слишком далеко! Попробуйте подойти ближе!");
    }

}

void GameMap::process_killed_player(int place)
{
    delete players_on_map[place];
    players_on_map.erase(players_on_map.begin() + (place));
    if(players_on_map.size() == 1)
        emit win_by_killing();
}

void GameMap::clear_ways()
{
    for(auto i = way_cells.begin(); i != way_cells.end(); i++)
    {
        (*i)->normalize();
    }
    QPointF pos = qobject_cast<Cell*>(sender())->pos();
    chosen_way = QPoint(pos.x() / scalex, pos.y() / scaley);
    want_to_move();
}

void GameMap::initialize()
{
    timer->stop();
    disconnect(timer, &QTimer::timeout, this, &GameMap::initialize);
    connect(timer, &QTimer::timeout, this, &GameMap::player_move);

    MapCell **field = DataBase::get_DataBase()->get_map();
    int battle_map_height = DataBase::get_DataBase()->get_height();
    int battle_map_width = DataBase::get_DataBase()->get_width();

    auto s = seq;

    QBrush brush(Qt::darkGreen);
    setSceneRect(0, 0, this->width(), this->height());
    cells = new Cell**[battle_map_height];
    for(int i = 0; i < battle_map_height; i++)
    {
        cells[i] = new Cell*[battle_map_width];
        for(int j = 0; j < battle_map_width; j++)
        {
           Cell* cell = new Cell(battle_map, scalex, scaley, brush, QString::fromStdString(field[i][j].get_tile_name()));

           connect(cell, &Cell::cell_signal, this, &GameMap::show_cell_info);
           connect(cell, &Cell::way_to_go, this, &GameMap::clear_ways);

           battle_map->addItem(cell);
           cell->setPos(j * scalex, i * scaley);
           cells[i][j] = cell;
        }
    }

    for(int i = 0; i < s->size(); i++)
    {
        PlayersModel *players_model = new PlayersModel(battle_map, 0.3 * scalex, 0.3 * scaley, QBrush(Qt::white), icons[i]);

        connect(players_model, &PlayersModel::target_to_attack, this, &GameMap::process_attack);

        players_on_map.push_back(players_model);
        players_model->set_connected_plaeyr(s->at(i));
        battle_map->addItem(players_model);
        players_model->setPos(s->at(i)->get_x() * scalex + positions[i].x() * 0.6 * scalex,
                              s->at(i)->get_y() * scaley + positions[i].y() * 0.6 * scaley);
    }

}
