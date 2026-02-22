#include<iostream>
#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h> //for sockaddr_in struct
#include<unistd.h>

int main(){
    int socketfd = -1;
    //socket creation , using SOCK_DGRAM Type
    socketfd=socket(AF_INET,SOCK_DGRAM,0);
    if(socketfd < 0){
        perror("Error Creating Socket ");
        return -1;
    }
    std::cout<<"Socket Created Successfully\n";

    //address creation and binding
    struct sockaddr_in ServerAddress{}, ClientAddress{};
    ServerAddress.sin_family = AF_INET;
    ServerAddress.sin_port = htons(9000);
    ServerAddress.sin_addr.s_addr = inet_addr("127.0.0.1");

    int ret = bind(socketfd, (struct sockaddr *)&ServerAddress, sizeof(ServerAddress));
    if(ret < 0){
        perror("Error Bindind The Socket");
        close(socketfd);
        return -1;
    }
    std::cout<<"Binding Successful to Address : "<<inet_ntoa(ServerAddress.sin_addr)<<" PORT : "<<ntohs(ServerAddress.sin_port)<<"\n";
    std::cout<<"Server Ready for Communication\n";
    char Buffer[50]{};

    //receiving message
    socklen_t CliLen = sizeof(ClientAddress);
    ret=recvfrom(socketfd,&Buffer,sizeof(Buffer),0,(struct sockaddr *)&ClientAddress, &CliLen);
    if(ret < 0){
        perror("Error receiving Message");
        close(socketfd);
        return -1; //only for this instance we are returning , normally we would handle the error
    }
    std::cout<<"Message Received From Address : "<<inet_ntoa(ClientAddress.sin_addr)<<" PORT : "<<ntohs(ClientAddress.sin_port)<<"\n";
    std::cout<<"CLIENT ::::: ";
    std::cout.write(Buffer,ret)<<"\n"; //only print the number of bytes that have been returned

    //sending message logic will come here
    close(socketfd);
    return 0;

}