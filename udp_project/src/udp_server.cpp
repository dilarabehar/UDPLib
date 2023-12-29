/**
 * @file udp_server.cpp
 * @author Dilara
 * @brief This file handle the server operations.
 * @version 0.1
 * @date 2023-09-20
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "../include/Server.h"
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <string.h>
#include <memory>
#include <arpa/inet.h>
#include <mutex>
#include <fstream>

struct client_info {
    int fd;              /**< File descriptor for the client connection. */
    int client_port;     /**< Port number of the client. */
    char *client_ip;     /**< IP address of the client. */
    int server_port;     /**< Port number of the server. */
};

/**
 * This function creates a socket for the server to use for communication.
 * 
 * @return The socket file descriptor.
*/
int Server::create_socket()
{
    servFd = socket(AF_INET,SOCK_DGRAM,0);

    if(servFd<0)
    {
        perror("socket function failed!");
        exit(EXIT_FAILURE);
    }

    memset(&srvaddr, 0, sizeof(srvaddr));

    return servFd;
}

/**
 * This function binds the socket to the server's address and port.
*/
int Server::bind_des()
{
    int bind_success{};

    bind_success = bind(servFd, (const struct sockaddr *)&srvaddr,sizeof(srvaddr));
    if(bind_success<0)
    {
        perror("bind function failed!\n");
        exit(EXIT_FAILURE);
    }

    return bind_success;
}

/**
 * This function sets up the server's address structure with the specified port.
*/
void Server::serv_init()
{
    srvaddr.sin_family = AF_INET; 
    srvaddr.sin_addr.s_addr = INADDR_ANY; 
    srvaddr.sin_port = htons(servPort);
}

/**
 * This function creates an epoll instance and adds the server socket to it for event monitoring using epoll_ctl.
 * The server socket will be monitored for EPOLLIN (input readiness) and EPOLLET (edge-triggered) events.
 * 
 * @note EPOLL_CTL_ADD is used to add the server socket to the epoll instance.
 *
 * @see epoll_ctl()
*/
void Server::epoll_form()
{
    epollFd = epoll_create1(0);

    if (epollFd == -1) {
        perror("Epoll creation failed");
        exit(EXIT_FAILURE);
    }
    
    event.events = EPOLLIN | EPOLLET;
    event.data.fd = servFd;

    if (epoll_ctl(epollFd, EPOLL_CTL_ADD, servFd, &event) == -1) {
        perror("Epoll control failed");
        exit(EXIT_FAILURE);
    }

}

/**
 * @brief Entry point for handling a client's request in a separate thread.
 *
 * This function is the entry point for handling a client's request in a separate thread. It invokes the handleClient method
 * of the Server class to process the client's request.
 *
 * @param arg: A pointer to a client_info structure.
 * @return NULL
 */
void * handlePass(void *arg){
    
    Server server;
    server.handleClient(arg);
    
    return NULL;
}

/**
 * This function processes a client's request and performs necessary operations.
 * 
 * @param arg: A pointer to a client_info structure.
 * @return NULL
 * 
*/
void* Server::handleClient(void *arg) {

    struct client_info *info = (struct client_info *)arg;
    return NULL;
}

/**
 * This function waits for incoming events using epoll and processes data received from clients.
 * For each client request, it creates a new thread to handle the request in parallel. The function also
 * prints relevant information about the received data.
 *
 * @return The number of bytes received, or 0 on successful execution.
 *
 * @details This function uses the epoll_wait function to wait for incoming events on the server socket.
 * When data is received from a client, it creates a new thread to handle the client's request using
 * the handlePass function. The received message is encapsulated in a MsgQueueItem object and pushed to
 * the ThreadPool's message queue for further processing. The function also prints information about the
 * source and destination ports, client and server IP addresses, and the received message.
 *
 * @see epoll_wait()
 * @see handlePass()
 * @see send_server()
 * @see MsgQueueItem
 * @see ThreadPool
*/
ssize_t Server::recv_server()
{
    memset(&srvaddr, 0, sizeof(srvaddr));

    while (true) {
        
        int numEvents = epoll_wait(epollFd, events, MAX_EVENTS, -1); 

        if (numEvents == -1) {
            perror("Epoll wait failed");
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < numEvents; ++i) {

            if (events[i].data.fd == servFd) {
                
                ssize_t numof_byte_recv{};
                numof_byte_recv = recvfrom(servFd, (char*)buffer, MAXLINE, 0, (struct sockaddr*)&clntaddr, &clientlen);
                                
                if (numof_byte_recv == -1) {
                    perror("Receive failed");
                    exit(EXIT_FAILURE);
                }
                        
                buffer[numof_byte_recv] = '\0';

                struct client_info *info = new client_info;
                info->client_port = ntohs(clntaddr.sin_port);
                info->client_ip = inet_ntoa(clntaddr.sin_addr);
                info->server_port = servPort;
                info->fd =servFd;
                item_srv_ip = inet_ntoa(srvaddr.sin_addr);

                writeFile(buffer);

                std::shared_ptr<MsgQueueItem> shared_ptr = std::make_shared<MsgQueueItem>(info->client_ip,item_srv_ip,info->client_port,info->server_port,buffer,MAXLINE);

                threadPool->PushQueue(shared_ptr);
               
                pthread_create(&thread_id,NULL, handlePass, info);
                pthread_detach(thread_id);

                send_server(info);
            }
        }
        epoll_form();
    }
    pthread_join(thread_id,0);
    return 0;
}

/**
 * This function sends data to clients using the thread pool for parallel processing.
 * 
 * @param info: A pointer to a client_info structure.
 * @return The number of bytes sent.
*/
ssize_t Server::send_server(struct client_info *info )
{
    threadPool->workerThread();

    threadPool->ondata_pass->sourceIP = info->client_ip;
    threadPool->ondata_pass->destinationIP = item_srv_ip;
    threadPool->ondata_pass->sourcePort = info->client_port;
    threadPool->ondata_pass->destinationPort = info->server_port;
    
    std::string hello = "Hello from server!\n";
    
    ssize_t numof_byte_sent = sendto(servFd, hello.c_str(), hello.size(), 
        0, (const struct sockaddr *) &clntaddr,clientlen);

    if(numof_byte_sent<0)
    {
        perror("sendto function failed!");
        exit(EXIT_FAILURE);
    }
   return numof_byte_sent;
}


void Server::writeFile(const char* data)
{
    std::ofstream file("package_output.txt", std::ios::app); 

    if (file.is_open())
    {
        file << data << std::endl;
        file.close();

    }else{
        perror("File error");
    }
}

/**
 * @brief Entry point for the server.
 *
 * This function is the entry point for the server and sets up the necessary components to start listening for client connections.
 *
 * @param arg: A pointer to a Server object.
 * @return NULL
 */
void *server_start(void *arg)
{
    Server *server = (Server *)arg;
    server->create_socket();
    server->serv_init();
    server->bind_des();
    server->epoll_form();
    server->recv_server();
    return NULL;
}


int main(int argc, char** argv)
{   

     pthread_t thread1;
    Server server(3000);
    
    pthread_create(&thread1, NULL,server_start,&server);
    pthread_join(thread1,NULL);

   
}
























