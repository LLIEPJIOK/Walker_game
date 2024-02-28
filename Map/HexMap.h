#pragma once

#include "playersmodel.h"

#include <QKeyEvent>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>
#include <QPainter>
#include <QGraphicsItemGroup>
#include <QApplication>
#include <QSet>
#include <QQueue>
#include "GraphMap.h"

class HexMap : public QGraphicsView
{
	Q_OBJECT
private:
	QPoint mousePosition;
    GraphMap* map;
	Location start;
    QPoint offset;

    QVector<QString> icons = {"knight", "wizzard", "dwarf", "elf"};


    QPair<int, int> battle_map_size;
    std::vector<PlayersModel*> players_on_map;
    QGraphicsScene *graphics_scene;
    QTimer *timer;

    std::unordered_set<Location> ways;
    QQueue<QPointF> coordinate_path;

    QSize screen_size;
    bool continue_moving;


    PlayersModel* current_players_model;


    void end_movement();
    
    QPixmap source_pixmap;
    QPainter source_pixmap_painter;
    QGraphicsPixmapItem* pixmap_item;

public:
    explicit HexMap(QWidget* parent, GraphMap* map_);
	~HexMap(){};

    QPoint get_offset() const noexcept;

    void set_x_offset(const int& x_offset_);
    void set_y_offset(const int& y_offset_);

    QSize get_source_pixmap_size() const;


    void set_current_players_model(int index);
    void next_players_model();
    void update_current_area(QPoint position);
    void move_to_player();

    void want_to_move();


    void trace_hexes(std::unordered_set<Location> hexes, QColor trace_color);
    void draw_hex(const Location& location);
    // обновляет координаты игрока
    void update_player_coords(Player *player);

    // обновляет координаты всех игроков
    void update_coords();

protected:
    void mousePressEvent(QMouseEvent* event) override;
    //void scrollContentsBy(int dx, int dy) override;
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
    void process_attack(PlayersModel* attacked_player);
    void process_killed_player(int place);
    void prepare_for_move(const Coordinates::Hex<int>& hex);
};
