/*
MIT License

Copyright (c) 2020 rysiof@gmail.com

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef _LOGGING_H
#define _LOGGING_H

#include <stdio.h>
#include <systemc.h>

enum __log_level
{
    E_LOG_ERROR,
    E_LOG_WARN,
    E_LOG_INFO,
    E_LOG_LOW,
    E_LOG_DEBUG,
};

/*
    @note   Let me explain 'printf' below. Maybe many of you like std::cout but
            I don't. Typical usage of std::cout is generaly slower. As we want
            our simulator to be as fast as possible, using std::cout just does
            not make a lot of sense. TLDR: autor
*/
#define LOG(level, format, ...)                                                 \
    do                                                                          \
    {                                                                           \
        (void)E_LOG_ ## level;                                                  \
        sc_time ___temp = sc_time_stamp();                                      \
        const char* t = ___temp.to_string().c_str();                            \
        printf("[%-5.5s][%-20.20s]\t" format "\n",  #level, t, ##__VA_ARGS__);  \
    } while(0)


#endif // _LOGGING_H