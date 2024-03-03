
#include "transceiver.h"

std::mutex send_mutex;

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
    while (!availibleIds.empty())
        availibleIds.pop();

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

void Transceiver::lobby_sync(std::vector<game_msg> states, int _id)
{
    send_to(states[0], _id);
    for (int i = 1; i < states.size(); i++){
        if (connected[i - 1] != INVALID_SOCKET)
            send_to({0, i, 2, 1, "set_connected"}, _id);
        else
            send_to({0, i, 2, 0, "set_connected"}, _id);

        send_to(states[i], _id);
    }
}

void Transceiver::start_receiving()
{
    connected.resize(1, INVALID_SOCKET);
    connected[0] = me;
    qDebug() << "Waiting for lobby data";
    // getting our id from msg

    std::thread t(&Transceiver::receive, this, 0);
    t.detach();
}

void Transceiver::pulse()
{
    pulsing = true;
    game_msg check_msg = {0, 0, -1, 0, "pulse"};
    char buff[200];
    serialise_msg(check_msg, buff);
    while (!terminated){
        //pulse_mutex.lock();
        for (int i = 0; i < connected.size(); i++){
            if (connected[i] == INVALID_SOCKET)
                continue;

            send_mutex.lock();
            int sbyteCount = ::send(connected[i], buff, 200, 0);
            send_mutex.unlock();
            if (sbyteCount == SOCKET_ERROR)
                emit user_disconnected(i + 1);
        }
        //pulse_mutex.unlock();

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

    if (!is_host)
        shutdown(me, 0);

    connected.clear();

    is_host = false;

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
            availibleIds.pop();

            // for lobby msg
            game_msg msg{0, avail_id, -1, max + 1, "join"};
            char buffer[200];
            serialise_msg(msg, buffer);

            // sending lobby data
            //pulse_mutex.lock();
            connected[avail_id - 1] = acceptSocket;
            // lobby settings
            ::send(connected[avail_id - 1], buffer, 200, 0);
            //pulse_mutex.unlock();

            // special thread for the socket
            std::thread t(&Transceiver::receive, this, avail_id - 1);
            t.detach();

            qDebug() << "package sent";

            // for the front
            emit connect_successful(avail_id);
            emit lobby_sync_init(avail_id);
            send_msg({avail_id, avail_id, 2,  1, "connect_upd"});
        }
    }

    listening = false;

    qDebug() << "listening has stopped";
}

void Transceiver::receive(int _id)
{
    receiving[_id] = true;
    while (!terminated && receiving[_id]){
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
    if (is_host){
        for (int i = 0; i < connected.size(); i++){
            if (connected[i] == INVALID_SOCKET || i + 1 == msg.id)
                continue;

            send_to(msg, i + 1);
        }
    }
    else
        send_to(msg, 0);

    qDebug() << "msg sent";
}

void Transceiver::send_to(game_msg msg, int _id)
{
    if (_id > connected.size()) {
        qDebug() << "Trying send msg out of connected boundaries";
        return;
    }

    char buffer[200];
    serialise_msg(msg, buffer);

    if (is_host) {
        send_mutex.lock();
        int sbyteCount = ::send(connected[_id - 1], buffer, 200, 0);
        send_mutex.unlock();
    }
    else if (_id == 0){
        send_mutex.lock();
        int sbyteCount = ::send(connected[0], buffer, 200, 0);
        send_mutex.unlock();
    }
}

void Transceiver::resend_msg(game_msg msg)
{
    //resending to others
}

std::vector<SOCKET> Transceiver::get_connected()
{
    return connected;
}



void Transceiver::process_msg(game_msg msg)
{
    if (msg.operation_type == 0){
        if (is_host)
            send_msg(msg);

        emit ready_check(msg);
    }
    else if (msg.operation_type == -1 && msg.extra > 1){
        qDebug() << "Lobby data received " << msg.extra << "pl";
        id = msg.target_id;

        emit initiate_lobby(msg.extra);
    }
    else if (msg.operation_type == 2){
        emit set_connected(msg.target_id, msg.extra);
    }
    else
        return;
}

void Transceiver::process_disconnect(int _id)
{
    availibleIds.push(_id);
    shutdown(connected[_id - 1], 0);
    receiving[_id - 1] = false;
    connected[_id - 1] = INVALID_SOCKET;
    send_msg({id, _id, 2,  0, "connect_upd"});
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

