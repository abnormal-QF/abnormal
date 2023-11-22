#include "server.h"

// 错误用法
// Head user_head = init_head();

Message sentry = {"", "", "", "", UNDEFINED, "", "", "", -1, -1, -1, -1, -1, -1, NULL, NULL};

// 用户头结点
Head user_head = &sentry;

// 记录头结点
Head record_head = &sentry;

// 读用户
void read_user(Head user_head)
{
    FILE *fp = fopen(FILE_PATH_USER, "a+");

    fseek(fp, 0, SEEK_END);
    long pos = ftell(fp);

    if (fp == NULL || pos < 1)
    {
        fp = fopen(FILE_PATH_USER, "w+");
        return;
    }

    fseek(fp, 0, SEEK_SET);

    while (!feof(fp))
    {
        Node p = (Message *)malloc(sizeof(Message));

        fscanf(fp, "%s %d %s %s %s %s %d %d %d\n",
               p->client_ip,
               &p->client_port,
               p->user_type,
               p->user_name,
               p->user_pw,
               p->msg_date,
               &p->sock_fd,
               &p->accept_fd,
               &p->online_state);

        p->msg_type = UNDEFINED;
        strcpy(p->msg, "NULL");
        p->need_return = -1;
        p->request_again = -1;
        p->last = NULL;
        p->next = NULL;

        insert_tail_node(user_head, p);
    }

    if (fclose(fp))
    {
        printf("关闭文件 read_user 失败\n");
        exit(1);
    }
}

// 读记录
void read_record(Head record_head)
{
    FILE *fp = fopen(FILE_PATH_RECORD, "a+");

    fseek(fp, 0, SEEK_END);
    long pos = ftell(fp);

    if (fp == NULL || pos < 1)
    {
        fp = fopen(FILE_PATH_RECORD, "w+");
        return;
    }

    fseek(fp, 0, SEEK_SET);

    while (!feof(fp))
    {
        Node p = (Message *)malloc(sizeof(Message));

        fscanf(fp, "%s %s %s %s %d %s\n",
               p->client_ip,
               p->msg_date,
               p->user_type,
               p->user_name,
               &p->msg_type,
               p->msg);

        strcpy(p->user_pw, "NULL");
        strcpy(p->sending_target, "NULL");

        p->sock_fd = -1;
        p->accept_fd = -1;
        p->client_port = -1;
        p->need_return = -1;
        p->request_again = -1;
        p->last = NULL;
        p->next = NULL;

        insert_tail_node(record_head, p);
    }

    if (fclose(fp))
    {
        printf("关闭文件 read_record 失败\n");
        exit(1);
    }
}

// 写用户
void save_user(Head user_head)
{
    FILE *fp = fopen(FILE_PATH_USER, "w");

    if (fp == NULL)
    {
        printf("不能打开 save_user 文件！\n");
        exit(1);
    }

    Node p = user_head->next;

    while (p != NULL)
    {
        fprintf(fp, "%-15s   %-6d  %s\t %-10s %-10s %s\t %d\t %d\t %d\n",
                p->client_ip,
                p->client_port,
                p->user_type,
                p->user_name,
                p->user_pw,
                p->msg_date,
                p->sock_fd,
                p->accept_fd,
                p->online_state);

        p = p->next;
    }

    if (fclose(fp))
    {
        printf("关闭文件 save_user 失败\n");
        exit(1);
    }

    rm_link(user_head);
}

// 写记录
void save_record(Head record_head)
{
    FILE *fp = fopen(FILE_PATH_RECORD, "w");

    if (fp == NULL)
    {
        printf("不能打开 save_record 文件！\n");
        exit(1);
    }

    Node p = record_head->next;

    while (p != NULL)
    {
        fprintf(fp, "%-15s   %s\t %s\t %-10s %d\t %s\n",
                p->client_ip,
                p->msg_date,
                p->user_type,
                p->user_name,
                p->msg_type,
                p->msg);

        p = p->next;
    }

    if (fclose(fp))
    {
        printf("关闭文件 save_record 失败\n");
        exit(1);
    }

    rm_link(record_head);
}

