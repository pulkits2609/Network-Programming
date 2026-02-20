#include<iostream>
#include<sys/socket.h>
#include<sys/types.h>
#include<unistd.h>
#include<netinet/in.h>
#include<arpa/inet.h> //for inet_addr conversion function
#include<cstdint> //for functions like memset memcpy
#include<cstring>

int main(){
	int SocketFD = -1;
	SocketFD = socket(AF_INET,SOCK_STREAM,0);
	int Connect_ret=-1;
	char Write_Buff[]="Hello from Client";
	char Read_Buff[20]{}; //initialize empty read buffer so that no garbage reads
    if(SocketFD < 0){
    perror("Socket Creation Failed"); //perror is uesd instead of printf or cout so that we know what is the reason for failure
    return -1;
	}
	std::cout<<"Socket Creation Successful \n";

	//now to connect to the server , client needs the servers address , creating client address is of no use because client
	//does not require its identity , its provided by the OS
	struct sockaddr_in ServerAddress= {}; //initialize empty server address properly
	ServerAddress.sin_family=AF_INET;
	ServerAddress.sin_port=htons(9000);
	ServerAddress.sin_addr.s_addr=inet_addr("127.0.0.1"); //servers address
	socklen_t ServerAddressLength = sizeof(ServerAddress);
	Connect_ret = connect(SocketFD,(struct sockaddr *)&ServerAddress,ServerAddressLength);
	if(Connect_ret < 0){
		perror("Server Connection Failed");
		close(SocketFD);
		return -1; //close the client , DO NOT CONTINUE
	}
	std::cout<<"Connected to Server On IP : "<<inet_ntoa(ServerAddress.sin_addr)<<" PORT : "<<ntohs(ServerAddress.sin_port)<<"\n";
	//-------------------WRITING TO SERVER------------------------
    ssize_t totalSent=0;
    ssize_t bytesToSend=sizeof(Write_Buff);
    while(totalSent < bytesToSend){
        ssize_t sent=write(SocketFD, Write_Buff + totalSent, bytesToSend - totalSent);
        if(sent <= 0){
            perror("Write Failed : ");
            close(SocketFD);
            return -1;
        }
        totalSent+=sent;
    }
	std::cout<<"Message Sent to Server Successfully\n";
    
	//-------------------READING FROM SERVER------------------------
	ssize_t totalReceived=0;
    while(totalReceived < totalSent){
        ssize_t received = read(SocketFD, Read_Buff+totalReceived, sizeof(Read_Buff)-totalReceived);
        if(received <= 0){
            perror("Read Failed : ");
            break;
        }
        totalReceived += received;
    }
    std::cout<<"Message Received From Server:\n";
    std::cout.write(Read_Buff, totalReceived);
    std::cout<<"\n";
	close(SocketFD);
	return 0;
}
