
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

    //int my_id;

    void listen();
    void receive();
public:
    Transceiver();

    int get_id();

    static Transceiver* get_transceiver();
private slots:
    void process_connection(int _id);
    void process_msg(game_msg msg);
public slots:
    void terminate();
    void reset();
    void connectTo(std::string IP);
    void startListening(int qnt);
    void start_receiving();
signals:
    void connect_successful(int id);
    void join_successful();
    void initiate_lobby(int id, int qnt);
    void msg_received(game_msg msg);
    void send_msg(game_msg);
};

#endif // TRANSCEIVER_H