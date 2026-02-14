#include<iostream>
#include<sys/socket.h> //for os level socket function calls
#include<sys/types.h> //for socket datatypes like length of socket
#include<sys/un.h> //socket family struct
#include<unistd.h> //read and write functions
#include<string>
#include<cstring> //for strcpy
int main(){
    int SocketFD = -1;
    int SessionFD = -1;
    char ReadBuffer[30];
    char WriteBuffer[] = "Server Says HELLO , KEMCHO !!!";

    SocketFD=socket(AF_UNIX,SOCK_STREAM,0);
    if(SocketFD < 0){
        std::cout<<"Failed to create Socket";
        return  -1;
    }
    std::cout<<"Socket Created Successfully";
    //after creating the socket we create the socket address
    struct sockaddr_un Server_Address={};
    Server_Address.sun_family=AF_UNIX;
    strcpy(Server_Address.sun_path,"./LocalSocketFile");

    //remove existing socket file if it exists
    unlink("./LocalSocketFile");
    //this unlinking is being done because suppose the program crashes , or stops abruptly , then the socket filesystem entry of that LocalSocketFile Remains , then the next time we try binding it , we will get the error where it might say that address already in use ! therefore before binding we unlink it ! 
    if(bind(SocketFD,(struct sockaddr *)&Server_Address,sizeof(Server_Address)) < 0){
        //we bind the socket with address , and check if it was done successfully or not !
        std::cout<<"Bind Failedn\n";
        close(SocketFD);
        return -1;
    }
    std::cout<<"Bind Success\n";
    //after binding we start to listen for active connections 
    listen(SocketFD,5); //5 Backlog
    std::cout<<"Listening.......... ajao bhai koi to\n";

    //accepting connections
    struct sockaddr_un ClientAddress={}; //empty initialization to avoid garbage
    while(1){
        socklen_t SocketLength = sizeof(ClientAddress);
        SessionFD = accept(SocketFD,(struct sockaddr *)&ClientAddress,&SocketLength);
        //when connection is established , we create a new socket for communication , because if we use the ServerSocket then the Server will stop working and solely work to interact with 1 client
        if(SessionFD < 0){
            std::cout<<"Accept Failed\n";
            continue; //dont close the server await more connections
        }
        ssize_t ReadWriteBytes = read(SessionFD,ReadBuffer,sizeof(ReadBuffer)); //if the content sent by the client is more than the size of ReadBuffer ,that data will still stay in the socket file , its just that it wont be read therefore it is important to ensure that the size of data is within a common bound of both server and client
        if(ReadWriteBytes <= 0){
            std::cout<<"Error in reading message from client\n";
        }
        else{
            std::cout<<"Data Received From The Client : "<<ReadBuffer<<"\n";
        }
        //send message to client
        ReadWriteBytes = write(SessionFD,WriteBuffer,sizeof(WriteBuffer));
        if(ReadWriteBytes <= 0){
            std::cout<<"Error Writing Data to Client\n";
        }
        else{
            std::cout<<"Data Sent To Client\n";
        }
        close(SessionFD);
        std::cout<<"Connection Closed with Client\n";
    } //end of while loop
    close(SocketFD);
    std::cout<<"Server Socket Closed\n";
    unlink("./LocalSocketFile");
    return 0;
}