#include "client.h"

// 登陆状态
int login_success = 0;
volatile int control_var = 0; // 控制线程一是否执行的变量

// 错误处理
void perror_t(char *str)
{
    perror(str);
    exit(1);
}

// 打印消息结构体全部信息
void traverse_all(Message *p)
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

// 获取当前时间
void get_current_time(char time_string[DATE_SIZE])
{
    time_t current_time;
    current_time = time(NULL);
    strftime(time_string, DATE_SIZE, "%Y-%m-%d_%H:%M:%S", localtime(&current_time));
}

// 用户账号注册
void user_register(int fd)
{
    printf("***********账号注册界面***********\n");
    char time_string[DATE_SIZE];

    char name[MIN_SIZE];
    char password[MIN_SIZE];

    printf("输入用户昵称:");
    scanf("%s", name);

    printf("输入用户密码:");
    scanf("%s", password);

    Message *msg = (Message *)malloc(sizeof(Message));
    char *buf = (char *)malloc(sizeof(Message));

    strcpy(msg->user_name, name);
    strcpy(msg->user_pw, password);
    strcpy(msg->user_type, "NULL");
    strcpy(msg->sending_target, "NULL");
    msg->msg_type = USER_REGISTER;
    strcpy(msg->msg, "NULL");
    strcpy(msg->client_ip, "NULL");
    msg->client_port = -1;
    msg->sock_fd = fd;
    msg->accept_fd = -1;
    msg->online_state = -1;
    msg->need_return = 1;
    msg->request_again = -1;

    msg->last = NULL;
    msg->next = NULL;

    get_current_time(time_string);
    strcpy(msg->msg_date, time_string);

    memcpy(buf, msg, sizeof(Message));

    if (send(fd, buf, sizeof(Message), 0) == -1)
    {
        perror_t("send error\n");
    }

    printf("**********************************\n");
}

// 用户账号登陆
void user_login(int fd)
{
    printf("***********账号登陆界面***********\n");
    char time_string[DATE_SIZE];

    char name[MIN_SIZE];
    char password[MIN_SIZE];

    printf("输入用户昵称:");
    scanf("%s", name);

    printf("输入用户密码:");
    scanf("%s", password);

    Message *msg = (Message *)malloc(sizeof(Message));
    char *buf = (char *)malloc(sizeof(Message));

    strcpy(msg->user_name, name);
    strcpy(msg->user_pw, password);
    strcpy(msg->user_type, "NULL");
    strcpy(msg->sending_target, "NULL");
    msg->msg_type = USER_LOGIN;
    strcpy(msg->msg, "NULL");
    strcpy(msg->client_ip, "NULL");
    msg->client_port = -1;
    msg->sock_fd = fd;
    msg->accept_fd = -1;
    msg->online_state = -1;
    msg->need_return = 1;
    msg->request_again = -1;

    msg->last = NULL;
    msg->next = NULL;

    get_current_time(time_string);
    strcpy(msg->msg_date, time_string);

    memcpy(buf, msg, sizeof(Message));

    if (send(fd, buf, sizeof(Message), 0) == -1)
    {
        perror_t("send error\n");
    }

    printf("**********************************\n");
}

// 菜单
void menu(int fd)
{
    printf("*********客户端系统界面*********\n");
    printf("\t--> 注册 <--           /按 1 进入注册界面\n");
    printf("\t--> 登陆 <--           /按 2 进入登陆界面\n");
    printf("\t--> 退出 <--           /按 0 退出界面\n");
    printf("*******************************\n");

    char select[MIN_SIZE];
    int flag = 1;

    do
    {
        fgets(select, sizeof(select), stdin);
        select[strlen(select)] = '\0';

        if (strlen(select) > 2)
        {
            printf("无效的选项，请重新输入:");
        }
        else
        {
            switch (select[0])
            {
            case '0':
                printf("已退出\n");
                exit(0);

                break;
            case '1':
                user_register(fd);

                flag = 0;
                break;
            case '2':
                user_login(fd);

                flag = 0;
                break;
            default:
                printf("无效的选项，请重新输入:");
                break;
            }
        }

    } while (flag);
}

// 处理服务器发来的注册信息
void handle_user_register(int fd, Message *msg)
{
    if (1 == msg->request_again)
    {
        printf("%s", msg->msg);
        user_register(fd);
    }
    else
    {
        printf("%s", msg->msg);
        user_login(fd);
    }
}

// 处理服务器发来的登陆信息
void handle_user_login(int fd, Message *msg)
{
    if (1 == msg->request_again)
    {
        printf("%s", msg->msg);
        user_login(fd);
    }
    else if (2 == msg->online_state)
    {
        printf("%s", msg->msg);
        login_success = 1;
        control_var = 0;
    }
    else
    {
        printf("%s", msg->msg);
        login_success = 1;
        control_var = 1;
    }
}

// 处理服务器发来的普通信息
void handle_normal_message(Message *msg)
{
    printf("%s\n", msg->msg);
}

// 处理服务器发来的信息
void handle_server_messages(int fd, Message *msg)
{
    enum message_type type = msg->msg_type;

    switch (type)
    {
    case USER_REGISTER:
        handle_user_register(fd, msg);
        break;

    case USER_LOGIN:
        handle_user_login(fd, msg);
        break;

    case QUIT:
        printf("%s\n", msg->msg);
        exit(0);
        break;

    case KIKC_OUT:
        printf("%s\n", msg->msg);
        exit(0);
        break;

    case MUTE:
        control_var = 0;
        printf("%s\n", msg->msg);
        break;

    case UNMUTE:
        control_var = 1;
        printf("%s\n", msg->msg);
        break;

    default:
        handle_normal_message(msg);
        break;
    }
}

// 处理正式聊天时 客户端发给服务器的信息
void handle_client_messages(Message *msg)
{
    msg->msg_type = NORMAL_CHAT;
    strcpy(msg->sending_target, "NULL");
    strcpy(msg->user_name, "NULL");
    strcpy(msg->user_pw, "NULL");
    strcpy(msg->user_type, "NULL");
    strcpy(msg->client_ip, "NULL");
    msg->client_port = -1;
    msg->accept_fd = -1;
    msg->online_state = -1;
    msg->request_again = -1;
    msg->need_return = 1;

    msg->last = NULL;
    msg->next = NULL;

    char time_string[DATE_SIZE];
    get_current_time(time_string);
    strcpy(msg->msg_date, time_string);
}

// 将字符串数组里的左侧、右侧空格删除，将中间的空格替换为下划线
void handle_spaces(char *str)
{
    int len = strlen(str);
    int left_space = 0;
    int right_space = 0;

    int i = 0;
    while (str[i] == ' ')
    {
        left_space++;
        i++;
    }

    i = len - 1;
    while (str[i] == ' ')
    {
        right_space++;
        i--;
    }

    int j = 0;
    for (i = left_space; i < (len - right_space); i++)
    {
        str[j++] = str[i];
    }
    str[j] = '\0';

    for (i = 0; i < strlen(str); i++)
    {
        if (str[i] == ' ')
        {
            str[i] = '_';
        }
    }
}
