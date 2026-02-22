#include<iostream>
#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<arpa/inet.h>

int main(){
    int socketfd=-1;
    int ret;
    //socket creation
    socketfd=socket(AF_INET,SOCK_DGRAM,0);
    if(socketfd < 0){
        perror("Error Creating Socket");
        return -1;
    }
    std::cout<<"Sesstion Created Successfully\n";
    //binding and address creation
    struct sockaddr_in ClientAddress{},ServerAddress{};
    ClientAddress.sin_family = (AF_INET);
    ClientAddress.sin_port = htons(9001);
    ClientAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    ret=bind(socketfd,(struct sockaddr *)&ClientAddress,sizeof(ClientAddress));
    if(ret < 0){
        perror("Error Binding Socket");
        close(socketfd);
        return -1;
    }
    std::cout<<"Bind Successful to Address : "<<inet_ntoa(ClientAddress.sin_addr)<<" PORT : "<<ntohs(ClientAddress.sin_port)<<"\n";
    
    //sending message 
    //before sending message , create server address and port as destination
    ServerAddress.sin_family = (AF_INET);
    ServerAddress.sin_port = htons(9000);
    ServerAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    socklen_t ServLen = sizeof(ServerAddress);

    char Buffer[] = "Hello From Client";
    ret=sendto(socketfd,&Buffer,sizeof(Buffer),0,(struct sockaddr *)&ServerAddress, ServLen);
    if(ret < 0){
        perror("Error Sending Message");
        close(socketfd);
        return -1;
    }
    std::cout<<"Data Sent to Server\n";
    char ReadBuffer[50]{};
    ret = recvfrom(socketfd,&ReadBuffer,sizeof(ReadBuffer),0,(struct sockaddr *)&ServerAddress,&ServLen);
    if(ret < 0){
        perror("Error Receiving Message or No Message Sent");
        close(socketfd);
        return -1;
    }
    std::cout<<"Message Received : \n";
    std::cout.write(ReadBuffer,ret)<<"\n";
    close(socketfd);
    return 0;
}
