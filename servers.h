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
using namespace std ;

class sock
{
        public: int port_number ;
	public: int sockfd,confd ;
	public: int network_family;
	public: struct sockaddr_in server_address;
public:
//Constructor to take in Protocol family and port number to which the servers should listen on from the object

	sock(int port_no,int family)
	{
		port_number = port_no;
		network_family = family;
	}
 //Virtual Function for server logic , the below code will never be executed unless this function is not over written in the derived class 

        virtual void server_logic()
	{
	 struct sockaddr_in client_addr;
	 int l_client_addr ;
	 //cout <<"In virtual func"<<endl;
	 char buff[6] = "Hello";
	 confd = accept(sockfd, (struct sockaddr*)&client_addr , (socklen_t*)&l_client_addr);
         write(confd,buff,sizeof(buff));
         //printf("Error Description is %s\n",strerror(errno));
         close (confd);

	}
	 void init_sock()
	{
		char buff[6]  = "Hello";
		//cout<<"Hello from header file"<<endl;
		sockfd = socket(AF_INET,network_family,0);
                fcntl(sockfd,F_SETFL,O_NONBLOCK); //Placing the created socket in NON BLOCKING mode , Chapter 16 of Reference 1
		bzero(&server_address,sizeof(server_address)); 
		server_address.sin_family = AF_INET;
		server_address.sin_addr.s_addr = htonl(INADDR_ANY);
		server_address.sin_port = htons(port_number);
		bind(sockfd,(struct sockaddr*)&server_address,sizeof(server_address));
		if(network_family == SOCK_STREAM) //appropriate logic to use when we are creating a TCP server or an UDP server
			listen(sockfd,3);
		//cout<<"Entering loop\n";
		for(;;)
		{
			this->server_logic();
			if (errno == EINTR) // logic to see if the child server process was scheduled out and restart the same , Chapter 5 of Reference 1
			{
				cout<<"EINTR encountered"<<endl;
				continue;
			}
		}
	}
	 int sendall(unsigned int s, char *buf, int *len) //Function to avoid partial send; code imported from Beejs Socket Tutorial Example
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



