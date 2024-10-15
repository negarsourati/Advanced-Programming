#ifndef CLIENT_H
#define CLIENT_H

#include "iostream"
#include <sys/types.h>
#include <sys/socket.h>
#include <mutex>
#include <unistd.h>
#include <thread>
#include <netinet/in.h>

using namespace std;

class Client
{
public:
    Client();
    void start_connecting();
    void start_communicating();
    void close_connection();
    void login();
    static void send_handler(Client* client);
    static void recv_handler(Client* client);
    void multi_print(string message, bool you);
    ~Client();
    int max_len;
    bool logged_in;
    bool exited;
    char* name;
    char* username;
    char* password;
    int client_socket;
    mutex print_mtx;
    thread* send_thread;
    thread* recv_thread;
    string menu = "1.change_name\n2.send_pv\n";

};
#endif