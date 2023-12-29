#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <memory>
#include <fstream>
#include"ThreadPool.h"
#define MAXLINE 1024
/** 
 * @def MAX_EVENTS 10
 * @brief Specifies the number of events that can be processed at once in the epoll mechanism.
 */
#define MAX_EVENTS 10

/**
 * @brief A class to represents a udp server.
 *
 * This class provides the core functionality of a network server and serves clients using a message queue
 * and a thread pool.
 */
class Server
{
    public:
 
    /**
     * @brief The thread pool object provides parallel processing capabilities for the server.
     */
    ThreadPool *threadPool = ThreadPool::getInstance();
   
    /**
     * @brief Default constructor for the Server class.
     * 
     * This constructor creates a Server object with default parameters.
     */
    Server()=default;

    /**
     * @brief Creates a Server object with the specified port.
     * 
     * @param port: The port on which the server will listen.
     */
    Server(int port)
    {
        clintPort = port;
        servPort = port;
    }

    /**
     * @brief Creates a socket for the server.
     */
    int create_socket();

    /**
     * @brief Binds the socket to the server's address.
     * @return The result of the bind operation.
     */
    int bind_des();

    /**
     * @brief Initializes the server's address structure.
     */
    void serv_init();

    /**
     * @brief Sets up the epoll event loop for the server.
     */
    void epoll_form();

    /**
     * @brief Handles a client's request.
     */
    void* handleClient(void *arg);

    /**
     * @brief Receives data from clients and processes it.
     */
    ssize_t recv_server();

    /**
     * @brief Sends data to clients.
     */
    ssize_t send_server(struct client_info* info);

    int servFd{};
    int clintFd{};
    int clintPort{};
    int servPort{};
    int epollFd{};
    char* item_srv_ip;

    void writeFile(const char* data);

    
    /**
     * @brief The sockaddr_in struct is used to store IPv4 server information.
     *
     * This struct contains information about the server's and client's IPv4 address and port number.
     */
    struct sockaddr_in srvaddr,clntaddr{}; 
    struct client_info *info{};
    /**
     * @brief The epoll_event struct represents an event in the epoll event loop.
     *
     * This struct is used to manage and monitor events that occur on file descriptors.
     * It is part of the epoll-based event handling mechanism used in the server.
     */
    struct epoll_event event;
    /**
     * @brief An array of epoll_event structs to store multiple events.
     *
     * This array is used to store and manage multiple events in the epoll event loop.
     * It can hold up to MAX_EVENTS events.
     */
    struct epoll_event events[MAX_EVENTS];
    
    socklen_t clientlen = sizeof(clntaddr);
    char buffer[MAXLINE];
    pthread_t thread_id;
    


};
