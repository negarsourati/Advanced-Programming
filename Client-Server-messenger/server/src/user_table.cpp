#include "../header/user_table.h"
int User_table::findIndex(string s)
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
    else if(s == "user_name")
    {
        index = 2;
    }
    else if(s == "password")
    {
        index = 3;
    }
    else if(s == "is_online")
    {
        index = 4;
    }
    return index;
}
void User_table::create_table()
{

}
void User_table::update_user(vector<string> conditions, vector<string> changes, char and_or)
{
    lock_guard<mutex> guard(user_mtx);
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
            cout << newValues[1];
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
    user_file.open("files/user.txt", ios::in);
    if(conditions.size() == 1)
    {
        string line;
        smatch regexmatch;
        regex_search(conditions[0], regexmatch, conditionpat);
        int index = findIndex(regexmatch[1]);
        while(getline(user_file, line))
        {
            vector<string> matches = split(line, ',');
            if(checkcondition1(conditions[0], matches, regexmatch, index))
            {
                cout << "here its ok!" << matches[1] << endl;
                string newline = "";
                for(int i = 0; i < 5; i++)
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
            while(getline(user_file, line))
            {
                vector<string> matches = split(line, ',');
                if((checkcondition1(conditions[0], matches, regexmatch1, index1) && checkcondition1(conditions[1], matches, regexmatch2, index2)))
                {
                   string newline = "";
                    for(int i = 0; i < 5; i++)
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
                    temp_file<< line << "\n";
                }
            }
        }
        else if(and_or == '|')
        {
            while(getline(user_file, line))
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
                    temp_file<< line << "\n";
                }
            }
        }
    }
    user_file.close();
    temp_file.close();
    remove("files/user.txt");
    rename("temp.txt", "files/user.txt");

}
void User_table::delete_user(vector<string> conditions, char and_or)
{
    lock_guard<mutex> guard(user_mtx);
    fstream temp_file;
    temp_file.open("temp.txt", ios::app | ios::out);
    user_file.open("files/user.txt", ios::in);
    regex conditionpat("\\s*(.+)\\s*(<|==|>)\\s*(.+)\\s*");
    if(conditions.size() == 1)
    {
        string line;
        smatch regexmatch;
        regex_search(conditions[0], regexmatch, conditionpat);
        int index = findIndex(regexmatch[1]);
        while(getline(user_file, line))
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
            while(getline(user_file, line))
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
            while(getline(user_file, line))
            {
                vector<string> matches = split(line, ',');
                if(!(checkcondition1(conditions[0], matches, regexmatch1, index1) || checkcondition1(conditions[1], matches, regexmatch2, index2)))
                {
                    temp_file << line << "\n";
                }
            }
        }
    }
    user_file.close();
    temp_file.close();
    remove("files/user.txt");
    rename("temp.txt", "files/user.txt");
}
bool User_table::checkcondition1(string condition, vector<string> matches, smatch regexmatch, int index)
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
        
        if(((matches[index]) > (regexmatch[3])))
        {
            return true;
        }
    }
    if(index == 4)
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
        if(((matches[index]) < (regexmatch[3])))
        {
            return true;
        }
    }
    if(index == 4)
    {
        if(((matches[index]) < (regexmatch[3])))
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
        if(((matches[index]) == (regexmatch[3])))
        {
            return true;
        }
    }
    if(index == 4)
    {
        if(((matches[index]) < (regexmatch[3])))
        {
            return true;
        }
    }
    }
    return false;
}
void User_table::insert_user(vector<string> vars)
{
    lock_guard<mutex> guard(user_mtx);
    //int ID, string name, string password, string username, bool is_online
    user_file.open("files/user.txt", ios::app | ios::out);
    for(int i = 0; i < vars.size(); i++)
    {
        if(i == vars.size() - 1)
            user_file<<vars[i]<<"\n";
        else
            user_file<<vars[i]<<",";    
    }
    user_file.close();
}
void User_table::select_user(vector<string> conditions, vector<string> columns, char and_or)
{
    lock_guard<mutex> guard(user_mtx);
    //cout << "select_func" << endl;
    smatch condmatches;
    regex conditionpat("\\s*(.+)\\s*(<|==|>)\\s*(.+)\\s*");
    vector<int> col(5);
    for(int i = 0; i < 5; i++)
        col[i] = 0;
    if(columns[0] == "*")
    {
        for(int i = 0; i < 5; i++)
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
    fstream selected_user_file;
    selected_user_file.open("files/selected_user.txt", ios::app | ios::out);
    user_file.open("files/user.txt", ios::in);
    if(conditions.size() == 1)
    {
        string line;
        smatch regexmatch;
        regex_search(conditions[0], regexmatch, conditionpat);
        int index = findIndex(regexmatch[1]);
        while(getline(user_file, line))
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
                selected_user_file << newline << "\n";
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
            while(getline(user_file, line))
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
                    selected_user_file << newline << "\n";
                }
            }
        }
        else if(and_or == '|')
        {
            while(getline(user_file, line))
            {
                vector<string> matches = split(line, ',');
                if((checkcondition1(conditions[0], matches, regexmatch1, index1) || checkcondition1(conditions[1], matches, regexmatch2, index2)))
                {
                    string newline = "";
                    for(int i = 0; i < 5; i++)
                    {
                        if(col[i] != 0)
                            newline += matches[i] +",";
                    }
                    newline.pop_back();
                    selected_user_file << newline << "\n";
                }
            }
        }
    }
    user_file.close();
    selected_user_file.close();
}
User_object::User_object(int id,int _client_socket)
{
    ID = id;
    
    client_socket = _client_socket;
}
void User_object::set_value(string n, string user , string pass, bool is_on)
{
    this->name = n;
    this->username = user;
    this->password = pass;
    this->is_online = is_on;
}
void User_object::delete_thread(User_object* user_object)
{
    if (user_object->client_thread)
    {
        if (user_object->client_thread->joinable())
        {
            user_object->client_thread->detach();
            delete user_object->client_thread;
        }
        user_object->client_thread = 0;
    }
    if (user_object->client_socket)
        close(user_object->client_socket);
}