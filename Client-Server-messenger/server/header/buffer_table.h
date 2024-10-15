#ifndef BUFFER_TABLE_H
#define BUFFER_TABLE_H
#include <string>
#include <fstream>
#include <istream>
#include "iostream"
#include <vector>
#include <string>
#include <regex>
#include "../header/split.h"
using namespace std;
class Buffer_table
{
    public:
    void insert_buffer(vector<string> vars);
    void delete_buffer(vector<string> condition, char and_or);
    int findIndex(string s);
    bool checkcondition1(string condition, vector<string> matches, smatch regexmatch, int index);
    fstream buffer_file;
    void update_buffer(vector<string> conditions, vector<string> changes, char and_or);
    void select_buffer(vector<string> conditions, vector<string> columns, char and_or);
    mutex buffer_mtx;
};
class buffer_object
{
    public:
    int user_id;
    int message_id;
    bool is_group;
    int ID;

};

#endif