// 聊天室说明书
void chat_room_description(Message *msg, int desc)
{
    if (1 == desc)
    {
        // 登陆文档声明
        strcpy(msg->msg, "账号登陆成功\n");
        strcat(msg->msg, "********************************欢迎使用友寻聊天室*******************************\n");
        strcat(msg->msg, "\t作者: abnormal-QF\n");
        strcat(msg->msg, "\t声明: 友寻聊天室是一款基于epoll I/O多路复用、支持多个用户的互联网聊天应用程序\n");
        strcat(msg->msg, "\t使用: 输入#help指令查看帮助文档能帮助您更好的使用友寻聊天室\n");
        strcat(msg->msg, "\t祝词: 交流沟通启心智，隔屏相望增友情。\n");
        strcat(msg->msg, "\t      适度交友宽视野，结识良朋共前行。\n\n");
        strcat(msg->msg, "项目开源地址: https://github.com/abnormal-QF/abnormal\n");
        strcat(msg->msg, "*********************************************************************************\n");

        if (2 == msg->online_state)
        {
            strcat(msg->msg, "您已被管理员禁言，解除禁言后才能正常聊天哦~\n");
        }
    }
    else if (2 == desc)
    {
        // 帮助文档
        strcpy(msg->msg, "********************************友寻聊天室帮助文档*******************************\n");
        strcat(msg->msg, "\t\t\t阅读以下信息能帮助您更好的使用本聊天室\n\n");
        strcat(msg->msg, "    输入内容        相关说明                               举例\n");
        strcat(msg->msg, "  1.@聊天对象/      私聊发送，信息只发送给在线目标对象     eg:@asd/这是信息内容\n");
        strcat(msg->msg, "  2.@*/             群聊发送，信息发送给所有在线对象       eg:@*/这是信息内容\n");
        strcat(msg->msg, "  3.#指令集         向服务器发送特殊指令(大小写皆可)       eg:#help\n");
        strcat(msg->msg, "                    #help      获取帮助文档\n");
        strcat(msg->msg, "    相关指令集      #quit      退出聊天室\n");
        strcat(msg->msg, "                    #online    查看在线用户\n");
        strcat(msg->msg, "                    #history   查看最近20条历史记录\n");
        strcat(msg->msg, "                    #...       敬请期待\n\n");
        strcat(msg->msg, "  4.:               表情符号快捷发送,英文:后跟相关符号     eg::)\n");
        strcat(msg->msg, "                    :(         苦脸\n");
        strcat(msg->msg, "    表情符号集      :)         笑脸\n");
        strcat(msg->msg, "                    :...       敬请期待\n\n");
        strcat(msg->msg, "  5.@对象/welcome   常用话语，对其中某些部分进行替换        eg:@admin/welcome\n");
        // strcat(msg->msg, "                    :(         苦脸\n");
        // strcat(msg->msg, "    常用话语集      :)          笑脸\n");
        // strcat(msg->msg, "                    :...       敬请期待\n\n");

        if (0 == strcmp(msg->user_type, "Administrator"))
        {
            strcat(msg->msg, "                 ********身为管理员的您持有以下特殊指令********\n");
            strcat(msg->msg, "                    @对象/#out      踢出目标用户\n");
            strcat(msg->msg, "                    @对象/#mute     禁言目标用户\n");
            strcat(msg->msg, "                    @对象/#unmute   解除禁言目标用户\n");
            strcat(msg->msg, "                 **********************************************\n");
        }

        strcat(msg->msg, "  5.这是信息内容    不符合上述任何要求，消息默认群发       eg:这是信息内容\n");
        strcat(msg->msg, "*********************************************************************************");
    }
    else
    {
        printf("聊天室说明书: ...敬请期待\n");
    }
}

