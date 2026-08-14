#include <iostream>
#include <cstring>

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int main()
{
    const int PORT = 8080;

    //1. Create socket
    int serverSocket = socket(
        AF_INET,
        SOCK_STREAM,
        0
    );

    if (serverSocket == -1)
    {
        std::cout<<"Failed to create socket\n";
        return 1;
    }

    //2. Create server address
    sockaddr_in serverAddress{};

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(PORT);

    //3. Bind socket to IP and PORT
    if (bind(
        serverSocket,
        (sockaddr*)&serverAddress,
        sizeof(serverAddress)
    )<0)
    {
       std::cout<<"Bind failed\n";
       close(serverSocket);
       return 1;
    }

    //4. Listen
    if (listen(serverSocket , 5 ) < 0 )
    {
        std::cout<<"Listen failed\n";
        close(serverSocket);
        return 1;
    }
    
    std::cout<<"Server is listening on port" << PORT << "....\n";

    //5. Accept client
    int clientSocket = accept(
        serverSocket,
        nullptr,
        nullptr
    );

    if (clientSocket < 0)
    {
        std::cout<<"Accept failed\n";
        close(serverSocket);
        return 1;
    }

    std::cout<<"Client connected!\n";

    //6. Receive data
    char buffer[1024];

    int bytesReceived = recv(
        clientSocket,
        buffer,
        sizeof(buffer) - 1,
        0
    );

    if (bytesReceived > 0)
    {
        buffer[bytesReceived] = '\0';
        std::cout<<"Client : "<< buffer << "\n";

        //7. Echo data back
        send(
            clientSocket,
            buffer,
            bytesReceived,
            0
        );
    }
    
    //8. Close
    close(clientSocket);
    close(serverSocket);

    return 0;
  
}