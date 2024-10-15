#ifndef BLOCK_LIST_TABLE_H
#define BLOCK_LIST_TABLE_H
#include <string>
#include <fstream>
#include <istream>
#include "iostream"
#include <vector>
#include <string>
#include <regex>
#include "../header/split.h"
using namespace std;
class block_list_table
{
    public:
        void insert_blocked(vector<string> vars);
        void delete_blocked(vector<string> condition, char and_or);
        int findIndex(string s);
        bool checkcondition1(string condition, vector<string> matches, smatch regexmatch, int index);
        fstream blocked_list_file;
        void update_blocked(vector<string> conditions, vector<string> changes, char and_or);
        void select_blocked(vector<string> conditions, vector<string> columns, char and_or);
        mutex block_mtx;

};
class block_object
{
    public:
        int user_id;
        int blocked_user_id;

};
#endif