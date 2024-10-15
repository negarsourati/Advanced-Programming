#include <iostream>
#include <vector>
#include <map>
#include <mutex>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fstream>
#include <thread>
#include "../header/commands.h"
using namespace std;

class Server
{
    public:
        Server();
        void start_listening();
        void start_accepting();
        bool login(User_object* user_object);
        void add_client(User_object* user_object);
        void get_user_from_file();
        void delete_all_clients();
        void check_user(int id, bool connected);
        void create_new_group(vector<int> members, string group_name,string desc, int group_id);
        void add_member(int user_id, int group_id);
        void remove_member(int user_id,  int group_id);
        void change_name(int id);
        void exit(int id);
        void block(int user_id);
        void unblock(int user_id);
        vector<string> get_group_members(int group_id);
        vector<string> joined_groups(int user_id);
        void send_message(int client_socket, string text);
        void send_pv(int id, User_object* sender, User_object* reciver, string text, string time);
        void check_group(int id, bool exist);
        void multi_print(string message);
        vector<string> all_contacts(int user_id);
        static void handle_client(Server* server,int id);
        void end_connection(int id);
        void close_connection();
        void do_for_user(int id, string message);
        ~Server();
        map<int, User_object*> clients;
        map<int, Group_object*> groups;
        int unique_id;
        int max_len;
        int server_socket;
        mutex print_mtx, clients_mtx;
        int ID_user = 0;
};