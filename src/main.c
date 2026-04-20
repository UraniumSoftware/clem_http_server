#include <netinet/tcp.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int handle_client_read(int socket){
    ssize_t n = 0;
    char buffer[100];

    for (;;){
        memset(buffer, 0, sizeof(buffer));
        n = read(socket, buffer, sizeof(buffer)-1);
        if (n < 0){
            perror("read(client);");
            return -1;
        }
        else if (n == 0) {
            printf("\nclient closed gracefully\n");
            break;
        }
        printf("%s\n", buffer);

    }
    return 0;
}

int main(void){

    int ret = 0;

    int tcp_socket = 0;
    tcp_socket = socket(
        AF_INET,  /*IP v4*/
        SOCK_STREAM, /*TCP enters here*/
        0   /*Irrelevant*/
    );
    if (tcp_socket == -1){
        perror("socket()");
        ret = 1;
        goto exit;
    }

    printf("Socket opened :)\n");

    struct sockaddr_in bind_addr = {0}; /*Initializes IPv4 socket*/
    bind_addr.sin_port =  htons(420), /*Port Number listening on*/
    bind_addr.sin_family = AF_INET, /*The family of the request (IPv4)*/
    bind_addr.sin_addr.s_addr= htonl(INADDR_LOOPBACK); /*Sets listening IP to localhost; not good for a functional http server but for now is here for safety reasons*/

    int rc = 0;
    rc = bind(tcp_socket,(const struct sockaddr*)&bind_addr,sizeof(bind_addr)); /*Binds socket to and address, gives it port and IP data which we cast into a generic, and size of the bind addr*/
    if (rc<0){
        perror("bind()");
        ret = 1;
        goto exit;
    }
    
    printf("Socket has a port and IP binded to it!\n");

    rc = listen(tcp_socket, SOMAXCONN); /*Now we are listening*/
    if (rc != 0){
        perror("listen()");
        ret = 1;
        goto exit;
    }

    int client_socket = 0; 

    for (;;){ /*While true loop that loops the waiting to accept a request*/
    client_socket = accept(tcp_socket, NULL,NULL);
    if (client_socket < 0){
        perror("accept();");
        ret = 1;
        goto exit;
    }

    printf("Socket listened and accepted a request!\n");
    if (handle_client_read(client_socket) != 0){
        ret = 1;
        goto exit;
    }
    printf("\nYipee!\n");
    }
    exit:
        close(tcp_socket);
        return ret;
 }