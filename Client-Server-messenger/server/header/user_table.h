#ifndef USER_TABLE_H
#define USER_TABLE_H
#include <string>
#include <fstream>
#include <istream>
#include "iostream"
#include <vector>
#include <string>
#include <regex>
#include <thread>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "../header/split.h"
using namespace std;
class User_table
{
    public:
    void create_table();
    void insert_user(vector<string> vars);
    void update_user(vector<string> conditions, vector<string> changes, char and_or);
    void delete_user(vector<string> conditions, char and_or);
    int findIndex(string s);
    bool checkcondition1(string condition, vector<string> matches, smatch regexmatch, int index);
    void select_user(vector<string> conditions, vector<string> columns, char and_or);
    fstream user_file;
    mutex user_mtx;
};
class User_object
{
    public:
        string username;
        string password;
        string name;
        bool is_online;
        int ID;
        int client_socket;
        thread*  client_thread;
        User_object(int id, int client_socket);
        void set_value(string n, string user , string pass, bool is_on);
        void delete_thread(User_object* user_object);
};

#endif