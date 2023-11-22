#include "client.h"

// 线程1执行写函数
void *client_read(void *arg)
{
    thread_sock_t ts = *((thread_sock_t *)arg);

    // 登陆菜单界面加载
    menu(ts.sockFd);

    Message *msg = (Message *)malloc(sizeof(Message)); // 分配内存
    char *buf = (char *)malloc(sizeof(Message));

    while (1)
    {
        if (login_success)
        {
            break;
        }
    }

    setbuf(stdin, NULL);

    while (1)
    {
        fgets(msg->msg, MSG_SIZE, stdin);
        msg->msg[strlen(msg->msg) - 1] = '\0';

        if (control_var == 0)
        {
            continue;
        }

        handle_spaces(msg->msg);

        if (strcmp(msg->msg, "") == 0)
        {
            /******************************************************
             * 客户端空消息不发送处理
             *****************************************************/
        }
        else
        {
            msg->sock_fd = ts.sockFd;

            handle_client_messages(msg);

            // traverse_all(msg);
            memcpy(buf, msg, sizeof(Message));
            // printf("客户端发送一次信息:-->");
            if (send(ts.sockFd, buf, sizeof(Message), 0) == -1)
            {
                perror_t("send error\n");
            }
        }
    }

    pthread_exit(NULL);
}

// 线程2执行读函数
void *client_write(void *arg)
{
    thread_sock_t ts = *((thread_sock_t *)arg);

    Message *msg = (Message *)malloc(sizeof(Message)); // 分配内存
    char *buf = (char *)malloc(sizeof(Message));

    int len;
    while (1)
    {
        len = recv(ts.sockFd, buf, sizeof(Message), 0);
        if (len < 0)
        {
            perror_t("recv error!\n");
        }
        else if (len == 0)
        {
            printf("服务器以下线!!!\n");
            exit(1);
        }
        else
        {
            // printf("客户端接收一次信息-->");
            memcpy(msg, buf, sizeof(Message));
            // traverse_all(msg);

            /******************************************************
             * 处理服务端发来的信息
             *****************************************************/
            handle_server_messages(ts.sockFd, msg);
        }
    }

    pthread_exit(NULL);
}

int main(int argc, char const *argv[])
{
#ifdef _WIN32
    WSADATA wsaData;
    result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0)
    {
        printf("WSAStartup failed: %d\n", result);
        return 1;
    }
#elif __linux__
    // printf("This is Linux OS\n");
#else
    printf("unknown OS\n");
#endif

    thread_sock_t thread_sock;

    if ((thread_sock.sockFd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror_t("socket error\n");
    }

    struct sockaddr_in ser_addr;
    socklen_t ser_len;
    ser_len = sizeof(ser_addr);

    ser_addr.sin_family = AF_INET;
    // ser_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    ser_addr.sin_addr.s_addr = inet_addr(NET_IP);
    ser_addr.sin_port = htons(PORT);

    if (connect(thread_sock.sockFd, (struct sockaddr *)&ser_addr, ser_len) == -1)
    {
        perror_t("connect error\n");
    }

    // 线程
    pthread_t threads[NUM_THREADS];
    int rc;
    long t;

    for (t = 0; t < NUM_THREADS; t++)
    {
        rc = pthread_create(&threads[t], NULL, (t == 0) ? client_read : client_write, &thread_sock);
        if (rc)
        {
            printf("Error: return code from pthread_creatc is %d\n", rc);
            exit(-1);
        }
    }

    // 等待两个线程结束
    for (t = 0; t < NUM_THREADS; t++)
    {
        if (pthread_join(threads[t], NULL))
        {
            printf("Error: return code from pthread_join() is %d\n", rc);
            exit(-1);
        }
    }

#ifdef _WIN32
    closesocket(sockFd);
    WSACleanup();
#elif __linux__
    close(thread_sock.sockFd);
#else
    printf("unknown OS\n");
#endif

    return 0;
}