
#include "transceiver.h"

Transceiver* Transceiver::trans;

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

    terminated = false;
    listening = false;
    pulsing = false;
    receiving.resize(3, false);

    connect(this, &Transceiver::join_successful, this, &Transceiver::start_receiving);
    connect(this, &Transceiver::connect_successful, this, &Transceiver::process_connection);
    connect(this, &Transceiver::user_disconnected, this, &Transceiver::process_disconnect);
    connect(this, &Transceiver::msg_received, this, &Transceiver::process_msg);
}

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
    for (int i = max; i > 0; i--)
        availibleIds.push(i);

    is_host = true;

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

    std::thread t1(&Transceiver::pulse, this);
    t1.detach();
}

void Transceiver::start_receiving()
{
    connected.push_back(me);
    qDebug() << "Waiting for lobby data";
    // getting our id from msg
    char buffer[200];
    int rbytes = recv(connected[0], buffer, 200, 0);
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

    id = msg.target_id;
    emit initiate_lobby(msg.extra);

    std::thread t(&Transceiver::receive, this, 0);
    t.detach();
}

void Transceiver::pulse()
{
    pulsing = true;
    while (!terminated){
        game_msg check_msg = {0, 0, -1, 0, "pulse"};
        char buff[200];
        serialise_msg(check_msg, buff);
        for (int i = 0; i < connected.size(); i++){
            if (connected[i] == INVALID_SOCKET)
                continue;

            int sbyteCount = ::send(connected[i], buff, 200, 0);
            if (sbyteCount == SOCKET_ERROR)
                emit user_disconnected(i + 1);
        }

        Sleep(1000);
    }

    qDebug() << "pulsing has been stopped";
    pulsing = false;
}

void Transceiver::terminate()
{
    terminated = true;
    WSACleanup();
    while (receiving[0] || receiving[1] || receiving[2] || listening || pulsing)
        continue;
    \
    terminated = false;
}

void Transceiver::listen()
{
    listening = true;
    while (!terminated){
        if (::listen(me, 1) != 0) {
            qDebug() << "listen(): Error listening on socket: " << WSAGetLastError();
            qDebug() << "listening has stopped";
            return;
        }
        else {
            qDebug() << "listen() is OK! I'm waiting for new connections...";
        }

        // Accept incoming connections
        SOCKET acceptSocket;
        acceptSocket = accept(me, nullptr, nullptr);

        // Check for successful connection
        if (acceptSocket == INVALID_SOCKET) {
            qDebug() << "accept failed: " << WSAGetLastError();
            WSACleanup();
        }
        else {
            qDebug() << "accept() is OK!";
            int avail_id = availibleIds.top();
            connected[avail_id - 1] = acceptSocket;
            std::thread t(&Transceiver::receive, this, avail_id - 1);
            t.detach();
            availibleIds.pop();
            emit connect_successful(avail_id); // ID
        }
    }

    listening = false;

    qDebug() << "listening has stopped";
}

void Transceiver::receive(int _id)
{
    receiving[_id] = true;
    while (!terminated){
        char buffer[200];
        int rbytes = recv(connected[_id], buffer, 200, 0);
        if (rbytes < 0){
            Sleep(250);
            continue;
        }
        else{
            qDebug() << "New msg received" << WSAGetLastError();
            game_msg msg;
            deserialise_msg(buffer, &msg);
            emit msg_received(msg);
        }
    }

    qDebug() << "receiver #" << _id << " has stopped";
    receiving[_id] = false;
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

void Transceiver::send_msg(game_msg msg)
{
    char buffer[200];
    serialise_msg(msg, buffer);
    if (is_host){
        for (int i = 0; i < max; i++)
            ::send(connected[i], buffer, 200, 0);
    }
    else{
        int sbyteCount = ::send(me, buffer, 200, 0);
        if (sbyteCount == SOCKET_ERROR) {
            qDebug() << "Client send error: " << WSAGetLastError();
        }
    }

    qDebug() << "msg sent";
}

void Transceiver::resend_msg(game_msg msg)
{
    //resending to others
}

void Transceiver::process_connection(int _id)
{
    // here we send user id and then states of users
    game_msg msg{0, _id, -1, max + 1, "join"};
    char buffer[200];
    serialise_msg(msg, buffer);
    ::send(connected[id], buffer, 200, 0);
    qDebug() << "package sent";
}

void Transceiver::process_msg(game_msg msg)
{
    if (msg.operation_type == 0){
        emit ready_check(msg);
    }
}

void Transceiver::process_disconnect(int _id)
{
    availibleIds.push(_id);
    connected[_id - 1] = INVALID_SOCKET;
}

void Transceiver::reset()
{
    terminate();

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
    is_host = false;
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

