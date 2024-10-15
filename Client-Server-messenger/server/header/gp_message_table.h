#ifndef GP_MESSAGE_TABLE_H
#define GP_MESSAGE_TABLE_H
#include <string>
#include <fstream>
#include <istream>
#include "iostream"
#include <vector>
#include <string>
#include <regex>
#include "../header/split.h"
#include "../header/timestamp.h"
using namespace std;
class gp_message_table
{
    public:
        void insert_gp_message(vector<string> vars);
        void delete_gp_message(vector<string> condition, char and_or);
        void update_gp_message(vector<string> conditions, vector<string> changes, char and_or);
        int findIndex(string s);
        bool checkcondition1(string condition, vector<string> matches, smatch regexmatch, int index);
        void select_gp_message(vector<string> conditions, vector<string> columns, char and_or);
        fstream gp_message_file;
        mutex gp_message_mtx;
};
class gp_message_object
{
    public:
        int ID;
        int sender_id;
        int gp_id;
        string text;
        string time;

};
#endif