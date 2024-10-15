#ifndef TIMESTAMP_H
#define TIMESTAMP_H

#include <iostream>
#include <regex>
#include <string>
#include <sstream>
using namespace std;

class timestamp
{
    public:
        int hour;
        int minute;
        int second;
        timestamp(string time);
        bool operator< (timestamp t);
        bool operator> (timestamp t);
        bool operator== (timestamp t);
};        
#endif