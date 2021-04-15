#ifndef _LOGGING_H
#define _LOGGING_H


#define LOG(level, format, ...)                         \
    do                                                  \
    {                                                   \
        sc_time ___temp = sc_time_stamp();              \
        const char* t = ___temp.to_string().c_str();    \
        printf("[%-5.5s][%-20.20s]\t" format "\n", #level, t, ##__VA_ARGS__);            \
    } while(0)


#endif // _LOGGING_H