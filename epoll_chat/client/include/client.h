#ifndef CLIENT_H
#define CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>  // perror
#include <unistd.h> // close
#include <ctype.h>  // toupper

#ifdef _WIN32
#include <winsock2.h> // socket, struct sockaddr_in
#include <ws2tcpip.h> // inet_addr
#include <windows.h>  // fd_set,select
// printf("This is Windows OS\n");

#elif __linux__
#include <sys/socket.h> // socket
#include <netinet/in.h> // struct sockaddr_in
#include <arpa/inet.h>  // inet_addr
// printf("This is Linux OS\n");

#else
printf("unknown OS\n");

#endif

#define NET_IP "192.168.11.128"
#define PORT 8000

#define MSG_SIZE 2048
#define DATE_SIZE 22
#define MAX_SIZE 1024
#define MIN_SIZE 128

#include <time.h>

#include <pthread.h>
#define NUM_THREADS 2

// 登陆状态
extern int login_success;
extern volatile int control_var; // 控制线程一是否执行的变量

// 消息类型
enum message_type
{
    USER_REGISTER = 100, // 注册
    USER_LOGIN,          // 登陆
    NORMAL_CHAT,         // 正常聊天
    GROUP_CHAT,          // 群聊
    PRIVATE_CHAT,        // 私聊
    HELP_CHAT,           // 帮助
    ONLINE_USER,         // 在线用户
    QUIT,                // 退出
    HISTORY_RECORD,      // 历史记录
    KIKC_OUT,            // 踢出用户
    MUTE,                // 禁言
    UNMUTE,              // 解除禁言
    UNDEFINED            // 未定义类型
};

// 消息结构体
typedef struct message
{
    char user_name[MIN_SIZE];      // 用户昵称
    char user_pw[MIN_SIZE];        // 用户密码
    char user_type[MIN_SIZE];      // 用户身份 regular_user普通用户  Administrator管理员
    char sending_target[MIN_SIZE]; // 此信息发送目标 NULL未分配

    int msg_type;             // 消息类型
    char msg[MSG_SIZE];       // 消息内容
    char msg_date[DATE_SIZE]; // 消息发送时间

    char client_ip[MIN_SIZE]; // 客户端ip地址
    int client_port;          // 客户端端口 -1未分配

    int sock_fd;      // 客户端分配的套接字 -1未分配
    int accept_fd;    // 服务端分配的套接字 -1未分配
    int online_state; // 用户在线状态 -1未分配 0下线 1在线 2禁言

    int need_return;   // 该消息是否需要返回值 -1未分配 0不需要 1需要
    int request_again; // 该消息是否需要再次请求  -1未分配 0不需要 1需要

    struct message *last; // 上一个结点
    struct message *next; // 下一个结点
} Message;

// 定义线程函数的参数结构体
typedef struct
{
    int sockFd;
    pthread_t thread_id;
} thread_sock_t;

// 错误处理
void perror_t(char *str);

// 打印消息结构体全部信息
void traverse_all(Message *p);

// 获取当前时间
void get_current_time(char time_string[DATE_SIZE]);

// 用户账号注册
void user_register(int fd);

// 用户账号登陆
void user_login(int fd);

// 菜单
void menu(int fd);

// 处理服务器发来的注册信息
void handle_user_register(int fd, Message *msg);

// 处理服务器发来的登陆信息
void handle_user_login(int fd, Message *msg);

// 处理服务器发来的普通信息
void handle_normal_message(Message *msg);

// 处理服务器发来的信息
void handle_server_messages(int fd, Message *msg);

// 处理正式聊天时 客户端发给服务器的信息
void handle_client_messages(Message *msg);

// 将字符串数组里的左侧、右侧空格删除，将中间的空格替换为下划线
void handle_spaces(char *str);

#endif // CLIENT_H