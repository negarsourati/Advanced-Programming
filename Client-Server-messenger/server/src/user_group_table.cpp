#include "../header/user_group_table.h"
int user_group_table::findIndex(string s)
{
    int index;
    if(s == "user_id")
    {   
        index = 0;
    }
    else if(s == "group_id")
    {
        index = 1;
    }
    return index;
}
void user_group_table::insert_user_group(vector<string> vars)
{
    lock_guard<mutex> guard(user_group_mtx);
    user_group_file.open("files/user_group.txt", ios::app);
    for(int i = 0; i < vars.size(); i++)
    {
        if(i == vars.size() - 1)
            user_group_file<<vars[i]<<"\n";
        else
            user_group_file<<vars[i]<<","; 
    }
    user_group_file.close();
}
void user_group_table::delete_user_group(vector<string> conditions, char and_or)
{
    lock_guard<mutex> guard(user_group_mtx);
    fstream temp_file;
    temp_file.open("temp.txt", ios::app | ios::out);
    user_group_file.open("files/user_group.txt", ios::in);
    regex conditionpat("\\s*(.+)\\s*(<|==|>)\\s*(.+)\\s*");
    if(conditions.size() == 1)
    {
        string line;
        smatch regexmatch;
        regex_search(conditions[0], regexmatch, conditionpat);
        int index = findIndex(regexmatch[1]);
        while(getline(user_group_file, line))
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
            while(getline(user_group_file, line))
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
            while(getline(user_group_file, line))
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
    user_group_file.close();
    temp_file.close();
    remove("files/user_group.txt");
    rename("temp.txt", "files/user_group.txt");
}
bool user_group_table::checkcondition1(string condition, vector<string> matches, smatch regexmatch, int index)
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
    }
    return false;
}
void user_group_table::update_user_group(vector<string> conditions, vector<string> changes, char and_or)
{
    lock_guard<mutex> guard(user_group_mtx);
    smatch condmatches;
    smatch changematches;
    regex conditionpat("\\s*(.+)\\s*(<|==|>)\\s*(.+)\\s*");
    vector<string> newValues(2);
    for(int i = 0; i < 2; i++)
        newValues[i] = "";
    for(int i = 0; i < changes.size(); i++)
    {
        regex_search(changes[i], changematches, conditionpat);
        if(findIndex(changematches[1]) == 0)
        {
            newValues[0] = changematches[3];
        }
        else
        {
            newValues[1] = changematches[3];
        }
    }
    fstream temp_file;
    temp_file.open("temp.txt", ios::app | ios::out);
    user_group_file.open("files/user_group.txt", ios::in);
    if(conditions.size() == 1)
    {
        string line;
        smatch regexmatch;
        regex_search(conditions[0], regexmatch, conditionpat);
        int index = findIndex(regexmatch[1]);
        while(getline(user_group_file, line))
        {
            vector<string> matches = split(line, ',');
            if(checkcondition1(conditions[0], matches, regexmatch, index))
            {
                string newline = "";
                for(int i = 0; i < 2; i++)
                {
                    if(newValues[i] != "")
                        newline += newValues[i] +",";
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
            while(getline(user_group_file, line))
            {
                vector<string> matches = split(line, ',');
                if((checkcondition1(conditions[0], matches, regexmatch1, index1) && checkcondition1(conditions[1], matches, regexmatch2, index2)))
                {
                    string newline = "";
                    for(int i = 0; i < 2; i++)
                    {
                        if(newValues[i] != "")
                            newline += newValues[i] +",";
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
            while(getline(user_group_file, line))
            {
                vector<string> matches = split(line, ',');
                //cout << matches[3] << endl;
                if((checkcondition1(conditions[0], matches, regexmatch1, index1) || checkcondition1(conditions[1], matches, regexmatch2, index2)))
                {
                    string newline = "";
                    for(int i = 0; i < 2; i++)
                    {
                        if(newValues[i] != "")
                            newline += newValues[i] +",";
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
    user_group_file.close();
    temp_file.close();
    remove("files/user_group.txt");
    rename("temp.txt", "files/user_group.txt");
}
void user_group_table::select_user_group(vector<string> conditions, vector<string> columns, char and_or)
{
    lock_guard<mutex> guard(user_group_mtx);
    smatch condmatches;
    smatch changematches;
    regex conditionpat("\\s*(.+)\\s*(<|==|>)\\s*(.+)\\s*");
    vector<int> col(2);
    for(int i = 0; i < 2; i++)
        col[i] = 0;
    if(columns[0] == "*")
    {
        for(int i = 0; i < 2; i++)
        col[i] = 1;
    }  
    else
    {  
        for(int i = 0; i < columns.size(); i++)
        {
            if(findIndex(columns[i]) == 0)
            {
                col[0] = 1;
            }
            else
            {
                col[1] = 1;
            }
        }  
    }    
    fstream selected_user_group_file;
    selected_user_group_file.open("files/selected_user_group.txt", ios::app | ios::out);
    user_group_file.open("files/user_group.txt", ios::in);
    if(conditions.size() == 1)
    {
        string line;
        smatch regexmatch;
        regex_search(conditions[0], regexmatch, conditionpat);
        int index = findIndex(regexmatch[1]);
        while(getline(user_group_file, line))
        {
            vector<string> matches = split(line, ',');
            if(checkcondition1(conditions[0], matches, regexmatch, index))
            {
                string newline = "";
                for(int i = 0; i < 2; i++)
                {
                    if(col[i] != 0)
                        newline += matches[i] +",";  
                }
                newline.pop_back();
                selected_user_group_file << newline << "\n";
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
            while(getline(user_group_file, line))
            {
                vector<string> matches = split(line, ',');
                if((checkcondition1(conditions[0], matches, regexmatch1, index1) && checkcondition1(conditions[1], matches, regexmatch2, index2)))
                {
                    string newline = "";
                    for(int i = 0; i < 2; i++)
                    {
                        if(col[i] != 0)
                            newline += matches[i] +",";  
                    }
                    newline.pop_back();
                    selected_user_group_file << newline << "\n";
                }
            }
        }
        else if(and_or == '|')
        {
            while(getline(user_group_file, line))
            {
                vector<string> matches = split(line, ',');
                //cout << matches[3] << endl;
                if((checkcondition1(conditions[0], matches, regexmatch1, index1) || checkcondition1(conditions[1], matches, regexmatch2, index2)))
                {
                    string newline = "";
                    for(int i = 0; i < 2; i++)
                    {
                        if(col[i] != 0)
                            newline += matches[i] +",";   
                    }
                    newline.pop_back();
                    selected_user_group_file << newline << "\n";
                }
            }
        }
    }
    user_group_file.close();
    selected_user_group_file.close();  
}

