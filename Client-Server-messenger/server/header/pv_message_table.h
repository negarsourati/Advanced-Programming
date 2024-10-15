#ifndef PV_MESSAGE_TABLE_H
#define PV_MESSAGE_TABLE_H
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
class pv_message_table
{
    public:
        int ID;
        int contact_id;
        string text;
        string time;
        
        void insert_pv_message(vector<string> vars);
        void delete_pv_message(vector<string> condition, char and_or);
        int findIndex(string s);
        bool checkcondition1(string condition, vector<string> matches, smatch regexmatch, int index);
        fstream pv_message_file;
        void update_pv_message(vector<string> conditions, vector<string> changes, char and_or);
        void select_pv_message(vector<string> conditions, vector<string> columns, char and_or);
        mutex pv_message_mtx;
};
class pv_message_object
{
    public:
        int ID;
        int contact_id;
        string text;
        string time;

};
#endif