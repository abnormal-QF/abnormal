#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>      // perror
#include <sys/socket.h> // socket
#include <netinet/in.h> // struct sockaddr_in
#include <arpa/inet.h>  // inet_addr
#include <unistd.h>     // close
#include <ctype.h>      // toupper

#include <sys/epoll.h>
#include <fcntl.h>

#define NET_IP "192.168.11.128"
#define FILE_PATH_USER "../build/file_resource/all_user_list.txt"
#define FILE_PATH_RECORD "../build/file_resource/all_record_list.txt"

#define PORT 8000

#define MAX_EVENTS 128 // 结点最大个数

#define MSG_SIZE 2048
#define DATE_SIZE 22
#define MAX_SIZE 1024
#define MIN_SIZE 128

// 表情类型
enum emojis
{
    SMILING_FACE, // 笑脸
    CRYING_FACE,  // 苦脸
    NO_EMOJI      // 未定义类型
};

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
    int online_state; // 用户在线状态 -1未分配 0下线 1在线 2禁言在线  3禁言下线

    int need_return;   // 该消息是否需要返回值 -1未分配 0不需要 1需要
    int request_again; // 该消息是否需要再次请求  -1未分配 0不需要 1需要

    struct message *last; // 上一个结点
    struct message *next; // 下一个结点
} Message, *Head, *Node;

/****************************server函数声明****************************/

// 读用户
void read_user(Head user_head);

// 读记录
void read_record(Head record_head);

// 写用户
void save_user(Head user_head);

// 写记录
void save_record(Head record_head);

// 聊天室说明书
void chat_room_description(Message *msg, int desc);

// 添加记录信息
void handle_add_record(int fd, Message *msg);

// 根据fd获取用户名
void get_user_name(int fd, char name[MIN_SIZE]);

// 根据accept查找对应用户
void get_target_message(int accept_fd, Message *p);

// 返回字串下标
int find_index(const char *haystack, const char *needle);

// 删除字符串中的第一个子串
void delete_first_substring(char *str, char *substr);

// 将两下标之间的字符串赋值到 substr
void assign_substr(char *str, int start, int end, char *substr);

// 替换字符
void displace_substr(char *str, char c);

// 处理用户帮助文档指令
void handle_help_chat(Message *msg);

// 处理用户在线指令
void handle_online_user(int fd, Message *msg);

// 处理用户历史记录指令
void handle_history_record(int fd, Message *msg);

// 解析发送对象信息
void parsing_normal_messages(int fd, Message *msg);

// 用户账号注册
void handle_user_register(int fd, Message *msg);

// 用户账号登陆
void handle_user_login(int fd, Message *msg);

// 错误处理
void perror_t(char *str);

// 添加套接字
void epoll_add_fd(int epoll_fd, int fd, int trigger, int clogged);

// 移除套接字
void epoll_remove_fd(int epoll_fd, int fd);

// 发送私聊信息
void handle_private_chat(int fd, Message *msg, char *buf, int desc);

// 处理客户端发来的群聊信息
void handle_group_chat(int fd, Message *msg, char *buf);

// 新链接上线
void handle_connect(int epoll_fd, int listen_fd, Message *msg);

// 用户下线处理
void handle_user_offline(int epoll_fd, int fd, Message *msg);

// 修改该描述符的监听事件
void mod_fd_event(int epoll_fd, int fd, int ev);

// 处理客户端发来的信息
void handle_client_messages(int epoll_fd, int fd, Message *msg);

// 消息处理
void handle_message(int epoll_fd, int fd, Message *msg);

// 处理服务端发给客户端的信息
void handle_send(int epoll_fd, int fd, Message *msg);

// 事件循环
void event_loop(int epoll_fd, int listen_fd);

// 初始化已有账号套接字
void init_sock(int epoll_fd, int listen_fd);

/****************************链表函数声明****************************/
//  遍历节点
void traverse(Head head);

// 遍历节点打印全部信息
void traverse_all(Head head);

// 打印该结点全部信息
void traverse_all_node(Message *p);

// 创建头节点
Head init_head();

// 创建链表节点
Node init_node(char user_name[MIN_SIZE],
               char user_pw[MIN_SIZE],
               char user_type[MIN_SIZE],
               char sending_target[MIN_SIZE],
               int msg_type,
               char msg[MSG_SIZE],
               char msg_date[DATE_SIZE],
               char client_ip[MIN_SIZE],
               int client_port,
               int sock_fd,
               int accept_fd,
               int online_state,
               int need_return,
               int request_again);

// 释放节点
void free_node(Node p);

// 节点插入(头插法)
void insert_head_node(Head head, Node p);

// 节点插入(尾插法)
void insert_tail_node(Head head, Node p);

// 查找节点
Node search_node(Head head, char user_name[MIN_SIZE]);

// 判断该结点中的accept是否在 用户链表中
Node is_accept_existing(Head head, int accept_fd);

// 判断该结点中的用户昵称是否在 用户链表中
int is_name_existing(Head head, char user_name[MIN_SIZE]);

// 判断该用户是否在 用户链表中, 返回该结点
Message *is_user_existing(Head head, char user_name[MIN_SIZE], char user_pw[MIN_SIZE]);

// 删除节点
void rm_node(Node p);

// 删除整个链表
void rm_link(Head head);

#endif // SERVER_H