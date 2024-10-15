#include "../header/group_table.h"
int Group_table::findIndex(string s)
{
    int index;
    if(s == "ID")
    {   
        index = 0;
    }
    else if(s == "name")
    {
        index = 1;
    }
    else if(s == "description")
    {
        index = 2;
    }
    else if(s == "member_count")
    {
        index = 3;
    }
    return index;
}
void Group_table::insert_group(vector<string> vars)
{
    lock_guard<mutex> guard(group_mtx);
    group_file.open("files/group.txt", ios::app);
    for(int i = 0; i < vars.size(); i++)
    {
        if(i == vars.size() - 1)
            group_file<<vars[i]<<"\n";
        else
            group_file<<vars[i]<<",";    
    }
    group_file.close();
}
void Group_table::delete_group(vector<string> conditions, char and_or)
{
    lock_guard<mutex> guard(group_mtx);
    fstream temp_file;
    temp_file.open("temp.txt", ios::app | ios::out);
    group_file.open("files/group.txt", ios::in);
    regex conditionpat("\\s*(.+)\\s*(<|==|>)\\s*(.+)\\s*");
    if(conditions.size() == 1)
    {
        string line;
        smatch regexmatch;
        regex_search(conditions[0], regexmatch, conditionpat);
        int index = findIndex(regexmatch[1]);
        while(getline(group_file, line))
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
        string line;
        if(and_or == '&')
        {
            while(getline(group_file, line))
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
            while(getline(group_file, line))
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
    group_file.close();
    temp_file.close();
    remove("files/group.txt");
    rename("temp.txt", "files/group.txt");
}
bool Group_table::checkcondition1(string condition, vector<string> matches, smatch regexmatch, int index)
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
       
        if((matches[index] > regexmatch[3]))
        {
            return true;
        }
    }
    if(index == 2)
    {
        if((matches[index] > regexmatch[3]))
        {
           return true;
        }
    }
    if(index == 3)
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
        if((matches[index] < regexmatch[3]))
        {
            return true;
        }
    }
    if(index == 2)
    {
        if((matches[index] < regexmatch[3]))
        {
           return true;
        }
    }
    if(index == 3)
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
        if((matches[index] == regexmatch[3]))
        {
            return true;
        }
    }
    if(index == 2)
    {
        if((matches[index] == regexmatch[3]))
        {
           return true;
        }
    }
    if(index == 3)
    {
        if((stoi(matches[index]) == stoi(regexmatch[3])))
        {
            return true;
        }
    }
    }
    return false;
}
void Group_table::update_group(vector<string> conditions, vector<string> changes, char and_or)
{
    lock_guard<mutex> guard(group_mtx);
    smatch condmatches;
    smatch changematches;
    regex conditionpat("\\s*(.+)\\s*(<|==|>)\\s*(.+)\\s*");
    vector<string> newValues(4);
    for(int i = 0; i < 4; i++)
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
        else
        {
            newValues[3] = changematches[3];
        }
    }

    fstream temp_file;
    temp_file.open("temp.txt", ios::app | ios::out);
    group_file.open("files/group.txt", ios::in);
    if(conditions.size() == 1)
    {
        string line;
        smatch regexmatch;
        regex_search(conditions[0], regexmatch, conditionpat);
        int index = findIndex(regexmatch[1]);
        while(getline(group_file, line))
        {
            vector<string> matches = split(line, ',');
            if(checkcondition1(conditions[0], matches, regexmatch, index))
            {
                string newline = "";
                for(int i = 0; i < 4; i++)
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
        string line;
        if(and_or == '&')
        {
            while(getline(group_file, line))
            {
                vector<string> matches = split(line, ',');
                if((checkcondition1(conditions[0], matches, regexmatch1, index1) && checkcondition1(conditions[1], matches, regexmatch2, index2)))
                {
                    string newline = "";
                    for(int i = 0; i < 4; i++)
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
            while(getline(group_file, line))
            {
                vector<string> matches = split(line, ',');
                //cout << matches[3] << endl;
                if((checkcondition1(conditions[0], matches, regexmatch1, index1) || checkcondition1(conditions[1], matches, regexmatch2, index2)))
                {
                    string newline = "";
                    for(int i = 0; i < 4; i++)
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
    group_file.close();
    temp_file.close();
    remove("files/group.txt");
    rename("temp.txt", "files/group.txt");
}
void Group_table::select_group(vector<string> conditions, vector<string> columns, char and_or)
{
    lock_guard<mutex> guard(group_mtx);
    smatch condmatches;
    regex conditionpat("\\s*(.+)\\s*(<|==|>)\\s*(.+)\\s*");
    vector<int> col(4);
    for(int i = 0; i < 4; i++)
        col[i] = 0;
    if(columns[0] == "*")
    {
        for(int i = 0; i < 4; i++)
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
            else if (findIndex(columns[i]) == 1)
            {
                col[1] = 1;
            }
            else if (findIndex(columns[i]) == 2)
            {
                col[2] = 1;
            }
            else
            {
                col[3] = 1;
            }
        }    
    }
    fstream selected_group_file;
    selected_group_file.open("selected_group.txt", ios::app | ios::out);
    group_file.open("group.txt", ios::in);
    if(conditions.size() == 1)
    {
        string line;
        smatch regexmatch;
        regex_search(conditions[0], regexmatch, conditionpat);
        int index = findIndex(regexmatch[1]);
        while(getline(group_file, line))
        {
            vector<string> matches = split(line, ',');
            if(checkcondition1(conditions[0], matches, regexmatch, index))
            {
                string newline = "";
                for(int i = 0; i < 4; i++)
                {
                    if(col[i] != 0)
                        newline += matches[i] +",";   
                }
                newline.pop_back();
                selected_group_file << newline << "\n";
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
        string line;
        if(and_or == '&')
        {
            while(getline(group_file, line))
            {
                vector<string> matches = split(line, ',');
                if((checkcondition1(conditions[0], matches, regexmatch1, index1) && checkcondition1(conditions[1], matches, regexmatch2, index2)))
                {
                    string newline = "";
                    for(int i = 0; i < 4; i++)
                    {
                        if(col[i] != 0)
                            newline += matches[i] +",";    
                    }
                    newline.pop_back();
                    selected_group_file << newline << "\n";
                }
            }
        }
        else if(and_or == '|')
        {
            while(getline(group_file, line))
            {
                vector<string> matches = split(line, ',');
                //cout << matches[3] << endl;
                if((checkcondition1(conditions[0], matches, regexmatch1, index1) || checkcondition1(conditions[1], matches, regexmatch2, index2)))
                {
                    string newline = "";
                    for(int i = 0; i < 4; i++)
                    {
                        if(col[i] != 0)
                            newline += matches[i] +",";   
                    }
                    newline.pop_back();
                    selected_group_file << newline << "\n";
                }
            }
        }
    }
    group_file.close();
    selected_group_file.close();
}

Group_object::Group_object(int _ID, string _name, string _desc, int _member_count)
{
    ID = _ID;
    name = _name;
    description = _desc;
    member_count = _member_count;
}