// 添加记录信息
void handle_add_record(int fd, Message *msg)
{
    read_record(record_head);

    struct sockaddr_in cli_addr;
    socklen_t cli_len = sizeof(cli_addr);
    getpeername(fd, (struct sockaddr *)&cli_addr, &cli_len);

    Node p = init_node(msg->user_name,
                       msg->user_pw,
                       msg->user_type,
                       msg->sending_target,
                       msg->msg_type,
                       msg->msg,
                       msg->msg_date,
                       inet_ntoa(cli_addr.sin_addr),
                       ntohs(cli_addr.sin_port),
                       msg->sock_fd,
                       msg->accept_fd,
                       msg->online_state,
                       msg->need_return,
                       msg->request_again);

    insert_tail_node(record_head, p);

    save_record(record_head);
}

// 根据accept查找对应用户
void get_target_message(int accept_fd, Message *p)
{
    read_user(user_head);
    Message *msg = user_head->next;

    while (msg != NULL)
    {
        if (msg->accept_fd == accept_fd)
        {
            strcpy(p->client_ip, msg->client_ip);
            p->client_port = msg->client_port;

            strcpy(p->user_name, msg->user_name);
            strcpy(p->user_pw, msg->user_pw);
            strcpy(p->user_type, msg->user_type);
            strcpy(p->msg_date, msg->msg_date);
            p->sock_fd = msg->sock_fd;
            p->accept_fd = msg->accept_fd;
            p->online_state = msg->online_state;

            p->msg_type = UNDEFINED;
            strcpy(p->sending_target, "NULL");
            strcpy(p->msg, "NULL");
            p->need_return = -1;
            p->request_again = -1;
            p->last = NULL;
            p->next = NULL;

            break;
        }

        msg = msg->next;
    }

    save_user(user_head);
}

// 根据fd获取用户名
void get_user_name(int fd, char name[MIN_SIZE])
{
    read_user(user_head);

    Node p = user_head->next;
    while (p != NULL)
    {
        if (fd == p->accept_fd)
        {
            strcpy(name, p->user_name);
            break;
        }

        p = p->next;
    }
    save_user(user_head);
}

// 返回字串下标
int find_index(const char *haystack, const char *needle)
{
    const char *found = strstr(haystack, needle);
    if (found)
    {
        return found - haystack;
    }
    else
    {
        return -1;
    }
}

// 删除字符串中的第一个子串
void delete_first_substring(char *str, char *substr)
{
    int len = strlen(substr);
    if (strstr(str, substr) != NULL)
    {
        memmove(strstr(str, substr), strstr(str, substr) + len, strlen(strstr(str, substr) + len) + 1);
    }
}

// 将两下标之间的字符串赋值到 substr
void assign_substr(char *str, int start, int end, char *substr)
{
    int len = strlen(str);
    if (start >= 0 && start < len && end >= 0 && end <= len)
    {
        strncpy(substr, str + start, end - start + 1);
        substr[end - start + 1] = '\0';
    }
    else
    {
        printf("Error: index out of range.\n");
    }
}

// 替换字符
void displace_substr(char *str, char c)
{
    int i = 0;
    while ('\0' != str[i])
    {
        if (c == str[i])
        {
            str[i] = ' ';
            break;
        }

        i++;
    }
}

// 处理用户帮助文档指令
void handle_help_chat(Message *msg)
{
    chat_room_description(msg, 2);
}

// 处理用户在线指令
void handle_online_user(int fd, Message *msg)
{
    read_user(user_head);

    sprintf(msg->msg, "********以下是在线用户名列表********\n\n");

    Node p = user_head->next;
    while (p != NULL)
    {
        if (fd != p->accept_fd && ((1 == p->online_state) || (2 == p->online_state)))
        {
            strcat(msg->msg, "\t\t");
            strcat(msg->msg, p->user_name);
            strcat(msg->msg, "\n");
        }

        p = p->next;
    }
    strcat(msg->msg, "\n************************************");

    save_user(user_head);
}

