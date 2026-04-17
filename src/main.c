#include <netinet/tcp.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>


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

    rc = listen(tcp_socket, SOMAXCONN); /*Now we are listening*/
    if (rc != 0){
        perror("listen()");
        ret = 1;
        goto exit;
    }

    exit:
        close(tcp_socket);
        return ret;
 }