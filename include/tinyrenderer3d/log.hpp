#ifndef TINYRENDERER3D_LOG_HPP
#define TINYRENDERER3D_LOG_HPP

#include <iostream>

#define DEBUG

#ifdef DEBUG
#define Log(msg, ...) do { printf("[%s]-%d:" msg "\n", __FUNCTION__, __LINE__, ##__VA_ARGS__); fflush(stdout); }while(0)
#else
#define Log(msg, ...)
#endif

#endif
