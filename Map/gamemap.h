#ifndef GAMEMAP_H
#define GAMEMAP_H

#include "cell.h"
#include "playersmodel.h"

#include <QMouseEvent>
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
    void move_to_player();
    void highlight_possible_ways(const std::vector<std::pair<int, int>> ways);
    void want_to_move();
    QPoint get_chosen_way();
    void clear_chosen_way();

    // обновляет координаты игрока
    void update_player_coords(Player *player);

    // обновляет координаты всех игроков
    void update_coords();

private:
    QVector<QString> icons = {"knight", "wizzard", "dwarf", "elf"};
    QPoint player_movement_positions[4] = {QPoint(0, 0), QPoint(1, 0), QPoint(0, 1), QPoint(1, 1)};

    Cell*** cells;
    std::set<Cell*> way_cells;
    std::vector<PlayersModel*> players_on_map;
    QGraphicsScene *battle_map;
    QTimer *timer;

    QSize screen_size;
    QPoint old_position;
    QPoint current_map_position;
    int cell_size;
    QPoint distance;
    QPoint direction;
    QPoint chosen_way;
    std::vector<std::pair<int, int>> ways;
    bool continue_moving;
    bool is_pressed_for_moving;

    void end_movement();
    void make_distance_and_direction();

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
signals:
    void event_triggered();
    void win_by_killing();
    void update_roll();
    void can_finish_turn();
    void item_was_picked(Equipment* item);
    void action(QString text);
    void was_initialized();
    void area_was_changed(QPoint position);

private slots:
    void initialize();

    void player_move();
    void process_attack();
    void process_killed_player(int place);
    void clear_ways();
};

#endif // GAMEMAP_H
