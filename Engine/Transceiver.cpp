
#include "transceiver.h"

Transceiver* Transceiver::trans;

void Transceiver::startListening(int qnt)
{
    reset();
    max = qnt;
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
}

void Transceiver::terminate()
{
    WSACleanup();
}

//void Transceiver::receive()
//{
//    // Receive data from the client
//    char receiveBuffer[200];
//    int rbyteCount = recv(connected[0], receiveBuffer, 200, 0);
//    if (rbyteCount < 0) {
//        qDebug() << "Server recv error: " << WSAGetLastError() << ;
//        return;
//    }
//    else {
//        qDebug() << "Received data: " << receiveBuffer << ;
//    }
//}

void Transceiver::listen()
{
    while (connected.size() < max){
        if (::listen(me, 1) != 0) {
            qDebug() << "listen(): Error listening on socket: " << WSAGetLastError();
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
            return;
        }
        else {
            qDebug() << "accept() is OK!";
        }

        connected.push_back(acceptSocket);
        emit connect_successful(1); // ID
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
}

Transceiver *Transceiver::get_transceiver()
{
    if (!trans)
        return trans = new Transceiver;

    return trans;
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

