#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <gtest/gtest.h>

class Client
{
    public:
        
      static void run_udp_client(Server &server) {          

      int client_socket = socket(AF_INET, SOCK_DGRAM, 0);

      if (client_socket == -1) {
          perror("Socket oluşturma başarısız.");
          return;
      }

      struct sockaddr_in server_address ;
      server_address.sin_family = AF_INET;
      server_address.sin_port = htons(3000);
      server_address.sin_addr.s_addr = htonl(INADDR_ANY);
        
      server.srvaddr=server_address;

      std::string message = "Hello from client!";

      int bytes_sent = sendto(client_socket, message.c_str(), message.length(), 0,
                          (struct sockaddr*)&server_address, sizeof(server_address));
      EXPECT_GE(bytes_sent,0);                        

      std::this_thread::sleep_for(std::chrono::milliseconds(100));

      struct sockaddr_in client_address ;
      client_address.sin_family = AF_INET;
      client_address.sin_port = htons(3000);
      client_address.sin_addr.s_addr = htonl(INADDR_ANY);
      server.clntaddr=client_address;

      struct client_info *info = new client_info;
      info->client_port = ntohs(server.clntaddr.sin_port);
      info->client_ip = inet_ntoa(server.clntaddr.sin_addr);
      info->server_port = server.servPort;
      info->fd =server.servFd;
    
      server.item_srv_ip = inet_ntoa(server.srvaddr.sin_addr);

      server.send_server(info);

        char response_buffer[MAXLINE];
        
        ssize_t num_bytes_received = recvfrom(server.servFd, response_buffer, MAXLINE, 0, NULL, NULL);
        EXPECT_GE(num_bytes_received,0);

        response_buffer[num_bytes_received] = '\0';

        server.writeFile(response_buffer);

        close(client_socket);
      }
    


};




