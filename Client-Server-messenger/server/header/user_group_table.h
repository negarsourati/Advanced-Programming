#ifndef USER_GROUP_TABLE_H
#define USER_GROUP_TABLE_H
#include <string>
#include <fstream>
#include <istream>
#include "iostream"
#include <vector>
#include <string>
#include <regex>
#include "../header/split.h"
using namespace std;
class user_group_table
{
    public:
        void insert_user_group(vector<string> vars);
        void delete_user_group(vector<string> condition, char and_or);
        int findIndex(string s);
        bool checkcondition1(string condition, vector<string> matches, smatch regexmatch, int index);
        fstream user_group_file;
        void update_user_group(vector<string> conditions, vector<string> changes, char and_or);
        void select_user_group(vector<string> conditions, vector<string> columns, char and_or);
        mutex user_group_mtx;
};
class user_group_object
{
    public:
        int user_id;
        int group_id;
};
#endif