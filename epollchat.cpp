#include <sys/socket.h>
#include <sys/types.h>
#include <sys/epoll.h>

#include <netinet/in.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

int set_nonblock(int sckt)
{
    int flags = fcntl(sckt, F_GETFD);
    if(flags < 0) return flags;

    return fcntl(sckt, F_SETFD, flags | O_NONBLOCK);
}

void close_socket(int sckt)
{
    shutdown(sckt, SHUT_RDWR);
    close(sckt);
}

int main(int argc, char** argv)
{
    int ServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(ServerSocket == -1)
    {
        printf("Can't create server socket");
        return 1;
    }

    if(set_nonblock(ServerSocket))
    {
        printf("Can't set to nonblock server socket\n");
        return 1;
    }

    int reuse = 1;
    if(setsockopt(ServerSocket, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuse, sizeof(reuse)) < 0)
    {
        printf("Can't set SO_REUSEADDR\n");
        return 1;
    };

    struct sockaddr_in ServerAddr;
    ServerAddr.sin_family = AF_INET;
    ServerAddr.sin_port = htons(6666);
    ServerAddr.sin_addr.s_addr = htonl(INADDR_ANY); //0.0.0.0

    if(bind(ServerSocket, (struct sockaddr*)(&ServerAddr), sizeof(ServerAddr)) < 0)
    {
        printf("Can't connect to the address");
        return 1;
    }

    if(listen(ServerSocket, 524) < -1)
    {
        printf("Can't start to listen on server socket");
        return 1;
    }

    int efd = epoll_create1(0);
    if(efd < 0)
    {
        printf("Can't create epoll instance");
        return 1;
    }

    struct epoll_event EpollEvent;
    EpollEvent.events = EPOLLIN;
    EpollEvent.data.fd = ServerSocket; 
    if(epoll_ctl(efd, EPOLL_CTL_ADD, ServerSocket, &EpollEvent) < 0)
    {
        printf("Can't register epoll event for server socket");
        return 1;
    }

    struct epoll_event Events[250];
    int N;
    while(true)
    {
        N = epoll_wait(efd, Events, 250, -1);
        if(N == -1)
        {
            printf("%s\n", strerror(errno));
            return 1;
        } else
        {
            //I have events
            for(int i = 0; i < N; i++)
            {
                if(Events[i].events & (EPOLLOUT | EPOLLERR | EPOLLHUP))
                {
                    switch(Events[i].events)
                    {
                        case EPOLLOUT:
                            printf("EPOLLOUT\n");
                            break;
                        case EPOLLERR:
                            printf("EPOLLERR\n");
                            break;
                        case EPOLLHUP:
                            printf("EPOLLHUP\n");
                            break;
                        case EPOLLIN:
                            printf("EPOLLIN\n");
                            break;
                        default:
                            printf("can't determine the event type");
                            break;
                    }
                    continue;
                }
                if(Events[i].data.fd == ServerSocket)
                {
                    //accept new socket
                    int ClientSocket = accept(ServerSocket, 0, 0);
                    printf("Accepted socket connection\n");
                    if(ClientSocket == -1)
                    {
                        printf("Client socket connection: %s\n", strerror(errno));
                        continue;
                    }

                    if(set_nonblock(ClientSocket) == -1)
                    {
                        printf("Can't set to nonblock the client socket");
                        close_socket(ClientSocket);
                        continue;
                    }


                    struct epoll_event ClientEvent;
                    ClientEvent.events = EPOLLIN;
                    ClientEvent.data.fd = ClientSocket;

                    if(epoll_ctl(efd, EPOLL_CTL_ADD, ClientSocket, &ClientEvent) == -1)
                    {
                        printf("Can't register client socket to epoll instance");
                        close_socket(ClientSocket);
                        continue;
                    }
                    printf("Registered socket to epoll instance\n");
                } else
                {
                    //client branch
                    static char Buffer[1024];
                    int Recv = recv(Events[i].data.fd, Buffer, 1024, MSG_NOSIGNAL);
                    if(Recv == 0 || errno == EAGAIN || errno == EWOULDBLOCK)
                    {
                    } else if (Recv == -1)
                    {
                        printf("Closing socket. (Recv < 0) %s\n", strerror(errno));
                        close_socket(Events[i].data.fd);
                    } else
                    {
                        int sent = send(Events[i].data.fd, Buffer, Recv, MSG_NOSIGNAL);
                        if(sent == -1)
                        {
                            printf("Closing socket. (sent < 0) %s\n", strerror(errno));
                            close_socket(Events[i].data.fd);
                        } else 
                        {
                            //check for remained data
                        }

                    }
                }
            }
        }
    }

    return 0;
}
