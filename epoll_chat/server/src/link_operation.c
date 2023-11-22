#include "server.h"

// 遍历节点
void traverse(Head head)
{
    Node p = head->next;

    while (p != NULL)
    {
        printf("%s %s\n", p->user_name, p->msg);
        p = p->next;
    }

    printf("\n");
}

// 遍历节点打印全部信息
void traverse_all(Head head)
{
    Node p = head->next;

    while (p != NULL)
    {
        printf(" %s\n %d\n %s\n %s\n %s\n %s\n %d\n %s\n %s\n %d\n %d\n %d\n %d\n %d\n\n",
               p->client_ip,
               p->client_port,
               p->user_type,
               p->user_name,
               p->user_pw,
               p->sending_target,
               p->msg_type,
               p->msg,
               p->msg_date,
               p->sock_fd,
               p->accept_fd,
               p->online_state,
               p->need_return,
               p->request_again);

        p = p->next;
    }
}

// 打印该结点全部信息
void traverse_all_node(Message *p)
{
    if (p != NULL)
    {
        printf(" %s  %d  %s  %s  %s  %s  %d  %s  %s  %d  %d  %d  %d  %d \n",
               p->client_ip,
               p->client_port,
               p->user_type,
               p->user_name,
               p->user_pw,
               p->sending_target,
               p->msg_type,
               p->msg,
               p->msg_date,
               p->sock_fd,
               p->accept_fd,
               p->online_state,
               p->need_return,
               p->request_again);
    }
}

// 创建头节点
Head init_head()
{
    Head head = (Message *)malloc(sizeof(Message));

    if (head == NULL)
    {
        printf("headfailed\n");
        exit(1);
    }

    head->msg_type = UNDEFINED;
    head->sock_fd = -1;
    head->accept_fd = -1;
    head->online_state - 1;
    head->need_return = -1;
    head->request_again = -1;

    head->last = NULL;
    head->next = NULL;

    return head;
}

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
               int request_again)
{
    Node p = (Message *)malloc(sizeof(Message));

    if (p == NULL)
    {
        printf("node failed\n");
        exit(1);
    }
    strcpy(p->user_name, user_name);
    strcpy(p->user_pw, user_pw);
    strcpy(p->user_type,user_type);
    strcpy(p->sending_target, sending_target);
    strcpy(p->msg, msg);
    strcpy(p->msg_date, msg_date);
    strcpy(p->client_ip, client_ip);

    p->client_port = client_port;
    p->msg_type = msg_type;
    p->sock_fd = sock_fd;
    p->accept_fd = accept_fd;
    p->online_state = online_state;
    p->need_return = need_return;
    p->request_again = request_again;

    p->last = NULL;
    p->next = NULL;

    return p;
}

// 释放节点
void free_node(Node p)
{
    free(p);
}

// 节点插入(头插法)
void insert_head_node(Head head, Node p)
{
    if (head->next != NULL)
    {
        p->next = head->next;
        head->next->last = p;
    }

    p->last = head;
    head->next = p;
}

// 节点插入(尾插法)
void insert_tail_node(Head head, Node p)
{
    Node h = head;

    while (h->next != NULL)
    {
        h = h->next;
    }

    h->next = p;
    p->last = h;
}

// 查找节点
Node search_node(Head head, char user_name[MIN_SIZE])
{
    Node p = head->next;

    while (p != NULL)
    {
        if (strcmp(p->user_name, user_name) == 0)
        {
            printf("search success!\n");
            return p;
        }

        p = p->next;
    }

    printf("Didn't find the node you wanted! (return a NULL)\n");
    return NULL;
}

// 判断该结点中的accept是否在 用户链表中
Node is_accept_existing(Head head, int accept_fd)
{
    Node p = head->next;

    while (p != NULL)
    {
        if (accept_fd == p->accept_fd)
        {
            return p;
        }

        p = p->next;
    }

    return NULL;
}

// 判断该结点中的用户昵称是否在 用户链表中
int is_name_existing(Head head, char user_name[MIN_SIZE])
{
    Node p = head->next;

    while (p != NULL)
    {
        if (strcmp(p->user_name, user_name) == 0)
        {
            // printf("用户昵称已存在\n");
            return 1;
        }

        p = p->next;
    }
    // printf("用户昵称未存在\n");
    return 0;
}

// 判断该用户是否在 用户链表中, 返回该结点
Message *is_user_existing(Head head, char user_name[MIN_SIZE], char user_pw[MIN_SIZE])
{
    Node p = head->next;

    while (p != NULL)
    {
        if ((strcmp(p->user_name, user_name) == 0) && (strcmp(p->user_pw, user_pw) == 0))
        {
            // printf("用户存在\n");
            return p;
        }

        p = p->next;
    }
    // printf("用户未存在\n");
    return NULL;
}

// 删除节点
void rm_node(Node p)
{
    if (p == NULL)
    {
    }
    else if (p->next == NULL)
    {
        p->last->next = NULL;
        p->last = NULL;
    }
    else
    {

        p->last->next = p->next;
        p->next->last = p->last;

        p->next = NULL;
        p->last = NULL;
    }

    free_node(p);
}

// 删除整个链表
void rm_link(Head head)
{
    Node pre = head->next;
    Node p = pre;

    while (pre != NULL)
    {
        pre = pre->next;
        rm_node(p);
        p = pre;
    }
}

// // 双向链表
// int main(int argc, char const *argv[])
// {
//     int i;

//     char user_name[MIN_SIZE] = "1--name--1"; // 用户昵称
//     char name[MIN_SIZE] = "2--name--1"; // 用户昵称
//     char user_pw[MIN_SIZE] = "pw";   // 用户密码
//     char msg[MSG_SIZE] = "msg";       // 消息内容
//     char msg_date[DATE_SIZE] = "date"; // 消息发送时间

//     Head h1, h2;
//     h1 = init_head();
//     h2 = init_head();

//     for (i = 0; i < 2; i++)
//     {
//         insert_head_node(h1, init_node(user_name, user_pw, 66,msg, msg_date, i, 10 + i, -1, -1,-1));
//         insert_tail_node(h2, init_node(user_name, user_pw, 66,msg, msg_date, i, 70 + i, -1, -1,-1));

//         strcpy(user_name,"1--name--2");
//         strcpy(user_name,"2--name--2");
//     }

//     traverse(h1);
//     traverse(h2);

//     rm_link(h1);
//     traverse(h1);

//     traverse_all(h2);

//     return 0;
// }
