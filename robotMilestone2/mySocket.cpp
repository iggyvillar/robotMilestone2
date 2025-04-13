#include "mySocket.h"
#include <iostream>


MySocket::MySocket(SocketType socketType, std::string ipAddress, unsigned int portNumber, ConnectionType connection_Type, unsigned int bufferSize = DEFAULT_SIZE){

    mySocket = socketType;
    IPAddr = ipAddress;
    port = portNumber;
    connectionType = connection_Type;
    maxSize = (bufferSize > 0) ? bufferSize : DEFAULT_SIZE;

    Buffer = new char[maxSize];

    // initialize winsock
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    //create socket based on connection type (TCP or UDP)
    if (connectionType == ConnectionType::UDP){
        connectionSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    }
    else{
        connectionSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    }
    
    

}