// 处理用户历史记录指令
void handle_history_record(int fd, Message *msg)
{
    read_record(record_head);

    sprintf(msg->msg, "***********以下是最近20条历史记录***********\n\n");

    int round = 0;
    int len = 0;
    Node p = user_head->next;
    while (p->next != NULL)
    {
        p = p->next;
        len++;
    }

    p = user_head->next;
    while (p->next != NULL)
    {
        round++;
        if (len - round > 20)
        {
            p = p->next;
            continue;
        }
        else
        {
            strcat(msg->msg, p->msg);
            strcat(msg->msg, "\n");
        }

        p = p->next;
    }

    strcat(msg->msg, "\n************************************\n");

    save_record(record_head);
}

// 解析发送对象信息
void parsing_normal_messages(int fd, Message *msg)
{
    // traverse_all_node(msg);

    Message *p = (Message *)malloc(sizeof(Message));
    get_target_message(fd, p);
    printf("%s 信息来啦\n", p->user_name);
    printf("身份:%s\n", p->user_type);
    strcpy(msg->user_name, p->user_name);
    strcpy(msg->user_type, p->user_type);

    char tmp[MAX_SIZE];
    char content_str[MAX_SIZE];
    strcpy(tmp, msg->msg);

    sprintf(msg->msg, "%s", tmp);
    msg->msg[strlen(msg->msg)] = '\0';
    handle_add_record(fd, msg);

    int index = find_index(tmp, "/");
    enum message_type select = UNDEFINED;
    enum emojis emm = NO_EMOJI;

    if ('@' == tmp[0])
    {
        assign_substr(tmp, index + 1, strlen(tmp), content_str);
        printf("-->%s\n", content_str);

        if (index < 2)
        {
            msg->msg_type = GROUP_CHAT;
        }
        else if (2 == index && tmp[1] == '*')
        {
            msg->msg_type = GROUP_CHAT;
        }
        else if (0 == strcmp(content_str, "welcome") || 0 == strcmp(content_str, "WELCOME")) // 欢迎
        {
            assign_substr(tmp, 1, index - 1, msg->sending_target);
            sprintf(tmp, "%s大侠，欢迎你来到友寻聊天室😎", msg->sending_target);
            msg->msg_type = GROUP_CHAT;
        }
        else if (0 == strcmp(msg->user_type, "Administrator") && (0 == strcmp(content_str, "#out") || 0 == strcmp(content_str, "#OUT"))) // 强制目标下线
        {
            assign_substr(tmp, 1, index - 1, msg->sending_target);
            msg->msg_type = KIKC_OUT;
            select = KIKC_OUT;
        }
        else if (0 == strcmp(msg->user_type, "Administrator") && (0 == strcmp(content_str, "#mute") || 0 == strcmp(content_str, "#MUTE"))) // 强制目标禁言
        {
            assign_substr(tmp, 1, index - 1, msg->sending_target);
            msg->msg_type = MUTE;
            select = MUTE;
        }
        else if (0 == strcmp(msg->user_type, "Administrator") && (0 == strcmp(content_str, "#unmute") || 0 == strcmp(content_str, "#UNMUTE"))) // 强制目标解除禁言
        {
            assign_substr(tmp, 1, index - 1, msg->sending_target);
            msg->msg_type = UNMUTE;
            select = UNMUTE;
        }
        else
        {
            assign_substr(tmp, 1, index - 1, msg->sending_target);
            msg->msg_type = PRIVATE_CHAT;
        }
    }
    else if ('#' == tmp[0])
    {
        if ((0 == strcmp(msg->msg, "#help")) || (0 == strcmp(msg->msg, "#HELP")))
        {
            msg->msg_type = HELP_CHAT;
            select = HELP_CHAT;
        }
        else if ((0 == strcmp(msg->msg, "#quit")) || (0 == strcmp(msg->msg, "#QUIT")))
        {
            msg->msg_type = QUIT;
            select = QUIT;
        }
        else if ((0 == strcmp(msg->msg, "#online")) || (0 == strcmp(msg->msg, "#ONLINE")))
        {
            msg->msg_type = ONLINE_USER;
            select = ONLINE_USER;
        }
        else if ((0 == strcmp(msg->msg, "#history")) || (0 == strcmp(msg->msg, "#HISTORY")))
        {
            msg->msg_type = HISTORY_RECORD;
            select = HISTORY_RECORD;
        }
        else if (0 == strcmp(msg->user_type, "Administrator") && (0 == strcmp(msg->msg, "#downline") || 0 == strcmp(msg->msg, "#DOWNLINE"))) // 管理员让服务器下线
        {
            printf("服务器下线咯~\n");
            exit(0);
        }
        else
        {
            msg->msg_type = GROUP_CHAT;
        }
    }
    else if (':' == tmp[0])
    {
        if ((0 == strcmp(msg->msg, ":)")) || (0 == strcmp(msg->msg, ":）")))
        {
            // sprintf(tmp, "%s在这里向大家做了个笑🦐脸!", msg->user_name);
            sprintf(tmp, "在这里向大家做了个笑🦐脸!");
        }
        else if ((0 == strcmp(msg->msg, ":(")) || (0 == strcmp(msg->msg, ":（")))
        {
            // sprintf(tmp, "%s在这里向大家做了个哭脸!", msg->user_name);
            sprintf(tmp, "在这里向大家做了个哭🥲脸!");
        }

        msg->msg_type = GROUP_CHAT;
    }
    else
    {
        msg->msg_type = GROUP_CHAT;
    }

    if (0 == strcmp(p->user_type, "Administrator"))
    {
        sprintf(msg->msg, "管%s:%s", msg->user_name, tmp);
        msg->msg[strlen(msg->msg)] = '\0';
    }
    else
    {
        sprintf(msg->msg, "%s:%s", msg->user_name, tmp);
        msg->msg[strlen(msg->msg)] = '\0';
    }

    switch (select)
    {
    case HELP_CHAT:
        handle_help_chat(msg);
        break;

    case QUIT:
        sprintf(msg->msg, "您已下线!");
        break;

    case KIKC_OUT:
        sprintf(msg->msg, "您已被管理员< %s >强制拿下!", msg->user_name);
        break;

    case MUTE:
        sprintf(msg->msg, "您已被管理员< %s >强制禁言!", msg->user_name);
        break;

    case UNMUTE:
        sprintf(msg->msg, "您已被管理员< %s >解除禁言！现在可以正常聊天啦~", msg->user_name);
        break;

    case ONLINE_USER:
        handle_online_user(fd, msg);
        break;

    case HISTORY_RECORD:
        handle_history_record(fd, msg);
        break;
    default:
        break;
    }
}

