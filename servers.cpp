
#include<signal.h>
#include<iostream>
#include<sys/ioctl.h>
#include<fcntl.h>
#include<stropts.h>
#include<sys/filio.h>
#include "servers.h"
#include<errno.h>
#include<algorithm>
#include<vector>
#include<sstream>
#include<string>
#include<limits.h>
#include<stdlib.h>
#include<stdint.h>
using namespace std ;



class aws: public sock // Dervied class for the AWS
{
public :char *func;
public :
	aws(int p,int f)  : sock(p,f) {
}
	void server_logic()
	{
		int len=0,count_nl=0,i,i1,i2,count=0,l1=0,ltest ;
		string b1,b2,b3;
		socklen_t l_client_addr;
        	struct sockaddr_in udp_server,udp_server1,udp_server2,client_addr,udp_cli,udp_cli1,udp_cli2;
		confd =  accept(sockfd,NULL,NULL); //Accepts Incoming TCP Connection from the client here
		//cout<<"Connection Accepted\n";
		while (!len && ioctl (confd,FIONREAD,&len) >= 0) //Logic to find out how much of data is available at the socket to be read
		sleep(1);
		if(len >0) // if length of the data to be read is more than 0 start reading data
        	{
			//cout<<strerror(errno);
			//cout<<"Length is " <<len<<"\n";
        		char *b = (char *)malloc(len * sizeof(char)); //dynamically allocate memory depending on the amount of data to be read
			bzero(b,sizeof(b));
    			recv(confd,b,len,0);
			//recvfrom(confd,b,len,0,(struct sockaddr*)&client_addr,&l_client_addr);
			b[len+1] = '\0';
			memcpy(func,b,sizeof(char)*3);
			func[3] = '\0';
			//cout<<func<<endl;
			//cout<<endl<<"Data Dump"<<endl<<b<<endl;
			int sockfd_udp = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP); //socket connection being established to Backend Server A
        		bzero(&udp_server,sizeof(udp_server));
			bzero(&udp_cli,sizeof(udp_cli));
			udp_cli.sin_family = AF_INET;
			udp_cli.sin_port = htons(24471); 
			inet_aton("127.0.0.1",&udp_cli.sin_addr);
			bind(sockfd_udp,(struct sockaddr*)&udp_cli,sizeof(udp_cli));
			udp_server.sin_family = AF_INET;
			udp_server.sin_port = htons(21471);
        		inet_aton("127.0.0.1",&udp_server.sin_addr);
			int sockfd_udp1 = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP); //socket connection being established to Backend Server B
        		bzero(&udp_server1,sizeof(udp_server1));
			bzero(&udp_cli1,sizeof(udp_cli1));
                        udp_cli1.sin_family = AF_INET;
                        udp_cli1.sin_port = htons(24471);
                        inet_aton("127.0.0.1",&udp_cli1.sin_addr);
                        bind(sockfd_udp1,(struct sockaddr*)&udp_cli1,sizeof(udp_cli1));
        		udp_server1.sin_family = AF_INET;
        		udp_server1.sin_port = htons(22471);
        		inet_aton("127.0.0.1",&udp_server1.sin_addr);
			int sockfd_udp2 = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP); //socket connection being established to Backend Server C
        		bzero(&udp_server2,sizeof(udp_server2));
			bzero(&udp_cli2,sizeof(udp_cli2));
                        udp_cli2.sin_family = AF_INET;
                        udp_cli2.sin_port = htons(24471);
                        inet_aton("127.0.0.1",&udp_cli2.sin_addr);
                        bind(sockfd_udp2,(struct sockaddr*)&udp_cli2,sizeof(udp_cli2));
       			udp_server2.sin_family = AF_INET;
        		udp_server2.sin_port = htons(23471);
        		inet_aton("127.0.0.1",&udp_server2.sin_addr);
			//char buf[6] = "qwert";
			//cout<<"Length of data sent test "<<sendto(sockfd_udp,&buf,strlen(buf),0,(const sockaddr*)&udp_server,sizeof(udp_server))<<endl;
			//cout<<strerror(errno)<<endl;
        		//cout<<"Here"<<endl;
			std::string b_str(b);
			count_nl = std::count(b_str.begin(),b_str.end(),'\n'); //Count the number of values to be sent to the client , Note: i am adding a new line character at the end of each value to distinguish them
			cout<<"System Log - The AWS has recieved "<<count_nl<<" numbers from the client using TCP over Port 25471"<<endl;
			cout<<"System Log - The AWS has sent    "<<count_nl/3<<" numbers to the backend server A"<<endl;
			cout<<"System Log - The AWS has sent    "<<count_nl/3<<" numbers to the backend server B"<<endl;
			cout<<"System Log - The AWS has sent    "<<count_nl/3<<" numbers to the backend server C"<<endl;
			std::stringstream ss(b);
			std::string to;
			b1 = func;
			b2 = func;
			b3 = func;
			if(b != NULL)
			{
				while(std::getline(ss,to,'\n'))
				{
					if(count < count_nl/3)
					{
					//cout<<"Printing First part"<<endl;
					//cout<<to<<endl;
					b1 = b1+to;
					b1 = b1 + "\n";
				}
				else if(count < (2*count_nl/3))
				{
					//cout<<"Printing Second part"<<endl;
					//cout<<to<<endl;
					b2 = b2 + to;
					b2 = b2 +"\n";
				}
				else if(count <(3*count_nl/3))
				{	
					//cout<<"Printing Third part"<<endl;
					//cout<<to<<endl;
					b3 = b3 + to;
					b3 = b3+ "\n";
				}
			count = count +1;
		}
	}
	//cout<<"The value of Count is "<<count<<endl;
	b1.erase(0,3);
	//cout<<"Printing First part"<<endl;
	//cout<<b1<<endl;
	//cout<<"Printing Second Part"<<endl;
	//cout<<b2<<endl;
	//cout<<"Printing Third part"<<endl;
	//cout<<b3<<endl;
	sendto(sockfd_udp,b1.data(),strlen(b1.data()),0,(const sockaddr*)&udp_server,sizeof(udp_server)); // Values being sent to backend server A
	sendto(sockfd_udp1,b2.data(),strlen(b2.data()),0,(const sockaddr*)&udp_server1,sizeof(udp_server1)); //Values being sent to backend server B
	sendto(sockfd_udp2,b3.data(),strlen(b3.data()),0,(const sockaddr*)&udp_server2,sizeof(udp_server2)); //Values being sent to backend server C
	//cout<<"Navaneeth123"<<endl;
	char bufe[11];
	char bufe1[11];
	char bufe2[11];
	int l_buf;
	long int l1,l2,l3;
	struct pollfd ufds[1];
        ufds[0].fd = sockfd_udp;
        ufds[0].events = POLLIN | POLLPRI ;
        int rv = poll(ufds,1,-1);
