#include <iostream>

//#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <cstring>

int main()
{
    const int PORT = 8080;

    //1. Create socket
    int clientSocket = socket(
        AF_INET,
        SOCK_STREAM,
        0
    );

    if (clientSocket == -1)
    {
        std::cout<<"Failed create socket\n";
        return 1;
    }

    //2. Server Address
    sockaddr_in serverAddress{};

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);

    inet_pton(
        AF_INET,
        "127.0.0.1",
        &serverAddress.sin_addr
    );

    //1. Connect to server
    if (connect(
        clientSocket,
        (sockaddr*)&serverAddress,
        sizeof(serverAddress)
    ) < 0 )
    {
        std::cout<<"Connection failed!\n";
        close(clientSocket);
        return 1;
    }

    std::cout<<"Connected to server!\n";

    //4. Send mess
    const char* message = "Hello Server!\n";

    send(
        clientSocket,
        message,
        strlen(message),
        0
    );

    //5. Receive response
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

        std::cout << "Server: "
                  << buffer
                  << '\n';
    }

    // 6. Close
    close(clientSocket);

    return 0;
    
    
}