// 用户账号注册
void handle_user_register(int fd, Message *msg)
{
    read_user(user_head);

    int flag = is_name_existing(user_head, msg->user_name);
    if (flag)
    {
        strcpy(msg->msg, "用户名已存在,请重新注册\n\n");
        msg->need_return = 0;
        msg->request_again = 1;
        save_user(user_head);
        return;
    }

    // printf("注册fd:%d\n", fd);

    struct sockaddr_in cli_addr;
    socklen_t cli_len = sizeof(cli_addr);
    getpeername(fd, (struct sockaddr *)&cli_addr, &cli_len);

    strcpy(msg->msg, "账号注册成功,请登录\n\n");
    msg->need_return = 0;
    msg->request_again = 0;

    Node p = init_node(msg->user_name,
                       msg->user_pw,
                       msg->user_type,
                       msg->sending_target,
                       msg->msg_type,
                       msg->msg,
                       msg->msg_date,
                       inet_ntoa(cli_addr.sin_addr),
                       ntohs(cli_addr.sin_port),
                       msg->sock_fd,
                       msg->accept_fd,
                       msg->online_state,
                       msg->need_return,
                       msg->request_again);

    insert_tail_node(user_head, p);

    save_user(user_head);
}

// 用户账号登陆
void handle_user_login(int fd, Message *msg)
{
    read_user(user_head);
    enum message_type type = msg->msg_type;
    char *buf = (char *)malloc(sizeof(Message));

    Message *p = is_user_existing(user_head, msg->user_name, msg->user_pw);
    if (p == NULL)
    {
        strcpy(msg->msg, "该账号未注册,请重新登陆\n\n");
        msg->need_return = 0;
        msg->request_again = 1;
        save_user(user_head);
        return;
    }
    else if ((1 == p->online_state) || (2 == p->online_state))
    {
        strcpy(msg->msg, "该账号已登陆,请换一个账号登陆\n\n");
        msg->need_return = 0;
        msg->request_again = 1;
        save_user(user_head);
        return;
    }

    int ret;
    Node all_p = user_head->next;

    msg->msg_type = UNDEFINED;

    if (0 == strcmp(p->user_type, "Administrator"))
    {
        sprintf(msg->msg, "管理员< %s >已上线，注意文明聊天！", msg->user_name);
    }
    else
    {
        sprintf(msg->msg, "用户< %s >上线啦~", msg->user_name);
    }

    memcpy(buf, msg, sizeof(Message));

    while (all_p != NULL)
    {
        if (fd != all_p->accept_fd && ((1 == all_p->online_state) || (2 == all_p->online_state)))
        {
            ret = send(all_p->accept_fd, buf, sizeof(Message), 0);
            if (ret < 0)
            {
                perror_t("send error");
            }
        }

        all_p = all_p->next;
    }

    // printf("登陆fd:%d\n", fd);
    strcpy(msg->user_type, p->user_type);
    msg->msg_type = type;
    struct sockaddr_in cli_addr;
    socklen_t cli_len = sizeof(cli_addr);
    getpeername(fd, (struct sockaddr *)&cli_addr, &cli_len);

    msg->need_return = 0;
    msg->request_again = 0;

    if (3 == p->online_state)
    {
        msg->online_state = 2;
        p->online_state = 2;
    }
    else
    {
        msg->online_state = 1;
        p->online_state = 1;
    }

    chat_room_description(msg, 1);

    strcpy(p->client_ip, inet_ntoa(cli_addr.sin_addr));
    p->client_port = ntohs(cli_addr.sin_port);
    p->accept_fd = fd;

    if (0 == strcmp(p->user_type, "Administrator"))
    {
        printf("管理员:< %s > 上线,  ip:%s,  port:%d\n", p->user_name, p->client_ip, p->client_port);
    }
    else
    {
        printf("用户:< %s > 上线,  ip:%s,  port:%d\n", p->user_name, p->client_ip, p->client_port);
    }

    save_user(user_head);
}

