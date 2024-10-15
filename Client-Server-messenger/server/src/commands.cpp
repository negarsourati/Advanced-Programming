#include "../header/commands.h"

void commandsHandler(string command)
{
    //INSERT INTO pv_message_table VALUES (1,hamid,h,111,1)
    cout <<"hey";
    User_table user;
    Group_table group;
    pv_message_table pvM;
    gp_message_table gpM;
    contacts_table contact;
    Buffer_table buf;
    user_group_table ug;
    block_list_table blck;
    //INSERT INTO User_table VALUES (1,hamid,h,111,1)
    regex INSERTpat("^INSERT INTO ([a-zA-Z_]+) VALUES \\((.+)\\)$");
    regex DELETEpat("DELETE FROM ([a-zA-Z_]+) WHERE (.*)");
    regex UPDATEpat("UPDATE ([a-zA-Z_]+) SET (.*) WHERE (.*)");
    regex SELECTpat("SELECT \\((([^\\*]*)|\\*)\\) FROM (.*) WHERE (.*)");
    smatch matches;
    if(regex_search(command, matches, INSERTpat))
    {
        string table_name = matches[1];
        vector<string> vars = split(matches[2], ',');
        if(table_name == "User_table")
        {
            user.insert_user(vars);
        }
        else if(table_name == "Group_table")
        {
            group.insert_group(vars);
        }
        else if(table_name == "pv_message_table")
        {
            pvM.insert_pv_message(vars);
        }
        else if(table_name == "gp_message_table")
        {
            
            gpM.insert_gp_message(vars);
        }
        else if(table_name == "contacts_table")
        {
            
            contact.insert_contact(vars);
        }
        else if(table_name == "buffer_table")
        {
            
            buf.insert_buffer(vars);
        }
        else if(table_name == "user_group_table")
        {
            
            ug.insert_user_group(vars);
        }
        else
        {
            blck.insert_blocked(vars);
        }

    }
    else if(regex_search(command, matches, DELETEpat))
    {
        string table_name = matches[1];
        if(!split(matches[2], '&').empty())
        {
            vector<string> conditions = split(matches[2], '&');
            if(table_name == "User_table")
            {
                user.delete_user(conditions, '&');
            }
            else if(table_name == "Group_table")
            {
                group.delete_group(conditions, '&');
            }
            else if(table_name == "pv_message_table")
            {

                pvM.delete_pv_message(conditions, '&');
            }
            else if(table_name == "gp_message_table")
            {

                gpM.delete_gp_message(conditions, '&');
            }
            else if(table_name == "contacts_table")
            {

                contact.delete_contact(conditions, '&');
            }
            else if(table_name == "buffer_table")
            {

                buf.delete_buffer(conditions, '&');
            }
            else if(table_name == "user_group_table")
            {

                ug.delete_user_group(conditions, '&');
            }
            else
            {
                blck.delete_blocked(conditions, '&');
            }
        }
        else if(!split(matches[3], '|').empty())
        {
            vector<string> conditions = split(matches[2], '|');
            if(table_name == "User_table")
            {
                user.delete_user(conditions, '|');
            }
            else if(table_name == "Group_table")
            {
                group.delete_group(conditions, '|');
            }
            else if(table_name == "pv_message_table")
            {

                pvM.delete_pv_message(conditions, '|');
            }
            else if(table_name == "gp_message_table")
            {

                gpM.delete_gp_message(conditions, '|');
            }
            else if(table_name == "contacts_table")
            {

                contact.delete_contact(conditions, '|');
            }
            else if(table_name == "buffer_table")
            {

                buf.delete_buffer(conditions, '|');
            }
            else if(table_name == "user_group_table")
            {

                ug.delete_user_group(conditions, '|');
            }
            else
            {
                blck.delete_blocked(conditions, '|');
            }
        }
        else
        {
            vector<string> conditions = split(matches[2], '.');
            if(table_name == "User_table")
            {
                user.delete_user(conditions, '.');
            }
            else if(table_name == "Group_table")
            {
                group.delete_group(conditions, '.');
            }
            else if(table_name == "pv_message_table")
            {

                pvM.delete_pv_message(conditions, '.');
            }
            else if(table_name == "gp_message_table")
            {

                gpM.delete_gp_message(conditions, '.');
            }
            else if(table_name == "contacts_table")
            {

                contact.delete_contact(conditions, '.');
            }
            else if(table_name == "buffer_table")
            {

                buf.delete_buffer(conditions, '.');
            }
            else if(table_name == "user_group_table")
            {

                ug.delete_user_group(conditions, '.');
            }
            else
            {
                blck.delete_blocked(conditions, '.');
            }
        }
    }   
    else if(regex_search(command, matches, UPDATEpat))
    {
        string table_name = matches[1];
        vector<string> changes = split(matches[2], ',');
        if(!split(matches[3], '&').empty())
        {
            vector<string> conditions = split(matches[3], '&');
            if(table_name == "User_table")
            {
                user.update_user(conditions, changes,'&');
            }
            else if(table_name == "Group_table")
            {
                group.update_group(conditions, changes, '&');
            }
            else if(table_name == "pv_message_table")
            {
                pvM.update_pv_message(conditions, changes, '&');
            }
            else if(table_name == "gp_message_table")
            {
                gpM.update_gp_message(conditions, changes, '&');
            }
            else if(table_name == "contacts_table")
            {
                contact.update_contact(conditions, changes, '&');
            }
            else if(table_name == "buffer_table")
            {
                buf.update_buffer(conditions, changes,'&');
            }
            else if(table_name == "user_group_table")
            {
                ug.update_user_group(conditions, changes,'&');
            }
            else
            {
                blck.update_blocked(conditions, changes, '&');
            }
        }
        else if(!split(matches[3], '|').empty())
        {
            vector<string> conditions = split(matches[3], '|');
            if(table_name == "User_table")
            {
                user.update_user(conditions, changes,'|');
            }
            else if(table_name == "Group_table")
            {
                group.update_group(conditions, changes, '|');
            }
            else if(table_name == "pv_message_table")
            {
                pvM.update_pv_message(conditions, changes, '|');
            }
            else if(table_name == "gp_message_table")
            {
                gpM.update_gp_message(conditions, changes, '|');
            }
            else if(table_name == "contacts_table")
            {
                contact.update_contact(conditions, changes, '|');
            }
            else if(table_name == "buffer_table")
            {
                buf.update_buffer(conditions, changes,'|');
            }
            else if(table_name == "user_group_table")
            {
                ug.update_user_group(conditions, changes,'|');
            }
            else
            {
                blck.update_blocked(conditions, changes, '|');
            }
        }
        else
        {
           vector<string> conditions = split(matches[3], '.');
            if(table_name == "User_table")
            {
                user.update_user(conditions, changes,'.');
            }
            else if(table_name == "Group_table")
            {
                group.update_group(conditions, changes, '.');
            }
            else if(table_name == "pv_message_table")
            {
                pvM.update_pv_message(conditions, changes, '.');
            }
            else if(table_name == "gp_message_table")
            {
                gpM.update_gp_message(conditions, changes, '.');
            }
            else if(table_name == "contacts_table")
            {
                contact.update_contact(conditions, changes, '.');
            }
            else if(table_name == "buffer_table")
            {
                buf.update_buffer(conditions, changes,'.');
            }
            else if(table_name == "user_group_table")
            {
                ug.update_user_group(conditions, changes,'.');
            }
            else
            {
                blck.update_blocked(conditions, changes, '.');
            }
        }    

    }
    else if(regex_search(command, matches, SELECTpat))
    {
        cout<<"regex"<<endl;
        string table_name = matches[3];
        vector<string> columns = split(matches[2], ',');
        if(!split(matches[4], '&').empty())
        {
            vector<string> conditions = split(matches[4], '&');
            if(table_name == "User_table")
            {
                user.select_user(conditions, columns,'&');
            }
            else if(table_name == "Group_table")
            {
                group.select_group(conditions, columns, '&');
            }
            else if(table_name == "pv_message_table")
            {
                pvM.select_pv_message(conditions, columns, '&');
            }
            else if(table_name == "gp_message_table")
            {
                gpM.select_gp_message(conditions, columns, '&');
            }
            else if(table_name == "contacts_table")
            {
                contact.select_contact(conditions, columns, '&');
            }
            else if(table_name == "buffer_table")
            {
                buf.select_buffer(conditions, columns,'&');
            }
            else if(table_name == "user_group_table")
            {
                ug.select_user_group(conditions, columns,'&');
            }
            else
            {
                blck.select_blocked(conditions, columns, '&');
            }
            
        }
        else if(!split(matches[4], '|').empty())
        {
            vector<string> conditions = split(matches[4], '|');
            if(table_name == "User_table")
            {
                user.select_user(conditions, columns,'|');
            }
            else if(table_name == "Group_table")
            {
                group.select_group(conditions, columns, '|');
            }
            else if(table_name == "pv_message_table")
            {
                pvM.select_pv_message(conditions, columns, '|');
            }
            else if(table_name == "gp_message_table")
            {
                gpM.select_gp_message(conditions, columns, '|');
            }
            else if(table_name == "contacts_table")
            {
                contact.select_contact(conditions, columns, '|');
            }
            else if(table_name == "buffer_table")
            {
                buf.select_buffer(conditions, columns,'|');
            }
            else if(table_name == "user_group_table")
            {
                ug.select_user_group(conditions, columns,'|');
            }
            else
            {
                blck.select_blocked(conditions, columns, '|');
            }

        }    
        else
        {
            vector<string> conditions = split(matches[4], '.');
            if(table_name == "User_table")
            {
                user.select_user(conditions, columns,'.');
            }
            else if(table_name == "Group_table")
            {
                group.select_group(conditions, columns, '.');
            }
            else if(table_name == "pv_message_table")
            {
                pvM.select_pv_message(conditions, columns, '.');
            }
            else if(table_name == "gp_message_table")
            {
                gpM.select_gp_message(conditions, columns, '.');
            }
            else if(table_name == "contacts_table")
            {
                contact.select_contact(conditions, columns, '.');
            }
            else if(table_name == "buffer_table")
            {
                buf.select_buffer(conditions, columns,'.');
            }
            else if(table_name == "user_group_table")
            {
                ug.select_user_group(conditions, columns,'.');
            }
            else
            {
                blck.select_blocked(conditions, columns, '.');
            }
        }
    }
}