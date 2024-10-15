#include "../header/timestamp.h"

timestamp::timestamp(string time)
{
    regex timepat("([0-9]+):([0-9]+):([0-9]+)");
    smatch matches;
    regex_search(time, matches, timepat);
    hour = stoi(matches[1]);
    minute = stoi(matches[2]);
    second = stoi(matches[3]);
}

bool timestamp::operator< (timestamp t)
{
    if(this->hour < t.hour)
        return true;
    if(this->hour == t.hour && this->minute < t.minute)
        return true;
    if(this->hour == t.hour && this->minute == t.minute && this->second < t.second)   
        return true;
    return false;     
}
bool timestamp::operator> (timestamp t)
{
    if(this->hour > t.hour)
        return true;
    if(this->hour == t.hour && this->minute > t.minute)
        return true;
    if(this->hour == t.hour && this->minute == t.minute && this->second > t.second)   
        return true;
    return false;     
}
bool timestamp::operator== (timestamp t)
{
    return (this->hour == t.hour && this->minute == t.minute && this->second == t.second);
}