// 错误处理
void perror_t(char *str)
{
    perror(str);
    exit(1);
}

// 添加套接字
void epoll_add_fd(int epoll_fd, int fd, int trigger, int clogged)
{
    struct epoll_event e;
    e.data.fd = fd;
    e.events = EPOLLIN;

    if (trigger)
    {
        e.events |= EPOLLET;
    }

    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &e);

    if (clogged)
    {
        int old_option = fcntl(fd, F_GETFL);
        int new_option = old_option | O_NONBLOCK;
        fcntl(fd, F_SETFL, new_option);
    }
}

// 移除套接字
void epoll_remove_fd(int epoll_fd, int fd)
{
    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, 0);
    close(fd);
}

// 发送私聊信息
void handle_private_chat(int fd, Message *msg, char *buf, int desc)
{
    read_user(user_head);
    int ret;

    Node p = user_head->next;
    while (p != NULL)
    {
        if ((0 == strcmp(p->user_name, msg->sending_target)) && ((1 == p->online_state) || (2 == p->online_state)))
        {
            if (1 == desc)
            {
                p->online_state = 2;
            }
            else if (2 == desc)
            {
                p->online_state = 1;
            }

            memcpy(buf, msg, sizeof(Message));

            ret = send(p->accept_fd, buf, sizeof(Message), 0);
            if (ret < 0)
            {
                perror_t("send error");
            }

            break;
        }

        p = p->next;
    }

    save_user(user_head);
}

