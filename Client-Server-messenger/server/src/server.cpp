#include "../header/server.h"

Server::Server()
{
    unique_id = 0;
    max_len = 150;
    clients.clear();
}

void Server::start_listening()
{
    if((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket");
        exit(-1);
    }
    struct sockaddr_in server;
    server.sin_family = AF_INET;
	server.sin_port = htons(10000);
	server.sin_addr.s_addr = INADDR_ANY;
    if((::bind(server_socket, (struct sockaddr*)&server ,sizeof(struct sockaddr_in))) == -1)
    {
        perror("bind");
        exit(-1);
    }
    if ((listen(server_socket, 5)) == -1)
	{
		perror("listen");
		exit(-1);
	}
    multi_print("... Chat Room start listening ...");  
}

void Server::start_accepting()
{
    struct sockaddr_in client;
    int client_socket;
    unsigned int len = sizeof(sockaddr_in);

    while (true)
    {
        if ((client_socket = accept(server_socket, (struct sockaddr *)&client, &len)) == -1)
        {
            perror("accept: ");
			exit(-1);
        } 
        //unique_id++;
        ID_user++;
        User_object* user_object = new User_object(ID_user, client_socket);
        thread* client_thread = new thread(handle_client, this, ID_user);
        user_object->client_thread = client_thread;
        add_client(user_object);
    }
}

void Server::add_client(User_object* user_object)
{
    lock_guard<mutex> guard(clients_mtx);
    clients[user_object->ID] =  user_object;
}

void Server::end_connection(int id)
{
    lock_guard<mutex> guard(clients_mtx);
    if (clients[id])
        clients[id]->delete_thread(clients[id]);
}

bool Server::login(User_object* user_object)
{
    while(true)
    {
        char stat[max_len];
        int byted_received = recv(user_object->client_socket, stat, sizeof(stat), 0);
        if(string(stat) == "login")
        {
            char username[max_len],password[max_len];
            int bytes_received = recv(user_object->client_socket, username, sizeof(username), 0);
            if(bytes_received <= 0)
                return false;
            bytes_received = recv(user_object->client_socket, password, sizeof(password), 0);
            if(bytes_received <= 0)
                return false;
            string SQL = "SELECT (user_name,password) FROM User_table WHERE user_name==" + string(username);
            commandsHandler(SQL);
            fstream selected;
            selected.open("files/selected_user.txt", ios::in);
            string line;
            getline(selected, line);
            selected.close();
            remove("files/selected_user.txt");
            vector<string> splited = split(line, ',');
            if(splited[0] == string(username) && splited[1] == string(password))
            {
                
                User_object* existed_user;
                for(auto& u : clients)
                {
                    cout << u.second->username << endl;
                    if(u.second->username == string(username))
                    {
                        
                        u.second->client_socket = user_object->client_socket;
                        u.second->client_thread = user_object->client_thread;
                        
                        existed_user = u.second;
                        //cout<<existed_user->ID << existed_user->username << endl;
                        //cout << user_object->ID ;
                        clients.erase(clients.find(user_object->ID));
                        //delete clients[user_object->ID];
                        cout << clients.size();
                        //delete user_object;
                        ID_user--;
                        break;
                    }
                }
                string welcome_message = "Server | welcome " + string(username);
                send_message(existed_user->client_socket, welcome_message);
                multi_print(welcome_message);
                clients[existed_user->ID]->is_online = true;
                SQL = "UPDATE User_table SET is_online==1 WHERE ID==" + to_string(existed_user->ID);
                //commandsHandler(SQL);  
                SQL = "DELETE FROM Buffer_table WHERE user_id==" + to_string(existed_user->ID);
                //commandsHandler(SQL);
                return true;
            }
            else
            {
                string error_message = "Server | incorrect password or username ";
                send_message(user_object->client_socket, error_message);
                multi_print(error_message);
                continue;
            }
        }
        else
        {
            char username[max_len],password[max_len], name[max_len];
            int bytes_received = recv(user_object->client_socket, name, sizeof(name), 0);
            if(bytes_received <= 0)
                return false;
            bytes_received = recv(user_object->client_socket, username, sizeof(username), 0);
            if(bytes_received <= 0)
                return false;
            bytes_received = recv(user_object->client_socket, password, sizeof(password), 0);
            if(bytes_received <= 0)
                return false;
            string signup_message = "Server | successfully signed up " + string(username);
            send_message(user_object->client_socket, signup_message);
            multi_print(signup_message);
            //int on = (is_online) ? 1 : 0;
            string SQL;
            SQL = "INSERT INTO User_table VALUES (" + to_string(user_object->ID) +','  + string(name) +','  + string(username) +','  + string(password) +','  + '1' +')';
            commandsHandler(SQL);
            clients[user_object->ID]->set_value(string(name), string(username), string(password), false);
            return true;
        }
    }
    cout << clients.size() << endl;
    //cout << clients[user_object->ID] << endl;
}

void Server::multi_print(string message)
{
    lock_guard<mutex> guard(print_mtx);
	cout<<message<<endl;
}

void Server::close_connection()
{
    close(server_socket);
}

void Server::get_user_from_file()
{
    fstream user_file;
    string line;
    user_file.open("user.txt", ios::in);
    while(getline(user_file, line))
    {
        vector<string> splited = split(line, ',');

    }
    
}

void Server::delete_all_clients()
{
    for (auto & u : clients)
    {
        u.second->delete_thread(u.second);
    }
    clients.clear();
}

void Server::check_user(int id, bool connected)
{   
    if(clients.find(id) == clients.end())
        throw "this user doesn't exist";
    if(connected && !clients[id]->client_socket)
        throw "this user doesn't connected";    
}

void Server::create_new_group(vector<int> members, string group_name, string desc, int group_id)
{
    string SQL, sql;
    SQL = "INSERT INTO Group_table VALUES (" + to_string(group_id) +',' + group_name +','  + desc  +',' + to_string(members.size()) + ')';
    commandsHandler(SQL);
    for(int i = 0; i < members.size(); i++)
    {
        sql = "INSERT INTO user_group_table VALUES (" + to_string(group_id) +','  + to_string(members[i]) + ')';
        commandsHandler(sql);
    }
    groups[group_id] = new Group_object(group_id, group_name, desc, members.size());
}

void Server::add_member(int user_id, int group_id)
{
    string SQL, sql;
    int size = groups[group_id]->member_count; 
    SQL = "UPDATE Group_table SET member_count=" + to_string(size++) + " WHERE " + "group_id==" + to_string(group_id);
    commandsHandler(SQL);
    sql ="INSERT INTO user_group_table VALUES (" + to_string(group_id) + to_string(user_id) + ')';
    commandsHandler(sql);
    groups[group_id]->member_count++;
}

void Server::remove_member(int user_id, int group_id)
{
    string SQL, sql;
    int size = groups[group_id]->member_count; 
    SQL = "UPDATE Group_table SET member_count=" + to_string(size--) + " WHERE " + "group_id==" + to_string(group_id);
    commandsHandler(SQL);
    sql ="DELETE FROM user_group_table WHERE user_id==" + to_string(user_id) + " & group_id==" + to_string(group_id);
    commandsHandler(sql);
    groups[group_id]->member_count--;
}

void Server::change_name(int id)
{
    char newname[max_len];
    send_message(clients[id]->client_socket, "Enter your new name : ");
    recv(clients[id]->client_socket, newname, max_len, 0);
    cout << string(newname) << endl;
    string SQL;
    SQL = "UPDATE User_table SET name==" + string(newname) + " WHERE user_name==" + (clients[id]->username);
    commandsHandler(SQL); 
    clients[id]->name = string(newname);
}

void Server::exit(int id)
{
    clients[id]->is_online = false;
}

void Server::block(int user_id)
{
    char block_username[max_len];
    send_message(clients[user_id]->client_socket, "Enter username: ");
    recv(clients[user_id]->client_socket, block_username, max_len, 0);
    string SQL;
    SQL = "SELECT (ID) FROM User_table WHERE user_name==" + string(clients[user_id]->username);
    commandsHandler(SQL);
    fstream selected;
    selected.open("selected_user.txt", ios::in);
    string line;
    getline(selected, line);
    vector<string> splitied = split(line, ',');
    int block_id = stoi(splitied[0]);
    SQL = "INSERT INTO block_list_table VALUES (" + to_string(user_id) + ',' + to_string(block_id) + ')';
    commandsHandler(SQL);
}

void Server::unblock(int user_id)
{
    char block_username[max_len];
    send_message(clients[user_id]->client_socket, "Enter username: ");
    recv(clients[user_id]->client_socket, block_username, max_len, 0);
    string SQL;
    SQL = "SELECT (ID) FROM User_table WHERE user_name==" + string(clients[user_id]->username);
    commandsHandler(SQL);
    fstream selected;
    selected.open("selected_user.txt", ios::in);
    string line;
    getline(selected, line);
    vector<string> splitied = split(line, ',');
    int block_id = stoi(splitied[0]);
    SQL = "DELETE FROM block_list_table WHERE user_id==" + to_string(user_id) + " & blocked_user_id==" + to_string(block_id);
    commandsHandler(SQL);
}

void Server::send_message(int client_socket, string text)
{
    send(client_socket, &text[0], max_len, 0);
}

void Server::send_pv(int id, User_object* sender, User_object* reciver, string text, string time)
{
    string SQL_friend1 = "INSERT INTO contacts_table VALUES (" + to_string(id) +',' + to_string(sender->ID) + ',' + to_string(reciver->ID) + ')';
    string SQL_friend2 = "INSERT INTO contacts_table VALUES (" + to_string(id) +','  + to_string(reciver->ID) + ',' + to_string(sender->ID) + ')';
    string SQL_find = "SELECT (ID) FROM contacts_table WHERE user_id==" + to_string(sender->ID) +" & friend_id==" + to_string(reciver->ID);
    commandsHandler(SQL_friend1);
    commandsHandler(SQL_friend2);
    commandsHandler(SQL_find);
    fstream selected_friend;
    selected_friend.open("files/selected_contact.txt", ios::in);
    string line;
    getline(selected_friend, line);
    string SQL_pv = "INSERT INTO pv_message_table VALUES (" + to_string(id) + ','+ line + text + ',' + time + ')';
    commandsHandler(SQL_pv);
    if(!reciver->is_online)
    {
        string SQL_buff = "INSERT INTO Buffer_table VALUES (" ;
    }
    selected_friend.close();
    remove("files/selected_contact.txt");
    send_message(reciver->client_socket, text);
}

vector<string> Server::get_group_members(int group_id)
{   
    string SQL = "SELECT (user_id) FROM user_group_table WHERE group_id==" + to_string(group_id);
    commandsHandler(SQL);
    vector<string> members;
    fstream selected_user_group_file;
    selected_user_group_file.open("files/selected_user_group.txt", ios::in);
    string line;
    while(getline(selected_user_group_file, line))
    {
        members.push_back(clients[stoi(line)]->username);
    }
    selected_user_group_file.close();
    remove("files/selected_user_group.txt");
    return members;
}

vector<string> Server::joined_groups(int user_id)
{
    string SQL = "SELECT (group_id) FROM user_group_table WHERE group_id==" + to_string(user_id);
    commandsHandler(SQL);
    vector<string> groupjoin;
    fstream selected_user_group_file;
    selected_user_group_file.open("files/selected_user_group.txt", ios::in);
    string line;
    while(getline(selected_user_group_file, line))
    {
        groupjoin.push_back(groups[stoi(line)]->name);
    }
    selected_user_group_file.close();
    remove("files/selected_user_group.txt");
    return groupjoin;
}

void Server::check_group(int id, bool exist)
{
    if(groups.find(id) == groups.end())
    {
        if(!exist)
            throw "this group doesn't exist";
    } 
    else if(exist)
            throw "this group is available"; 

}

vector<string> Server::all_contacts(int user_id)
{
    string SQL = "SELECT (friend_id) FROM contacts_table WHERE user_id==" + to_string(user_id);
    commandsHandler(SQL);
    vector<string> friends;
    fstream friends_file;
    friends_file.open("files/selected_contacts.txt", ios::in);
    string line;
    while(getline(friends_file, line))
    {   
        friends.push_back(clients[stoi(line)]->username);
    }
    friends_file.close();
    remove("files/selected_contacts.txt");
    return friends;
}

void Server::handle_client(Server* server, int id)
{
    
    if(server->login(server->clients[id]))
    {
        char message[server->max_len];
        int bytes_received;
        while(true)
        { 
            bytes_received = recv(server->clients[id]->client_socket, message, sizeof(message), 0);
            if(bytes_received <= 0)
                break; 
            server->multi_print(string(server->clients[id]->name) + " : " + string(message));		
            server->do_for_user(id, message);
        }
        server->clients[id]->client_socket = 0;
    }
    server->end_connection(id);
}

void Server::do_for_user(int id, string message)
{
    //vector<string> splited = split(message, ' ');
    if(message == "change_name")
    {
        //cout << user_object->ID << endl;
        change_name(id);
    }
    if(message == "all_my_friends")
    {
        vector<string> friends;
        friends = all_contacts(id);
        string result = "";
        for(int i = 0; i < friends.size(); i++)
        {
            result += friends[i] + '\n';
        }
        send_message(clients[id]->client_socket, result);
    }
    if(message == "joind_groups")
    {
        vector<string> groups;
        groups = joined_groups(id);
        string result = "";
        for(int i = 0; i < groups.size(); i++)
        {
            result += groups[i] + '\n';
        }
        send_message(clients[id]->client_socket, result);
    }
    if(message == "block")
    {
        block(id);
    }
    if(message == "unblock")
    {
        unblock(id);
    }

}

Server::~Server()
{
    //delete_all_clients();
    close_connection();
}