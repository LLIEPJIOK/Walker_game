#ifndef GAMEMAP_H
#define GAMEMAP_H

#include "cell.h"
#include "playersmodel.h"
#include "infocell.h"

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>
#include <QPainter>
#include <QGraphicsItemGroup>
#include <QApplication>
#include <QSet>

class GameMap : public QGraphicsView
{
    Q_OBJECT

public:
    explicit GameMap(QWidget *parent = nullptr);
    ~GameMap();
    void update_current_area(QPoint point);
    void set_delta(QPoint point);
    void highlight_possible_ways(const std::vector<std::pair<int, int>> ways);
    void want_to_move();
    QPoint get_chosen_way();
    void clear_chosen_way();

    // обновляет координаты игрока
    void update_player_coords(Player *player);

    // обновляет координаты всех игроков
    void update_coords();

private:
    QVector<QString> icons;
    Cell*** cells;
    std::set<Cell*> way_cells;
    std::vector<PlayersModel*> players_on_map;
    QPoint delta;
    QGraphicsScene *battle_map;
    QTimer *timer;
    InfoCell *info;

    QSize screen_size;
    int scalex;
    int scaley;
    QPoint positions[4] = {QPoint(0,0), QPoint(1, 0), QPoint(0,1), QPoint(1, 1)};
    QPoint chosen_way;
    std::vector<std::pair<int, int>> ways;
    bool continue_moving;

    void end_movement();

signals:
    void event_triggered();
    void win_by_killing();
    void update_roll();
    void can_finish_turn();
    void item_was_picked(Equipment* item);
    void action(QString text);

private slots:

    void initialize();
    void show_cell_info();

    void player_move();
    void process_attack();
    void process_killed_player(int place);
    void clear_ways();


};

#endif // GAMEMAP_H
