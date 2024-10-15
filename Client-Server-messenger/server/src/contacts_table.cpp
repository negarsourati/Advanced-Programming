#include "../header/contacts_table.h"
int contacts_table::findIndex(string s)
{
    int index;
    if(s == "ID")
    {   
        index = 0;
    }
    else if(s == "user_id")
    {   
        index = 1;
    }
    else if(s == "friend_id")
    {
        index = 2;
    }
    return index;
}
void contacts_table::insert_contact(vector<string> vars)
{
    lock_guard<mutex> guard(contact_mtx);
    contacts_file.open("files/contacts.txt", ios::app);
    for(int i = 0; i < vars.size(); i++)
    {
        if(i == vars.size() - 1)
            contacts_file<<vars[i]<<"\n";
        else
            contacts_file<<vars[i]<<",";    
    }
    contacts_file.close();
}
void contacts_table::delete_contact(vector<string> conditions, char and_or)
{
    lock_guard<mutex> guard(contact_mtx);
    fstream temp_file;
    temp_file.open("temp.txt", ios::app | ios::out);
    contacts_file.open("files/ontacts.txt", ios::in);
    regex conditionpat("\\s*(.+)\\s*(<|==|>)\\s*(.+)\\s*");
    if(conditions.size() == 1)
    {
        string line;
        smatch regexmatch;
        regex_search(conditions[0], regexmatch, conditionpat);
        int index = findIndex(regexmatch[1]);
        while(getline(contacts_file, line))
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
            while(getline(contacts_file, line))
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
            while(getline(contacts_file, line))
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
    contacts_file.close();
    temp_file.close();
    remove("files/contacts.txt");
    rename("temp.txt", "files/contacts.txt");
}
bool contacts_table::checkcondition1(string condition, vector<string> matches, smatch regexmatch, int index)
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
void contacts_table::update_contact(vector<string> conditions, vector<string> changes, char and_or)
{
    lock_guard<mutex> guard(contact_mtx);
    smatch condmatches;
    smatch changematches;
    regex conditionpat("\\s*(.+)\\s*(<|==|>)\\s*(.+)\\s*");
    vector<string> newValues(3);
    for(int i = 0; i < 3; i++)
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
        else
        {
            newValues[2] = changematches[3];
        }
    }
    fstream temp_file;
    temp_file.open("temp.txt", ios::app | ios::out);
    contacts_file.open("files/contacts.txt", ios::in);
    if(conditions.size() == 1)
    {
        string line;
        smatch regexmatch;
        regex_search(conditions[0], regexmatch, conditionpat);
        int index = findIndex(regexmatch[1]);
        while(getline(contacts_file, line))
        {
            vector<string> matches = split(line, ',');
            if(checkcondition1(conditions[0], matches, regexmatch, index))
            {
                string newline = "";
                for(int i = 0; i < 3; i++)
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
            while(getline(contacts_file, line))
            {
                vector<string> matches = split(line, ',');
                if((checkcondition1(conditions[0], matches, regexmatch1, index1) && checkcondition1(conditions[1], matches, regexmatch2, index2)))
                {
                    string newline = "";
                    for(int i = 0; i < 3; i++)
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
            while(getline(contacts_file, line))
            {
                vector<string> matches = split(line, ',');
                if((checkcondition1(conditions[0], matches, regexmatch1, index1) || checkcondition1(conditions[1], matches, regexmatch2, index2)))
                {
                    string newline = "";
                    for(int i = 0; i < 3; i++)
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
    contacts_file.close();
    temp_file.close();
    remove("files/contacts.txt");
    rename("temp.txt", "files/contacts.txt");
}
void contacts_table::select_contact(vector<string> conditions, vector<string> columns, char and_or)
{
    lock_guard<mutex> guard(contact_mtx);
    smatch condmatches;
    regex conditionpat("\\s*(.+)\\s*(<|==|>)\\s*(.+)\\s*");
    vector<int> col(3);
    for(int i = 0; i < 3; i++)
        col[i] = 0;
    if(columns[0] == "*")
    {
        for(int i = 0; i < 3; i++)
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
            else if(findIndex(columns[i]) == 1)
            {
                col[1] = 1;
            }
            else
            {
                col[2] = 1;
            }
        }    
    }
    fstream selected_contacts_file;
    selected_contacts_file.open("files/selected_contacts.txt", ios::app | ios::out);
    contacts_file.open("files/contacts.txt", ios::in);
    if(conditions.size() == 1)
    {
        string line;
        smatch regexmatch;
        regex_search(conditions[0], regexmatch, conditionpat);
        int index = findIndex(regexmatch[1]);
        while(getline(contacts_file, line))
        {
            vector<string> matches = split(line, ',');
            if(checkcondition1(conditions[0], matches, regexmatch, index))
            {
                string newline = "";
                for(int i = 0; i < 3; i++)
                {
                    if(col[i] != 0)
                        newline += matches[i] +",";    
                }
                newline.pop_back();
                selected_contacts_file << newline << "\n";
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
            while(getline(contacts_file, line))
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
                    selected_contacts_file << newline << "\n";
                }
            }
        }
        else if(and_or == '|')
        {
            while(getline(contacts_file, line))
            {
                vector<string> matches = split(line, ',');
                if((checkcondition1(conditions[0], matches, regexmatch1, index1) || checkcondition1(conditions[1], matches, regexmatch2, index2)))
                {
                    string newline = "";
                    for(int i = 0; i < 3; i++)
                    {
                        if(col[i] != 0)
                            newline += matches[i] +",";   
                    }
                    newline.pop_back();
                    selected_contacts_file << newline << "\n";
                }
            }
        }
    }
    contacts_file.close();
    selected_contacts_file.close();
}

