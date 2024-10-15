#include "../header/split.h"
vector<string> split(string S, char delimiter)
{
    string temp = "";
    vector<string> v;
    for(int i = 0; i < S.size(); i++)
    {
        if(S[i] != delimiter)
        {
            temp += S[i];
        }
        else
        {
            v.push_back(temp);
            temp = "";
        }
    }
    v.push_back(temp);
    return v;
}
