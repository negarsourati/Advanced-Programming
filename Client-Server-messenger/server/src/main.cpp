#include "../header/server.h"
#include "signal.h"
Server* server = 0;
void exit_app(int sig_num);
int main()
{
    //User_table t;
    //vector<string> v;
    // v.push_back("1");
    // v.push_back("negar");
    // v.push_back("1234");
    // v.push_back("n");
    // v.push_back("1");
    // vector<string> v1;
    // v1.push_back("2");
    // v1.push_back("darya");
    // v1.push_back("797");
    // v1.push_back("d");
    // v1.push_back("0");
    // t.insert_user(v);
    // t.insert_user(v1);
    // vector<string> column;
    // column.push_back("*");
    // vector<string> cond;
    // cond.push_back("name==negar");
    // t.select_user(cond, column, '.');
    //string str = "INSERT INTO User_table VALUES (\"Hamid\",\"17:05:00\",45000)";
    //string str;
    //getline(cin, str);
    //commandsHandler(str);
    //t.insert_user("2","darya", "567", "d", "online");
    //t.insert_user("3","ali", "5657", "a","ofline");
    // vector<string> cond;
    // cond.push_back("password==567");
    // cond.push_back("name<negar");
    // //t.delete_user(cond, '|');
    // vector<string> column;
    // column.push_back("user_name");
    // column.push_back("ID");
    // t.select_user(cond, column, '|');

    // Group_table g;
    // vector<string> v;
    // vector<string> v2;
    // v.push_back("1");
    // v.push_back("first");
    // v.push_back("lablablab");
    // v.push_back("5");
    // g.insert_group(v);
    // v2.push_back("2");
    // v2.push_back("second");
    // v2.push_back("blawerf");
    // v2.push_back("10");
    // g.insert_group(v2);
    // vector<string> conds;
    // conds.push_back("name==first");
    // //conds.push_back("member_count>4");
    // //g.delete_group(conds, '&');
    // vector<string> changes;
    // changes.push_back("name==salam");
    // //changes.push_back("member_count==789");
    // g.update_group(conds, changes, '.');

    // pv_message_table p;
    // p.insert_pv_message(1, 123, "salam", "10:23:56");
    // p.insert_pv_message(2, 254, "khubi?", "12:45:10");
    // vector<string> cond;
    // cond.push_back("time<11:00:00");
    // p.delete_pv_message(cond, '?');
    //block_list_table b;
    //b.insert_blocked(v);
    
    server = new Server();
    signal(SIGINT, exit_app);
    server->start_listening();
    server->start_accepting();
    exit_app(0);
}
void exit_app(int sig_num)
{
    if (server)
        delete server;
    cout<<"...bye..."<<endl;
    exit(sig_num);
}