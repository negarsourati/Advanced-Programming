#include "../header/client.h"
#include "signal.h"
Client* client = 0;
void exit_app(int sig_num);
int main()
{
    client = new Client();
    signal(SIGINT, exit_app);
    client->start_connecting();
    client->start_communicating();
    exit_app(0);
}

void exit_app(int sig_num)
{
    if (client)
        delete client;
    cout<<"...bye..."<<endl;
    exit(sig_num);
}