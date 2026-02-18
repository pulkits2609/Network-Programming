//this server is going to be a concurrent echo server , meaning that it can handle multiple clients at once !
//to handle multiple clients , it uses the fork() method to create child processes
//each child process is a connected client

#include<iostream>
#include<sys/socket.h>
#include<sys/types.h>
#include<unistd.h> 
#include<netinet/in.h> //defines the structs sockaddr_in , in_addr , byte order conversions like htons htonl and vice versa , and protocol like IPPROTO_TCP , IPPROTO_UDP
#include<arpa/inet.h>
#include<cstdint>
#include<cstring>
int main(){
    int SocketFD = -1; //main socket that will only listen
    int SessionFD =-1; //client session socket that handles clients !
    int pid=-1; //process id for fork
    char ReadBuffer[60];
    SocketFD=socket(AF_INET,SOCK_STREAM,0);
    if(SocketFD < 0){
        perror("Socket Creation Failed");
        return -1;
    }
    std::cout<<"Socket Creation Successfull \n";

    struct sockaddr_in SocketAddress;
    memset(&SocketAddress,0,sizeof(SocketAddress));
    SocketAddress.sin_family=AF_INET;
    SocketAddress.sin_port=htons(9000); //we have manually assigned the port here and converted it into network byte order
    SocketAddress.sin_addr.s_addr= inet_addr("127.0.0.1"); //loopback address , localhost , only local processes can connect to it
    //now that our IP Address has been created ! we bind it 

    if(bind(SocketFD,(struct sockaddr *)&SocketAddress,sizeof(SocketAddress)) < 0){
        perror("Binding to Socket Failed");
        close(SocketFD);
        return -1;
    }
    //before printing the address and port , we need to convert them appropriately
    //inet_ntoa : internet network to address
    std::cout<<"Bind Success with : "<<inet_ntoa(SocketAddress.sin_addr)<<" Port : "<<ntohs(SocketAddress.sin_port)<<"\n";
    std::cout<<"................................"; //just a separator
    //now we will be listening for client connections
    int lst_ret = -1; //value returned after return
    lst_ret = listen(SocketFD,5); //we keep the backlog 5
    if(lst_ret < 0){
        perror("Listening Failed");
        close(SocketFD);
        return -1;
    }
    std::cout<<"Listening for Incoming Clients : \n";
    //now to accept for client connections , we need a client address ! 
    struct sockaddr_in ClientAddress;
    memset(&ClientAddress,0,sizeof(ClientAddress));
    socklen_t ClientAddressLength=0;
    //we dont need to set any client address values because they will be written automatically during accept
    while(1){
        //initialize the Client Address Length Again and again
        memset(&ClientAddress,0,sizeof(ClientAddress)); //reset memory again because after a client disconnects , new client details are coming        
        ClientAddressLength=sizeof(ClientAddress);
        SessionFD=accept(SocketFD,(struct sockaddr *)&ClientAddress,&ClientAddressLength); //this will return the session fd
        if(SessionFD < 0){
            perror("Failed to Accept Client Request ");
            continue;
        }
        std::cout<<"Client Request Accepted from : "<<inet_ntoa(ClientAddress.sin_addr)<<" : "<<ntohs(ClientAddress.sin_port)<<"\n";

        ///////////////////////////FORK//////////////////////////
        pid = fork();
        if(pid>0){
            //parent process
            //the parent process wont be connecting to any client so close the session FD and continue listening for new clients
            close(SessionFD);
            continue;
        }
        else if(pid==0){
            //child process
            //each child will be processing communication with client , so we add all communication code here
            ssize_t ReadWriteBytes=0; //to have count of Number of Bytes Sent and Received
            ReadWriteBytes = read(SessionFD,ReadBuffer,sizeof(ReadBuffer));
            if(ReadWriteBytes <=0){
                perror("Error Receiving Message from Client !");
            }
            else{
                std::cout<<"Received Message From Client : \n";
                std::cout.write(ReadBuffer,ReadWriteBytes)<<"\n";
            }
            ReadWriteBytes = write(SessionFD,ReadBuffer,ReadWriteBytes); //write exactly the number of bytes that are read , not less or more
            if(ReadWriteBytes <=0){
                perror("Error Writing to client !");
            }
            else{
                std::cout<<"Written To Client Successfully ! \n";
            }
            std::cout<<"Closing Client Connection \n";
            close(SessionFD);
            close(SocketFD);//we need to close listening socket too because the child process inherits EVERYTHING FROM Parent process including the listening socket
            _exit(0); //terminate the child process after client disconnection
        }

        //////////////////////////FORK///////////////////////////

    }// while loop end (it runs infinetly until program stops)
    close(SocketFD);
    
    return 0;
}