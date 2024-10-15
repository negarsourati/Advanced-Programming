#ifndef GROUP_TABLE_H
#define GROUP_TABLE_H
#include <string>
#include <fstream>
#include <istream>
#include "iostream"
#include <vector>
#include <string>
#include <regex>
#include "../header/split.h"
using namespace std;
class Group_table
{
    public:
    
        void insert_group(vector<string> vars);
        void delete_group(vector<string> condition, char and_or);
        void update_group(vector<string> conditions, vector<string> changes, char and_or);
        int findIndex(string s);
        bool checkcondition1(string condition, vector<string> matches, smatch regexmatch, int index);
        void select_group(vector<string> conditions, vector<string> columns, char and_or);
        fstream group_file;
        mutex group_mtx;
};
class Group_object
{
    public:
        Group_object(int _ID, string _name, string _desc, int _member_count);
        string name;
        int member_count;
        string description;
        int ID;
};

#endif