//Below is the poll logic to recieve data from a non blocking socket asynchronously from the 3 UDP backend servers , referenced from Chapter 16 of Reference 1 
        if(ufds[0].revents & POLLIN) 
        {
		// cout<<"I am navneeth1"<<endl;
		 //while(!len && ioctl(sockfd,FIONREAD,&len) >=0)
                 //sleep(1);
		 //bufe = (char *) malloc (sizeof(char) * l_buf);
		 recv(sockfd_udp,&l1,sizeof(l1),0); //Recieve data from backend server A
		 //cout <<"Data Recieved is "<<l1<<endl;
		 cout <<"System Log -The AWS has recieved the reduction result of "<<func<<" from backend serverA using UDP over port 24471  and it is "<<l1<<endl;
		  
	}
	struct pollfd ufds1[1];
        ufds1[0].fd = sockfd_udp1;
        ufds1[0].events = POLLIN | POLLPRI ;
        int rv1 = poll(ufds1,1,-1);
        if(ufds1[0].revents & POLLIN)
        {
		
               // cout<<"I am navneeth2"<<endl;
		recv(sockfd_udp1,&l2,sizeof(l2),0); //Recieve Data from Backend Server B
		//cout<<"Data Recieved is"<<l2<<endl;
		cout <<"System Log -The AWS has recieved the reduction result of "<<func<<" from backend serverB using UDP over port 24471  and it is "<<l2<<endl;
        }
	struct pollfd ufds2[1];
        ufds2[0].fd = sockfd_udp2;
        ufds2[0].events = POLLIN | POLLPRI ;
        int rv2 = poll(ufds2,1,-1);
        if(ufds2[0].revents & POLLIN)
        {
               // cout<<"I am navneeth3"<<endl;
		recv(sockfd_udp2,&l3,sizeof(l3),0); //Recieve Data from Backend Server C
		//cout<<strerror(errno);
		//cout<<"Data Recieved is"<<l3<<endl;
		 cout <<"System Log -The AWS has recieved the reduction result of "<<func<<" from backend serverC using UDP over port 24471  and it is "<<l3<<endl;
        }
	
	//cout<<"Hello i am navneeth nice to meet you"<<endl;
	//cout<<"Converting from string to integer of recieved values"<<endl;
	int max,min,sum,sos ;
	//cout<<"Converted Values "<<endl;
	//cout<<l1<<endl;
	//cout<<l2<<endl;
	//cout<<l3<<endl;
	if((strcmp(func,"MAX") == 0) || (strcmp(func,"max") ==0))
	{
 		max = std::max(std::max(l1, l2),l3);
		cout<<"System Log -The AWS has successfully finished the reduction MAX:"<<max<<endl;
 		send(confd,&max,sizeof(max),0); //Send Final data back to the client if function is MAX
		cout<<"System Log -The AWS has successfully finished the reduction value to the client "<<endl;
 	//	cout<<strerror(errno);
 	//	cout <<"FINAL ANSWER " <<max<<endl;
	}
	else if ((strcmp(func,"MIN") == 0) || (strcmp(func,"min") == 0))
	{
		min = std::min(std::min(l1,l2),l3);
		cout<<"System Log - The AWS has successfully finished the reduction MIN:"<<min<<endl;
		send(confd,&min,sizeof(min),0); //Send Final Data back to the client if function is MIN
	//	cout<<"FINAL ANSWER "<<min<<endl;
		cout<<"System Log - The AWS has succesfully finished the reduction value to the client "<<endl;
	}
	else if((strcmp(func,"SUM") == 0) || (strcmp(func,"sum") == 0))
	{
		sum = l1+l2+l3;
		cout<<"System Log - The AWS has successfully finished the reduction SUM:"<<sum<<endl;
		send(confd,&sum,sizeof(sum),0); //Send Final Data Back to the client if function is SUM
	//	cout<<"Final Answer "<<sum<<endl;
		cout<<"System Log - The AWS has succesffully finished the reduction value to the client "<<endl;
	}
	else if((strcmp(func,"SOS") == 0) || (strcmp(func,"sos") == 0))
	{
		sos = l1+l2+l3;
		cout<<"System Log - The AWS has successfully finished the reduction SOS:"<<sos<<endl;
		send(confd,&sos,sizeof(sum),0); //Send final Data back to the client if function is SOS
		cout<<"System Log - The AWS has successfully finished the reduction value to the client "<<endl;
	//	cout<<"Final Answer "<<sos<<endl;
	}
	close(sockfd_udp);
	close(sockfd_udp1);
	close(sockfd_udp2);
	close(confd);
	}
	}
};
class server1 : public sock
{
	public:
	server1(int p,int f) : sock(p,f){
}
	void server_logic()
	{	
		 long int len =0,i,min =INT_MAX,max=INT_MIN,l,sum=0,sos=0;
		int arr[0],num;
		char func[4] ;
		struct sockaddr_in udp_client;
		l = sizeof(udp_client);
		socklen_t *udp_c;
	//	cout<<"In server logic"<<endl;
		struct pollfd ufds[1];
	        ufds[0].fd = sockfd;
        	ufds[0].events = POLLIN | POLLPRI ;
        	int rv = poll(ufds,1,-1);
//Logic to recieve data from the AWS non blocking socket , Chapter 16 of reference 1
		if(ufds[0].revents & POLLIN)
        	{
			char b[256];
			char *bu;
			bzero(&b,256);
			int len =0;
			while(!len && ioctl(sockfd,FIONREAD,&len) >=0) //Find the length of the data to be read from the socket 
			sleep(1);
			bu =(char *) malloc(len * sizeof(char)+1);
			bzero(bu,len *sizeof(char)+1);
			//cout<<"Length of data to be read "<<len<<endl;
			recvfrom(sockfd,bu,len+1,0,(struct sockaddr*)&udp_client,(socklen_t*)&l);
			//recv(sockfd,bu,len+1,0);
			//memcpy(func,bu,sizeof(char)*3);
			//func[3] = '\0';
			sleep(1);
			memcpy(func,bu,sizeof(char)*3); //Copy out function name from the values sent by the AWS, the protocol is that the AWS sends <func name><data>
			func[3] ='\0'; //Adding Null at the end of the function
			std::string str(bu);	
			str.erase(0,3);
			int count_nl = std::count(str.begin(),str.end(),'\n');
			//cout<<port_number;
			if(port_number == 21471)
			cout<<"System Log - The ServerA has recievd "<<count_nl<<" numbers"<<endl;
			else if(port_number == 22471)
			cout<<"System Log - The Server B has recieved "<<count_nl<<" numbers"<<endl;
			else if(port_number == 23471)
			cout<<"System Log - The Server C has recieved " <<count_nl<<" numbers"<<endl;
			bu = (char *)str.c_str();
			//cout<<"Printing from function "<<str<<endl;  
			//cout<<"Data recieved"<<endl;
			std::stringstream ss(bu);
			std::string to;
			// Logic to convert number to string , Sourced from reference 2
			if(bu != NULL)
			{
				while(std::getline(ss,to,'\n'))
				{	//cout<<"Number"<<endl;
					istringstream convert(to);
					convert >> num;
					//cout<<to<<endl;
					if(num < min)
					min = num;
					if(num > max)
					{
					//cout<<"I am here";
					//cout<<"Processing num " <<num<<endl;
					max = num;
					}
					sum = sum + num ;
					sos = sos + (num * num);
				}
			}
	//cout<<" HI my function value is"<<func<<endl;
	//cout <<"The minimum value is "<<min<<endl;   
	//cout<<"The maximum value is "<<max<<endl;
	//cout<<"The sum is "<<sum<<endl;
	//cout<<"The sum of squares is "<<sos<<endl;
	//cout<<"Socket port number is"<<ntohs(udp_client.sin_port);
	//char buf[6] = "hif";
	string tmp_buffer ;
	unsigned long int t_buffer ;
	std::stringstream out ;
	if ((strcmp(func,"MAX")==0)  ||(strcmp(func,"max")==0))
	{
	//	cout<<"FUNCTION CHOSEN IS MAX"<<endl;
		out<<max;
		t_buffer = max;
	}
	else if((strcmp(func,"MIN")==0) || (strcmp(func,"min")==0))
	{
		out<<min;
		t_buffer  = min;
	}
	else if((strcmp(func,"SUM")==0) or (strcmp(func,"sum")==0))
	{
		out<<sum;
		t_buffer = sum;
	}
	else if((strcmp(func,"SOS")==0) or (strcmp(func,"sos")==0))
	{
		out<<sos;
		t_buffer = sos;
	}
	tmp_buffer = out.str();
//Since i am creating objects for the same class , appropriate error messages are printed by checking out the port numbers 
	if(port_number ==21471)
	{
		cout<<"System Log -The server A has sucessfully finished reduction "<<func<<endl;
		cout<<"System Log - The server A has successfully finished sending the reduction value to AWS"<<endl;
	}
	else if(port_number == 22471)
	{
		cout<<"System Log - The server B has sucessfully finished reduction "<<func<<endl;
		cout<<"System Log - The server B has successfully finished sending the reduction value to AWS"<<endl;
	}
	else if(port_number == 23471)
	{
		cout<<"System Log - The server C has successfully finished reduction "<<func<<endl;
		cout<<"System Log - The server C has successfully finished sending the reduction value to AWS"<<endl;
	}
	//cout<<"Temp buffer"<<t_buffer;
	sendto(sockfd,&t_buffer,sizeof(t_buffer),0,(struct sockaddr*)&udp_client,(socklen_t)l); //Processed values sent to the AWS
	//cout<<strerror(errno)<<endl;
	}

	}
};

