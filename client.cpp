
#include<iostream>
#include<sys/wait.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<sys/types.h>
#include<netdb.h>
#include<string.h>
#include<errno.h>
#include<unistd.h>
#include<stdlib.h>
#include<strings.h>
#include<errno.h>
#include<fstream>
using namespace std ;

//Client class defining the function of what a client must do 
class client 
{
	int sockfd ;
	struct sockaddr_in server_address;
	int port_no;
	char *ip_address ;
	public :char *cmd_arg;
	public :
	client(int p , char *ip,char *arg)
	{
		port_no = p;
		ip_address = ip ;
		cmd_arg = arg ;
	}
	void client_sock_init()
	{       
		sockfd = socket(AF_INET , SOCK_STREAM , 0); //Establish a socket connection to the AWS
		bzero(&server_address,sizeof(server_address));
		server_address.sin_family =  AF_INET ;
		server_address.sin_port = htons(port_no);
		inet_pton(AF_INET,ip_address,&server_address.sin_addr);
		 connect(sockfd, (struct sockaddr *)&server_address , sizeof(server_address)); //Connects to the AWS server
		//cout <<strerror(errno)<<"\n";
	}

	void client_logic()
	{       int len = strlen(cmd_arg);
		unsigned long int final_answer;
 		//cout<<"The sizeof command line argument is "<<len<<"\n";
		this->sendall(sockfd,cmd_arg,&len);
		std::ifstream file("nums.csv"); //Reading the nums.csv file line by line
		std::string num;
		while(std::getline(file,num))
		{
			//cout<<num ;
			num = num+"\n";//Adding new lines after each value
			len = strlen(num.data());
			//this->sendall(sockfd,(char *)num.data(),&len);
			 send(sockfd,(char *)num.data(),strlen(num.data()),0);

		}
//Polling for data from the non blocking socket of AWS 
		struct pollfd ufds[1];
	        ufds[0].fd = sockfd;
       		ufds[0].events = POLLIN | POLLPRI ;
        	int rv = poll(ufds,1,-1);
        	if(ufds[0].revents & POLLIN)
        	{
			recv(sockfd,&final_answer,sizeof(final_answer),0); //Recieve data from the AWS
			cout<<"System Log - The client has recieved reduction "<<cmd_arg<<" : "<<final_answer<<endl;
		}

		close(sockfd);
	}
//Function code sourced out from Beejs Socket programming tutorial to handle partial Send
	int sendall(unsigned int s, char *buf, int *len)
	{
    		int total = 0;        // how many bytes we've sent
    		int bytesleft = *len; // how many we have left to send
    		int n;

    		while(total < *len) {
        		n = send(s, buf+total, bytesleft, 0);
			//cout<<"\n"<<n<<"\n";
        		if (n == -1) { break; }
        		total += n;
        		bytesleft -= n;
    			}

    			*len = total; // return number actually sent here

    			return n==-1?-1:0; // return -1 on failure, 0 on success
	} 

};
int main(int argc , char *argv[])
{
	
	client t(25471,"localhost",argv[1]);//Client Object created here
	cout<<"System Log - The client is up and running "<<endl;
	cout<<"System Log - The client has sent reduction type "<<argv[1]<<" to AWS"<<endl;
	t.client_sock_init();
	t.client_logic();
}
