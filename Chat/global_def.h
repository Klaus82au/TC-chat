#ifndef GLOBAL_DEF_H
#define GLOBAL_DEF_H


//errors code
enum n_errors{SUCCESS = 0,
              ERROR,
              SOCKET_ERROR,
              SETSOCKOPT_ERROR,
              SENDTO_ERROR,
              BIND_ERROR,
              RECVRFOM_ERROR,
              GETIFADDRS_ERROR,
              MY_MSG_ERROR,
              SENDER_PROC_ERROR,
              UNKNOWN_TYPE_MSG_ERROR,
              PTHREAD_CREATE_ERROR};

enum types{TYPE_STATUS = 1,
           TYPE_NEW_MESSAGE,
           TYPE_WHO_IS_THERE};

#define BUF_SIZE 255
#define STR_LEN 255
#define UDP_LISTENER_PORT 12345
#define BUSY 1
#define ONLINE 2

//in msec
#define TIMER_INTERVAL 5000
#define POPUP_TIME_LIVE 20000

#endif // GLOBAL_DEF_H
