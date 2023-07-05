#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <thread>
#include "server.h"
using namespace std;

void Server::create_server()
{
    int portNum = 8899;
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htons(INADDR_ANY);
    server_addr.sin_port = htons(portNum);

}

void Server::listening(int socket)
{
    while(1)
    {
        socklen_t size;
        int newsockfd;
        struct sockaddr_in server_addr;
        size = sizeof(server_addr);

        listen(socket, 5);
        newsockfd = accept(socket,(struct sockaddr *)&server_addr ,&size);
        if(newsockfd < 0){
            std::cerr<<"Error: Failed to connect to incoming connection.\n";
        }
        std::thread t1(&Server::handle_connection, this, newsockfd);
        t1.detach();
    }
}

void Server::handle_connection(int server)
{
    socklen_t size;
    int client;
    bool isExit = false;
    int bufsize = 1024;
    char buffer[bufsize];
    struct sockaddr_in server_addr;
    client = socket(AF_INET, SOCK_STREAM, 0);
    size = sizeof(server_addr);

    if (client < 0) 
    {
        cout << "\nError establishing socket..." << endl;
        exit(1);
    }

    cout << "\n=> Socket server has been created..." << endl;

    if (bind(client, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) 
    {
        cout << "=> Error binding connection, the socket has already been established..." << endl;
        return;
    }
    size = sizeof(server_addr);
    cout << "=> Looking for clients..." << endl;
    int clientCount = 1;
    server = accept(client,(struct sockaddr *)&server_addr,&size);

    if (server < 0) 
        cout << "=> Error on accepting..." << endl;


    while (server > 0) 
    {
        strcpy(buffer, "=> Server connected...\n");
        send(server, buffer, bufsize, 0);
        cout << "=> Connected with the client #" << clientCount << ", you are good to go..." << endl;
        cout << "\n=> Enter # to end the connection\n" << endl;
        cout << "Client: ";
        do {
            recv(server, buffer, bufsize, 0);
            cout << buffer << " ";
            if (*buffer == '#') {
                *buffer = '*';
                isExit = true;
            }
        } while (*buffer != '*');

        do {
            cout << "\nServer: ";
            do {
                cin >> buffer;
                send(server, buffer, bufsize, 0);
                if (*buffer == '#') {
                    send(server, buffer, bufsize, 0);
                    *buffer = '*';
                    isExit = true;
                }
            } while (*buffer != '*');

            cout << "Client: ";
            do {
                recv(server, buffer, bufsize, 0);
                cout << buffer << " ";
                if (*buffer == '#') {
                    *buffer = '*';
                    isExit = true;
                }
            } while (*buffer != '*');
        } while (!isExit);
        cout << "\n\n=> Connection terminated with IP " << inet_ntoa(server_addr.sin_addr);
        close(server);
        cout << "\nGoodbye..." << endl;
        isExit = false;
        exit(1);
    }

    close(client);
}
