# Game    是c++游戏服务器
# Client  是c#客户端demo网络库是用c++ libevent写的 windows用iocp linux用epoll
# Cxxdll  是在vs2022下编译动态库dll用的
# Cxxso   是在linux下编译动态库so用的懒的写Makefile直接用g++ -shared -fpic -lm -ldl -levent -lpthread -std=c++11 -O3 -o Cxxdll.so *.cpp
