#include "gtest/gtest.h"
#include <arpa/inet.h>
#include <fstream>
#include "../src/udp_server.cpp"
#include"udp_client.h"
//#include"../include/Server.h"

class ServerTest : public testing::Test {
 public:


};

TEST_F(ServerTest, TestCreateSocket) {

    // create_socket işlemini çağırın (test edilecek işlev)
    Server server;
    int result = server.create_socket();
    EXPECT_GE(result, 0); //sonucun sıfıra eşit veya büyük olmasını bekliyoruz.

}


TEST_F(ServerTest, TestServInit) {
    Server server(3000);
    server.serv_init();
    struct sockaddr_in servAddr = server.srvaddr;
    EXPECT_EQ(servAddr.sin_family, AF_INET);
    EXPECT_EQ(servAddr.sin_addr.s_addr, htonl(INADDR_ANY));
    EXPECT_EQ(servAddr.sin_port, htons(3000));
}

TEST_F(ServerTest, TestBindDes) {

    Server server(3000);
    server.create_socket();
    int result = server.bind_des();
    EXPECT_EQ(result, 0);

}

TEST_F(ServerTest, EpollForm) {

    Server server;
    server.epoll_form();
    EXPECT_GE(server.epollFd, 0);

}


TEST_F(ServerTest, RecvServer) {
  
  Server server(3000);
  int server_socket = server.create_socket();
  server.servFd =server_socket;
  server.serv_init();
  server.bind_des();

  std::thread recvThread([&]() {
    server.epoll_form();
    server.recv_server();
  });

    Client::run_udp_client(server);
    
    pthread_cancel(recvThread.native_handle());

    close(server.servFd);
    close(server.epollFd);
  
  recvThread.join();
}

/*
int main(int argc, char** argv) {

    ::testing::InitGoogleTest(&argc, argv);
    testing::FLAGS_gtest_filter = "ServerTest*"; 
    return RUN_ALL_TESTS();

}

*/