// 发送群聊信息
void handle_group_chat(int fd, Message *msg, char *buf)
{
    read_user(user_head);
    int ret;

    Node all_p = user_head->next;
    while (all_p != NULL)
    {
        if (fd != all_p->accept_fd && ((1 == all_p->online_state) || (2 == all_p->online_state)))
        {
            memcpy(buf, msg, sizeof(Message));
            ret = send(all_p->accept_fd, buf, sizeof(Message), 0);
            if (ret < 0)
            {
                perror_t("send error");
            }
        }

        all_p = all_p->next;
    }

    save_user(user_head);
}

// 新链接上线
void handle_connect(int epoll_fd, int listen_fd, Message *msg)
{
    struct sockaddr_in cli_addr;
    socklen_t cli_len = sizeof(cli_addr);

    int conn_fd = accept(listen_fd, (struct sockaddr *)&cli_addr, &cli_len);
    if (conn_fd < 0)
    {
        perror_t("accept error");
    }

    // printf("accept分配fd:%d\n", conn_fd);

    epoll_add_fd(epoll_fd, conn_fd, 1, 0);
}

// 用户下线处理
void handle_user_offline(int epoll_fd, int fd, Message *msg)
{
    read_user(user_head);
    enum message_type type = msg->msg_type;
    char *buf = (char *)malloc(sizeof(Message));

    Node p = is_accept_existing(user_head, fd);
    if (NULL == p)
    {
    }
    else
    {
        if (0 == strcmp(p->user_type, "Administrator"))
        {
            printf("管理员:< %s > 下线,  ip:%s,  port:%d\n", p->user_name, p->client_ip, p->client_port);
        }
        else
        {
            printf("用户:< %s > 下线,  ip:%s,  port:%d\n", p->user_name, p->client_ip, p->client_port);
        }

        strcpy(msg->user_name, p->user_name);

        if (2 == p->online_state)
        {
            p->online_state = 3;
        }
        else
        {
            p->online_state = 0;
        }

        p->accept_fd = -1;

        msg->msg_type = UNDEFINED;

        if (0 == strcmp(p->user_type, "Administrator"))
        {
            sprintf(msg->msg, "管理员< %s >已下线，注意文明聊天！", msg->user_name);
        }
        else
        {
            sprintf(msg->msg, "用户< %s >下线啰~~~", msg->user_name);
        }

        memcpy(buf, msg, sizeof(Message));

        int ret;
        Node all_p = user_head->next;
        while (all_p != NULL)
        {
            if (fd != all_p->accept_fd && ((1 == all_p->online_state) || (2 == all_p->online_state)))
            {
                ret = send(all_p->accept_fd, buf, sizeof(Message), 0);
                if (ret < 0)
                {
                    perror_t("send error");
                }
            }

            all_p = all_p->next;
        }

        msg->msg_type = type;
    }

    epoll_remove_fd(epoll_fd, fd);
    save_user(user_head);
}

// 修改该描述符的监听事件
void mod_fd_event(int epoll_fd, int fd, int ev)
{
    struct epoll_event e;
    e.data.fd = fd;
    e.events = ev | EPOLLET;
    epoll_ctl(epoll_fd, EPOLL_CTL_MOD, fd, &e);
}

// 处理客户端发来的信息
void handle_client_messages(int epoll_fd, int fd, Message *msg)
{
    enum message_type type = msg->msg_type;
    int is_return = msg->need_return;
    // printf("---------");
    // traverse_all_node(msg);

    switch (type)
    {
    case USER_REGISTER:
        handle_user_register(fd, msg);
        break;

    case USER_LOGIN:
        handle_user_login(fd, msg);
        break;

    case NORMAL_CHAT:
        parsing_normal_messages(fd, msg);
        break;

    default:
        break;
    }

    if (1 == is_return)
    {
        mod_fd_event(epoll_fd, fd, EPOLLOUT);
    }
}