//Signal Handler for any Control C signal , makes sure all processes are killed, also lets us know if any process has died abnormally during the execution of the program
void sig_chld(int signo)
{
	pid_t pid ;
	int stat ;
	while((pid =waitpid(-1,&stat,WNOHANG))>0)
	{
		cout<<"Child terminated \n"<<pid;
	}
}

int main()
{
	signal(SIGCHLD,sig_chld); //Registering Signal Handler 
	int t = fork();
	if(t == 0)
	{ 
 		aws t(25471,SOCK_STREAM);//Aws object created
		cout<<"System Log - The AWS is up and running "<<endl;
 		t.init_sock();
	}
	int p = fork();
	if(p == 0)
	{
		server1 s3(21471,SOCK_DGRAM); //Backend Server A object created
		cout<<"System Log - The Server A is up and running using UDP on port 21471"<<endl;
		s3.init_sock();
	}
	int p1 = fork();
	if(p1 ==0)
	{
		server1 s5(22471,SOCK_DGRAM);//Backend Server B object created
		cout<<"System Log - The Server B is up and running using UDP on port 22471"<<endl;
		s5.init_sock();
	}
	server1 s6(23471,SOCK_DGRAM);//Backend Server C object created
	cout<<"System Log - The Server C is up and running using UDP on port 23471"<<endl;
	s6.init_sock();
}

     
