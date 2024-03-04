
#ifndef TRANSCEIVER_H
#define TRANSCEIVER_H

#include <stack>
#define _WINSOCK_DEPRECATED_NO_WARNINGS


#include <QObject>
#include"Ws2tcpip.h"

#include<WinSock2.h>
#include<process.h>
#include<iostream>
#pragma comment(lib,"WS2_32")

struct game_msg{
    int id; // who sent
    int target_id; // who is the target of the operation
    int operation_type; // what type of operation it is
    int extra; // just in case we need smth

    char buffer[127]; // needed msg (equipment name, slot name, potion name etc.
};



class Transceiver : public  QObject {
    Q_OBJECT
private:
    static Transceiver* trans;

    std::stack<int> availibleIds;

    SOCKET me;
    std::vector<SOCKET> connected; // if is host

    int id;
    int max;
    bool is_host;

    bool pulsing;
    bool listening;
    std::vector<bool> receiving;

    bool terminated;

    //int my_id;

    void listen();
    void receive(int _id);
public:
    Transceiver();

    int get_id();

    static Transceiver* get_transceiver();
    void send_msg(game_msg msg);
    void send_to(game_msg msg, int _id);
    void resend_msg(game_msg msg);
    std::vector<SOCKET> get_connected();
private slots:
    void process_msg(game_msg msg);
public slots:
    void process_disconnect(int _id);
    void terminate();
    void reset();
    void connectTo(std::string IP);
    void startListening(int qnt);
    void lobby_sync(std::vector<game_msg> states, int _id);
    void start_receiving();
    void pulse();
signals:
    void msg_received(game_msg msg);

    // LOBBY
    void connect_successful(int _id);
    void join_successful();
    void initiate_lobby(int qnt);
    void lobby_sync_init(int id);
    void ready_check(game_msg);
    void user_disconnected(int id);
    void set_connected(int id, bool val);
    void start_game();

    // GAME
    void send_cell_data(game_msg msg);
    void add_item(game_msg msg);
    void apply_effect(game_msg msg);
    void move_to(game_msg msg);
};

#endif // TRANSCEIVER_H