// 消息处理
void handle_message(int epoll_fd, int fd, Message *msg)
{
    char *buf = (char *)malloc(sizeof(Message));

    int n = recv(fd, buf, sizeof(Message), 0);
    if (n < 0)
    {
        perror_t("recv error!\n");
    }
    else if (n == 0)
    {
        handle_user_offline(epoll_fd, fd, msg);
    }
    else
    {
        memcpy(msg, buf, sizeof(Message));

        /******************************************************
         * 处理客户端发来的信息
         *****************************************************/
        // printf("服务器接收一次信息\n");
        handle_client_messages(epoll_fd, fd, msg);
    }
}

// 处理服务端发给客户端的信息
void handle_send(int epoll_fd, int fd, Message *msg)
{
    char *buf = (char *)malloc(sizeof(Message));

    enum message_type type = msg->msg_type;
    msg->need_return = 0;

    int ret;
    switch (type)
    {
    case PRIVATE_CHAT:
        handle_private_chat(fd, msg, buf, 0);
        break;

    case GROUP_CHAT:
        handle_group_chat(fd, msg, buf);
        break;

    case KIKC_OUT:
        handle_private_chat(fd, msg, buf, 0);
        break;

    case MUTE:
        handle_private_chat(fd, msg, buf, 1);
        break;

    case UNMUTE:
        handle_private_chat(fd, msg, buf, 2);
        break;

    default:
        memcpy(buf, msg, sizeof(Message));

        int ret = send(fd, buf, sizeof(Message), 0);
        if (ret < 0)
        {
            perror_t("send error");
        }

        break;
    }

    mod_fd_event(epoll_fd, fd, EPOLLIN);
}

// 事件循环
void event_loop(int epoll_fd, int listen_fd)
{
    struct epoll_event events[MAX_EVENTS];

    Message *msg = (Message *)malloc(sizeof(Message)); // 分配内存

    int ret, fd;
    while (1)
    {
        ret = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        if (ret < 0)
        {
            perror_t("epoll_wait error");
        }

        for (int i = 0; i < ret; i++)
        {
            fd = events[i].data.fd;

            if (fd == listen_fd)
            {
                handle_connect(epoll_fd, listen_fd, msg);
            }
            else if (events[i].events & EPOLLIN)
            {
                handle_message(epoll_fd, fd, msg);
            }
            else if (events[i].events & EPOLLOUT)
            {
                handle_send(epoll_fd, fd, msg);
            }
            else
            {
                perror_t("---------------\n");
            }
        }
    }
}

// 初始化已有账号套接字
void init_sock(int epoll_fd, int listen_fd)
{
    read_user(user_head);

    Node p = user_head->next;
    while (p != NULL)
    {
        if (p->accept_fd > listen_fd)
        {
            p->accept_fd = -1;
            p->online_state = 0;
            epoll_add_fd(epoll_fd, p->accept_fd, 1, 0);
        }

        p = p->next;
    }

    save_user(user_head);
}

int main(int argc, char const *argv[])
{

    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd < 0)
    {
        perror_t("socket error");
    }

    struct sockaddr_in ser_addr, cli_addr;
    socklen_t ser_len, cli_len;

    ser_len = sizeof(ser_addr);
    cli_len = sizeof(cli_addr);

    bzero(&ser_addr, ser_len);
    bzero(&cli_addr, cli_len);

    ser_addr.sin_family = AF_INET;
    // ser_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    ser_addr.sin_addr.s_addr = inet_addr(NET_IP);
    ser_addr.sin_port = htons(PORT);

    if (bind(listen_fd, (struct sockaddr *)&ser_addr, ser_len) == -1)
    {
        perror_t("bind error");
    }

    if (listen(listen_fd, 5) == -1)
    {
        perror_t("listen error");
    }

    printf("The server is ready!!!\n");

    int epoll_fd = epoll_create(1);
    if (epoll_fd < 0)
    {
        perror_t("epoll_create error");
    }

    // printf("listen_fd:%d\n", listen_fd);
    epoll_add_fd(epoll_fd, listen_fd, 1, 0);
    init_sock(epoll_fd, listen_fd);
    event_loop(epoll_fd, listen_fd);
    close(listen_fd);

    return 0;
}