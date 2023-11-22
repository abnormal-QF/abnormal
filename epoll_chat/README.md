# 友寻聊天室设计文档
- 项目名称：友寻网路聊天室
- 开发环境：Linux、C 语言、Visual Studio Code
- 项目开源地址：[https://github.com/abnormal-QF/abnormal](https://github.com/abnormal-QF/abnormal)
- 作者：abnormal-QF
- 周期：2023.11.12 ~ 2023.11.22



## 友寻聊天室功能相关介绍
- 采用Client/Server架构，TCP/IP网络通信协议，确保数据传输的稳定性和安全性
- 数据存储采用文件存储形式，以双向链表作为数据链接
- 为Client A提供更便捷的登录方式，只需注册一次ID和密码即可轻松登录
- 允许多个Client X同时登录，并与其他在线用户进行实时通讯
- Client A登录成功后，可查看当前聊天室内其他在线用户的Client X列表
- 提供私密聊天功能，Client A可以选择向特定Client X发送悄悄话
- 提供群发消息功能，Client A可以选择向所有在线用户发送消息
- Client A在退出时Server端自动保存聊天记录，确保聊天内容的完整性和可追溯性
- Server端维护一个所有登录用户的聊天记录文件，方便查询和备查
- Server 内建一个特殊权限的账号 admin,用于管理聊天室
	客户端Client登陆这个账号可以管理聊天室

- 管理员可以将某个Client X “踢出聊天室” 
- 管理员可以将某个Client X ”设为只能旁听,不能发言”及禁言状态
	注：就算强制退出再次登录也处于禁言状态
	
- 管理员可以远程关闭服务器Server
- Client 端发言增加表情符号,可以设置某些自定义的特殊组合来表达感情
	如输入:),则会自动发送”XXX向大家做了个笑脸” 

- Client 端增加某些常用话语,可以对其中某些部分进行”姓名替换”,
	@ClientA/welcome,则会自动发送”ClientA 大侠,欢迎您来到友寻聊天室”

- Client 端空格优化，将输入数据的左右侧空格清除，并将中间的空格以下斜线的形式替换，这是必要的优化操作，使得软件更加稳定可靠

- 对于所有已登陆用户，其他用户的一些相关的操作会向您做出一些提示
	用户< asd >上线啦~，管理员< admin >已上线，注意文明聊天！
	您已被管理员< admin >强制禁言!
	您已被管理员< admin >强制拿下!



## 服务端和客户端相关设计
- 传输数据载体是一个自定义结构体
```c
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
}

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
```

- 服务端采用epoll多路I/O复用的方式处理客户端发送来的信息
- 客户端采用多线程进行读写操作，读端可处理服务端发送来的信息

- 服务器启动时将用户分配的描述符清空，并设置账号状态为结束时的状态(如禁言状态)，当需要数据时都会读取文件里的数据进行操作

- 用户从客户端的终端输入数据，并对此数据进行简单数据处理（主要是处理空格），客户端将解析这些信息，加上对应的类型标记发送给服务器
- 服务器接收到这些信息后，保存信息记录到文件中，并解析这些信息作出相应的处理
- 当用户下线会有相关信息通知在线用户
- 当服务器关闭后，客户端接收信息并退出，显示为服务器已下线
- 客户端实现了Linux和windows两端通用
