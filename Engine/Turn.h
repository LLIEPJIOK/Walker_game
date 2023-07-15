#ifndef TURN_H
#define TURN_H

#include "Player.h"
#include "Json.h"
#include "Events/Event.h"

#include <iostream>
#include <ctime>
#include <string>
#include <cmath>
#include <iomanip>
#include <thread>
#include <chrono>

class Turn
{
private:
    static Turn *turn;
    Player* player;

    int turn_number;
    bool game_over;

    int roll;

    bool is_moving;
    bool has_already_moved;
    bool has_attacked;
    bool event_finished;
    Equipment* picked_item;
    Event* activated_event;


    int start_x, start_y;
    std::pair<int, int> chosen_direction;

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
    void set_start_x(int _start_x);
    void set_start_y(int _start_y);
    void set_turn_number(int _turn_number);

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

    std::pair<int, int>* get_chosen_direction();

    int get_start_x();
    int get_start_y();

    //Движение игрока
    void set_chosen_direction(int x, int y);
    std::vector<std::pair<int, int>> move_player();
    std::vector<std::pair<int, int>> find_possible_ways();
    //void change_player_position(const int& x1, const int& y1, const int& x2, const int& y2) const;

    std::set<Player*> check_players_in_range() const;

    void save(std::ofstream& out);
    void load(std::ifstream& in);

    // был ли бросок кубика в это ходу
    bool was_roll();
};

#endif //TURN_H
