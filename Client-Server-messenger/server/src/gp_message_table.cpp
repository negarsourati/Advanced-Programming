#include "../header/gp_message_table.h"
int gp_message_table::findIndex(string s)
{
    int index;
    if(s == "ID")
    {   
        index = 0;
    }
    else if(s == "sender_id")
    {
        index = 1;
    }
    else if(s == "gp_id")
    {
        index = 2;
    }
    else if(s == "text")
    {
        index = 3;
    }
    else
    {
        index = 4;
    }
    return index;
}
void gp_message_table::insert_gp_message(vector<string> vars)
{
    lock_guard<mutex> guard(gp_message_mtx);
    gp_message_file.open("files/gp_message.txt", ios::app);
    for(int i = 0; i < vars.size(); i++)
    {
        if(i == vars.size() - 1)
            gp_message_file<<vars[i]<<"\n";
        else
            gp_message_file<<vars[i]<<",";    
    }
    gp_message_file.close();
}
void gp_message_table::delete_gp_message(vector<string> conditions, char and_or)
{
    lock_guard<mutex> guard(gp_message_mtx);
    fstream temp_file;
    temp_file.open("temp.txt", ios::app | ios::out);
    gp_message_file.open("files/gp_message.txt", ios::in);
    regex conditionpat("\\s*(.+)\\s*(<|==|>)\\s*(.+)\\s*");
    if(conditions.size() == 1)
    {
        string line;
        smatch regexmatch;
        regex_search(conditions[0], regexmatch, conditionpat);
        int index = findIndex(regexmatch[1]);
        while(getline(gp_message_file, line))
        {
            vector<string> matches = split(line, ',');
            if(!checkcondition1(conditions[0], matches, regexmatch, index))
            {
                temp_file << line << "\n";
            }
        }
    }
    else
    {
        smatch regexmatch1;
        regex_search(conditions[0], regexmatch1, conditionpat);
        int index1 = findIndex(regexmatch1[1]);
        smatch regexmatch2;
        regex_search(conditions[1], regexmatch2, conditionpat);
        int index2 = findIndex(regexmatch2[1]);
        //cout << index1 << index2 << endl;
        string line;
        if(and_or == '&')
        {
            while(getline(gp_message_file, line))
            {
                vector<string> matches = split(line, ',');
                if(!(checkcondition1(conditions[0], matches, regexmatch1, index1) && checkcondition1(conditions[1], matches, regexmatch2, index2)))
                {
                    temp_file << line << "\n";
                }
            }
        }
        else if(and_or == '|')
        {
            while(getline(gp_message_file, line))
            {
                vector<string> matches = split(line, ',');
                //cout << matches[3] << endl;
                if(!(checkcondition1(conditions[0], matches, regexmatch1, index1) || checkcondition1(conditions[1], matches, regexmatch2, index2)))
                {
                    temp_file << line << "\n";
                }
            }
        }
    }
    gp_message_file.close();
    temp_file.close();
    remove("files/gp_message.txt");
    rename("temp.txt", "files/gp_message.txt");
}
bool gp_message_table::checkcondition1(string condition, vector<string> matches, smatch regexmatch, int index)
{
    
    if(regexmatch[2] == '>')
    {
    
    if(index == 0)
    {
        
        if((stoi(matches[index]) > stoi(regexmatch[3])))
        {
            return true;
        }
    }
    if(index == 1)
    {
       
        if((stoi(matches[index]) > stoi(regexmatch[3])))
        {
            return true;
        }
    }
    if(index == 2)
    {
        if((stoi(matches[index]) > stoi(regexmatch[3])))
        {
           return true;
        }
    }
    if(index == 3)
    {
        
        if(((matches[index]) > (regexmatch[3])))
        {
            return true;
        }
    }
    if(index == 4)
    {
        timestamp t1(matches[index]);
        timestamp t2(regexmatch[3]);
        if(t1 > t2)
            return true;
    }
    
    }
    else if(regexmatch[2] == '<')
    {
        
    if(index == 0)
    {
        if((stoi(matches[index]) < stoi(regexmatch[3])))
        {
            return true;
        }
    }
    if(index == 1)
    {
        if((stoi(matches[index]) < stoi(regexmatch[3])))
        {
            return true;
        }
    }
    if(index == 2)
    {
        if((stoi(matches[index]) < stoi(regexmatch[3])))
        {
           return true;
        }
    }
    if(index == 3)
    {
        if(((matches[index]) < (regexmatch[3])))
        {
            return true;
        }
    }
    if(index == 4)
    {
        timestamp t1(matches[index]);
        timestamp t2(regexmatch[3]);
        if(t1 < t2)
            return true;
    }
    }
    else
    {
    if(index == 0)
    {
        if((stoi(matches[index]) == stoi(regexmatch[3])))
        {
            return true;
        }
    }
    if(index == 1)
    {
        if((stoi(matches[index]) == stoi(regexmatch[3])))
        {
            return true;
        }
    }
    if(index == 2)
    {
        if(stoi(matches[index]) == stoi(regexmatch[3]))
        {
           return true;
        }
    }
    if(index == 3)
    {
        if(((matches[index]) == (regexmatch[3])))
        {
            return true;
        }
    }
    if(index == 4)
    {
        timestamp t1(matches[index]);
        timestamp t2(regexmatch[3]);
        if(t1 == t2)
            return true;
    }
    }
    return false;
}
void gp_message_table::update_gp_message(vector<string> conditions, vector<string> changes, char and_or)
{
    lock_guard<mutex> guard(gp_message_mtx);
    smatch condmatches;
    smatch changematches;
    regex conditionpat("\\s*(.+)\\s*(<|==|>)\\s*(.+)\\s*");
    vector<string> newValues(5);
    for(int i = 0; i < 5; i++)
        newValues[i] = "";
    for(int i = 0; i < changes.size(); i++)
    {
        regex_search(changes[i], changematches, conditionpat);
        if(findIndex(changematches[1]) == 0)
        {
            newValues[0] = changematches[3];
        }
        else if (findIndex(changematches[1]) == 1)
        {
            newValues[1] = changematches[3];
        }
        else if(findIndex(changematches[1]) == 2)
        {
            newValues[2] = changematches[3];
        }
        else if(findIndex(changematches[1]) == 3)
        {
            newValues[3] = changematches[3];
        }
        else
        {
            newValues[4] = changematches[3];
        }
    }
    fstream temp_file;
    temp_file.open("temp.txt", ios::app | ios::out);
    gp_message_file.open("files/gp_message.txt", ios::in);
    if(conditions.size() == 1)
    {
        string line;
        smatch regexmatch;
        regex_search(conditions[0], regexmatch, conditionpat);
        int index = findIndex(regexmatch[1]);
        while(getline(gp_message_file, line))
        {
            vector<string> matches = split(line, ',');
            if(checkcondition1(conditions[0], matches, regexmatch, index))
            {
                string newline = "";
                for(int i = 0; i < 5; i++)
                {
                    if(newValues[i] != "")
                        newline += newValues[i] + ",";
                    else
                        newline += matches[i] + ",";    
                }
                newline.pop_back();
                temp_file << newline << "\n";
            }
            else
            {
                temp_file << line << "\n";
            }
        }
    }
    else
    {
        smatch regexmatch1;
        regex_search(conditions[0], regexmatch1, conditionpat);
        int index1 = findIndex(regexmatch1[1]);
        smatch regexmatch2;
        regex_search(conditions[1], regexmatch2, conditionpat);
        int index2 = findIndex(regexmatch2[1]);
        //cout << index1 << index2 << endl;
        string line;
        if(and_or == '&')
        {
            while(getline(gp_message_file, line))
            {
                vector<string> matches = split(line, ',');
                if((checkcondition1(conditions[0], matches, regexmatch1, index1) && checkcondition1(conditions[1], matches, regexmatch2, index2)))
                {
                    string newline = "";
                    for(int i = 0; i < 5; i++)
                    {
                        if(newValues[i] != "")
                            newline += newValues[i] + ",";
                        else
                            newline += matches[i] + ",";    
                    }
                    newline.pop_back();
                    temp_file << newline << "\n";
                }
                else
                {
                    temp_file << line << "\n";
                }
            }
        }
        else if(and_or == '|')
        {
            while(getline(gp_message_file, line))
            {
                vector<string> matches = split(line, ',');
                //cout << matches[3] << endl;
                if((checkcondition1(conditions[0], matches, regexmatch1, index1) || checkcondition1(conditions[1], matches, regexmatch2, index2)))
                {
                    string newline = "";
                    for(int i = 0; i < 5; i++)
                    {
                        if(newValues[i] != "")
                            newline += newValues[i] + ",";
                        else
                            newline += matches[i] + ",";    
                    }
                    newline.pop_back();
                    temp_file << newline << "\n";
                }
                else
                {
                    temp_file << line << "\n";
                }
            }
        }
    }
    gp_message_file.close();
    temp_file.close();
    remove("files/gp_message.txt");
    rename("temp.txt", "files/gp_message.txt");
}
void gp_message_table::select_gp_message(vector<string> conditions, vector<string> columns, char and_or)
{
    lock_guard<mutex> guard(gp_message_mtx);
    smatch condmatches;
    regex conditionpat("\\s*(.+)\\s*(<|==|>)\\s*(.+)\\s*");
    vector<int> col(5);
    for(int i = 0; i < 5; i++)
        col[i] = 0;
    if(columns[0] == "*")
    {
        for(int i = 0; i < 5; i++)
        col[i] = 0;
    }  
    else
    {  
        for(int i = 0; i < columns.size(); i++)
        {
            if(findIndex(columns[i]) == 0)
            {
                col[0] = 1;
            }
            else if (findIndex(columns[i]) == 1)
            {
                col[1] = 1;
            }
            else if (findIndex(columns[i]) == 2)
            {
                col[2] = 1;
            }
            else if (findIndex(columns[i]) == 3)
            {
                col[3] = 1;
            }
            else
            {
                col[4] = 1;
            }  
        }
    }  
    fstream selected_gp_message_file;
    selected_gp_message_file.open("files/selected_gp_message.txt", ios::app | ios::out);
    gp_message_file.open("files/gp_message.txt", ios::in);
    if(conditions.size() == 1)
    {
        string line;
        smatch regexmatch;
        regex_search(conditions[0], regexmatch, conditionpat);
        int index = findIndex(regexmatch[1]);
        while(getline(gp_message_file, line))
        {
            vector<string> matches = split(line, ',');
            if(checkcondition1(conditions[0], matches, regexmatch, index))
            {
                string newline = "";
                for(int i = 0; i < 5; i++)
                {
                    if(col[i] != 0)
                        newline += matches[i] +",";
                }
                newline.pop_back();
                selected_gp_message_file << newline << "\n";
            }
        }
    }
    else
    {
        smatch regexmatch1;
        regex_search(conditions[0], regexmatch1, conditionpat);
        int index1 = findIndex(regexmatch1[1]);
        smatch regexmatch2;
        regex_search(conditions[1], regexmatch2, conditionpat);
        int index2 = findIndex(regexmatch2[1]);
        //cout << index1 << index2 << endl;
        string line;
        if(and_or == '&')
        {
            while(getline(gp_message_file, line))
            {
                vector<string> matches = split(line, ',');
                if((checkcondition1(conditions[0], matches, regexmatch1, index1) && checkcondition1(conditions[1], matches, regexmatch2, index2)))
                {
                    string newline = "";
                    for(int i = 0; i < 5; i++)
                    {
                        if(col[i] != 0)
                            newline += matches[i] +",";    
                    }
                    newline.pop_back();
                    selected_gp_message_file << newline << "\n";
                }
            }
        }
        else if(and_or == '|')
        {
            while(getline(gp_message_file, line))
            {
                vector<string> matches = split(line, ',');
                //cout << matches[3] << endl;
                if((checkcondition1(conditions[0], matches, regexmatch1, index1) || checkcondition1(conditions[1], matches, regexmatch2, index2)))
                {
                    string newline = "";
                    for(int i = 0; i < 5; i++)
                    {
                        if(col[i] != 0)
                            newline += matches[i] +",";
                    }
                    newline.pop_back();
                    selected_gp_message_file << newline << "\n";
                }
            }
        }
    }
    gp_message_file.close();
    selected_gp_message_file.close();   
}

