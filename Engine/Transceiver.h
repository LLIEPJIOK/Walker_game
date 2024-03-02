
#ifndef TRANSCEIVER_H
#define TRANSCEIVER_H

#define _WINSOCK_DEPRECATED_NO_WARNINGS


#include <QObject>
#include"Ws2tcpip.h"

#include<WinSock2.h>
#include<process.h>
#include<iostream>
#pragma comment(lib,"WS2_32")

class Transceiver : public  QObject {
    Q_OBJECT
private:
    static Transceiver* trans;

    SOCKET me;
    std::vector<SOCKET> connected; // if is host

    int max;
    bool is_host;

    //int my_id;

    void listen();
public:
    Transceiver();

    static Transceiver* get_transceiver();
public slots:
    void terminate();
    void reset();
    void connectTo(std::string IP);
    void startListening(int qnt);
signals:
    void connect_successful(int id);
    void join_successful();
};

#endif // TRANSCEIVER_H
