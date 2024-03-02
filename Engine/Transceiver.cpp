
#include "transceiver.h"

Transceiver* Transceiver::trans;

void serialise_msg(game_msg& msg, char* output) {
    int* q  = (int*)output;
    *q = msg.id; q++;
    *q = msg.target_id; q++;
    *q = msg.operation_type; q++;
    *q = msg.extra; q++;

    char* p = (char*)q;
    for (int i = 0; i < 127; i++) {
        *p = msg.buffer[i];
        p++;
    }
}


void deserialise_msg(char* input, game_msg* msg) {
    int* q = (int*)input;
    msg->id = *q; q++;
    msg->target_id = *q; q++;
    msg->operation_type = *q; q++;
    msg->extra = *q; q++;

    char* p = (char*)q;
    for (int i = 0; i < 127; i++) {
        msg->buffer[i] = *p;
        p++;
    }
}

void Transceiver::startListening(int qnt)
{
    reset();
    connected.resize(qnt);
    id = 0;
    max = qnt;
    for (int i = max; i > 0 ; i--)
        availibleIds.push(i - 1);

    is_host =true;

    // Bind the socket
    sockaddr_in service;
    service.sin_family = AF_INET;
    service.sin_addr.s_addr = inet_addr("0.0.0.0");
    service.sin_port = htons(55555);  // Choose a port number

    // Use the bind function
    if (bind(me, reinterpret_cast<SOCKADDR*>(&service), sizeof(service)) != 0) {
        qDebug() << "bind() failed: " << WSAGetLastError();
        closesocket(me);
        WSACleanup();
        return;
    }
    else {
        qDebug() << "bind() is OK!";
        qDebug() << service.sin_addr.S_un.S_addr;
    }

    std::thread t(&Transceiver::listen, this);
    t.detach();

    std::thread t2(&Transceiver::receive, this);
    t2.detach();
}

void Transceiver::start_receiving()
{
    qDebug() << "Waiting for lobby data";
    // getting our id from msg
    char buffer[200];
    int rbytes = recv(me, buffer, 200, 0);
    if (rbytes < 0){
        qDebug() << "Client receive error " << WSAGetLastError();
    }
    else{
        qDebug() << "Lobby data received, connecting " << WSAGetLastError();
    }

    game_msg msg;
    deserialise_msg(buffer, &msg);

    if (msg.target_id < 1 || msg.extra < 2){
        qDebug() << "Lobby data is invalid";
    }

    id = msg.target_id + 1;
    emit initiate_lobby(msg.target_id, msg.extra);

    std::thread t(&Transceiver::receive, this);
    t.detach();
}

void Transceiver::terminate()
{
    WSACleanup();
    connected.clear();
}

void Transceiver::listen()
{
    while (true){
        if (::listen(me, 1) != 0) {
            qDebug() << "listen(): Error listening on socket: " << WSAGetLastError();
        }
        else {
            qDebug() << "listen() is OK! I'm waiting for new connections...";
        }

        while (connected.size() >= max) {
            continue;
        }
        // Accept incoming connections
        SOCKET acceptSocket;
        acceptSocket = accept(me, nullptr, nullptr);

        // Check for successful connection
        if (acceptSocket == INVALID_SOCKET) {
            qDebug() << "accept failed: " << WSAGetLastError();
            WSACleanup();
            return;
        }
        else {
            qDebug() << "accept() is OK!";
        }

        connected[availibleIds.top()] = acceptSocket;
        emit connect_successful(availibleIds.top() + 1); // ID
        availibleIds.pop();
    }
}

void Transceiver::receive()
{
    while (true){
        char buffer[200];
        int rbytes = recv(me, buffer, 200, 0);
        if (rbytes < 0){
            Sleep(50);
            continue;
        }
        else{
            qDebug() << "New msg received" << WSAGetLastError();
        }

        game_msg msg;
        deserialise_msg(buffer, &msg);
        emit msg_received(msg);
    }
}

Transceiver::Transceiver()
{
    WSADATA wsaData;
    int wsaerr;
    WORD wVersionRequested = MAKEWORD(2, 2);
    wsaerr = WSAStartup(wVersionRequested, &wsaData);

    if (wsaerr != 0) {
        qDebug() << "The Winsock dll not found!";
        return;
    }
    else {
        qDebug() << "The Winsock dll found";
        qDebug() << "The status: " << wsaData.szSystemStatus;
    }

    // Create a socket
    me = INVALID_SOCKET;
    me = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    // Check for socket creation success
    if (me == INVALID_SOCKET) {
        qDebug() << "Error at socket(): " << WSAGetLastError();
        WSACleanup();
        return;
    }
    else {
        qDebug() << "Socket is OK!";
    }

    max = 2;

    connect(this, &Transceiver::join_successful, this, &Transceiver::start_receiving);
    connect(this, &Transceiver::connect_successful, this, &Transceiver::process_connection);
}

int Transceiver::get_id()
{
    return trans->id;
}

Transceiver *Transceiver::get_transceiver()
{
    if (!trans)
        return trans = new Transceiver;

    return trans;
}

void Transceiver::process_connection(int _id)
{
    // here we send user id and then states of users
    game_msg msg{id, _id, -1, max + 1, "join"};
    char buffer[200];
    serialise_msg(msg, buffer);
    ::send(connected[id], buffer, 200, 0);
    qDebug() << "package sent";
}

void Transceiver::process_msg(game_msg msg)
{

}

void Transceiver::reset()
{
    WSACleanup();
    connected.clear();

    WSADATA wsaData;
    int wsaerr;
    WORD wVersionRequested = MAKEWORD(2, 2);
    wsaerr = WSAStartup(wVersionRequested, &wsaData);

    if (wsaerr != 0) {
        qDebug() << "The Winsock dll not found!";
        return;
    }
    else {
        qDebug() << "The Winsock dll found";
        qDebug() << "The status: " << wsaData.szSystemStatus;
    }

    // Create a socket
    me = INVALID_SOCKET;
    me = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    // Check for socket creation success
    if (me == INVALID_SOCKET) {
        qDebug() << "Error at socket(): " << WSAGetLastError();
        WSACleanup();
        return;
    }
    else {
        qDebug() << "Socket is OK!";
    }
}

void Transceiver::connectTo(std::string IP)
{
    reset();
    // Connect to the server
    sockaddr_in clientService;
    clientService.sin_family = AF_INET;
    clientService.sin_addr.s_addr = inet_addr(IP.c_str());  // Replace with the server's IP address
    clientService.sin_port = htons(55555);  // Use the same port as the server

    // Use the connect function
    if (::connect(me, reinterpret_cast<SOCKADDR*>(&clientService), sizeof(clientService)) == SOCKET_ERROR) {
        qDebug() << "Client: connect() - Failed to connect: " << WSAGetLastError();
        WSACleanup();
        return;
    }
    else {
       qDebug() << "Client: Connect() is OK!";
       qDebug() << "Client: Can start sending and receiving data...";
       emit join_successful();
    }
}

