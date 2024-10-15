#include "../header/client.h"

Client::Client()
{
    max_len = 150;
    logged_in = false;
    exited = false;
    name = new char[150];
    username = new char[150];
    password = new char[150];
}

void Client::start_connecting()
{
    if ((client_socket=socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("socket: ");
		exit(-1);
	}

    struct sockaddr_in client;
	client.sin_family=AF_INET;
	client.sin_port=htons(10000);
	client.sin_addr.s_addr=INADDR_ANY;
    if ((connect(client_socket, (struct sockaddr *)&client, sizeof(struct sockaddr_in))) == -1)
	{
		perror("connect: ");
		exit(-1);
	}
}

void Client::multi_print(string message, bool you = true)
{
    lock_guard<mutex> guard(print_mtx);
	if (message.length())
		cout<<"\33[2K \r"<<message<<endl;
	if (you)
		cout<<"\33[2K \r"<<"You : ";
}

void Client::login()
{
    while(true)
    {
        char stat[max_len];
        cout<<"sign_up or login ?\n";
	    cin.getline(stat, max_len);
        send(client_socket, stat, sizeof(stat), 0);
        if(string(stat) == "sign_up")
        {
            cout << "Enter your name : \n";
            cin.getline(name, max_len);
	        send(client_socket, name, sizeof(name), 0);
            cout<<"Enter your username : \n";
	        cin.getline(username, max_len);
	        send(client_socket, username, sizeof(username), 0);
	        cout<<"Enter your password : \n";
	        cin.getline(password, max_len);
	        send(client_socket, password, sizeof(password), 0);
            multi_print("Your sign up request has been sent!", false);
	        char answer[max_len];
	        int bytes_received = recv(client_socket, answer, sizeof(answer), 0);
	        if(bytes_received <= 0)
	        	continue;
	        multi_print(answer, false);
	        if (string(answer) == "Server | successfully signed up " + string(username))
	        {
                cout <<"yes";
                multi_print(menu);
		     	break;
            }
        }
        else    
        {
            cout<<"Enter your username : ";
	        cin.getline(username, max_len);
	        send(client_socket, username, sizeof(username), 0);
	        cout<<"Enter your password : ";
	        cin.getline(password, max_len);
	        send(client_socket, password, sizeof(password), 0);
            multi_print("Your login request has been sent!", false);
		    char answer[max_len];
		    int bytes_received = recv(client_socket, answer, sizeof(answer), 0);
		    if(bytes_received <= 0)
		    	continue;
		    multi_print(answer, false);
		    if (string(answer) == "Server | welcome " + string(username))
            {
                cout <<"yes";
                multi_print(menu);
		     	break;
            }
            else
            {
                multi_print("try again");
                continue;
            }
        }
    }
}

void Client::start_communicating()
{
    
    login();    
    send_thread = new thread(send_handler, this);
    recv_thread = new thread(recv_handler, this);	
	if (send_thread->joinable())
		send_thread->join();
	if (recv_thread->joinable())
		recv_thread->join();
}

void Client::send_handler(Client* client)
{
    while(true)
	{
		client->multi_print("");
		char str[client->max_len];
		cin.getline(str,client->max_len);
		if (string(str) == "#exit")
		{
			client->exited = true;
			client->recv_thread->detach();
			close(client->client_socket);
			return;
		}
		send(client->client_socket, str, sizeof(str),0);
	}
}

void Client::recv_handler(Client* client)
{
    while(!client->exited)
	{
		char message[client->max_len];
		int bytes_received = recv(client->client_socket, message, sizeof(message), 0);
		if(bytes_received <= 0)
			continue;
		client->multi_print(message);
		fflush(stdout);
	}  
}

void Client::close_connection()
{
    if (send_thread)
	{
		if (send_thread->joinable())
		{
			send_thread->detach();
			delete send_thread;
		}
		send_thread = 0;
	}
	if (recv_thread)
	{
		if (recv_thread->joinable())
		{
			recv_thread->detach();
			delete recv_thread;
		}
		recv_thread = 0;
	}
    close(client_socket);
	multi_print("=== The socket turned off ===", false);
}

Client::~Client()
{
	close_connection();
    delete [] name;
	delete [] username;
	delete [] password;
}