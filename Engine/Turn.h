#ifndef TURN_H
#define TURN_H

#include "Player.h"
#include "Events/Event.h"

#include <iostream>
#include <ctime>
#include <string>
#include <cmath>
#include <iomanip>
#include <thread>
#include <chrono>
#include <QFile>

class Turn
{
private:
    static Turn *turn;
    Player* player;

    int turn_number;
    bool game_over;

    int roll;
    int rolled; // Никита, сохрани это поле!

    bool is_moving;
    bool has_already_moved;
    bool has_attacked;
    bool event_finished;
    Equipment* picked_item;
    Event* activated_event;

    Turn();
    const Turn& operator = (const Turn&) = delete;
    Turn(const Turn&) = delete;

public:
    static Turn* get_Turn();
    ~Turn();

    Player* get_player();
    void set_player(Player* player);
    void next_player();
    int get_turn_number();

    void dice_roll();
    int get_roll();
    int get_rolled();

    //Состояние текущего игрока во время хода
    bool get_moving();
    bool get_already_moved();
    bool get_game_over();
    void set_already_moved(bool moved);
    bool get_has_attacked();
    bool event_is_finished();

    void set_event_is_finished(bool finished);
    void set_has_attacked(bool atk);
    void set_game_over(bool over);
    void set_roll(int _roll);
    void set_is_moving(bool moving);
    void set_turn_number(int _turn_number);


    void process_end_of_movement();
    /*
    "game_over" :"0"
    "roll" :"0"
    "is_moving" :"0"
    "has_already_moved" :"1"
    "picked_item" :"Leather Leggings"
    "start_x" :"0"
    "start_y" :"0"
    */

    Equipment* get_picked_item();
    Event* get_activated_event();

    int get_start_x();
    int get_start_y();

    void save(QFile &out);
    void load(QFile& in);

    // был ли бросок кубика в это ходу
    bool was_roll();
};

#endif //TURN_H
