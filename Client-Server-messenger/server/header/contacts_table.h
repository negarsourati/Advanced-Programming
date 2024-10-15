#ifndef CONTACTS_TABLE_H
#define CONTACTS_TABLE_H
#include <string>
#include <fstream>
#include <istream>
#include "iostream"
#include <vector>
#include <string>
#include <regex>
#include "../header/split.h"
using namespace std;
class contacts_table
{
    public:
        void insert_contact(vector<string> vars);
        void delete_contact(vector<string> condition, char and_or);
        int findIndex(string s);
        bool checkcondition1(string condition, vector<string> matches, smatch regexmatch, int index);
        fstream contacts_file;
        void update_contact(vector<string> conditions, vector<string> changes, char and_or);
        void select_contact(vector<string> conditions, vector<string> columns, char and_or);
        mutex contact_mtx;
};
// class contact_object
// {
//     public:
//         User_object* user;
//         User_object* friend;
//         int ID;
//         contact_object(User_object* user, User_object* friend);

// };
#endif